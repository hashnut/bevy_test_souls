
#include "GsMultiLevelRankManager.h"
#include "Quest/GsQuestData.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectStatSet.h"

#include "Message/MessageParam/GsMultiLevelRankMessageParam.h"
#include "Message/MessageParam/GsQuestMessageParam.h"

#include "GameObject/Quest/GsQuestHandler.h"

#include "Net/GsNetSendServiceWorld.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupMultiLevelRankPromotion.h"

#include "MultiLevelRank/ERankLinkType.h"


void FGsMultiLevelRankManager::Initialize()
{
	// 메시지 등록
	if (FGsMessageHolder* msg = GMessage())
	{
		_msgHandleUserInfoList.Emplace(msg->GetUserBasicInfo().AddRaw(MessageUserInfo::EXP
			, this, &FGsMultiLevelRankManager::OnChangeUserExp));

		_msgHandleContentQuestList.Emplace(msg->GetQuest().AddRaw(MessageContentQuest::QUEST_RANK_COMPLETED
			, this, &FGsMultiLevelRankManager::OnQuestRankCompleted));
	}

	// 데이터 초기화
	ResetData();

	// 등급 별 초기 데이터 설정
	LoadRankStaticData();

	// 각 등급 서로 연결
	MakeLinkedRankData();
}

void FGsMultiLevelRankManager::Finalize()
{
	// 메시지 해제
	if (FGsMessageHolder* msg = GMessage())
	{
		for (auto& iter : _msgHandleUserInfoList)
		{
			msg->GetUserBasicInfo().Remove(iter);
		}
		_msgHandleUserInfoList.Empty();

		for (auto& iter : _msgHandleContentQuestList)
		{
			msg->GetQuest().Remove(iter);
		}
		_msgHandleContentQuestList.Empty();
	}

	// 데이터 초기화
	ResetData();
}

void FGsMultiLevelRankManager::ResetData()
{
	_rankDataList.Empty();
	_passivityCollectionData.Empty();
	_selectedPledgeGroupId = INDEX_NONE;
	_maxAccumulateExp = 0;
	_activeLastSlot = nullptr;
	_inProgressSlot = nullptr;
}

void FGsMultiLevelRankManager::LoadRankStaticData()
{
	const UGsTableMultiLevelRank* RankTable = Cast<UGsTableMultiLevelRank>(FGsSchemaMultiLevelRank::GetStaticTable());
	if (nullptr == RankTable)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("UGsTableMultiLevelRank Nullptr Error"));
#endif
		return;
	}

	TArray<const FGsSchemaMultiLevelRank*> RankDatas;
	RankDatas.Reserve(RankTable->Num());
	if (!RankTable->GetAllRows(RankDatas))
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("MultiLevelRank Table Error"));
#endif
		return;
	}

	_rankDataList.Empty();
	for (const FGsSchemaMultiLevelRank* RankSchema : RankDatas)
	{
		int32 SubNodeMaxCount = RankSchema->multiLevelRankInfoList.Num();
		if (0 >= SubNodeMaxCount)
		{
#if WITH_EDITOR
			// 그룹당 한개 이상의 노드는 반드시 필요하다.
			GSLOG(Warning, TEXT("MultiLevelRank $d SubNodeCount %d "), RankSchema->rank, SubNodeMaxCount);
#endif
			continue;
		}

		TArray<FGsMultiLevelRankSlotData> Slots;
		Slots.Reserve(SubNodeMaxCount);

		for (int count = 0; count < SubNodeMaxCount; ++count)
		{
			FGsMultiLevelRankSlotData SlotData;

			SlotData._slotSchema = RankSchema->multiLevelRankInfoList[count].GetRow();
			SlotData._slotState = MultiLevelRankSlotState::LOCK;
			SlotData._linkedPrevSlot = nullptr;
			SlotData._ownerRank = nullptr;
			SlotData._storyState = RankStoryState::Normal;

			// Add..
			Slots.Emplace(SlotData);
		}

		FGsMultiLevelRankData RankData;
		RankData._rankSchema = RankSchema;
		RankData._slotList = Slots;

		// Add..
		_rankDataList.Emplace(RankData);
	}
}

