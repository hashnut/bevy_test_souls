// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIManager.h"

#include "TimerManager.h"
#include "GameFrameWork/PlayerController.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UILib/Container/GsWidgetPool.h"
#include "UI/UILib/Manager/GsUIController.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UILib/Manager/GsDealScrollManager.h"
#include "UI/UILib/Manager/GsEmoticonManager.h"
#include "UI/UILib/Manager/GsTooltipManager.h"
#include "UI/UIContent/Loading/GsUILoadingScreen.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "GSGameInstance.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsLobbyConstant.h"

#include "DataSchema/Movie/GsSchemaMovieData.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Data/GsLobbyConstant.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"

#include "Input/GsInputEventMsgBase.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "Data/GsLobbyConstant.h"

#include "T1Project.h"

#define USE_TOUCH_EFFECT

const FString DefaultButtonPressedSound = TEXT("/Game/Sound/UI/Snd_UI_Click.Snd_UI_Click");

void UGsUIManager::Initialize()
{
	if (nullptr == _uiController)
	{
		_uiController = NewObject<UGsUIController>(this);
	}

	if (nullptr == _widgetPoolManager)
	{
		_widgetPoolManager = NewObject<UGsWidgetPoolManager>(this);
		_widgetPoolManager->Initialize();
	}

	if (nullptr == _dealScrollManager)
	{
		_dealScrollManager = NewObject<UGsDealScrollManager>(this);
		_dealScrollManager->Initialize();
	}

	if (nullptr == _emoticonManager)
	{
		_emoticonManager = NewObject<UGsEmoticonManager>(this);
		_emoticonManager->Initialize();
	}

	if (nullptr == _tickerHandler)
	{
		_tickerHandler = MakeUnique<FGsUITickerHandler>();
		_tickerHandler->Initialize();
	}

	if (nullptr == _loadingController)
	{
		_loadingController = NewObject<UGsLoadingScreenController>(this);
		_loadingController->Initialize();
	}

	if (nullptr == _tooltipManager)
	{
		_tooltipManager = NewObject<UGsTooltipManager>(this);
		_tooltipManager->Initialize();
	}

	if (nullptr == _blockUIController)
	{
		_blockUIController = NewObject<UGsBlockUIController>(this);
		_blockUIController->Initialize();
	}

	_hideFlagStateMap.Emplace(EGsUIHideFlags::STATE_DIALOG, EGsUIHideFlags::SETTING_DIALOG);
	_hideFlagStateMap.Emplace(EGsUIHideFlags::STATE_CINEMATIC, EGsUIHideFlags::SETTING_CINEMATIC);
	_hideFlagStateMap.Emplace(EGsUIHideFlags::STATE_DIE, EGsUIHideFlags::SETTING_DIE);
	_hideFlagStateMap.Emplace(EGsUIHideFlags::STATE_HUD_CAMERA_MODE, EGsUIHideFlags::SETTING_HUD_CAMERA_MODE);
	_hideFlagStateMap.Emplace(EGsUIHideFlags::STATE_NPC_SHOP, EGsUIHideFlags::SETTING_NPC_SHOP);
	_hideFlagStateMap.Emplace(EGsUIHideFlags::STATE_SCENE_WINDOW, EGsUIHideFlags::SETTING_SCENE_WINDOW);
	_hideFlagStateMap.Emplace(EGsUIHideFlags::STATE_SUMMON, EGsUIHideFlags::SETTING_SUMMON);
	_hideFlagStateMap.Emplace(EGsUIHideFlags::STATE_OFFLINE_PLAY, EGsUIHideFlags::SETTING_OFFLINE_PLAY);
	_hideFlagStateMap.Emplace(EGsUIHideFlags::STATE_MOVE_EFFECT, EGsUIHideFlags::SETTING_MOVE_EFFECT);
	_hideFlagStateMap.Emplace(EGsUIHideFlags::STATE_SAVE_BATTERY, EGsUIHideFlags::SETTING_SAVE_BATTERY);
}

void UGsUIManager::Finalize()
{
	RemoveAll();
}

