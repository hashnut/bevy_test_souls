#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageUI.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUIHUDScanButton.generated.h"

/*
* hud 스캔 버튼
*/
class UGsButton;
class UWidget;

class UProgressBar;
class UGameViewportClient;
class APlayerController;

class UGsUIScanDragObject;

struct IGsMessageParam;


UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDScanButton : public UGsUIHUD, public IGsTutorialInteractionInterface
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	// 스캔 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnScan;
	// on 효과 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _panelOnEffect;

	// 쿨타임 프로그래스바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarCoolTime;

	MsgUIHandleArray _msgUIHandlerList;
	// 쿨타임 중인가
	bool _isCoolTimeOn = false;
	// 쿨타임 시작 시간
	int64 _coolStartTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIScanDragObject* _widgetDrag;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _panelButton;
	UPROPERTY(Category = "GsUI", EditAnywhere)
	float _touchMoveZ = 3.0f;

	// 클릭 시 보일 이펙트 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	FSoftObjectPath _clickVFXPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	int32 _clickVFXZOrder = 1000;

	// 가상 함수
protected:
	// 최초 한번 호출
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;

	// 이벤트
public:
	void OnAutoScanActive(const IGsMessageParam* InParam);


public:
	void OnHide();

public:
	// 스캔 클릭
	UFUNCTION()
	void OnClickScan();

	// 스캔 쿨타임 시작
	void StartScanCoolTime();

	void StartScanByUI(bool In_isByDrag = false);

	void OnTouchStart();
	void OnTouchEnd();
	// set, get
public:
	// 액티브 auto 표시 처리
	void SetActiveAuto(bool In_isActive);
	// 스캔 쿨타임 rate
	float GetScanCoolTimeRate();
	
	void SetIsDragOn(bool In_isOn);

	void SetTestDragDiff(float In_val);

	bool GetIsCoolTimeOn()
	{
		return _isCoolTimeOn;
	}

	// Tutorial
private:
	bool _bIsTutorialInteraction = false;
	FGsOnTutorialInteraction OnTutorialInteraction;

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial) override { _bIsTutorialInteraction = bIsTutorial; }
	virtual bool IsTutorialInteraction() const override { return _bIsTutorialInteraction; }
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() override { return OnTutorialInteraction; }
};
