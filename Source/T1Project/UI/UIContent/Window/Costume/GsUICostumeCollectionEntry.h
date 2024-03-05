#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUICostumeCollectionEntry.generated.h"

/**
 * 페어리 콜렉션 리스트 아이템 UI
 */
class UImage;
class UScrollBox;
class UTextBlock;
class UGsButton;
class UWidgetSwitcher;
class UPanelWidget;
class UGsUIListCostumeCollectionData;

UENUM(BlueprintType)
enum class CostumeCollectionType : uint8
{
	NOACTION = 0,
	POPUP_ACTION,
};

UCLASS()
class T1PROJECT_API UGsUICostumeCollectionEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnSelectedCostume, CostumeId);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	CostumeCollectionType WidgetType { CostumeCollectionType::NOACTION };
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listCostume;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRewardComplete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCollectionTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _collectedSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _collectedTxtSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;

public:
	FOnSelectedCostume	OnCollectionPopup;

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
	CreatureGenderType _genderType{ CreatureGenderType::ALL };
	TFunction<void(UGsUIListCostumeCollectionData*)> _cbSelection = nullptr;
	bool IsValideCallback() { return _cbSelection != nullptr; }
	void SetSelectionCallback(TFunction<void(UGsUIListCostumeCollectionData*)> cbSelection) { _cbSelection = cbSelection; }
};