void FGsMultiLevelRankManager::MakeLinkedRankData()
{
	for (int idx = 0; idx < _rankDataList.Num(); ++idx)
	{
		FGsMultiLevelRankData* prevRank = FindRankByIndex(idx - 1);

		for (FGsMultiLevelRankSlotData& slot : _rankDataList[idx]._slotList)
		{
			slot._ownerRank = &_rankDataList[idx];
			slot._linkedPrevSlot = nullptr;

			if (nullptr != prevRank)
			{	// 아래와 같은 타입은 항상 연결 시켜준다.
				if (prevRank->_rankSchema->linkType == ERankLinkType::SingleOnly)
				{
					// singleXXX 타입은 slot이 하나만 존재.
					slot._linkedPrevSlot = &(prevRank->_slotList[0]);
					continue;
				}
				else if (prevRank->_rankSchema->linkType == ERankLinkType::SingleToMulti)
				{
					// singleXXX 타입은 slot이 하나만 존재.
					slot._linkedPrevSlot = &(prevRank->_slotList[0]);
					_pledgePrevSlot = &(prevRank->_slotList[0]);
					continue;
				}

				slot._linkedPrevSlot = prevRank->_slotList.FindByPredicate([slot](const FGsMultiLevelRankSlotData& InSlot)
							{
								// 현재 슬롯 스키마
								const FGsSchemaMultiLevelRankSlot* currentSlotSchema = slot._slotSchema;
								// 이전 슬롯 스키마
								const FGsSchemaMultiLevelRankSlot* prevSlotSchema = InSlot._slotSchema;
								// 그룹 아이디가 같으면 같은 그룹
								return (currentSlotSchema->groupId == prevSlotSchema->groupId);
							}
				);
			}
		}
	}

	if (FGsMultiLevelRankData* rankData = FindRankByIndex(_rankDataList.Num() - 1))
	{
		if (rankData->_rankSchema->linkType == ERankLinkType::SingleToMulti)
		{
			_pledgePrevSlot = &(rankData->_slotList[0]);
		}
	}
}

bool FGsMultiLevelRankManager::SetSlotState(FGsMultiLevelRankSlotData* InSlot, MultiLevelRankSlotState InState)
{
	if (nullptr == InSlot || InSlot->_slotState == InState)
	{
		return false;
	}

	InSlot->_slotState = InState;
	switch (InState)
	{
	case MultiLevelRankSlotState::LOCK:
		InSlot->_storyState = RankStoryState::Normal;
		break;
	case MultiLevelRankSlotState::IN_PROGRESS:
		InSlot->_storyState = RankStoryState::InProgress;
		break;
	case MultiLevelRankSlotState::FINISH:
		InSlot->_storyState = RankStoryState::Completed;
		break;
	case MultiLevelRankSlotState::ACTIVE:
		InSlot->_storyState = RankStoryState::Completed;
		break;
	}

	return true;
}

void FGsMultiLevelRankManager::ChangeLinkedSlotState(ChangedSlotState InType
													, FGsMultiLevelRankSlotData* InBeginSlot 
													, MultiLevelRankSlotState InState)
{
	if (nullptr == InBeginSlot)
		return;

	MultiLevelRankSlotState nextState = InState;
	FGsMultiLevelRankSlotData* slot = InBeginSlot;
	while (true)
	{
		if (nullptr == slot)
			break;

		// 링크를 따라 상태를 쭉 바꿔준다.
		switch (InType)
		{
		case ChangedSlotState::Force:
			SetSlotState(slot, nextState);
			break;
		case ChangedSlotState::SortBy:
			if (static_cast<int32>(slot->_slotState) <= static_cast<int32>(nextState))
			{
				SetSlotState(slot, nextState);
			}
			break;
		};

		/**
		 * etc setting
		 */
		if (nextState == MultiLevelRankSlotState::IN_PROGRESS)
		{	// 2021/08/13 PKT - 퀘스트 진행 상태라면 링크를 따라 상태를 바꾸지 않고 바로 빠져 나간다.			
			return;
		}

		slot = slot->_linkedPrevSlot;
	}
}

