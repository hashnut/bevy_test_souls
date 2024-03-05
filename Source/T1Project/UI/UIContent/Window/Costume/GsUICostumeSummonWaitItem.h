#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "GsUICostumeSummonWaitItem.generated.h"

class UImage;
class UTextBlock;
class UGsButton;
class UCanvasPanel;
class USizeBox;
class UScrollBox;
class UWidgetSwitcher;

class UGsUICostumeItem;
class UGsUIFairyStat;
class UGsUICurrencyButton;
class UGsUICostumeSkillTooltip;
class UGsUICostumeSkillIconItem;
class UGsDynamicPanelSlotHelper;

struct FGsCostumeData;
struct FGsCostumeSummonConfirmData;

UCLASS()
class T1PROJECT_API UGsUICostumeSummonWaitItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
public:
	DECLARE_DELEGATE_OneParam(FOnSelected, TWeakPtr<FGsCostumeSummonConfirmData>);
	DECLARE_DELEGATE_OneParam(FOnSelectedCollection, CostumeId);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICostumeItem* _portrait;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _costumeName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _costumeGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnChange;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtRemainCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtRemainTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSummonConfirm;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFindCostume;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnViewCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChangeInfo;

private:
	TWeakPtr<FGsCostumeSummonConfirmData> _data;
	TArray<UGsUIFairyStat*> _itemMainStatList;
	TArray<UGsUIFairyStat*> _itemSubStatList;
	TArray<UImage*> _skillIcons;
	CreatureGenderType _genderType{ CreatureGenderType::ALL };
	CreatureWeaponType _weaponType{ CreatureWeaponType::ALL };

	bool _useCheck{ false };

	struct TempListData
	{
		bool _isActive{ false };
		int32 _id{ 0 };
		CreatureWeaponType _type{ CreatureWeaponType::NONE };
		const FSoftObjectPath _iconPath;
	};
	TArray<TempListData> _skillList;

public:
	FOnSelected OnSummonChange;
	FOnSelected OnSummonConfirm;
	FOnSelectedCollection OnCollectionPopup;
	FOnSelectedCollection OnFindCostumeInfo;
	FOnSelected OnProbabilityPopup;

public:
	void SetData(TWeakPtr<FGsCostumeSummonConfirmData> data, CreatureGenderType genderType = CreatureGenderType::ALL)
	{
		_data = data;
		_genderType = genderType;
		_useCheck = false;
	}
	void RefreshUI();

	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSummonChange();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSummonConfirm();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickFindCostume();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickViewCostumeCollection();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCostumeChangeInfo();

	FSlateColor GetRemainAmountTextColor() const;
	FSlateColor GetRemainTimeTextColor() const;

private:
	void RefreshRemainTimeSet();
	void RefreshCostumeData(TWeakPtr<FGsCostumeData> data);
};