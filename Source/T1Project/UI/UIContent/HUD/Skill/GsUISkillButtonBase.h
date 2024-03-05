// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsNameTooltipInterface.h"
#include "Skill/GsSkillSlotData.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "GsUISkillButtonBase.generated.h"

class FGsSkill;
class UGsButton;

/**
 * 스킬버튼 부모 클래스
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class T1PROJECT_API UGsUISkillButtonBase : public UUserWidget, public IGsNameTooltipInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSoftObjectPtr<class UPaperSprite> _emptyIconRef;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDimmed;

protected:
	int32 _slotId = 0;
	int32 _skillId = 0;
	float _coolTime = 0.0f; // 총 쿨타임
	float _remainSec = 0.f; // 남은 쿨타임 받은 값(이 값을 차감하는게 아니다)
	double _syncCoolTime = 0; // 남은 쿨타임 받은 시간

	// 다른 요인에 의해 IsEnableButton 체크 시 이전 상태가 날라가지 않도록 저장
	bool _bIsCoolTime = false;

	// Buff Skill 등의 스킬 사용 제한 없는 경우 연출을 위한 UI 작동 딜레이를 준다.
	float _deltaEnableDelay = 0.f;
	bool _enableStartDelay = false;

	// True일 경우, 무한스크롤 연출을위해 추가된 페이지. 메시지를 보내지 않도록 막을 것.
	bool _bIsFakePage = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

public:
	virtual void InitializeSlot(int32 InSlotId) { _slotId = InSlotId; }
	virtual void UpdateManagerTick(float InDeltaTime);
	virtual void SetData(const FGsSkill* InData) {} // 자손 클래스에서 구현
	virtual bool UpdateData(const class FGsSkill* InData);
	virtual void SetReserveSlot(bool InReserve) {} // 자식 클래스에서 구현
	virtual void SetAutoUse(SkillId InSkillId, bool bInIsAuto) {} // 자식 클래스에서 구현
	virtual void SetEmptySlot();
	virtual void SetEnableDelayTime(float delta);
	virtual void PlayChangeStepEffect(const FGsSkill* InPrevSkill, const FGsSkill* InSkill) {} // 자손 클래스에서 구현
	virtual void PlaySkillStartEffect() {} // 자손 클래스에서 구현
	virtual void OnKeyboardPress() {} // 자손 클래스에서 구현
	virtual void OnKeyboardRelease() {} // 자손 클래스에서 구현	
	virtual void OnMountVehicle(bool bIsMounted) {} // 자손 클래스에서 구현

	virtual void OpenNameTooltip() override;

protected:
	virtual void SetEnableButton(bool InEnable);
	virtual void SetCooltimeUI(float InRemainSec, float InRate) {}

	UFUNCTION()
	virtual void OnClickDimmedButton();

public:
	int32 GetSlotId() const { return _slotId; }	
	int32 GetCachedSkillId() const { return _skillId; }
	bool IsEmpty() const { return (0 > _skillId) ? true : false; }
	void SetCoolTime(float InRemainTime, bool bInCheckEnableButton = true);
	void UpdateEnableButton();

	// 무한스크롤 연출을 위한 가짜 페이지(SendEvent하는것은 막고 연출만 보이게 하기 위함)
	void SetIsFakePage(bool bIsFake) { _bIsFakePage = bIsFake; }

	virtual void GetNameText(OUT FText& OutNameText) {} // 자손 클래스에서 구현
	virtual ItemGrade GetSkillGrade() { return ItemGrade::NORMAL; } // 자손 클래스에서 구현

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill")
	void OnFinishCoolTime();

protected:
	void SetCoolTimeInter(float InRemainSec);
	class UPaperSprite* GetEmptyIcon();
	float GetElapsedTime();
};