void FGsMultiLevelRankManager::AddPassivity()
{
	if (nullptr == _activeLastSlot)
	{
		return;
	}
	
	_passivityCollectionData.Empty();

	const FGsMultiLevelRankSlotData* prevSlot = _activeLastSlot;
	while (true)
	{
		if (nullptr == prevSlot)
		{
			break;
		}

		for (const FGsSchemaPassivitySetRow& row : prevSlot->_slotSchema->passivityIdList)
		{
			const FGsSchemaPassivitySet* passivity = row.GetRow();
			if (passivity)
			{
				_passivityCollectionData.Add(passivity->id);
			}
		}

		prevSlot = prevSlot->_linkedPrevSlot;
	}

	if ( 0 < _passivityCollectionData.GetPassivityCount() )
	{	// send message..
		FGsPrimitiveInt32 param(static_cast<int32>(PassivityOwnerType::MULTI_LEVEL_RANK));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &param);
	}	
}

bool FGsMultiLevelRankManager::CheckQuestCompleted()
{	
	// 현재 진행 중인 퀘스트가 있다면 그 퀘스트가 모두 완료 되었는지를 체크..	
	const FGsSchemaQuestMultiLevelRankStory* Stroy = (_inProgressSlot) ?
																_inProgressSlot->_slotSchema->storyId.GetRow() : nullptr;
	if (nullptr == Stroy)
	{
		return false;
	}		

	TArray<TSharedPtr<FGsQuestData>> outQuestList;
	if (false == GSQuest()->GetRankQuestDataList(Stroy->id, outQuestList))
	{	// 스토리에 퀘스트가 없다??
#if WITH_EDITOR
		GSLOG(Error, TEXT("FGsMultiLevelRankManager::CheckQuestCompleted : invaild story data [%d]"), Stroy->id);
#endif
		return false;
	}
		
	bool isCompleted = true;
	TArray<QuestId> questIdList;
	for (TSharedPtr<FGsQuestData> quest : outQuestList)
	{
		FGsQuestDynamicData* dynamicData = quest->GetQuestDynamicData();
		if (nullptr == dynamicData || QuestState::COMPLETED != dynamicData->GetQuestState())
		{	// Dynamic Data가 없는 경우는 이미 퀘스트 완료 처리를 했거나 or 퀘스트를 받지 않았다거나..
			isCompleted = false;
			break;
		}
		questIdList.Emplace(quest->GetQuestId());
	}

	if (true == isCompleted)
	{
		FGsMultiLevelRankSlotData* slot = FindRankSlotByRankId(_inProgressSlot->_slotSchema->rankId);
		if (nullptr != slot)
		{
			if (nullptr == _inProgressSlot || slot->_slotSchema->rankId != _inProgressSlot->_slotSchema->rankId)
			{
#if WITH_EDITOR
				GSLOG(Error, TEXT("invaild slot [%d]"), slot->_slotSchema->rankId);
#endif
			}

			SetSlotState(slot, MultiLevelRankSlotState::FINISH);

			const FGsMultiLevelRankData* rankData = slot->_ownerRank;
			GMessage()->GetMultiLevelRank().SendMessage(MessageContentMultiLevelRank::INVALIDATE_SLOT_ALL, nullptr);

			// 2021/08/13 PKT - Update HUD RedDot
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MULTILEVELRANK, false);
		}
	}	

	return isCompleted;
}

void FGsMultiLevelRankManager::ReleaseQuestData()
{
	if (nullptr != _inProgressSlot)
	{
		FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
		if (nullptr != questHandler)
		{
			const FGsSchemaQuestMultiLevelRankStory* Stroy = _inProgressSlot->_slotSchema->storyId.GetRow();
			if (nullptr != Stroy)
			{
				questHandler->RemoveRankQuest(Stroy->id);
			}			
		}
	}
}

void FGsMultiLevelRankManager::OnChangeUserExp(uint64 InExp)
{
	if (InExp > _maxAccumulateExp)
	{
		_maxAccumulateExp = InExp;
	}

	// window send msg..
	GMessage()->GetMultiLevelRank().SendMessage(MessageContentMultiLevelRank::INVALIDATE_SLOT_ALL, nullptr);
}

