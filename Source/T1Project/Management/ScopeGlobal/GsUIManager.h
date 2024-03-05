// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "UI/UILib/Container/GsWidgetQueue.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UILib/Manager/GsUITickerHandler.h"
#include "UI/UILib/Manager/GsLoadingScreenController.h"
#include "UI/UILib/Manager/GsBlockUIController.h"
#include "Option/GsOptionEnumType.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GsUIManager.generated.h"

class UGsEmoticonManager;
class AGsMoviePostProcess;
class UGsUILoadingScreen;
struct FGsLobbyUIData;
/**
 * UGsUIManager
 * - ZOrder 우선순위는 GsUIDefine.h의 UI_DEPTH_DEFAULT ... 참고
 * WBP에서 활용할 함수는 가능한 매니저에 직접 호출하지 말고, UGsBlueprintFunctionLibraryUI를 이용하도록 구현할 것.
 */
UCLASS()
class T1PROJECT_API UGsUIManager : 
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUIManagerTick, float /*InDeltaTime*/);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUIManagerResetUI, int);

private:
	// 위젯의 생성, 스택, 캐싱 등 관리
	UPROPERTY(Transient)
	class UGsUIController* _uiController;
	UPROPERTY(Transient)
	class UGsWidgetPoolManager* _widgetPoolManager;
	UPROPERTY(Transient)
	class UGsDealScrollManager* _dealScrollManager;
	UPROPERTY(Transient)
	class UGsEmoticonManager* _emoticonManager;
	UPROPERTY(Transient)
	class UGsLoadingScreenController* _loadingController;
	UPROPERTY(Transient)
	class UGsTooltipManager* _tooltipManager;
	UPROPERTY(Transient)
	class UGsBlockUIController* _blockUIController;

private:
	// UI 하이드 상태 Flag
	EGsUIHideFlags _hideFlags = EGsUIHideFlags::UI_HIDE_NONE;
	// UI Scale Flag
	EGsUIScaleFlags _uiScaleFlags = EGsUIScaleFlags::UI_SCALE_NONE;

	// 순서대로 오픈해야 하는 UI 관리
	class FGsWidgetQueue _reservedQueue;
	// 큐 관리되는 위젯 중 현재 떠있는 위젯 
	TWeakObjectPtr<UGsUIWidgetBase> _currQueueingWidget;
	// Add 지연을 위한 타이머. ProcessQueue 내부 주석 참고.
	FTimerHandle _queueTimerHandle;
	// HUD 의 모드
	EGsUIHUDMode _hudMode = EGsUIHUDMode::NORMAL;
	TMap<EGsUIHideFlags, EGsUIHideFlags> _hideFlagStateMap;	

	TUniquePtr<FGsUITickerHandler> _tickerHandler;
	// 순차적으로 열기 위해 큐에 넣었던 항목들을 잠시 막음. 튜토리얼 시 사용.
	bool _bLockQueueProcess = false;
	// true 시 드래그 감지 거리를 일부로 넓혀서 민감도를 인위적으로 낮춤(스킬창 슬롯 드래그 등에서 쓰임)
	bool _bLongDragTriggerDist = false;

	FSoftObjectPath _touchVfxPath;

	// 리스트 뷰 들에서 사용할 클릭 사운드
	TWeakObjectPtr<class USoundBase> _pressSound;

	// 개선: 로비랑 같이쓸 위젯들 시간날 때 관리방법 모색해보자
	// 개선: 해상도 관련팝업, blockUI, 시스템 팝업들, (dimmed?)
	TPair<FName, FSoftObjectPath> _popupResolutionList;
	TPair<FName, FSoftObjectPath> _popupResolutionYesNo;
	TPair<FName, FSoftObjectPath> _trayDimmed;
	TPair<FName, FSoftObjectPath> _trayTickerLauncher;
	TPair<FName, FSoftObjectPath> _popupAccountDelete;

