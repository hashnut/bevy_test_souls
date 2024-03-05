// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupQuestBase.h"

#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"
#include "Components/PanelWidget.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "DataSchema/Quest/Main/GsSchemaQuestMainStory.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "UMG/Public/Components/TextBlock.h"
#include "DataSchema/KeyMapping/GsSchemaKeyMapping.h"
#include "Option/GsOptionEnumType.h"
#include "Reward/GsRewardHelper.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "Input/GsInputEventMsgBase.h"

void UGsUIPopupQuestBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_rewardInfo->SetDetailPopup(true, false);
}

void UGsUIPopupQuestBase::NativeConstruct()
{
	Super::NativeConstruct();

	// [B1] | ejrrb10 | C2URWQ-5599 | 대화 연출 중 워프 스크롤/자동 전투/스킬 등 사용을 막는다
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_QUEST_POPUP, true);
	if (FGsMessageHolder* messageHolder = GMessage())
	{
		messageHolder->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}
}

void UGsUIPopupQuestBase::NativeDestruct()
{
	_rewardMainId = INVALID_REWARD_ID;
	_rewardSubId = INVALID_REWARD_ID;

	// [B1] | ejrrb10 | C2URWQ-5599 | 대화 연출 중 워프 스크롤/자동 전투/스킬 등의 블락을 해제한다
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_QUEST_POPUP, false);
	if (FGsMessageHolder* messageHolder = GMessage())
	{
		messageHolder->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}

	Super::NativeDestruct();
}

void UGsUIPopupQuestBase::CloseInternal()
{
	// 혹시 상세정보창이 떠있다면 함께 닫는다
	FGsItemDetailCloseMessageParam param;
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);

	Super::CloseInternal();
}

void UGsUIPopupQuestBase::OnClickOk()
{
	OnInputOk();
}

void UGsUIPopupQuestBase::OnClickCancel()
{
	OnInputCancel();
}

//void UGsUIPopupQuestBase::InputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent)
//{
//	switch (keyMapType)
//	{
//	case EGsKeyMappingType::KEY_MAPPING_NO:
//		OnInputCancel();
//		break;
//	default:
//		break;
//	}
//}

void UGsUIPopupQuestBase::SetData(const IGsMessageParam* InParam)
{
	const FGsQuestPopUpParam* paramTarget = InParam->Cast<const FGsQuestPopUpParam>();
	if (nullptr != paramTarget)
	{
		// QuestID
		_storyId = paramTarget->_storyId;
		_questId = paramTarget->_questId;
		_questIndex = paramTarget->_questIndex;
		_questType = paramTarget->_questType;
		_spawnId = paramTarget->_npcId;

		// 텍스트 세팅;
		_titleText = paramTarget->_titleText;
		_descText = paramTarget->_descriptionText;

		// 보상 세팅
		_rewardMainId = paramTarget->_mainRewardId;
		_rewardSubId = paramTarget->_subRewardId;

		_questContentsType = paramTarget->_questContentsType;
		
		SetQuestTypeText();
	}
}

void UGsUIPopupQuestBase::SetRewardData(RewardId InRewardId, RewardId InSubRewardId)
{
	if (RewardType::PICK_REWARD_BOX_LIST == FGsRewardHelper::GetRewardType(InRewardId) ||
		RewardType::PICK_REWARD_BOX_LIST == FGsRewardHelper::GetRewardType(InSubRewardId))
	{
		TArray<RewardBoxItemIdPair> boxIdPairList;
		FGsRewardHelper::GetRewardBoxItemIdPairList(_questId, _questIndex, boxIdPairList);

		_rewardInfo->SetData(InRewardId, InSubRewardId, boxIdPairList);
	}
	else
	{
		_rewardInfo->SetData(InRewardId, InSubRewardId);
	}
}

void UGsUIPopupQuestBase::SetQuestTypeText()
{
	FText findText;
	FText::FindText(TEXT("QuestUIText"), TEXT("QuestTitle"), findText);	

	switch (_questType)
	{
	case QuestType::MAIN:
		FText::FindText(TEXT("QuestUIText"), TEXT("MainText"), findText);
		break;
	case QuestType::SUB:
		{
			if (QuestContentsType::INVADE == _questContentsType)
			{
				FText::FindText(TEXT("QuestUIText"), TEXT("Sub_InvadeText"), findText);
			}
			else
			{
				FText::FindText(TEXT("QuestUIText"), TEXT("SubText"), findText);
			}
		}		
		break;
	case QuestType::REPEAT:
		FText::FindText(TEXT("QuestUIText"), TEXT("RepeatText"), findText);
		break;
	case QuestType::MULTI_LEVEL_RANK:
		break;
	case QuestType::GUILD:
		FText::FindText(TEXT("QuestUIText"), TEXT("GuildText"), findText);
		break;
	case QuestType::GUIDE:
		FText::FindText(TEXT("QuestUIText"), TEXT("GuideText"), findText);
		break;
	default:
		break;
	}

	if (_typeText)
	{
		_typeText->SetText(findText);
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("UGsUIPopupQuestBase::SetQuestTypeText() _typeText == nullptr"));
#endif
	}
}

void UGsUIPopupQuestBase::SetIsTest(bool bIsTest)
{
#if WITH_EDITOR
	_bIsTest = bIsTest;
#endif
}

bool UGsUIPopupQuestBase::IsTest() const
{
#if WITH_EDITOR
	return _bIsTest;
#else
	return false;
#endif
}