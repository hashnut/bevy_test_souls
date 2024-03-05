#include "GsUIHUDScanButton.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Input/GsInputBindingLocalPlayer.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/HUD/Scan/GsUIScanDragObject.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Scan/GsScanHandler.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/Engine/Player.h"
#include "Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Classes/Engine/GameViewportClient.h"
#include "Engine/Public/Slate/SceneViewport.h"

#include "Engine.h"

#include "UMG/Public/Components/ProgressBar.h"

// 최초 한번 호출
void UGsUIHUDScanButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnScan->OnClicked.AddDynamic(this, &UGsUIHUDScanButton::OnClickScan);

	_widgetDrag->CallbackScanStart = [this](bool In_isDrag)
	{
		StartScanByUI(In_isDrag);
	};

	_widgetDrag->CallbackTouchStart = [this]()
	{
		OnTouchStart();
	};

	_widgetDrag->CallbackTouchEnd = [this]()
	{
		OnTouchEnd();
	};
}

void UGsUIHUDScanButton::NativeConstruct()
{
	Super::NativeConstruct();

	FGsMessageHolder* msg = GMessage();
	MUI& msgUI = msg->GetUI();
	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::AUTO_SCAN_ACTIVE,
		this, &UGsUIHUDScanButton::OnAutoScanActive));

	// 프로그래스 바  초기화
	_progressBarCoolTime->SetPercent(0.0f);

	SetIsDragOn(false);
}


// 스캔 클릭
void UGsUIHUDScanButton::OnClickScan()
{
	StartScanByUI();
}

// 액티브 auto 표시 처리
void UGsUIHUDScanButton::SetActiveAuto(bool In_isActive)
{
	ESlateVisibility visibility = (In_isActive == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_panelOnEffect->SetVisibility(visibility);
}

void UGsUIHUDScanButton::OnAutoScanActive(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamBool* param = InParam->Cast<const FGsUIMsgParamBool>();
	if (param)
	{
		SetActiveAuto(param->_data);
	}
}

void UGsUIHUDScanButton::NativeDestruct()
{
	if(FGsMessageHolder* message = GMessage())
	{ 
		for (MsgUIHandle& msgHandler : _msgUIHandlerList)
		{
			message->GetUI().Remove(msgHandler);
		}
	}
	_msgUIHandlerList.Empty();

	_bIsTutorialInteraction = false;
	OnTutorialInteraction.Unbind();

	Super::NativeDestruct();
}

// 스캔 쿨타임 rate
float UGsUIHUDScanButton::GetScanCoolTimeRate()
{
	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _coolStartTime;

	static float maxTimeTick =
		GData()->GetGlobalData()->_scanClickCoolTime * ETimespan::TicksPerSecond;

	float rate = diffTick / maxTimeTick;
	return rate;
}

// 스캔 쿨타임 시작
void UGsUIHUDScanButton::StartScanCoolTime()
{
	_coolStartTime = FDateTime::UtcNow().GetTicks();
	_isCoolTimeOn = true;
}

bool UGsUIHUDScanButton::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	if (_isCoolTimeOn == true)
	{
		float rate = GetScanCoolTimeRate();
		_progressBarCoolTime->SetPercent(rate);

		// 쿨 끝이다
		if (rate >= 1.0f)
		{
			_progressBarCoolTime->SetPercent(0.0f);
			_isCoolTimeOn = false;
		}
	}

	return true;
}

void UGsUIHUDScanButton::SetIsDragOn(bool In_isOn)
{
	_widgetDrag->SetIsDragOn(In_isOn);
}

void UGsUIHUDScanButton::StartScanByUI(bool In_isByDrag)
{
	if (_isCoolTimeOn == true)
	{
		FText findText;
		FText::FindText(TEXT("ScanText"), TEXT("CoolTime"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	StartScanCoolTime();

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsScanHandler* scanHandler = hud->GetScanHandler();
	if (scanHandler == nullptr)
	{
		return;
	}
	if (In_isByDrag == false)
	{
		scanHandler->StartScan(true);
	}
	else
	{	
		UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		if (player == nullptr)
		{
			return;
		}
		UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>();

		if (local == nullptr)
		{
			return;
		}
		UGsInputBindingLocalPlayer* inputBinder = local->GetInputBinder();

		if (nullptr == inputBinder)
		{
			return;
		}
		FVector2D touchPos = _widgetDrag->GetCurrentTouchPos();

		FGeometry ViewportGeo = GEngine->GameViewport->GetGameViewport()->GetCachedGeometry();
		FVector2D viewportAbsolutePos = ViewportGeo.GetAbsolutePosition();
		FVector2D screenPos = touchPos - viewportAbsolutePos;

		FVector scanCenterPosLand;
		if (false == inputBinder->GetTerrainPosByScreenPos(screenPos, scanCenterPosLand))
		{
			return;
		}

		scanHandler->StartScan(true, false, scanCenterPosLand);
	}

	GMessage()->GetUI().SendMessage(MessageUI::SCAN_CLICK, nullptr);

	if (IsTutorialInteraction())
	{
		OnTutorialInteraction.ExecuteIfBound();
	}
}

void UGsUIHUDScanButton::SetTestDragDiff(float In_val)
{
	_widgetDrag->SetTestDragDiff(In_val);
}

void UGsUIHUDScanButton::OnHide()
{
	SetIsDragOn(false);
}

void UGsUIHUDScanButton::OnTouchStart()
{
	FVector2D calcPos(0.0f, _touchMoveZ);
	_panelButton->SetRenderTranslation(calcPos);

	if (_clickVFXPath.IsValid())
	{
		UClass* vfxClass = GPath()->GetClassByObjectPath(_clickVFXPath);
		if (vfxClass)
		{
			UGsBlueprintFunctionLibraryUI::AddVFXToPointerPos(vfxClass, _clickVFXZOrder);
		}
	}

}
void UGsUIHUDScanButton::OnTouchEnd()
{
	_panelButton->SetRenderTranslation(FVector2D::ZeroVector);
}