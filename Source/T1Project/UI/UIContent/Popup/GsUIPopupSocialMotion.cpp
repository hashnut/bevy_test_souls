// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupSocialMotion.h"

#include "Shared/Client/SharedEnums/SharedChatEnum.h"
#include "DataSchema/SocialEmotion/GsSchemaEmoji.h"
#include "DataSchema/SocialEmotion/GsSchemaSocialEmotion.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/IdleEmotion/GsIdleEmotionLocalRunner.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "UI/UIContent/Popup/Chat/GsUIEmojiChatEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/HUD/Chat/GsUISocialMotionEntry.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"

#include "Net/GsNetSendService.h"
#include "Net/GsNetSendServiceWorld.h"

#include "ForEach.h"
#include "Count.h"

void UGsUIPopupSocialMotion::BeginDestroy()
{
	_motionWidgets.Reset();
	_emojiWidgets.Reset();

	Super::BeginDestroy();
}

void UGsUIPopupSocialMotion::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	DataInitialize();

	_toggleGroup.AddToggleWidget(_tabBtnEmoji);
	_toggleGroup.AddToggleWidget(_tabBtnMotion);

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupSocialMotion::OnTabChanged);
}

void UGsUIPopupSocialMotion::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_selectedIndex == -1)
	_toggleGroup.SetSelectedIndex(_toggleGroup.GetSelectedIndex(), true);
}

void UGsUIPopupSocialMotion::NativeConstruct()
{
	Super::NativeConstruct();

	_isEnableMotion = true;
	_messageHandlers.Add(GMessage()->GetUI().AddUObject(MessageUI::SOCIALMOTION_STATE_UPDATE,
		this, &UGsUIPopupSocialMotion::OnMessageUpdateUI));
	_messageHandlers.Add(GMessage()->GetUI().AddUObject(MessageUI::SOCIALMOTION_ENABLE_UPDATE,
		this, &UGsUIPopupSocialMotion::OnMessageEnableMotion));

	if (GSChat()->IsResetMotion())
	{
		DataInitialize();
	}
}

void UGsUIPopupSocialMotion::NativeDestruct()
{
	_isEnableMotion = false;
	for (auto& handler : _messageHandlers)
		GMessage()->GetUI().Remove(handler);

	_messageHandlers.Reset();

	Super::NativeDestruct();
}

void UGsUIPopupSocialMotion::OnClickClose()
{
	Close();
}

void UGsUIPopupSocialMotion::OnTabChanged(int32 index)
{
	if (index != _selectedIndex)
	{
		_selectedIndex = index;
		_switcherTab->SetActiveWidgetIndex(index);
	}
}

void UGsUIPopupSocialMotion::OnSelectedEmoji(int32 emojiId)
{
	FString emojiTxt = FString::FromInt(emojiId);
	FGsNetSendService::SendReqChat(ChatType::EMOTICON, emojiTxt);

	Close();
}

void UGsUIPopupSocialMotion::OnSelectedSocialMotion(TWeakPtr<FSocialEmotionMotion> data)
{
	static auto IsEnableEmotion = []()->bool {
		if (auto localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (localPlayer->IsPeacefulIdle())
			{
				if (auto handler = localPlayer->GetSkillHandler())
				{
					if (auto emotionRunner = StaticCastSharedPtr<FGsIdleEmotionLocalRunner>(handler->GetIdleEmotionRunner().Pin()))
					{
						return emotionRunner->IsEnableEmotionMotion();
					}
				}
			}
		}
		return false;
	};

	if (!data.IsValid()) return;

	if (IsEnableEmotion())
	{
		//GSChat()->SetSocialEmotion(GGameData()->GetUserData()->mGameId, data.Pin()->GetMotionId()); //test
		FGsNetSendServiceWorld::SendReqUserSocialEmotion(data.Pin()->GetSocialEmotionType());
	}
	else
	{
		FText SocialAction_Warning; //지금은 사용할 수 없는 소셜 액션 입니다
		FText::FindText(TEXT("ChatText"), TEXT("SocialAction_Warning"), SocialAction_Warning);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, SocialAction_Warning);
	}
}

void UGsUIPopupSocialMotion::DataInitialize()
{
	TArray<const FGsSchemaEmoji*> emoticonList;
	GSChat()->GetEmoticonList(emoticonList);

	TArray<TWeakPtr<FSocialEmotionMotion>> motionList;
	GSChat()->GetSocialMotionList(motionList);

	TArray<UWidget*> childList = _emojiPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIEmojiChatEntry>())
		{
			child->RemoveFromParent();
		}
	};

	childList = _motionPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUISocialMotionEntry>())
		{
			child->RemoveFromParent();
		}
	};

	int32 useIndex = 0;

	_emojiWidgets.Reset();
	for (const auto e : emoticonList)
	{
		int index = _emojiWidgets.Add(Cast<UGsUIEmojiChatEntry>(CreateWidget(GetWorld(), _entryEmojiWidget)));
		TWeakObjectPtr<UGsUIEmojiChatEntry> emoticon = _emojiWidgets[index];

		emoticon->SetEmojiID(e->socialId, e->emoticon);
		emoticon->OnClickSlot.BindUObject(this, &UGsUIPopupSocialMotion::OnSelectedEmoji);
		_emojiPanel->AddChildToUniformGrid(emoticon.Get(), useIndex / EmojiColumnCount, useIndex % EmojiColumnCount);

		useIndex++;
	}

	_emojiPanel->InvalidateLayoutAndVolatility();

	useIndex = 0;
	_motionWidgets.Reset();
	for (const auto e : motionList)
	{
		int index = _motionWidgets.Add(Cast<UGsUISocialMotionEntry>(CreateWidget(GetWorld(), _entryMotionWidget)));
		TWeakObjectPtr<UGsUISocialMotionEntry> motionEmoticon = _motionWidgets[index];

		motionEmoticon->SetMotionData(e);
		motionEmoticon->OnClickSlot.BindUObject(this, &UGsUIPopupSocialMotion::OnSelectedSocialMotion);
		_motionPanel->AddChildToUniformGrid(motionEmoticon.Get(), useIndex / MotionColumnCount, useIndex % MotionColumnCount);

		useIndex++;
	}

	_motionPanel->InvalidateLayoutAndVolatility();

	GSChat()->ResetMotionFlag(false);
}

void UGsUIPopupSocialMotion::OnMessageUpdateUI(const IGsMessageParam* param)
{	
	auto motionEntryList = _motionPanel->GetAllChildren();
	Algo::ForEach(motionEntryList, [](auto e) {
		if (auto entry = Cast<UGsUISocialMotionEntry>(e)) entry->RefreshUI(); });
}

void UGsUIPopupSocialMotion::OnMessageEnableMotion(const IGsMessageParam* param)
{
	
}

void UGsUIPopupSocialMotion::OnTouchedExtraRange()
{
	Close();
}
FReply UGsUIPopupSocialMotion::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	OnTouchedExtraRange();
	
	return FReply::Unhandled();
}

void UGsUIPopupSocialMotion::OnInputCancel()
{
	Close();
}