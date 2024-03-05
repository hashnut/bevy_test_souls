 //Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMultiLevelRankQuest.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"

#include "GameObject/Stat/GsStatHelper.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "MultiLevelRank/GsMultiLevelRankData.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsMultiLevelRankManager.h"

#include "Message/MessageParam/GsMultiLevelRankMessageParam.h"

#include "Quest/GsQuestData.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiLevelRankQuestEntry.h"
#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiLevelRankStatEntry.h"
#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiRankRewardData.h"

#include "PaperSprite.h"

#include "T1Project.h"


void UGsUIPopupMultiLevelRankQuest::BeginDestroy()
{
	if (nullptr != _slotHelperQuest)
	{
		_slotHelperQuest->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnRefreshQuestEntryItem);
	}
	_slotHelperQuest = nullptr;

	if (nullptr != _slotHelperRewardStat)
	{
		_slotHelperRewardStat->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnRefreshRewardEntryStat);
	}
	_slotHelperRewardStat = nullptr;

	if (nullptr != _slotHelperRewardItem)
	{
		_slotHelperRewardItem->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnRefreshRewardEntryItem);
	}
	_slotHelperRewardItem = nullptr;

	if (nullptr != _btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnClickClose);
	}
	_btnClose = nullptr;

	if (nullptr != _btnLeft)
	{
		_btnLeft->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnClickLeft);
	}
	_btnLeft = nullptr;

	if (nullptr != _btnRight)
	{
		_btnRight->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnClickRight);
	}
	_btnRight = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMultiLevelRankQuest::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperQuest = NewObject<UGsDynamicPanelSlotHelper>(this);
	if (nullptr != _slotHelperQuest)
	{
		_slotHelperQuest->Initialize(_subClassOfQuestEntry, _scrollBoxQuest);
		_slotHelperQuest->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnRefreshQuestEntryItem);
	}

	_slotHelperRewardStat = NewObject<UGsDynamicPanelSlotHelper>(this);
	if (nullptr != _slotHelperRewardStat)
	{
		_slotHelperRewardStat->Initialize(_subClassOfRewardStat, _scrollRewardStat);
		_slotHelperRewardStat->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnRefreshRewardEntryStat);
	}

	_slotHelperRewardItem = NewObject<UGsDynamicPanelSlotHelper>(this);
	if (nullptr != _slotHelperRewardItem)
	{
		_slotHelperRewardItem->Initialize(_subClassOfRewardItem, _scrollRewardItem);
		_slotHelperRewardItem->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnRefreshRewardEntryItem);
	}

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnClickClose);
	_btnLeft->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnClickLeft);
	_btnRight->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankQuest::OnClickRight);
}

void UGsUIPopupMultiLevelRankQuest::NativeConstruct()
{
	Super::NativeConstruct();

	// 메시지 등록
	FGsMessageHolder* msg = GMessage();
	MHud& contentsHud = msg->GetContentsHud();

	_hudHandlerList.Emplace(contentsHud.AddUObject(MessageContentHud::INVALIDATE_QUEST,
		this, &UGsUIPopupMultiLevelRankQuest::InvalidateQuestInternal));

	_pledgeGroupId = 0;
	_rankGroupId = 0;
}