void UGsUIManager::InitializeAfterResourceDownload()
{
	if (_loadingController)
	{
		_loadingController->InitializeAfterResourceDownload();
	}
}

void UGsUIManager::Update(float InTick)
{
	if (OnManagerTick.IsBound())
	{
		OnManagerTick.Broadcast(InTick);
	}

	if (_blockUIController)
	{
		_blockUIController->Update(InTick);
	}
}

void UGsUIManager::BeginDestroy()
{
	// GsGameInstance에서 Finalize를 호출, 싱글턴이 제거되면서 불린다.
	// 따라서 해제 로직들은 Finalize에 나열한다.
	Super::BeginDestroy();
}

void UGsUIManager::OnLauncherStart(const UGsLobbyConstant* InData)
{
	if (_blockUIController)
	{
		// 내부에서 널체크 함
		_blockUIController->OnLauncherStart(InData);
	}

	if (nullptr == InData)
	{
		return;
	}

	_popupResolutionList.Key = InData->_pathPopupResolutionList.Id;
	_popupResolutionList.Value = InData->_pathPopupResolutionList.Path;

	_popupResolutionYesNo.Key = InData->_pathPopupResolutionYesNo.Id;
	_popupResolutionYesNo.Value = InData->_pathPopupResolutionYesNo.Path;

	_trayDimmed.Key = InData->_pathTrayDimmed.Id;
	_trayDimmed.Value = InData->_pathTrayDimmed.Path;

	_trayTickerLauncher.Key = InData->_pathTrayTickerUI.Id;
	_trayTickerLauncher.Value = InData->_pathTrayTickerUI.Path;

	_popupAccountDelete.Key = InData->_pathPopupAccountDelete.Id;
	_popupAccountDelete.Value = InData->_pathPopupAccountDelete.Path;
}

void UGsUIManager::RemoveAll()
{
	OnManagerTick.Clear();
	OnUIManagerResetUI.Clear();

	ClearQueueTimer();
	_reservedQueue.Empty();
	_currQueueingWidget = nullptr;
	_bLockQueueProcess = false;

	_hideFlagStateMap.Empty();

	if (_uiController)
	{
		_uiController->Finalize();
	}
	_uiController = nullptr;

	if (_dealScrollManager)
	{
		_dealScrollManager->Finalize();
	}
	_dealScrollManager = nullptr;

	if (_emoticonManager)
	{
		_emoticonManager->Finalize();
	}
	_emoticonManager = nullptr;

	if (_widgetPoolManager)
	{
		_widgetPoolManager->Finalize();
	}
	_widgetPoolManager = nullptr;

	if (_tickerHandler.IsValid())
	{
		_tickerHandler->Finalize();
		_tickerHandler.Reset();
	}

	if (_loadingController)
	{
		_loadingController->Finalize();
	}
	_loadingController = nullptr;

	if (_tooltipManager)
	{
		_tooltipManager->Finalize();
	}
	_tooltipManager = nullptr;

	if (_blockUIController)
	{
		_blockUIController->Finalize();
	}
	_blockUIController = nullptr;
}

void UGsUIManager::RemoveWhenFlowChanged()
{
	OnManagerTick.Clear();

	// 오픈 대기중인 위젯정보 큐 비우기
	ClearQueueTimer();
	_reservedQueue.Empty();
	_currQueueingWidget = nullptr;
	_bLockQueueProcess = false;

	if (_tickerHandler.IsValid())
	{
		_tickerHandler->RemoveAll();
	}

	// yjchoung: 아래 Pool 처리와 같은 이유로 전부 제거하지 않고, 오래된 위젯만 제거하도록 함
	if (_uiController)
	{
		_uiController->RemoveWhenLevelChanged();
	}

	if (_dealScrollManager)
	{
		_dealScrollManager->Reset();
	}

	//bak1210 : 로비/인게임 전환간 풀 전체 삭제하는 로직은 주석 처리합니다
	//기존에 로비 - 인게임 상호간 전환은 자주 있는 이슈는 아니지만 재연결등의 이슈로 로비로 돌아가는 상황은 발생합니다.
	
	// yjchoung: 위 이유로 Pool 삭제 로직 -> 반납 로직으로 변경함
	if (_widgetPoolManager)
	{
		_widgetPoolManager->ReleaseAll();
	}
}

