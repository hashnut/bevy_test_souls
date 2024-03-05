#include "GsUIMessageHandlerGlobal.h"
#include "T1Project.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "UI/UILib/Manager/GsLoadingScreenController.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/ScaleRule/GsUIScaleRule.h"
#if WITH_EDITOR
#include "Kismet/GameplayStatics.h"
#include "Trigger/BaseTriggerActor.h"
#endif


FGsUIMessageHandlerGlobal::~FGsUIMessageHandlerGlobal()
{
}


void FGsUIMessageHandlerGlobal::InitializeMessage()
{
	// 패킷 바인딩
	
	
	MUI& ui = GMessage()->GetUI();

	InitializeMessageDelegateList(&ui);

	AddMessageDelegate(ui.AddRaw(MessageUI::PLAY_MOVIE,
		this, &FGsUIMessageHandlerGlobal::OnMoviePlay));
	AddMessageDelegate(ui.AddRaw(MessageUI::STOP_MOVIE,
		this, &FGsUIMessageHandlerGlobal::OnMovieStop));

	AddMessageDelegate(ui.AddRaw(MessageUI::HIDE_UI_FOR_MOVIE,
		this, &FGsUIMessageHandlerGlobal::OnHideUIForMovie));

	AddMessageDelegate(ui.AddRaw(MessageUI::OPTION_SELECT_LANGUAGE,
		this, &FGsUIMessageHandlerGlobal::OnSelectLanguage));

	AddMessageDelegate(ui.AddRaw(MessageUI::PREV_LOAD_MAP,
		this, &FGsUIMessageHandlerGlobal::OnPrevLoadMap));
	AddMessageDelegate(ui.AddRaw(MessageUI::POST_LOAD_MAP,
		this, &FGsUIMessageHandlerGlobal::OnPostLoadMap));

	AddMessageDelegate(ui.AddRaw(MessageUI::INVALIDATE_UI_SCALE,
		this, &FGsUIMessageHandlerGlobal::OnInvalidateUIScale));
}

void FGsUIMessageHandlerGlobal::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsUIMessageHandlerGlobal::OnMoviePlay(const IGsMessageParam*)
{
	if (UGsGameObjectManager* objManager = GSGameObject())
	{
		if (UGsGameObjectBase* local = objManager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			local->CastGameObject<UGsGameObjectLocalPlayer>()->ChangeFreezeState();
		}
	}
}

void FGsUIMessageHandlerGlobal::OnMovieStop(const IGsMessageParam*)
{
	if (UGsGameObjectManager* objManager = GSGameObject())
	{
		if (UGsGameObjectBase* local = objManager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (FGsGameObjectStateManager* fsm = local->GetBaseFSM())
			{
				fsm->ProcessEvent(EGsStateBase::UnFreeze);
			}
		}
	}
}

void FGsUIMessageHandlerGlobal::OnHideUIForMovie(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamBool* param = InParam->Cast<const FGsUIMsgParamBool>();
	if (nullptr == param)
	{
		return;
	}

	bool bIsVisible = param->_data;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return;
	}

	uiManager->SetVisibility(bIsVisible);

#if WITH_EDITOR
	TArray<AActor*> triggers;
	UGameplayStatics::GetAllActorsOfClass(uiManager->GetWorld(), ABaseTriggerActor::StaticClass(), triggers);

	for (AActor* temp : triggers)
	{
		if (temp)
		{
			if (ABaseTriggerActor* trigger = Cast<ABaseTriggerActor>(temp))
			{
				trigger->SetVisible(bIsVisible);
			}
		}
	}
#endif
}

void FGsUIMessageHandlerGlobal::OnSelectLanguage(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamLanguege* param = InParam->Cast<const FGsUIMsgParamLanguege>();
	if (nullptr == param)
	{
		return;
	}

	int32 langIndex = param->_index;
	bool bIsVoice = param->_bIsVoice;
	
	FText textMsg;
	if (bIsVoice)
	{
		// TEXT: 목소리 변경 시 게임이 재실행됩니다.
		FText::FindText(TEXT("OptionText"), TEXT("ChangeLanguageVoiceDesc"), textMsg);
	}
	else
	{
		// TEXT: 언어 변경 시 게임이 재실행됩니다.
		FText::FindText(TEXT("OptionText"), TEXT("ChangeLanguageDesc"), textMsg);
	}
	
	FGsUIHelper::PopupSystemYesNo(textMsg, [langIndex, bIsVoice](bool bIsOk)
		{
			if (bIsOk)
			{
				GLocalization()->OnSelectLanguage(langIndex, bIsVoice);
			}
			else
			{
				GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
			}
		});
}

void FGsUIMessageHandlerGlobal::OnPrevLoadMap(const IGsMessageParam*)
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsLoadingScreenController* loadingController = uiMgr->GetLoadingController())
		{
			loadingController->OnPrevLoadMap();
		}
	}
}

void FGsUIMessageHandlerGlobal::OnPostLoadMap(const IGsMessageParam*)
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsLoadingScreenController* loadingController = uiMgr->GetLoadingController())
		{
			loadingController->OnPostLoadMap();
		}
	}
}

void FGsUIMessageHandlerGlobal::OnInvalidateUIScale(const IGsMessageParam* InParam)
{
	float uiScaleValue = 1.f;

	if (const FGsUIMsgParamBool* param = InParam->Cast<const FGsUIMsgParamBool>())
	{
		if (param->_data)
		{
			if (UGsGameUserSettings* settings = GGameUserSettings())
			{
				uiScaleValue = settings->GetNormalizedUIScaleValue();
			}
		}
	}

	UGsUIScaleRule::SetUIScaleValue(uiScaleValue);
}