void UGsUIPopupMultiLevelRankQuest::NativeDestruct()
{
	// 메시지 해제
	FGsMessageHolder* msg = GMessage();
	for (MsgHudHandle& msgHandler : _hudHandlerList)
	{
		msg->GetContentsHud().Remove(msgHandler);
	}
	_hudHandlerList.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupMultiLevelRankQuest::OnClickClose()
{
	if (_isFromSlot)
	{
		FGsMultiLevelRankSingleDataMessageParam param(_focusRankSlotData->_ownerRank->_rankSchema->rank);
		GMessage()->GetMultiLevelRank().SendMessage(MessageContentMultiLevelRank::CHANGE_FOCUSED_SLOT, &param);
	}
	else
	{
		GUI()->Open(TEXT("PopupMultiLevelRankPledgeSelect"));
	}
	
	Close();
}

void UGsUIPopupMultiLevelRankQuest::OnClickLeft()
{
	if (nullptr == _leftRankSlotData)
	{
		InvalidateButtonInternal();
		return;
	}

	InvalidateQuest(_leftRankSlotData, _pledgeGroupId, _rankGroupId, _isFromSlot);
}

void UGsUIPopupMultiLevelRankQuest::OnClickRight()
{
	if (nullptr == _rightRankSlotData)
	{
		InvalidateButtonInternal();
		return;
	}
		

	InvalidateQuest(_rightRankSlotData, _pledgeGroupId, _rankGroupId, _isFromSlot);
}

void UGsUIPopupMultiLevelRankQuest::OnRefreshQuestEntryItem(int32 InIndex, class UWidget* InEntry)
{
	if (false == _refreshQuestBuffer.IsValidIndex(InIndex))
	{
		return;
	}

	TSharedPtr<FGsQuestData> questData = _refreshQuestBuffer[InIndex].questData;
	if (false == questData.IsValid())
	{
#if  WITH_EDITOR
		GSLOG(Warning, TEXT("UGsUIWindowMultiLevelRank::OnRefreshQuestEntryItem : invaild Quest Data : %d"), InIndex);
#endif	//WITH_EDITOR
		return;
	}

	/**
	 * Quest는 ObjectiveList를 갖고 있기 때문에 Index순으로 접근하면 안된다.
	 * beginePoint는 InIndex가 실제적으로 접근해야 하는 기준을 잡아준다.
	 *
	 * EX) 2개의 Quest가 각각 3개의 퀘스트를 갖고 있다면 2번째 Quest의 3번째 Objective index는 3이 되어야 한다.
	 * 하지만 InIndex는 5(2 + 3)이다.
	 * InIndex가 5일때 beginePoint 값은 2가 된다.(2번째 Quest의 3번째 Objective에 접근)
	 */
	int32 objectiveIndex = InIndex - _refreshQuestBuffer[InIndex].beginePoint;

	// current quest data
	const FGsQuestData* rankQuestData = questData.Get();

	FText questName;				// name
	int32 questObjectveValue = 0;	// static value
	int32 questDynamicValue = 0;	// dynamic value
	FText questDesc;
	QuestId questId = questData->GetQuestId();
	GSQuest()->GetTitleStringId(questId, questDesc);
	rankQuestData->GetObjectiveDestText(objectiveIndex, questName);
	rankQuestData->GetObjectiveValue(objectiveIndex, questObjectveValue);
	rankQuestData->GetObjectiveDynamicValue(objectiveIndex, questDynamicValue);

	// 카운트를 보여줘야 할지 말아야 할지..
	bool IsShowParam = questData->IsShowUIObjectiveCount(objectiveIndex);

	// 퀘스트 상황에 맞게 Color 값 조절
	EGsUIColorType colorType = EGsUIColorType::MULTILEVELRANK_QUEST_NORMAL_COLOR;

	if (nullptr != _focusRankSlotData)
	{
		switch (_focusRankSlotData->_slotState)
		{
		case MultiLevelRankSlotState::LOCK:
		{
			// 퀘스트 미진행( 조건 안됨! )
			colorType = EGsUIColorType::MULTILEVELRANK_QUEST_NORMAL_COLOR;
		}	break;
		case MultiLevelRankSlotState::IN_PROGRESS:
		{	// 퀘스트 진행 중
			colorType = EGsUIColorType::MULTILEVELRANK_QUEST_INPROGRESS;

			if (0 < questObjectveValue && questObjectveValue <= questDynamicValue)
			{
				colorType = EGsUIColorType::MULTILEVELRANK_QUEST_COMPLECTED;
			}
		}	break;
		case MultiLevelRankSlotState::FINISH:
		{
			// 퀘스트 완료 / 비활성화
			colorType = EGsUIColorType::MULTILEVELRANK_QUEST_COMPLECTED;
		}	break;
		case MultiLevelRankSlotState::ACTIVE:
		{
			// 퀘스트 완료 / 활성화
			colorType = EGsUIColorType::MULTILEVELRANK_QUEST_COMPLECTED;
		}	break;
		}
	}

	if (EGsUIColorType::MULTILEVELRANK_QUEST_COMPLECTED == colorType)
	{	// 퀘스트가 완료 상태라면 최종 값을 맞춰 준다.
		questDynamicValue = questObjectveValue;
	}

	// invalidate...
	TWeakObjectPtr<UGsUIMultiLevelRankQuestEntry> item = Cast<UGsUIMultiLevelRankQuestEntry>(InEntry);
	if (false == item.IsValid())
		return;

	if (_isInitialize)
	{
		item->InitializeData(questName, questDesc, (false == IsShowParam), _refreshQuestBuffer[InIndex].iconPath);
	}

	item->SetProgressData(questDynamicValue, questObjectveValue);
	item->SetColor(FGsUIColorHelper::GetColor(colorType));
}

void UGsUIPopupMultiLevelRankQuest::OnRefreshRewardEntryStat(int32 InIndex, UWidget* InEntry)
{
	if (false == _refreshStatBuffer.IsValidIndex(InIndex))
		return;

	const TPair<StatType, int32>& statValue = _refreshStatBuffer[InIndex];

	FText statName;
	if (false == FGsStatHelper::GetDisPlayStatName(statValue.Key, statName))
		return;

	TWeakObjectPtr<UGsUIMultiLevelRankStatEntry> item = Cast<UGsUIMultiLevelRankStatEntry>(InEntry);
	if (item.IsValid())
	{
		FText textAfterStat;
		FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statValue.Key, statValue.Value, textAfterStat);

		item->SetData(statName, textAfterStat);
	}
}