void UGsUIManager::RemoveWhenLevelChanged()
{
	// yjchoung: RemoveWhenFlowChanged의 전부 삭제 코드가 없어지며 코드가 동일해져서 같은 함수 부르도록 함
	RemoveWhenFlowChanged();
}

void UGsUIManager::OnChangeLevel()
{	
	RemoveWhenLevelChanged();	
}

void UGsUIManager::OnInputTouchClick()
{
	if (EGsUIHUDMode::CAMERA == _hudMode)
	{
		SetHUDMode(EGsUIHUDMode::NORMAL);
	}
}

void UGsUIManager::Open(FName InKey)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GetWidgetByKey(InKey);
	if (widget.IsValid())
	{
		// 중복 생성이 가능한 위젯은 새로 생성해서 넘기기 위해 무시
		if (false == widget->CanMultipleInstance())
		{
			return;
		}
	}

	OpenInter(InKey, GetWidgetClassByName(InKey));
}

TWeakObjectPtr<UGsUIWidgetBase> UGsUIManager::OpenAndGetWidget(const FName& InKey)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GetWidgetByKey(InKey);
	if (widget.IsValid())
	{
		// 중복 생성이 가능한 위젯은 새로 생성해서 넘기기 위해 무시
		if (false == widget->CanMultipleInstance())
		{
			return widget;
		}
	}

	return OpenInter(InKey, GetWidgetClassByName(InKey));
}

TWeakObjectPtr<UGsUIWidgetBase> UGsUIManager::OpenAndGetWidgetByPath(const FName& InKey, const FSoftObjectPath& InPath)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GetWidgetByKey(InKey);
	if (widget.IsValid())
	{
		// 중복 생성이 가능한 위젯은 새로 생성해서 넘기기 위해 무시
		if (false == widget->CanMultipleInstance())
		{
			return widget;
		}
	}

	return OpenInter(InKey, UGsPathManager::GetClassStatic(InPath));
}

void UGsUIManager::RemoveWidget(UGsUIWidgetBase* InWidget)
{
	if (_uiController)
	{
		_uiController->RemoveWidget(InWidget);
	}

	if (InWidget)
	{
		ProcessQueue(InWidget);
	}
}

void UGsUIManager::PopStack(UGsUIWidgetBase* InWidget, bool bRecoverPrevStack)
{
	if (_uiController)
	{
		_uiController->PopStack(InWidget, bRecoverPrevStack);
	}
}

void UGsUIManager::Close(UGsUIWidgetBase* InWidget)
{
	if (InWidget)
	{
		InWidget->Close();
	}
}

void UGsUIManager::CloseByKeyName(const FName& InKey, bool bImmediate, bool bRecoverPrevStack)
{
	if (_uiController)
	{
		UGsUIWidgetBase* widget = _uiController->GetCachedWidgetByName(InKey, true);
		if (widget)
		{
			widget->Close(bImmediate, bRecoverPrevStack);
		}
	}
}

void UGsUIManager::CloseAllStack(UGsUIWidgetBase* InTopWidget, bool bInRecoverDefaultState)
{
	if (_uiController)
	{
		_uiController->CloseAllStack(InTopWidget, bInRecoverDefaultState);
	}
}

TWeakObjectPtr<UGsUIWidgetBase> UGsUIManager::CreateNotAddToViewport(const FName& InKey)
{
	if (UWorld* world = GetWorld())
	{
		if (UGameInstance* gameInstance = world->GetGameInstance())
		{
			if (UClass* widgetClass = GetWidgetClassByName(InKey))
			{
				if (_uiController)
				{
					return _uiController->CreateNotAddToViewport(gameInstance, widgetClass, InKey);
				}
			}
		}
	}

	return nullptr;
}

UClass* UGsUIManager::GetWidgetClassByName(const FName& InKey)
{
	if (UGsPathManager* pathMgr = GPath())
	{
		if (UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, InKey))
		{
			if (widgetClass->IsChildOf<UGsUIWidgetBase>())
			{
				return widgetClass;
			}
		}
	}

	GSLOG(Error, TEXT("[UI] Fail to get widget Class. Key: %s"), *InKey.ToString());

	return nullptr;
}