void FGsMultiLevelRankManager::OnQuestRankCompleted(const IGsMessageParam*)
{
	if (nullptr == _inProgressSlot)
	{
		return;
	}

	const FGsMultiLevelRankData* rankData = _inProgressSlot->_ownerRank;
	if (nullptr == rankData)
		return;

	// 퀘스트가 완료 되었는지를 체크 해서 완료 되었다면 서버에게 완료가 되었다고 보낸다.
	CheckQuestCompleted();

	// window send msg..
	FGsMultiLevelRankSingleDataMessageParam param;
	param.SetData(rankData->_rankSchema->rank);
	GMessage()->GetMultiLevelRank().SendMessage(MessageContentMultiLevelRank::INVALIDATE_QUEST, &param);
}

void FGsMultiLevelRankManager::Notify_MultiLevelRankList(const TArray<MultiLevelRankSlotInfo>& InSlotList, Exp InExp)
{
	_maxAccumulateExp = InExp;

	if (0 < InSlotList.Num())
	{
		FGsMultiLevelRankData* rankData = nullptr;
		FGsMultiLevelRankSlotData* slotData = nullptr;
		int32 lastSlotIndex = 0;
		for (int32 slotListIndex = 0;slotListIndex < InSlotList.Num();++slotListIndex)
		{
			const MultiLevelRankSlotInfo& data = InSlotList[slotListIndex];
			FGsMultiLevelRankData* rankTempData = _rankDataList.FindByPredicate([data](const FGsMultiLevelRankData& InData)
				{
					return (InData._rankSchema->rank == data.mRank);
				});

			if (nullptr == rankTempData)
			{
				continue;
			}

			if (nullptr == rankData || rankTempData->_rankSchema->rank > rankData->_rankSchema->rank)
			{
				rankData = rankTempData;

				for (int32 index = 0;index < rankData->_slotList.Num();++index)
				{
					if (rankData->_slotList[index]._slotSchema->rankId == data.mRankId)
					{
						slotData = &(rankData->_slotList[index]);
						lastSlotIndex = slotListIndex;
						break;
					}
				}
			}
			else if (nullptr != rankData && rankTempData->_rankSchema->rank == rankData->_rankSchema->rank)
			{
				for (int32 index = 0; index < rankData->_slotList.Num(); ++index)
				{
					if (rankData->_slotList[index]._slotSchema->rankId == data.mRankId)
					{
						if (data.mSlotState == MultiLevelRankSlotState::ACTIVE ||
							data.mSlotState == MultiLevelRankSlotState::IN_PROGRESS)
						{
							slotData = &(rankData->_slotList[index]);
							lastSlotIndex = slotListIndex;
							break;
						}
					}
				}
			}
		}

		if (slotData)
		{
			ChangeLinkedSlotState(ChangedSlotState::SortBy, slotData->_linkedPrevSlot, MultiLevelRankSlotState::ACTIVE);
			SetSlotState(slotData, InSlotList[lastSlotIndex].mSlotState);

			// 슬롯값 저장
			if (InSlotList[lastSlotIndex].mSlotState == MultiLevelRankSlotState::IN_PROGRESS ||
				InSlotList[lastSlotIndex].mSlotState == MultiLevelRankSlotState::FINISH)
			{
				_inProgressSlot = slotData;
				_activeLastSlot = slotData->_linkedPrevSlot;
			}
			else if (InSlotList[lastSlotIndex].mSlotState == MultiLevelRankSlotState::ACTIVE)
			{
				_inProgressSlot = nullptr;
				_activeLastSlot = slotData;
			}
		}
	}
	UpdatePledgeInfo();

	if (nullptr != _inProgressSlot)
	{	// 이 부분 필요한 이유가 오프라인 플레이 진행 시 퀘스트가 완료가 될수도 있기 때문에 혹여나 지금 진행 중인 퀘스트가 
		// 있다면 완료 했는지 여부를 확인 해 볼 필요가 있음.
		CheckQuestCompleted();
	}

	if (nullptr != _activeLastSlot)
	{	// 패시비티 데이타 업데이트
		AddPassivity();
	}
	/**
	 * 이 시점에서 굳이 UI쪽에 Message를 보낼 필요는 없다.
	 */

	// 2021/08/13 PKT - Update HUD RedDot
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MULTILEVELRANK, false);
}

