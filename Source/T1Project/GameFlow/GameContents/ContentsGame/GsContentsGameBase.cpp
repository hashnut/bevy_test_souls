#include "GsContentsGameBase.h"

#include "DataSchema/GsSchemaEnums.h"

#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "Sound/GsSoundMixerController.h"


void FGsContentsGameBase::Enter()
{
	if (IsChangeSoundMixer())
	{
		if (UGsSoundManager* soundManager = GSound())
		{
			if (UGsSoundMixerController* mixerController = soundManager->GetOrCreateSoundMixerController())
			{
				mixerController->OnMixChanged(EGsSoundMixType::Menu);
			}
		}
	}

	OnResetUI(GUI()->OnUIManagerResetUI);
}

void FGsContentsGameBase::Exit()
{
	if (IsChangeSoundMixer())
	{
		if (UGsSoundManager* soundManager = GSound())
		{
			if (UGsSoundMixerController* mixerController = soundManager->GetOrCreateSoundMixerController())
			{
				mixerController->OffMixMode(EGsSoundMixType::Menu);
			}
		}
	}
	GUI()->OnUIManagerResetUI.Clear();
}

void FGsContentsGameBase::SetNavigationBar(CurrencyType InType)
{
	UGsUIManager* uiMgr = GUI();
	FName nameNav(TEXT("TrayNavigationBar"));
	if (false == uiMgr->IsActiveWidget(nameNav))
	{
		uiMgr->Open(nameNav);
	}

	FGsUIMsgParamInt param(static_cast<int32>(InType));
	GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_CURRENCY, &param);
}

void FGsContentsGameBase::SetNavigationBarItemId(ItemId InItemId)
{
	UGsUIManager* uiMgr = GUI();
	FName nameNav(TEXT("TrayNavigationBar"));
	if (false == uiMgr->IsActiveWidget(nameNav))
	{
		uiMgr->Open(nameNav);
	}

	FGsUIMsgParamInt param(static_cast<int32>(InItemId));
	GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_ITEM, &param);
}

void FGsContentsGameBase::CloseNavigationBar()
{
	GUI()->CloseByKeyName(TEXT("TrayNavigationBar"));
}

void FGsContentsGameBase::OpenNavigationBar()
{
	UGsUIManager* uiMgr = GUI();
	FName nameNav(TEXT("TrayNavigationBar"));
	if (false == uiMgr->IsActiveWidget(nameNav))
	{
		uiMgr->Open(nameNav);
	}
}