UGsUIWidgetBase* UGsUIManager::OpenInter(const FName& InKey, UClass* InWidgetClass)
{
	if (nullptr == InWidgetClass)
	{
		return nullptr;
	}

	if (UWorld* world = GetWorld())
	{
		if (_uiController)
		{
			auto widget = _uiController->CreateAndAdd(world->GetGameInstance(),
				TSubclassOf<UGsUIWidgetBase>(InWidgetClass), InKey);

			FGsTutorialMsgParamName param(InKey);
			GMessage()->GetTutorial().SendMessage(MessageContentTutorial::OPEN_UI, &param);

			return widget;
		}
	}

	return nullptr;
}

void UGsUIManager::ShowLoading(int32 InMapId, bool bIsInvadeWorld, int32 InPlayerLevel)
{
	if (_loadingController)
	{
		_loadingController->ShowLoading(InMapId, bIsInvadeWorld, InPlayerLevel);
	}
}

void UGsUIManager::HideLoading()
{
	if (_loadingController)
	{
		_loadingController->HideLoading();
	}
}

void UGsUIManager::SetTouchVfxPath()
{
#ifdef USE_TOUCH_EFFECT
	if (UGsLobbyDataManager* lobbyMgr = LSLobbyData())
	{
		if (const UGsLobbyConstant* lobbyConst = lobbyMgr->GetLobbyConstantData())
		{
			_touchVfxPath = lobbyConst->_touchVfx;

			return;
		}
	}

	// 없으면 비우기. 이 부분을 타면 정상 상황이 아님
	GSLOG(Error, TEXT("[TEST_BLOCK] UGsUIManager::SetTouchVfxPath(). Cannot Find lobbyConst"));
	_touchVfxPath = FSoftObjectPath();
#endif
}

void UGsUIManager::PlayPressSound()
{
	if (false == _pressSound.IsValid())
	{
		UObject* soundObject = FSoftObjectPath(DefaultButtonPressedSound).TryLoad();
		if (soundObject)
		{
			_pressSound = Cast<USoundBase>(soundObject);

			UGameplayStatics::PlaySound2D(this, _pressSound.Get());
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, _pressSound.Get());
	}
}

TWeakObjectPtr<UGsUIWidgetBase> UGsUIManager::GetWidgetByKey(const FName& InKey, bool InActiveCheck)
{
	if (_uiController)
	{
		return _uiController->GetCachedWidgetByName(InKey, InActiveCheck);
	}

	return nullptr;
}

void UGsUIManager::SetVisibility(bool InFlag)
{
	if (InFlag)
	{
		ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
	}
	else
	{
		SetHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
	}
}

void UGsUIManager::SetHideFlags(EGsUIHideFlags InFlag)
{
	// 중첩이 하나라도 없으면 Hide 수행
	if (false == EnumHasAllFlags(_hideFlags, InFlag))
	{
		// State에 해당하는 Setting값으로 HideFlag를 세팅한다
		if (EGsUIHideFlags* settingFlag = _hideFlagStateMap.Find(InFlag))
		{
			_hideFlags |= *settingFlag;
		}
		else
		{
			_hideFlags |= InFlag;
		}

		if (FGsMessageHolder* msgMgr = GMessage())
		{
			FGsUIMsgParamUInt param(static_cast<uint32>(_hideFlags));
			msgMgr->GetUI().SendMessage(MessageUI::HIDE_UI, &param);
		}
	}

	// 조이스틱 체크
	CheckJoyStickVisibility();
}