public:
	FOnUIManagerTick OnManagerTick;
	FOnUIManagerResetUI OnUIManagerResetUI;

public:
	virtual void BeginDestroy() override;	

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void InitializeAfterResourceDownload() override;
	virtual void Update(float InTick) override;

public:
	void OnLauncherStart(const class UGsLobbyConstant* InData);
	void OnChangeLevel();

public:
	// UGsInputBindingLocalPlayer::OnTouchRelease 터치시간이 짧으면 클릭으로 처리
	// 스크린샷 모드 해제, UI 아닌 곳 터치 인식 등에 사용 가능
	void OnInputTouchClick();

public:
	// 게임 종료 시 호출. 메니저까지 모두 제거.
	void RemoveAll();
	// Flow가 바뀔 때 호출. 풀 제거. 캐싱 UI 제거.
	void RemoveWhenFlowChanged();
	// 레벨 로드 시 호출. 풀 반납. 캐시된 UI 중 오랫동안 사용하지 않은 것 제거.
	void RemoveWhenLevelChanged();

	// 값을 리턴받지 않음. bDuplicateCheck가 true이면, 뷰포트에 이미 있는지 체크해서 없으면 생성.	
	void Open(FName InKey);
	// 푸시하며 값을 얻어옴. bDuplicateCheck가 true이면, 뷰포트에 이미 있는지 체크해서 없으면 생성해서 줌.
	TWeakObjectPtr<UGsUIWidgetBase> OpenAndGetWidget(const FName& InKey);
	// 관리를 위해 키값은 넣어야 함
	TWeakObjectPtr<UGsUIWidgetBase> OpenAndGetWidgetByPath(const FName& InKey, const FSoftObjectPath& InPath);	

	// 대상 위젯 제거	
	void Close(UGsUIWidgetBase* InWidget);

	// 이름으로 대상 위젯 제거.
	// 주의: 한개 이상 만들 수 있는 위젯(bCanMultipleInstance == true)은 사용하면 안된다.	
	void CloseByKeyName(const FName& InKey, bool bImmediate = false, bool bRecoverPrevStack = true);

	// 스택상에 있는 UI 모두 닫기. 닫기 애니연출을 위해 InTopWidget을 넣음
	void CloseAllStack(UGsUIWidgetBase* InTopWidget = nullptr, bool bInRecoverDefaultState = true);

	// 속도 등의 이유로 위젯을 생성만 해놓을 때 사용
	TWeakObjectPtr<UGsUIWidgetBase> CreateNotAddToViewport(const FName& InKey);

	// 순차적으로 출력해야하는 UI의 경우 여기서 등록. 중복을 허용하지 않으려면 IsExistInQueue 검사 후 이용
	void PushQueue(const FName& InKey, int32 InId = 0, TFunction<void(UGsUIWidgetBase*)> InInitFunc = nullptr);

	// 안드로이드 Back키 처리
	void Back();

	// 로딩창, HUD등 특수 UI 관리
	void ShowLoading(int32 InMapId, bool bIsInvadeWorld, int32 InPlayerLevel);
	void HideLoading();

	// 터치이펙트
	void SetTouchVfxPath();
	FSoftObjectPath GetTouchVfxPath() const { return _touchVfxPath; }

	// ListView등 버튼에서 사운드 처리 안될 경우 활용
	void PlayPressSound();

	// Ticker 관리
	void AddTicker(UGsUIWidgetBase* widget);
	void RemoveTicker(UGsUIWidgetBase* widget);
	// 현재 enable된 Ticker중, 우선 순위가 아닌 Ticker들을 종류에 따라 close/hide 시키고, show 되기전 불리는 ticker들도 hide/close
	void HideTicker(); 
	// Ticker 상태 정상화. 순서에 맞게 show
	void ShowTicker();

	FGsUITickerHandler::TrayWidgetState GetTickerState();	