// 승급
void FGsMultiLevelRankManager::Ack_MultiLevelRankPromotion(MultiLevelRankId InRankId)
{	
	FGsMultiLevelRankSlotData* slot = FindRankSlotByRankId(InRankId);
	if (nullptr != slot)
	{		
		// 강제로 해당 분기의 노드를 모두 ACTIVE 상태로 바꾸겠다.
		ChangeLinkedSlotState(ChangedSlotState::Force, slot, MultiLevelRankSlotState::ACTIVE);
		// 최종 슬롯 값 저장
		_activeLastSlot = slot;
		_inProgressSlot = nullptr;

		UpdatePledgeInfo();

		AddPassivity();

		// 승급 팝업 오픈
		TWeakObjectPtr<UGsUIPopupMultiLevelRankPromotion> popup = Cast<UGsUIPopupMultiLevelRankPromotion>(GUI()->OpenAndGetWidget(TEXT("PopupMultiLevelRankPromotion")));
		if (popup.IsValid())
		{
			popup->SetData(slot->_slotSchema->iconPath, slot->_slotSchema->subIconPath, slot->_slotSchema->subIconCount, slot->_slotSchema->NameText);
		}

		// window send msg..
		GMessage()->GetMultiLevelRank().SendMessage(MessageContentMultiLevelRank::INVALIDATE_SLOT_ALL, nullptr);

		// 2021/08/13 PKT - Update HUD RedDot
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MULTILEVELRANK, false);
	}	
}

// 랭크 변경
void FGsMultiLevelRankManager::Ack_MultiLevelRankChangeRank(MultiLevelRankId InPrevRankId, MultiLevelRankId InNextRankId)
{
	FGsMultiLevelRankSlotData* nextSlot = FindRankSlotByRankId(InNextRankId);
	if (nullptr == nextSlot)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("Invalid MultiLevelRankId : InNextRankId - %u"), InNextRankId);
#endif
		return;
	}
	
	if (_cheatSended)
	{
		ChangeLinkedSlotState(ChangedSlotState::Force, nextSlot, MultiLevelRankSlotState::ACTIVE);
		_inProgressSlot = nullptr;
		_activeLastSlot = nextSlot;
	}
	else
	{
		// 기존 진행 중이던 퀘스트가 있을 수도 있으니 퀘스트 지우는 것을 시도.
		ReleaseQuestData();

		ChangeLinkedSlotState(ChangedSlotState::Force, nextSlot->_linkedPrevSlot, MultiLevelRankSlotState::ACTIVE);

		if (_inProgressSlot && (_inProgressSlot->_slotState == MultiLevelRankSlotState::IN_PROGRESS || _inProgressSlot->_slotState == MultiLevelRankSlotState::FINISH))
		{
			SetSlotState(nextSlot, MultiLevelRankSlotState::IN_PROGRESS);

			_inProgressSlot = nextSlot;
			_activeLastSlot = nextSlot->_linkedPrevSlot;
		}
		else
		{
			SetSlotState(nextSlot, MultiLevelRankSlotState::ACTIVE);

			_inProgressSlot = nullptr;
			_activeLastSlot = nextSlot;
		}

		// 서약이 변경되었습니다.
		FText textMessageTicker;
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Ticker_PledgeChanged"), textMessageTicker);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);
	}
	
	UpdatePledgeInfo();

	AddPassivity();

	if (false == _cheatSended)
	{
		GUI()->Open(TEXT("PopupMultiLevelRankPledgeSelect"));
	}

	// window send msg..
	GMessage()->GetMultiLevelRank().SendMessage(MessageContentMultiLevelRank::INVALIDATE_SLOT_ALL, nullptr);
}

// 수락
void FGsMultiLevelRankManager::Ack_QuestMultiLevelRankAccept(MultiLevelRankId InRankId)
{
	FGsMultiLevelRankSlotData* slot = FindRankSlotByRankId(InRankId);
	if (nullptr != slot)
	{	
		SetSlotState(slot, MultiLevelRankSlotState::IN_PROGRESS);
		_inProgressSlot = slot;

		UpdatePledgeInfo();

		// window send msg..
		GMessage()->GetMultiLevelRank().SendMessage(MessageContentMultiLevelRank::INVALIDATE_SLOT_ALL, nullptr);
	}

	// 승급 임무가 시작되었습니다.
	FText textMessageTicker;
	FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Ticker_GetQuest"), textMessageTicker);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);

	// 승급 임무 수락 사운드 출력
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->PlaySound2D(TEXT("UI_Start_Mission"));
	}
}

