// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "CanvasPanel.h"
#include "GsUICharacterSlot.generated.h"

/*
	ΩΩ∑‘ ≈∏¿‘
*/
UENUM()
enum class ECharacterSlotType : uint8
{
	SLOT_NORMAL,
	SLOT_SLEECTED,
	SLOT_EMPTY,
	SLOT_LOCKED,
	MAX
};

class UWidgetSwitcher;
class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICharacterSlot :
	public UUserWidget,
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE_TwoParams(FOnClickSlot, ECharacterSlotType, uint8);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnEmpty;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnLocked;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherWeaponNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherWeaponSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _deletingCanvas;

	bool _bIsSelected = false;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textLevel;

	UPROPERTY()
	uint8 _characterIndex;

	UPROPERTY()
	ECharacterSlotType _slotType;

public:
	FOnClickSlot OnClickSlot;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	void SetData(uint8 InIndex, ECharacterSlotType InType, FString InName = TEXT(""), 
							uint16 InLevel = 0, MapId InMapId = 0, int32 InWeaponType = 0, bool InisDeleteing = false);

protected:
	UFUNCTION()
	void OnClickNormal();

	UFUNCTION()
	void OnClickSelected();

	UFUNCTION()
	void OnClickEmpty();

	UFUNCTION()
	void OnClickLocked();	
};