void UGsUIPopupMultiLevelRankQuest::OnRefreshRewardEntryItem(int32 InIndex, UWidget* InEntry)
{
	if (false == _refreshItemBuffer.IsValidIndex(InIndex))
		return;

	TWeakObjectPtr<UGsUIMultiRankRewardData> item = Cast<UGsUIMultiRankRewardData>(InEntry);
	if (item.IsValid())
	{
		const FGsRefreshRewardBuffer& data = _refreshItemBuffer[InIndex];
		item->InvalidateData(data);
	}
}

void UGsUIPopupMultiLevelRankQuest::InvalidateQuest(const FGsMultiLevelRankSlotData* InFocusData, int32 InPledgeGroupId, int32 InRankGroupId, bool InIsFromSlot)
{
	_focusRankSlotData = InFocusData;
	_pledgeGroupId = InPledgeGroupId;
	_rankGroupId = InRankGroupId;
	_isFromSlot = InIsFromSlot;

	_isInitialize = true;

	InvalidateQuestInternal();
	InvalidateRewardInternal();
	InvalidateButtonInternal();
}

void UGsUIPopupMultiLevelRankQuest::InvalidateQuestInternal()
{
	if (nullptr == _focusRankSlotData || nullptr == _slotHelperQuest)
	{
		return;
	}

	FGsMultiLevelRankManager* rankManager = GSMultiLevelRank();
	if (nullptr == rankManager)
		return;

	const FGsSchemaQuestMultiLevelRankStory* questStory = _focusRankSlotData->_slotSchema->storyId.GetRow();
	if (nullptr == questStory)
	{
#if  WITH_EDITOR
		GSLOG(Warning, TEXT("UGsUIPopupMultiLevelRankQuest::InvalidateQuestInternal : Invaild Quest StoryRow"));
#endif	//WITH_EDITOR		
		return;
	}

	_refreshQuestBuffer.Empty();

	int32 beginQuestNum = 0;
	int32 questObjectiveNum = 0;
	TArray<TSharedPtr<FGsQuestData>> outQuestList;
	if (true == GSQuest()->GetRankQuestDataList(questStory->id, outQuestList))
	{
		for (uint8 i = 0;i < outQuestList.Num();++i)
		{
			TSharedPtr<FGsQuestData> quest = outQuestList[i];

			questObjectiveNum = 0;

			const FGsQuestData* rankData = (quest.IsValid()) ? quest.Get() : nullptr;
			if (nullptr == rankData)
			{
#if  WITH_EDITOR
				GSLOG(Warning, TEXT("UGsUIPopupMultiLevelRankQuest::InvalidateQuestInternal : nullptr == rankData"));
#endif	//WITH_EDITOR	
				continue;
			}

			for (int idx = 0; idx < rankData->GetObjectiveNum(); ++idx)
			{
				++questObjectiveNum;

				FGsRefreshQuestBuffer buffer;
				buffer.questData = quest;
				buffer.beginePoint = beginQuestNum;
				if (questStory->iconList.IsValidIndex(i))
				{
					buffer.iconPath = &questStory->iconList[i];
				}
				
				// Add..
				_refreshQuestBuffer.Emplace(buffer);
			}

			beginQuestNum += questObjectiveNum;
		}
	}

	// set rank name
	FText findText;
	FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Popup_Quest_Name"), findText);
	FText titleText = FText::Format(findText, _focusRankSlotData->_slotSchema->NameText);
	_textBlockName->SetText(titleText);

	FText stateText;
	switch (_focusRankSlotData->_slotState)
	{
	case MultiLevelRankSlotState::LOCK:
	case MultiLevelRankSlotState::UNLOCK:
	{
		if (false == rankManager->CheckExpConditionById(_focusRankSlotData->_ownerRank->_rankSchema->rank) ||
			false == rankManager->CheckPrevRankCondition(_focusRankSlotData))
		{
			// 진행 요건 불충족
			FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Popup_Quest_NotEnough"), stateText);
			break;
		}
		// 임무 수락 대기 중
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Popup_Quest_Ready"), stateText);
		
	}	break;
	case MultiLevelRankSlotState::IN_PROGRESS:
	{	// 임무 진행 중
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Popup_Quest_InProgress"), stateText);
	}	break;
	case MultiLevelRankSlotState::FINISH:
	case MultiLevelRankSlotState::ACTIVE:
	{
		// 임무 완료
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Popup_Quest_Completed"), stateText);
	}	break;
	}
	_textBlockState->SetText(stateText);

	// invaildateAll
	_slotHelperQuest->RefreshAll(_refreshQuestBuffer.Num());
	_isInitialize = false;
}