public:
	// 모든 UI Show/Hide 설정
	void SetVisibility(bool InFlag);
	// 일부 UI Show/Hide 설정
	void SetHideFlags(EGsUIHideFlags InFlag);
	void ClearHideFlags(EGsUIHideFlags InFlag);

	void SetHUDMode(EGsUIHUDMode InMode, bool bInRefresh = true);

	void AddUIScaleFlag(EGsUIScaleFlags InFlag, bool bInvalidate = true);
	void RemoveUIScaleFlag(EGsUIScaleFlags InFlag, bool bInvalidate = true);
	bool IsHUDScale() const;

	bool IsRemovingWidgets() const;

	void SetLongDragTriggerDist(bool bInEnable) { _bLongDragTriggerDist = bInEnable; }
	bool IsLongDragTriggerDist() const { return _bLongDragTriggerDist; }

private:
	void InvalidateUIScale();

	// -----------------------------------------------------------
	// 주의: UGsUIWidgetBase만 호출하는 함수. 외부에서 호출하지 말것!
public:	
	void RemoveWidget(UGsUIWidgetBase* InWidget);
	void PopStack(UGsUIWidgetBase* InWidget, bool bRecoverPrevStack = true);
	// -----------------------------------------------------------

private:
	UClass* GetWidgetClassByName(const FName& InKey);

	UGsUIWidgetBase* OpenInter(const FName& InKey, UClass* InWidgetClass);
	
	void ProcessQueue(UGsUIWidgetBase* InWidget);
	void OnEndQueueTimer();
	void ClearQueueTimer();

	void CheckJoyStickVisibility();
	void SetJoystickVisibility(bool InVal);

public:
	bool IsReservedQueueEmpty() { return _reservedQueue.IsEmpty() ? true : false; }
	void SetLockQueueProcess(bool bLock);

public:
	// InActiveCheck: 현재 활성화 중인 UI인지 체크하여 활성화 중이 아니면 nullptr을 리턴한다.
	TWeakObjectPtr<UGsUIWidgetBase> GetWidgetByKey(const FName& InKey, bool InActiveCheck = true);
	class UGsWidgetPoolManager* GetWidgetPoolManager() const { return _widgetPoolManager; }
	class UGsDealScrollManager* GetDealScrollManager() const { return _dealScrollManager; }
	class UGsEmoticonManager* GetEmoticonManager() const { return _emoticonManager; }
	class UGsTooltipManager* GetTooltipManager() const { return _tooltipManager; }
	UGsLoadingScreenController* GetLoadingController() const { return _loadingController; }
	UGsBlockUIController* GetBlockUIController() const { return _blockUIController; }
	EGsUIHUDMode GetHUDMode() const { return _hudMode; }	
	UGsUIController* GetUIController() { return _uiController; }
	EGsUIHideFlags GetHideFlagsCurrent() const { return _hideFlags; }	

	bool IsActiveWidget(const FName& InKey) const;
	bool IsExistInQueue(const FName& InKey, int32 InId) const;	
	bool IsLocked() const;
	bool IsActiveLoadingUI() const;

public:
	void OnKeyboardActionOptionKey(EGsKeyMappingType InKeyMappingType, EInputEvent InEventType);

	void ResetUIByReconnection();
public:
	void PrintLog();

	// 개선: 추후 수정
public:
	TPair<FName, FSoftObjectPath> GetPathPopupResolutionList() const { return _popupResolutionList; }
	TPair<FName, FSoftObjectPath> GetPathPopupResolutionYesNo() const { return _popupResolutionYesNo; }
	TPair<FName, FSoftObjectPath> GetPathTrayDimmed() const { return _trayDimmed; }
	TPair<FName, FSoftObjectPath> GetPathTrayTickerLauncher() const { return _trayTickerLauncher; }
	TPair<FName, FSoftObjectPath> GetPathIPopupAccountDelete() const { return _popupAccountDelete; }
};

#define GUI() UGsScopeHolder::GetGlobalManager<UGsUIManager>(UGsScopeGlobal::EManagerType::Ui)