void UGsUIManager::ClearHideFlags(EGsUIHideFlags InFlag)
{
	// 해당 플래그가 있으면 수행
	if (EnumHasAnyFlags(_hideFlags, InFlag))
	{
		if (EGsUIHideFlags::UI_HIDE_ALL == InFlag)
		{
			_hideFlags = EGsUIHideFlags::UI_HIDE_NONE;
		}
		else
		{
			if (EGsUIHideFlags* settingFlag = _hideFlagStateMap.Find(InFlag))
			{
				_hideFlags ^= *settingFlag;
			}
			else
			{
				_hideFlags ^= InFlag;
			}

			// State별 Hide 시켜야 하는 UI가 중복되므로, 남아있는 State의 세팅대로 UI를 Hide 시킨다.
			for (TPair<EGsUIHideFlags,EGsUIHideFlags>& itPair : _hideFlagStateMap)
			{
				if (EnumHasAnyFlags(_hideFlags, itPair.Key))
				{
					_hideFlags |= itPair.Value;
				}
			}
		}

		if (FGsMessageHolder* msgMgr = GMessage())
		{
			FGsUIMsgParamUInt param(static_cast<uint32>(_hideFlags));
			msgMgr->GetUI().SendMessage(MessageUI::HIDE_UI, &param);
		}
	}

	// 조이스틱 체크
	CheckJoyStickVisibility();
}

void UGsUIManager::SetHUDMode(EGsUIHUDMode InMode, bool bInRefresh)
{
	EGsUIHUDMode prevMode = _hudMode;
	_hudMode = InMode;

	if (bInRefresh)
	{
		FGsUIMsgParamInt param(static_cast<int32>(_hudMode));
		GMessage()->GetUI().SendMessage(MessageUI::CHANGE_HUD_MODE, &param);

		// 카메라 모드일 때 UI 가리기 설정
		if (EGsUIHUDMode::CAMERA == InMode)
		{
			SetHideFlags(EGsUIHideFlags::STATE_HUD_CAMERA_MODE);

			// 카메라 모드 인풋 막기 설정
			FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_SCREENSHOT, true);
			GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
		}
		else if(EGsUIHUDMode::SPECTATOR == InMode)
		{
			SetHideFlags(EGsUIHideFlags::STATE_OFFLINE_PLAY);

			// 카메라 모드 인풋 막기 설정
			FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_SCREENSHOT, true);
			GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
		}
		else
		{
			// 카메라 모드 빠져나갈 때 UI 가리기 해제
			if (EGsUIHUDMode::CAMERA == prevMode)
			{
				ClearHideFlags(EGsUIHideFlags::STATE_HUD_CAMERA_MODE);

				// 인풋 막기 풀기
				FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_SCREENSHOT, false);
				GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
			}
			else if (EGsUIHUDMode::SPECTATOR == prevMode)
			{
				ClearHideFlags(EGsUIHideFlags::STATE_OFFLINE_PLAY);

				// 인풋 막기 풀기
				FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_SCREENSHOT, false);
				GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
			}
		}
	}
}

void UGsUIManager::AddUIScaleFlag(EGsUIScaleFlags InFlag, bool bInvalidate)
{
	EnumAddFlags(_uiScaleFlags, InFlag);

	if (bInvalidate)
	{
		InvalidateUIScale();
	}

	if (EGsUIScaleFlags::UI_SCALE_HUD_STATE == InFlag)
	{
		// HUD로 돌아오며 터치 이펙트 위치가 이상한 위치로 가는 모습때문에 강제 반납 처리
		if (UClass* fxTouchClass = GPath()->GetClass(EGsPathType::UI, TEXT("fxTouch")))
		{
			if (UGsWidgetPool* pool = _widgetPoolManager->FindPool(fxTouchClass))
			{
				pool->ReleaseAll();
			}
		}
	}
}

void UGsUIManager::RemoveUIScaleFlag(EGsUIScaleFlags InFlag, bool bInvalidate)
{
	EnumRemoveFlags(_uiScaleFlags, InFlag);

	if (bInvalidate)
	{
		InvalidateUIScale();
	}

	if (EGsUIScaleFlags::UI_SCALE_HUD_STATE == InFlag)
	{
		// HUD로 돌아오며 터치 이펙트 위치가 이상한 위치로 가는 모습때문에 강제 반납 처리
		if (UClass* fxTouchClass = GPath()->GetClass(EGsPathType::UI, TEXT("fxTouch")))
		{
			if (UGsWidgetPool* pool = _widgetPoolManager->FindPool(fxTouchClass))
			{
				pool->ReleaseAll();
			}
		}
	}
}