// 퀘스트 완료
void FGsMultiLevelRankManager::Ack_QuestMultiLevelRankComplete(MultiLevelRankId InRankId)
{
	FGsMultiLevelRankSlotData* slot = FindRankSlotByRankId(InRankId);
	if (nullptr != slot)
	{	
		if (nullptr == _inProgressSlot || slot->_slotSchema->rankId != _inProgressSlot->_slotSchema->rankId)
		{
			GSLOG(Error, TEXT("invaild slot [%d]"), slot->_slotSchema->rankId );
		}

		SetSlotState(slot, MultiLevelRankSlotState::FINISH);

		GMessage()->GetMultiLevelRank().SendMessage(MessageContentMultiLevelRank::INVALIDATE_SLOT_ALL, nullptr);

		// 2021/08/13 PKT - Update HUD RedDot
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MULTILEVELRANK, false);
	}
}

void FGsMultiLevelRankManager::SendQuestAccept(StoryId InId)
{
	TArray<TSharedPtr<FGsQuestData>> outQuestList;
	GSQuest()->GetRankQuestDataList(InId, outQuestList);
	TArray<QuestId> questIdList;
	for (TSharedPtr<FGsQuestData> quest : outQuestList)
	{
		questIdList.Emplace(quest->GetQuestId());
	}

	// send...
	FGsNetSendServiceWorld::SendQuestMultiLevelRankAccept(InId, questIdList);
}

void FGsMultiLevelRankManager::SendQuestFinish(StoryId InId, const TArray<QuestId>& InQuestList)
{	// send..
	FGsNetSendServiceWorld::SendQuestMultiLevelRankComplete(InId, InQuestList);
}

void FGsMultiLevelRankManager::SendRankPromotion(MultiLevelRankId InId)
{	// send..
	FGsNetSendServiceWorld::SendReqMultiLevelRankPromotion(InId);
}

void FGsMultiLevelRankManager::SendRankChange(MultiLevelRankId InId)
{	// send..
	FGsNetSendServiceWorld::SendReqMultiLevelRankChangeRank(InId);
}

FGsMultiLevelRankData* FGsMultiLevelRankManager::FindRankById(MultiLevelRank InId)
{
	return _rankDataList.FindByPredicate([InId](FGsMultiLevelRankData& InData)
										{
											return (InData._rankSchema->rank == InId);
										});
}

FGsMultiLevelRankData* FGsMultiLevelRankManager::FindRankByIndex(int32 InIndex)
{
	return (true == _rankDataList.IsValidIndex(InIndex)) ? &(_rankDataList[InIndex]) : nullptr;
}

FGsMultiLevelRankSlotData* FGsMultiLevelRankManager::FindRankSlotByRankId(MultiLevelRankId InRankId)
{
	FGsMultiLevelRankSlotData* findData = nullptr;
	for (FGsMultiLevelRankData& rank : _rankDataList)
	{
		findData = rank._slotList.FindByPredicate([InRankId](FGsMultiLevelRankSlotData& item)
													{
														return (item._slotSchema->rankId == InRankId);
													});
		if (nullptr != findData)
			break;
	}	
	return findData;
}

bool FGsMultiLevelRankManager::CheckRedDot(bool bInUpdate/* = true*/)
{
	// 1. 임무 수락 활성화 확인 : 다음 슬롯의 경험치 조건 만족 && 현재 슬롯의 활성화 여부 확인
	bool canAcceptQuest = CanAcceptQuest(_activeLastSlot);

	// 2. 승급 가능 상태 확인
	bool canPromtion = CanPromotion();

	// 3. 서약 가능 상태 확인
	bool canSelectPledge = CanSelectPledge();

	SetRedDot(canAcceptQuest || canPromtion || canSelectPledge);

	return IsRedDot();
}

TArray<FGsMultiLevelRankData> FGsMultiLevelRankManager::GetRankDataList()
{
	return _rankDataList;
}

