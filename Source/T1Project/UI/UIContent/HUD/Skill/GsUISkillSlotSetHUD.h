// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageGameObject.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUISkillSlotSetHUD.generated.h"

class UScrollBox;
class UPanelWidget;
class UWidgetSwitcher;
class UGsUISkillSlotPageHUD;;
class UGsUISkillButtonBase;
struct IGsMessageParam;

/**
 * HUD의 스킬 프리셋. 일단 Active, Buff 모두 이거로 쓰고, 필요시 상속받아 분리.
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUISkillSlotSetHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	using DefSkillSlotList = TArray<UGsUISkillButtonBase*>;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootPage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootPageIcon;

	// 페이지 하나 크기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUISkillSlotSet")
	float _unitOffset = 580.f;
	// 무한 스크롤을 위해 페이지 교체 시 Offset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUISkillSlotSet")
	float _pageChangeOffset = 80.f;
	// 페이지 snap 판정 offset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUISkillSlotSet")
	float _pageSnapOffset = 50.f;
	// 페이지 Snap 스크롤 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUISkillSlotSet")
	float _autoScrollSpeed = 2000.f;

protected:
	int32 _currPage = 1; // 현재 페이지(1부터 시작하는 수)
	bool _bIsScrolling = false;
	float _destOffset = 0.f; // 자동 스크롤 시 목표 위치
	int32 _maxPage = 0; // 페이지 실 데이터(무한 스크롤을 위해 추가된 앞, 뒤의 한 개씩을 뺀 값)		
	float _backOffset = 0.f; // 뒤로 드래그 시 무한스크롤 넘어가는 위치	
	float _startOffset = 0.f; // 드래그 시작한 오프셋 위치
	float _minOffsetLock = 0.f;
	float _maxOffsetLock = 0.f;

	FTimerHandle _autoScrollTimer; // 자동 스크롤 타이머

	TArray<TWeakObjectPtr<UGsUISkillSlotPageHUD>> _pageList;
	TArray<TWeakObjectPtr<UWidgetSwitcher>> _pageIconList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void UpdateManagerTick(float InDeltaTime);

	void OnKeyboardPress(int32 InSlotId);
	void OnKeyboardRelease(int32 InSlotId);

	// 스크롤 관련 함수
protected:
	UFUNCTION()
	void OnScrolledEvent(float CurrentOffset);

	void OnScrollStarted();
	void OnScrollFinished();

	void OnAutoScrollTimer();
	void OnEndAutoScroll();
	void ClearAutoScrollTimer();

	// index는 0부터 시작하는 실제 위치, Page는 1부터 시작하는 실제 슬롯
	int32 GetPageByIndex(int32 InIndex);
	int32 GetIndexByOffset(float InOffset);
	void SetPageIcon(int32 InPage);

public:
	void FindSlots(int32 InSlotId, OUT DefSkillSlotList& OutList);
	void FindSlotsBySkillId(SkillId InSkillId, OUT DefSkillSlotList& OutList);
	void UpdateAllSlots();

//---------------------------------------------------------------------------------------------------------------------
	// Tutorial
private:
	int32 _tutorialSlotId = -1;
	float _tutorialOffset = 0.f;

public:
	class UGsUISkillButtonBase* SetTutorialTarget(int32 InSlotId);	
	bool IsTutorial() const { return (0 <= _tutorialSlotId) ? true : false; }
};