bool UGsUIManager::IsHUDScale() const
{
	return (EGsUIScaleFlags::UI_SCALE_HUD_STATE == _uiScaleFlags) ? true : false;
}

bool UGsUIManager::IsRemovingWidgets() const
{
	if (_uiController)
	{
		return _uiController->IsRemovingWidgets();
	}

	return false;
}

void UGsUIManager::InvalidateUIScale()
{
	FGsUIMsgParamBool param(false);

	// 이 경우에만 스케일 함
	if (EGsUIScaleFlags::UI_SCALE_HUD_STATE == _uiScaleFlags)
	{
		param._data = true;
	}

	GMessage()->GetUI().SendMessage(MessageUI::INVALIDATE_UI_SCALE, &param);
}

bool UGsUIManager::IsActiveWidget(const FName& InKey) const
{
	return _uiController->IsActiveWidget(InKey);
}

void UGsUIManager::CheckJoyStickVisibility()
{
	if (EnumHasAnyFlags(_hideFlags, EGsUIHideFlags::UI_HIDE_HUD) ||
		EnumHasAnyFlags(_hideFlags, EGsUIHideFlags::STATE_HUD_CAMERA_MODE))
	{
		SetJoystickVisibility(false);
	}
	else
	{
		SetJoystickVisibility(true);
	}
}

void UGsUIManager::SetJoystickVisibility(bool InVal)
{
	if (APlayerController* playerController = GetWorld()->GetFirstPlayerController())
	{
		playerController->SetVirtualJoystickVisibility(InVal);
	}
}

void UGsUIManager::PushQueue(const FName& InKey, int32 InId, TFunction<void(UGsUIWidgetBase*)> InitFunc)
{
	_reservedQueue.Enqueue(FGsWidgetQueueData(InKey, InId, InitFunc));

	ProcessQueue(nullptr);
}

void UGsUIManager::ProcessQueue(UGsUIWidgetBase* InWidget)
{
	// 중복체크(광클방지 등)를 위해, 닫을 때 큐에서 제거한다.

	if (_currQueueingWidget.IsValid())
	{
		if (InWidget)
		{
			// 현재 오픈 중인 것이 닫혔으면 큐에서 제거
			if (InWidget == _currQueueingWidget.Get())
			{
				_reservedQueue.Pop();
				_currQueueingWidget = nullptr;

				if (_reservedQueue.IsEmpty())
				{
					GMessage()->GetUI().SendMessage(MessageUI::UI_RESERVED_QUEUE_EMPTY, nullptr);
					// 튜토리얼과의 순서 보장을 위해 메시지를 가름
					GMessage()->GetUI().SendMessage(MessageUI::UI_RESERVED_QUEUE_EMPTY_FOR_BM, nullptr);
				}
			}
			else
			{
				// 현재 오픈중인 것이 아니므로 무시
				return;
			}
		}
		else
		{
			// 현재 오픈 중인 것이 있으므로 대기
			return;
		}
	}

	if (_bLockQueueProcess)
	{
		return;
	}

	if (false == _reservedQueue.IsEmpty())
	{
		// 동일한 위젯을 RemoveFromParent로 지우자마자 AddToViewport를 수행하면
		// AddToViewport 가 무시되는 현상이 있따.
		// 따라서 타이머로 잠깐 텀을 준 후에 AddToViewport를 수행한다.
		if (_queueTimerHandle.IsValid())
		{
			return;
		}

		GetWorld()->GetTimerManager().SetTimer(_queueTimerHandle, this, &UGsUIManager::OnEndQueueTimer, 0.01f, false);
	}
}

bool UGsUIManager::IsExistInQueue(const FName& InKey, int32 InId) const
{
	return _reservedQueue.IsExist(InKey, InId);
}

bool UGsUIManager::IsLocked() const
{
	// InputLock 관련 처리
	if (IsActiveLoadingUI())
	{
		return true;
	}

	return false;
}

bool UGsUIManager::IsActiveLoadingUI() const
{
	if (_loadingController)
	{
		return _loadingController->IsVisible();
	}

	return false;
}