int32 FGsMultiLevelRankManager::GetRankMaxCount() const
{
	return _rankDataList.Num();
}

const FGsMultiLevelRankData* FGsMultiLevelRankManager::GetRankByIndex(int32 InIndex)
{	
	return FindRankByIndex(InIndex);
}

int32 FGsMultiLevelRankManager::GetIndexByRankData(const FGsMultiLevelRankData* InRank)
{
	if (nullptr == InRank)
		return INDEX_NONE;

	return _rankDataList.IndexOfByPredicate([InRank](const FGsMultiLevelRankData& InData)
												{
													return (InRank->_rankSchema->rank == InData._rankSchema->rank);
												});
}

const FGsMultiLevelRankData* FGsMultiLevelRankManager::GetRankById(MultiLevelRank InId)
{
	return FindRankById(InId);
}


int32 FGsMultiLevelRankManager::GetIndexByRankId(MultiLevelRank InRank)
{
	if (InRank == INVALID_MULTI_LEVEL_RANK)
		return INDEX_NONE;

	return _rankDataList.IndexOfByPredicate([InRank](const FGsMultiLevelRankData& InData)
		{
			return (InData._rankSchema->rank == InRank);
		});
}

const FGsMultiLevelRankSlotData* FGsMultiLevelRankManager::GetActiveLastSlot() const
{
	return _activeLastSlot;
}

const FGsMultiLevelRankSlotData* FGsMultiLevelRankManager::GetQuestInProgressSlot() const
{
	return _inProgressSlot;
}

const FGsMultiLevelRankSlotData* FGsMultiLevelRankManager::GetTopLevleRankSlot(const FGsMultiLevelRankSlotData* InTarget) const
{
	if (nullptr == InTarget || nullptr == InTarget->_ownerRank)
		return nullptr;

	auto FindTopLevelSlot = [InTarget](const FGsMultiLevelRankData* InRankData)
	{
		return InRankData->_slotList.FindByPredicate([InTarget](const FGsMultiLevelRankSlotData& InSlot)
			{
				return (InSlot._slotSchema->groupId == InTarget->_slotSchema->groupId && InSlot._slotState == InTarget->_slotState);
			}
		);
	};

	const FGsMultiLevelRankData* ownerRank = InTarget->_ownerRank;
	int32 rankIndex = _rankDataList.IndexOfByPredicate([InTarget](const FGsMultiLevelRankData& InRankData)
									{
											const FGsMultiLevelRankData* ownerRank = InTarget->_ownerRank;
											return (InRankData._rankSchema->rank == ownerRank->_rankSchema->rank);
									});
	

	const FGsMultiLevelRankSlotData* returnSlotData = InTarget;
	for (int32 idx = rankIndex + 1; idx < _rankDataList.Num(); ++idx)
	{
		const FGsMultiLevelRankData* nextRank = &(_rankDataList[idx]);
		const FGsMultiLevelRankSlotData* findSlotData = FindTopLevelSlot(nextRank);
		if (nullptr != findSlotData)
		{
			returnSlotData = findSlotData;
		}
		else
		{
			break;
		}
	}

	return returnSlotData;
}

bool FGsMultiLevelRankManager::IsPledgeSelected()
{
	if (_selectedPledgeGroupId == INVALID_MULTI_LEVEL_RANK_GROUP_ID ||
		_selectedPledgeGroupId == INDEX_NONE)
	{
		return false;
	}

	return true;
}

int32 FGsMultiLevelRankManager::GetSelectedPledgeId()
{
	return _selectedPledgeGroupId;
}

bool FGsMultiLevelRankManager::CheckExpConditionById(MultiLevelRank InId)
{
	uint64 requireRankExp = 0;
	const FGsMultiLevelRankData* rankData = FindRankById(InId);
	if (nullptr != rankData)
	{	// 요구 경험치
		requireRankExp = rankData->_rankSchema->exp;
	}

	return (_maxAccumulateExp >= requireRankExp);
}

bool FGsMultiLevelRankManager::CheckPrevRankCondition(const FGsMultiLevelRankSlotData* InSlotData)
{
	const FGsMultiLevelRankSlotData* prevData = InSlotData->_linkedPrevSlot;
	if (nullptr != prevData)
	{
		if (prevData->_slotState != MultiLevelRankSlotState::ACTIVE)
		{
			return false;
		}
	}

	return true;
}

