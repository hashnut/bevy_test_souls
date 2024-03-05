// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIHUDSocialEmojiButton.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"

//#include "GameObject/ObjectClass/GsGameObjectBase.h"
//#include "GameObject/Skill/GsSkillHandlerBase.h"
//#include "GameObject/Skill/IdleEmotion/GsIdleEmotionLocalRunner.h"

#include "UI/UIContent/Popup/GsUIPopupSocialMotion.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

void UGsUIHUDSocialEmojiButton::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIHUDSocialEmojiButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnEmoticon->OnClicked.AddDynamic(this, &UGsUIHUDSocialEmojiButton::OnClickOpenPopupEmoticon);
}

void UGsUIHUDSocialEmojiButton::NativeConstruct()
{
	Super::NativeConstruct();

	//_isEnableSocialMotion = true;

	//_messageHandlers.Add(GMessage()->GetUI().AddUObject(MessageUI::SOCIALMOTION_ENABLE_UPDATE,
	//	this, &UGsUIHUDSocialEmojiButton::OnMessageEnableMotion));
}

void UGsUIHUDSocialEmojiButton::NativeDestruct()
{
	//_isEnableSocialMotion = false;

	/*for(auto& handler : _messageHandlers)
		GMessage()->GetUI().Remove(handler);

	_messageHandlers.Reset();*/
	Super::NativeDestruct();
}

void UGsUIHUDSocialEmojiButton::OnClickOpenPopupEmoticon()
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (auto popup = uiManager->GetWidgetByKey(TEXT("PopupSocialMotion")).Get())
		{
			popup->Close();
		}
		else
		{
			uiManager->OpenAndGetWidget(TEXT("PopupSocialMotion"));
		}
	}
}

//void UGsUIHUDSocialEmojiButton::OnMessageEnableMotion(const IGsMessageParam* param)
//{
//	if (_isEnableSocialMotion != IsEnableSocialMotion())
//	{
//		_isEnableSocialMotion = !_isEnableSocialMotion;
//		if (auto popup = Cast<UGsUIPopupSocialMotion>(GUI()->GetWidgetByKey(TEXT("PopupSocialMotion")).Get()))
//		{
//			popup->SetEnableSocialMotion(_isEnableSocialMotion);
//		}
//	}
//}

//bool UGsUIHUDSocialEmojiButton::IsEnableSocialMotion()
//{
//	static auto GetEmotionRunner = []()->TWeakPtr<FGsIdleEmotionLocalRunner> {
//		if (auto localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
//		{
//			if (localPlayer->IsPeacefulIdle())
//			{
//				if (auto handler = localPlayer->GetSkillHandler())
//				{
//					return StaticCastSharedPtr<FGsIdleEmotionLocalRunner>(handler->GetIdleEmotionRunner().Pin());
//				}
//			}
//		}
//		return nullptr;
//	};
//
//	if (auto emotionRunner = GetEmotionRunner().Pin())
//	{
//		return emotionRunner->IsEnableEmotionMotion();
//	}
//
//	return false;
//}