void UGsUIManager::Back()
{
	// 로딩 중에는 처리하지 않기
	if (true == IsActiveLoadingUI())
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		if (netMgr->IsBlockingSend())
		{
			return;
		}	
	}

	if (_blockUIController->IsOpened())
	{
		return;
	}

	if (_uiController->Back())
	{
		return;
	}

	if (_uiController->IsBackDelayed() == false)
	{
		// 게임 나가기 메시지 보내기. GameFlowManager에서 받아 각 State에 따라 처리된다.
		//GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);
		GMessage()->GetSystem().SendMessage(MessageSystem::QUIT_GAME);
	}
}

void UGsUIManager::OnEndQueueTimer()
{
	if (false == _reservedQueue.IsEmpty())
	{
		FGsWidgetQueueData data;
		if (_reservedQueue.Peek(data))
		{
			auto widget = OpenAndGetWidget(data._key);
			if (widget.IsValid())
			{
				if (data.InitFunc)
				{
					data.InitFunc(widget.Get());
				}

				_currQueueingWidget = widget;
			}
			else
			{
				// 생성 실패하면 바로 다음 것을 처리하도록 넘김
				ProcessQueue(nullptr);
			}
		}
	}

	ClearQueueTimer();
}

void UGsUIManager::ClearQueueTimer()
{
	if (_queueTimerHandle.IsValid())
	{
		if (UWorld* world = GetWorld())
		{
			world->GetTimerManager().ClearTimer(_queueTimerHandle);
		}
	}
}

void UGsUIManager::SetLockQueueProcess(bool bLock)
{
	_bLockQueueProcess = bLock;

	if (bLock)
	{
		// 다음 큐가 진행되지 않도록 타이머제거
		ClearQueueTimer();
	}
	else
	{
		// 큐에 쌓여있던 것이 다시 진행되도록 호출
		ProcessQueue(nullptr);
	}
}


void UGsUIManager::PrintLog()
{
	if (_uiController)
	{
		_uiController->PrintLog();
	}

	if (_widgetPoolManager)
	{
		_widgetPoolManager->PrintLog();
	}
}

void UGsUIManager::AddTicker(UGsUIWidgetBase* widget)
{
	if (_tickerHandler.IsValid() && widget)
	{
		_tickerHandler->AddWidget(widget);
	}
}

void UGsUIManager::RemoveTicker(UGsUIWidgetBase* widget)
{
	if (_tickerHandler.IsValid())
	{
		_tickerHandler->RemoveWidget(widget);
	}
}

void UGsUIManager::HideTicker()
{
	if (_tickerHandler)
		_tickerHandler->HideAll();
}

void UGsUIManager::ShowTicker()
{
	if (_tickerHandler)
		_tickerHandler->ResetShow();
}

FGsUITickerHandler::TrayWidgetState UGsUIManager::GetTickerState()
{
	if (_tickerHandler)
	{
		return _tickerHandler->GetCurrentTickerState();
	}

	return FGsUITickerHandler::TrayWidgetState::None;
}

void UGsUIManager::OnKeyboardActionOptionKey(EGsKeyMappingType InKeyMappingType, EInputEvent InEventType)
{
	switch (InKeyMappingType)
	{
	case EGsKeyMappingType::KEY_MAPPING_OK:
	case EGsKeyMappingType::KEY_MAPPING_NO:
	{
		// 팝업 OK, NO 에 대한 경우, 최상위 팝업에만 이벤트 전달처리
		if (UGsUIWidgetBase* topStackWidget = _uiController->StackPeek(true))
		{
			topStackWidget->CommandInputMapKeyAction(InKeyMappingType, InEventType);
			return;
		}
	}
	break;
	}

	FGsInputEventMsgWindowKey InputMsg(static_cast<int>(InKeyMappingType), InEventType);
	GMessage()->GetWindowInput().SendMessage(MessageInput::Window_keyboard_Action, &InputMsg);
}

void UGsUIManager::ResetUIByReconnection()
{
	//connection에 의한 UIReset
	OnUIManagerResetUI.Broadcast(1);
}