bool FGsMultiLevelRankManager::CanAcceptQuest(const FGsMultiLevelRankSlotData* InSlotData)
{
	if (nullptr != _inProgressSlot)
	{	// 이미 진행중인 슬롯이 있다면 퀘스트를 수락할 수 없다.
		return false;
	}

	if (InSlotData)
	{	// 활성화된 슬롯이 있을 때
		int32 CurrentIndex = GetIndexByRankData(InSlotData->_ownerRank);
		if (const FGsMultiLevelRankData* NextRank = GetRankByIndex(CurrentIndex + 1))
		{
			bool nextExpCondition = CheckExpConditionById(NextRank->_rankSchema->rank);
			bool completeCondition = (InSlotData->_slotState == MultiLevelRankSlotState::ACTIVE);
			return nextExpCondition && completeCondition;
		}
	}
	else
	{	// 활성화된 슬롯이 없을 때
		const FGsMultiLevelRankData* NextRank = GetRankByIndex(0);
		if (NextRank)
		{
			return CheckExpConditionById(NextRank->_rankSchema->rank);
		}
	}

	return false;
}

bool FGsMultiLevelRankManager::CanPromotion()
{
	/**
	* 승급이 가능 할때.
	*/
	if (nullptr == _inProgressSlot)
	{	// 2021/08/13 PKT - 진행중인 슬롯이 없다.
		return false;
	}

	if (MultiLevelRankSlotState::FINISH != _inProgressSlot->_slotState)
	{	// 2021/08/13 PKT - 퀘스트가 완료한 상태가 아니라면 승급 가능 상태가 아님!
		return false;
	}

	return true;
}

bool FGsMultiLevelRankManager::CanSelectPledge()
{
	// 서약이 이미 선택되었는가?
	if (IsPledgeSelected())
		return false;

	// 서약할 차례가 아닌가?
	if (nullptr == _activeLastSlot || nullptr == _pledgePrevSlot)
		return false;

	if (_activeLastSlot->_ownerRank->_rankSchema->rank != _pledgePrevSlot->_ownerRank->_rankSchema->rank)
		return false;

	if (false == CheckExpConditionById(_pledgePrevSlot->_ownerRank->_rankSchema->rank + 1))
		return false;

	if (_pledgePrevSlot->_slotState != MultiLevelRankSlotState::ACTIVE)
		return false;

	return true;
}

const FGsMultiLevelRankSlotData* FGsMultiLevelRankManager::GetRankSlot(MultiLevelRank InRank, int32 InGroupId)
{
	if (const FGsMultiLevelRankData* rankData = GetRankById(InRank))
	{
		for (int32 i = 0;i < rankData->_slotList.Num();i++)
		{
			if (rankData->_slotList[i]._slotSchema->groupId == InGroupId)
			{
				return &rankData->_slotList[i];
			}
		}
	}

	return nullptr;
}

const FGsMultiLevelRankSlotData* FGsMultiLevelRankManager::GetPledgePrevSlot() const
{
	return _pledgePrevSlot;
}

void FGsMultiLevelRankManager::UpdatePledgeInfo()
{
	int32 prevPledgeGroupId = _selectedPledgeGroupId;
	const FGsMultiLevelRankSlotData* slotData = nullptr;
	if (_inProgressSlot)
	{
		slotData = _inProgressSlot;
	}
	else if (_activeLastSlot)
	{
		slotData = _activeLastSlot;
	}

	if (_pledgePrevSlot && slotData &&
		_pledgePrevSlot->_ownerRank->_rankSchema->rank < slotData->_ownerRank->_rankSchema->rank)
	{
		_selectedPledgeGroupId = slotData->_slotSchema->groupId;
	}
	else
	{
		_selectedPledgeGroupId = INDEX_NONE;
	}

	if (prevPledgeGroupId != _selectedPledgeGroupId)
	{
		GMessage()->GetMultiLevelRank().SendMessage(MessageContentMultiLevelRank::INITIALIZE_SLOT_ALL, nullptr);
	}
}