void UGsUIPopupMultiLevelRankQuest::InvalidateRewardInternal()
{
	if (nullptr == _focusRankSlotData ||
		nullptr == _focusRankSlotData->_ownerRank ||
		nullptr == _focusRankSlotData->_ownerRank->_rankSchema)
	{
#if  WITH_EDITOR
		GSLOG(Warning, TEXT("UGsUIPopupMultiLevelRankQuest::InvalidateRewardInternal : nullptr == _focusRankSlotData"));
#endif	//WITH_EDITOR	
		return;
	}

	const FGsSchemaMultiLevelRank* rankData = _focusRankSlotData->_ownerRank->_rankSchema;
	switch (rankData->rewardType)
	{
	case EGsMultiLevelRewardType::COSTUME:
	case EGsMultiLevelRewardType::FAIRY:
	{
		_switcherReward->SetActiveWidgetIndex(0);

		// Get Gender
		CreatureGenderType genderType = CreatureGenderType::MALE;
		if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (FGsGameObjectDataLocalPlayer* localData = player->GetCastData<FGsGameObjectDataLocalPlayer>())
			{
				genderType = localData->GetCreatureGenderType();
			}
		}

		// Set Image
		if (genderType == CreatureGenderType::MALE)
		{
			AsyncLoadSprite(_imageReward, rankData->manRewardLongImage);
		}
		else
		{
			AsyncLoadSprite(_imageReward, rankData->womanRewardLongImage);
		}

		// Set Text
		_textBlockRewardDesc->SetText(rankData->rewardPopupDesc);
		break;
	}
	case EGsMultiLevelRewardType::STAT:
	{
		_switcherReward->SetActiveWidgetIndex(1);

		// 이 슬롯의 보상 스탯 구하기
		FGsEffectStringHelper::FGsEffectAddData outStatList;
		for (const FGsSchemaPassivitySetRow& SetRow : _focusRankSlotData->_slotSchema->passivityIdList)
		{
			FGsEffectStringHelper::AddPassivityEffect(SetRow.GetRow(), outStatList);
		}

		// 현재 스탯에 대한 정보 저장
		_refreshStatBuffer.Empty();
		for (const auto& data : outStatList._statMap)
		{
			if (data.Value != 0)
			{
				_refreshStatBuffer.Emplace(data.Key, data.Value);
			}
		}

		_slotHelperRewardStat->RefreshAll(_refreshStatBuffer.Num());
		break;
	}
	case EGsMultiLevelRewardType::ITEM:
	{
		_switcherReward->SetActiveWidgetIndex(2);

		_refreshItemBuffer.Empty();

		const FGsSchemaRewardData* rewardData = rankData->rewardId.GetRow();
		if (nullptr == rewardData)
		{
#if  WITH_EDITOR
			GSLOG(Warning, TEXT("UGsUIPopupMultiLevelRankQuest::InvalidateRewardInternal : Invaild Rank Reward Data"));
#endif	//WITH_EDITOR		
			return;
		}

		/**
		 * 2021/06/30 PKT - 보상 관련 규칙 변경..."struct RefreshRewardBuffer" 주석 참조.
		 */

		bool isInputCurrency = false;
		for (uint8 i = 0; i < rewardData->rewardBoxList.Num(); ++i)
		{
			const FGsSchemaRewardBox box = rewardData->rewardBoxList[i];
			const FGsSchemaRewardBoxData* findData = box.rewardBoxDataRow.GetRow();
			if (nullptr == findData)
			{
#if  WITH_EDITOR
				GSLOG(Warning, TEXT("UGsUIPopupMultiLevelRankQuest::InvalidateRewardInternal : not find RewardBax Data"));
#endif	//WITH_EDITOR		
				continue;
			}

			for (const FGsSchemaRewardItemBagData& bagData : findData->itemBagList)
			{
				if (CurrencyType::NONE == bagData.currencyType)
				{	// 2021/06/30 PKT - Item
					FGsRefreshRewardBuffer buffer;
					buffer._type = FGsRefreshRewardBuffer::Type::Item;
					buffer._id = bagData.itemId;
					buffer._minCount = bagData.itemCountMin;
					buffer._maxCount = bagData.itemCountMax;

					// 2021/06/30 PKT - Add..
					_refreshItemBuffer.Emplace(buffer);
				}
				else
				{	// 2021/06/30 PKT - Currency
					FGsRefreshRewardBuffer buffer;
					buffer._type = FGsRefreshRewardBuffer::Type::Currency;
					buffer._id = static_cast<int32>(bagData.currencyType);
					buffer._minCount = bagData.currencyAmountMin;
					buffer._maxCount = bagData.currencyAmountMax;

					// 2021/06/30 PKT - Add..
					_refreshItemBuffer.Emplace(buffer);

					// 2021/06/30 PKT - "FGsSchemaRewardItemBagData"에 재화 정보가 들어 있다면 보상 정보(rewardData)에는 재화 정보가 없어야 한다.
					isInputCurrency = true;
				}
			}
		}

		if (CurrencyType::NONE != rewardData->currencyType)
		{
			if (false == isInputCurrency)
			{
				FGsRefreshRewardBuffer buffer;
				buffer._type = FGsRefreshRewardBuffer::Type::Currency;
				buffer._id = static_cast<int32>(rewardData->currencyType);
				buffer._minCount = rewardData->currencyAmountMin;
				buffer._maxCount = rewardData->currencyAmountMax;

				// 2021/06/30 PKT - Add..
				_refreshItemBuffer.Emplace(buffer);
			}
			else
			{
#if WITH_EDITOR
				// 2021/06/30 PKT - 경고 정도 하나 띄어주자.
				GSLOG(Warning, TEXT("UGsUIPopupMultiLevelRankQuest::InvalidateRewardInternal : Duplicate Reward Warning"));
#endif
			}
		}

		_slotHelperRewardItem->RefreshAll(_refreshItemBuffer.Num());
		break;
	}
	}
}

