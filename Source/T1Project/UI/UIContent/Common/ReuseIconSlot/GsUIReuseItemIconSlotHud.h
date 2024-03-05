// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseItemIconSlotSimple.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "UI/UIContent/Common/Skill/GsSlotDragDownLogic.h"
#include "GsUIReuseItemIconSlotHud.generated.h"

class UGsUIVFX;
class UGsButton;
class UGsUIIconBase;
/**
 * Hud QuickSlot 에서 사용될 Slot Class
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIReuseItemIconSlotHud : public UGsUIReuseItemIconSlotSimple, public IGsTutorialInteractionInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _removeBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _quickSlotBlinkEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _quickSlotUseAutoEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _notOwnedUseAutoEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _quickSlotEditEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIDragDown")
	float _dragDownSensitive = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIDragDown")
	float _dragDownValue = 15.f;

private:
	bool _isBlockState = false;

	// 드래그 옵션 관련
protected:
	FGsSlotDragDownLogic _dragDownLogic;
	bool _bCanDragDown = false;
	bool _bAutoChanged = false;
	bool _bIsCaptureMouse = false;
	FVector2D _pressStartPosAbs;
	
	// -1: 초기화값, 0: 오토Off, 1: 오토On/아이템있음, 2: 오토Off/아이템없음
	int32 _autoEffectState = -1;	

protected:
	virtual void OnClickIconSlot(class UGsUIIconItem& InIcon) override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetBlinkEffect(bool InState);
	void SetEditModeEffect(bool InState);
	void SetUseAutoEffect(bool InAutoUse, bool IsItemDBId, bool IsTemplateId);
	void SetGrayScaleImage(const bool InState);
	void SetAutoUseImage(const bool InState);
	void SetEquipTextImage(const bool InState);
	void SetUnEquipTextImage(const bool InState);
	void SetUseTextImage(const bool InState);
	void SetSlotDimmed(const bool InState);
	void ResetSelection();
	void InvalidateAutoState();

private:
	int32 GetAutoEffectState(bool InAutoUse, bool IsItemDBId, bool IsTemplateId);	

public:
	virtual void SetSlotItemIcon(const uint64 InItemDBID, const int32 InItemTID, const int64 InAmount, const uint8 InSlotIndex = 0);

public:
	UFUNCTION()
	void OnClickSlotRemove();

protected:
	virtual void OnRefreshIconWrapBox(int32 InIndex, UGsUIIconBase* InIcon) override;

protected:
	void OnItemSlotButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent);
	void OnItemSlotButtonRelease();
	void OnSlotDragDownTranslation(const FVector2D& InTranslation);
	void OnSlotDragDownStateChanged();

public:
	void OnClickIconWindowInput();

//----------------------------------------------------------------------------------------------------------------------
	// Tutorial
private:
	FGsOnTutorialInteraction OnTutorialInteraction;

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial) { }
	virtual bool IsTutorialInteraction() const { return OnTutorialInteraction.IsBound(); }
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() { return OnTutorialInteraction; }
};
