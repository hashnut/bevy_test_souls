// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UILib/Manager/GsUIController.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "Message/GsMessageUI.h"
#include "UI/UILib/Base/WidgetBase/GsUICurveAnimation.h"
#include "T1Project/T1Project.h"
#include "Option/GsOptionEnumType.h"
#include "UI/UILib/Interface/GsEditableInterface.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GsUIWidgetBase.generated.h"


/**
 * GsUIManager에서 관리받는 위젯의 기본클래스. GsUIWindow, GsUIPopup, GsUITray 를 사용할 것.
 * 개선: abstract 클래스로 만들면 좋겠다
 */
UCLASS()
class T1PROJECT_API UGsUIWidgetBase : 
	public UUserWidget
{
	GENERATED_BODY()

	friend class UGsUIController;
	friend class IGsEditableInterface;

protected:
	// Window, Popup, Tray 순서보장 관리를 받지 않고 강제로 세팅하고 싶을 경우 true로 설정
	UPROPERTY(EditDefaultsOnly, Category = "GsUI")
	bool _bNotUseManagedZOrder = false;
	// 뎁스 조절을 위해 더해질 값. 최적화를 위해 가능한 사용하지 말 것.
	UPROPERTY(EditDefaultsOnly, Category = "GsUI")
	int32 _addZOrder = 0;
	UPROPERTY(EditDefaultsOnly, Category = "GsUICurveAnimation")
	TArray<FGsUICurveAnimation> _curveAnimationList;

protected:
	bool _bIsHide = false;
	ESlateVisibility _backupVisibility;
	bool _bNotDestroy = false; //_bEnableAutoDestroy
	MsgUIHandle _hideMessageHandle;
	MsgUIHandle _hitTestInvisibleHandle;
	FName _tableKey;
	bool _bIsDestructed = false; // NativeConstruct / Destruct 두번하지 않기 위한 검사
	FDateTime _lastCloseTime;

protected:
	FDelegateHandle _managerTickHandle;

public:
	UGsUIWidgetBase(const FObjectInitializer& ObjectInitializer);

public:
	// UUserWidget override.
	virtual void RemoveFromParent() override;

protected:
	// UUserWidget override.
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;	

public:
	// 이미 Viewport에 존재하는데 다시 Open을 시도할 경우에 불린다.
	virtual void Reset();
	virtual void OnCurveAnimationFinished(EGsUIAnimationTriggerType InTriggerType);

	virtual void OnHyperlink(const FString& InStr) {}

public:
	// 스택 관리되는 UI인가
	virtual bool IsStackable() const { return false; }
	// 윈도우 타입(화면을 다 가리는 타입. GsUIWindow 상속객체)인가
	virtual bool IsWindow() const { return false; }
	// 레벨 로드 시 파괴되지 않는 UI인가
	virtual bool IsNotDestroyedDuringLoading() const { return false; }
	// 중복 생성이 가능한 위젯(Popup과 Tray에만 허용한다)
	virtual bool CanMultipleInstance() const { return false; }
	// 시스템 스택에 관리될 객체인가
	virtual bool IsManagedBySystemStack() const { return false; }
	// 자동 진행시 대화 이벤트 등이 발생했을 경우, 자동으로 Close되는 것을 막을 것인지
	virtual bool GetPreventAutoClose() const { return false; }
	// Hide 이벤트 받았을 때 구분자
	virtual EGsUIHideFlags GetHideFlagType() const { return EGsUIHideFlags::UI_HIDE_DEFAULT; }
	// Window < Popup < Tray 뎁스 보장을 위한 값. 자손 클래스에서 값 부여(Window: 10, Popup: 100, Tray: 500)
	virtual int32 GetManagedDefaultZOrder() const { return UI_DEPTH_DEFAULT; }
	virtual int32 GetManagedZOrder() const;
	// Stack 관련함수 window, popup 에서만 사용되고 있다
	virtual void SetStackRecoverFlag(bool bIsOn) {}
	virtual bool IsRemainStack() const { return false; }
	virtual void SetRemainStack(bool bIsRemain) {}

protected:
	virtual void SetHide(EGsUIHideFlags InHideFlags);
	// Hide 이벤트 핸들 함수
	virtual void OnHide(const struct IGsMessageParam* InParam);

	// 2021/10/21 PKT - Hit Test Invisible
	virtual void OnInterectionVisibleFlag(const struct IGsMessageParam* InParam);

	// Back키가 눌렸을 때 처리할 내용. false를 리턴 시 게임종료 팝업 띄움.
	virtual bool OnBack() { return true; }

	// 닫을 때는 Close 함수를 이용할 것.
	// 종료 애니메이션등이 재생된 후 실제로 RemoveFromParent 호출을 위해 불리는 함수.
	virtual void CloseInternal();

	// Visible이 Hidden, Collapse일 경우, NativeTick이 들어오지 않는다. 
	// True일 경우, UIManager에 Tick Delegate를 연결하여 외부에서 OnManagerTick 함수를 호출한다.
	virtual bool IsEnableManagerTick() const { return false; }
	virtual void OnManagerTick(float InDeltaTime) {}

public:
	// UI를 닫을 때 사용. UIController 내부 로직에서 닫을때는 부르지 않으므로,
	// 해제 등의 처리는 NativeDestruct(Viewport에서 제거되면 불림)를 이용할 것.
	// bImmediate가 true일 경우, 애니메이션 커브를 무시하고 바로 Pop처리.
	// Stack에서는 바로제거, 화면에서는 애니메이션 재생 후 제거한다.
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual void Close(bool bImmediate = false, bool bRecoverPrevStack = true);

	// UI Manager가 강제로 명령해줄 때 사용
	virtual void CommandInputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent) {};

	// 자신이 binding 할 때 사용되는 함수들. 입력 가능한 UI가 Input을 빼앗기지 않을때나, 특수하게 처리해야 하는 키가 있을 때 매핑하여 사용
