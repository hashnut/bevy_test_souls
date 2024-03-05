// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUIFairyCollectionEntry.generated.h"

/**
 * 페어리 콜렉션 리스트 아이템 UI
 */
class UImage;
class UScrollBox;
class UTextBlock;
class UGsButton;
class UWidgetSwitcher;
class UPanelWidget;
class UGsUIListItemFairyCollectionData;

UENUM(BlueprintType)
enum class FairyCollectionType : uint8
{
	NOACTION = 0,
	POPUP_ACTION,
};

UCLASS()
class T1PROJECT_API UGsUIFairyCollectionEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnSelectedFairy, FairyId);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	FairyCollectionType WidgetType { FairyCollectionType::NOACTION };
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listFairy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRewardComplete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCollectionTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _collectedSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _collectedTxtSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;

public:
	FOnSelectedFairy	OnCollectionPopup;

	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshEntry(int32 index, UWidget* entry);

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;	

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

public:
	TFunction<void(UGsUIListItemFairyCollectionData*)> _cbSelection = nullptr;
	bool IsValideCallback() { return _cbSelection != nullptr; }
	void SetSelectionCallback(TFunction<void(UGsUIListItemFairyCollectionData*)> cbSelection) { _cbSelection = cbSelection; }
};