void UGsUIPopupMultiLevelRankQuest::InvalidateButtonInternal()
{
	FGsMultiLevelRankManager* rankManager = GSMultiLevelRank();
	if (nullptr == rankManager)
		return;
	
	if (nullptr == _focusRankSlotData)
		return;

	// 이전, 다음 슬롯 정보 유무 확인
	_leftRankSlotData = _focusRankSlotData->_linkedPrevSlot;
	_rightRankSlotData = nullptr;
	
	int32 nextIndex = rankManager->GetIndexByRankData(_focusRankSlotData->_ownerRank) + 1;
	if (const FGsMultiLevelRankData* nextRankData = rankManager->GetRankByIndex(nextIndex))
	{
		if (1 < nextRankData->_slotList.Num())
		{
			if (_pledgeGroupId >= nextRankData->_pledgeGroupId)
			{
				for (uint8 index = 0;index < nextRankData->_slotList.Num();++index)
				{
					//if (nextRankData->_slotList[index]._slotSchema->groupId == rankManager->GetSelectedPledgeId())
					// 다중 서약 구현할 땐 같은 PledgeGroupId 일땐 _rankGroupId 사용하고 작으면 매니저에서 선택한 서약정보 사용해야함
					if (nextRankData->_slotList[index]._slotSchema->groupId == _rankGroupId)
					{
						_rightRankSlotData = &(nextRankData->_slotList[index]);
						break;
					}
				}
			}
		}
		else
		{
			_rightRankSlotData = &(nextRankData->_slotList[0]);
		}
	}

	// 이전, 다음 슬롯 정보에 따라 버튼 활성/비활성 결정
	ESlateVisibility leftVisibility = (_leftRankSlotData == nullptr) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	ESlateVisibility rightVisibility = (_rightRankSlotData == nullptr) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;

	_btnLeft->SetVisibility(leftVisibility);
	_btnRight->SetVisibility(rightVisibility);
}

void UGsUIPopupMultiLevelRankQuest::AsyncLoadSprite(UImage* InImage, const FSoftObjectPath& InImagePath)
{
	InImage->SetVisibility(ESlateVisibility::Hidden);
	TWeakObjectPtr<UImage> weakThis(InImage);
	FGsUIHelper::AsyncLoadSpriteFromPath(InImagePath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			if (UImage* strongThis = weakThis.Get())
			{
				strongThis->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
			}
		}));
}


void UGsUIPopupMultiLevelRankQuest::OnInputCancel()
{
	OnClickClose();
}