protected:
	virtual void InputAxis(float Value) {};
	virtual void InputTouch(ETouchIndex::Type FingerIndex, FVector Location) {};
	virtual void InputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent) {};

public:
	bool AddToViewportEx();

	// StopAllAnimation을 쓸 경우 UMGSequencePlayer 쌓이는 엔진 버그 수정을 위한 함수. 
	// UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation 와 동일 기능.
	// 버전이 올라가며 고쳐진 것 같아 주석처리 함
	void StopAllAnimationsEx();

	// 활성화 상태인가(파괴되는오브젝트/되지 않는 오브젝트 구분에 유의)
	bool IsActive() const;
	FName GetTableKey() const { return _tableKey; }

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	class UGsUIManager* GetUIManager();

protected:
	// 주의: 초기화, 파괴시에만 사용할 것. 초기화 시 bNotDestroy 에 의해 설정. 씬전환 시 파괴처리여부.
	void SetNotDestroy(bool bNotDestroy);

	UFUNCTION(BlueprintCallable, Category = "GsUICurveAnimation")
	void SetCurveAnimationTarget(EGsUIAnimationTriggerType InTriggerType, class UWidget* InWidget);
	UFUNCTION(BlueprintCallable, Category = "GsUICurveAnimation")
	void SetCurveAnimationTargetByIndex(int32 InIndex, class UWidget* InWidget);
	UFUNCTION(BlueprintCallable, Category = "GsUICurveAnimation")
	bool PlayCurveAnimation(EGsUIAnimationTriggerType InTriggerType);
	UFUNCTION(BlueprintCallable, Category = "GsUICurveAnimation")
	bool PlayCurveAnimationByIndex(int32 InIndex);
	
	bool IsPlayingCurveAnimation(EGsUIAnimationTriggerType InTriggerType);
};
