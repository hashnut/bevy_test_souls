// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "../UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIAlternateItemEntry.generated.h"


UCLASS()
class T1PROJECT_API UGsUIAlternateItemEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSelectedBoard;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSelectedEdge;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textItemName;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;

	ItemId _data = INVALID_ITEM_ID;

public:
	DECLARE_DELEGATE_OneParam(FOnClicked, UUserWidget*);
	FOnClicked OnClicked;

public:
	void BeginDestroy() override;

protected:
	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	class UGsUIIconItemInventory* GetIcon();

	UFUNCTION()
	void OnClickedSelected();

public:
	void SetData(const class FGsItemCollectionCondition* InData, int32 InIndex);
	void SetIsSelected(bool InFlag);

	ItemId GetData() const { return _data; }
};
