// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupComposeFailure.generated.h"

class UPanelWidget;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaFairyCompose;
struct FGsSchemaCostumeCompose;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupComposeFailure : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _getItemPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _panelWidgetHelper;

protected:
	TArray<const FGsSchemaFairyCompose*> _composeFairyGradeList;
	TArray<const FGsSchemaCostumeCompose*> _composeCostumeGradeList;
	const FGsSchemaFairyCompose*	_selectFairyGrade{ nullptr };
	const FGsSchemaCostumeCompose*	_selectCostumeGrade{ nullptr };

public:
	struct PopupInitData
	{
		enum class ComposeType : uint8 { Fairy, Costume };
		ComposeType composeType{ ComposeType::Fairy };
		int composeGrade{ -1 };
	};

	void InitializeData(PopupInitData* param);
	void RefreshUI();

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnRefreshEntry(int32 index, UWidget* entry);

protected:
	virtual void OnInputCancel() override;
};

using ComposeType = UGsUIPopupComposeFailure::PopupInitData::ComposeType;
