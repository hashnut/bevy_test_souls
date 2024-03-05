// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Skill/GsUISkillSlotEquipBase.h"
#include "UI/UIContent/Common/Skill/GsSlotDragDownLogic.h"
#include "GsUISkillSlotEquip.generated.h"

class UGsButton;
class UWidgetSwitcher;
class FGsSkill;

/**
 * 스킬창 장착슬롯
 */
UCLASS()
class T1PROJECT_API UGsUISkillSlotEquip : public UGsUISkillSlotEquipBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, UGsUISkillSlotEquip*);

	enum EGsSkillSlotEffectType
	{
		NORMAL = 0, // 파랑
		SAME_SKILL, // 노랑		
		SAME_GROUP, // 빨강
	};
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRemove;
	// 현재스킬, 장착가능 표시 이펙트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherEffect;
	// 오토 이펙트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _effectAuto;

protected:
	FGsSlotDragDownLogic _dragDownLogic;
	bool _bAutoChanged = false;
	bool _bCanDragDown = false;
	bool _bIsCaptureMouse = false;
	FVector2D _pressStartPosAbs;

public:
	FOnClickSlot OnClickSkillSlot;
		
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	virtual bool SetData(const FGsSkill* InData) override;
	virtual void SetEmpty() override;

protected:
	virtual void OnClickSlot() override;

protected:
	UFUNCTION()
	void OnClickRemove();
	UFUNCTION()
	void OnSlotReleased();

protected:
	void OnMouseButtonDownEx(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	void OnSlotDragDownTranslation(const FVector2D& InTranslation);
	void OnSlotDragDownStateChanged();

public:
	void SetEnableEquip(bool bInEnable, bool bIsEquipMode, const FGsSkill* InSelectedSkill);
	void SetIsAuto(bool bInIsAuto);
	void RequestRemoveSlot();
};
