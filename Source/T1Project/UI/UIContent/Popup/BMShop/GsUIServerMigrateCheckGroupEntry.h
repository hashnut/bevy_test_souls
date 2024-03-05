// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIServerMigrateCheckGroupEntry.generated.h"

class UTextBlock;
class UUserWidget;
class UVerticalBox;
class UGsDynamicPanelSlotHelper;
struct MigrateGroupItem;
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIServerMigrateCheckGroupEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textGroupName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticlaBoxConditionSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _conditionItemWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _conditionItemSlotHelper;
	
protected:
	TWeakPtr<MigrateGroupItem> _data{ nullptr };

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void ConditionItemRefresh(int32 InIndex, UWidget* InEntry);	

public:
	void SetData(TWeakPtr<MigrateGroupItem> inData) { _data = inData; }
	void RefreshUI();
};
