// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupQuestAcceptRepeat.h"
#include "GsUIPopupQuestBase.h"

#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendServiceQuest.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"

#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemConsumable.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/GsUICurrencyRefreshCountButton.h"

#include "Image.h"
#include "TextBlock.h"
#include "PaperSprite.h"
#include "Reward/GsRewardHelper.h"

#include "Quest/Repeat/GsSchemaQuestRepeatScroll.h"
#include "Quest/Repeat/GsSchemaQuestRepeatScrollRefreshInfoSet.h"
#include "UTIL/GsTableUtil.h"

#include "Engine/AssetManager.h"


void UGsUIPopupQuestAcceptRepeat::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnRefresh->OnClickRefreshButton.BindUObject(this, &UGsUIPopupQuestAcceptRepeat::OnClickRefresh);
}

void UGsUIPopupQuestAcceptRepeat::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();
}

void UGsUIPopupQuestAcceptRepeat::NativeDestruct()
{
	UnbindMessages();

	Super::NativeDestruct();
}

void UGsUIPopupQuestAcceptRepeat::SetData(const IGsMessageParam* InParam)
{
	const FGsQuestRepeatPopUpParam* paramTarget = InParam->Cast<const FGsQuestRepeatPopUpParam>();
	if (nullptr != paramTarget)
	{
		// QuestID
		_storyId = paramTarget->_storyId;
		_questId = paramTarget->_questId;
		_questIndex = paramTarget->_questIndex;
		_questType = QuestType::REPEAT;

		// 텍스트 세팅;
		_titleText = paramTarget->_titleText;
		_descText = paramTarget->_descriptionText;

		// 보상 세팅
		_rewardMainId = paramTarget->_mainRewardId;
		_rewardSubId = paramTarget->_subRewardId;

		_textTitle->SetColorAndOpacity(GSQuest()->GetTextColor(QuestType::REPEAT, _storyId));

		if (paramTarget->_iconPath.IsValid())
		{
			if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(paramTarget->_iconPath))
			{
				if (UPaperSprite* sprite = Cast<UPaperSprite>(loadObject))
				{
					FSlateAtlasData atlasData = sprite->GetSlateAtlasData();
					FVector2D size = atlasData.GetSourceDimensions();

					_iconImg->SetBrushFromAtlasInterface(sprite);
					_iconImg->SetBrushSize(size);
				}
			}
		}

		// 새로고침 버튼 처리
		_btnRefresh->SetVisibility(paramTarget->_isRefreshReward ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (paramTarget->_isRefreshReward)
		{
			SetRefreshData(_questId, _questIndex);
		}

		// 보상 세팅
		bool bIsRequestRepeatReward = false;
		 if (RewardType::PICK_REWARD_BOX_LIST == FGsRewardHelper::GetRewardType(_rewardMainId) ||
			 RewardType::PICK_REWARD_BOX_LIST == FGsRewardHelper::GetRewardType(_rewardSubId))
		{
			 bIsRequestRepeatReward = SendRequestRepeatRewardBox();
		}

		if (false == bIsRequestRepeatReward)
		{
			SetRewardData(_rewardMainId, _rewardSubId);
		}

		SetQuestTypeText();
	}
}

void UGsUIPopupQuestAcceptRepeat::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	_msgGameObjectHandleList.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::QUEST_REPEAT_REWARD_BOX_RECEIVE,
		this, &UGsUIPopupQuestAcceptRepeat::OnReceiveRepeatRewardBox));
}

void UGsUIPopupQuestAcceptRepeat::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MGameObject& msgGameObject = msgMgr->GetGameObject();
	for (MsgGameObjHandle& handle : _msgGameObjectHandleList)
	{
		msgGameObject.Remove(handle);
	}
	_msgGameObjectHandleList.Empty();
}

bool UGsUIPopupQuestAcceptRepeat::SendRequestRepeatRewardBox()
{
	if (false == IsTest())
	{
		TArray<RewardBoxItemIdPair> boxIdPairList;
		if (false == FGsRewardHelper::GetRewardBoxItemIdPairList(_questId, _questIndex, boxIdPairList))
		{
			// 주의: Accept 시에는 해당 리스트가 없는 경우가 없어야 함
			//FGsNetSendServiceQuest::SendReqQuestRepeatRewardBox(_questIndex);
			return true;
		}
	}

	return false;
}

void UGsUIPopupQuestAcceptRepeat::SetRefreshData(QuestId inQuestId, QuestIndex inQuestIndex)
{
	TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(inQuestId);
	if (false == questData.IsValid())
	{
		return;
	}

	if (FGsQuestRepeatData* repeatData = static_cast<FGsQuestRepeatData*>(questData.Get()))
	{
		if (const FGsSchemaQuestRepeatScrollRefreshInfoSet* refreshInfoSet = 
			GetRefrehTable(repeatData->GetRepeatScrollId(inQuestIndex)))
		{
			uint8 refreshCount = repeatData->GetRefreshCount(inQuestIndex);
			int32 remainCount = refreshInfoSet->currencyInfoList.Num() - refreshCount;

			if (refreshInfoSet->currencyInfoList.IsValidIndex(refreshCount))
			{
				const FGsSchemaQuestRepeatScrollRefreshInfo& refreshInfo = refreshInfoSet->currencyInfoList[refreshCount];
				_btnRefresh->SetData(remainCount, refreshInfo.costCurrencyType, refreshInfo.costCurrencyAmount);
				
				return;
			}
		}
	}

	// 갱신횟수 모두 소모 or 데이터 설정 실패 시 비활성화
	_btnRefresh->SetData(0, CurrencyType::NONE, 0);
}

void UGsUIPopupQuestAcceptRepeat::OnReceiveRepeatRewardBox(const IGsMessageParam* InParam)
{	
	// 랜덤 보상 목록 수령 후 보상 목록 업데이트
	if (const FGsQuestRepeatPopUpParam* param = InParam->Cast<const FGsQuestRepeatPopUpParam>())
	{
		if (param->_questIndex == _questIndex)
		{
			SetData(param);
			SetRewardData(_rewardMainId, _rewardSubId);
		}
	}
}

void UGsUIPopupQuestAcceptRepeat::OnClickRefresh()
{
	if (INVALID_QUEST_ID == _questId)
	{
		return;
	}

	FGsQuestParam param(_questId, _questIndex);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_REPEAT_REWARD_REFRESH, &param);
}

const FGsSchemaQuestRepeatScrollRefreshInfoSet* UGsUIPopupQuestAcceptRepeat::GetRefrehTable(int32 inRepeatScrollId)
{
	if (const FGsSchemaQuestRepeatScroll* row =
		UGsTableUtil::FindRowById<UGsTableQuestRepeatScroll, FGsSchemaQuestRepeatScroll>(inRepeatScrollId))
	{
		return row->refreshInfoSet.GetRow();
	}

	return nullptr;
}
