#pragma once
#include "CoreMinimal.h"
#include "UI/UIContent/Window/Fairy/GsUISummonPortraitBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Costume/GsCostumeCollection.h"
#include "Costume/GsCostumeData.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUICostumeItem.generated.h"

class UImage;
class UTextBlock;
class UGsButton;
class UCanvasPanel;
class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUICostumeItem : public UGsUISummonPortraitBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnSelected, CostumeId);

	enum class UIItemType : uint8 { NONE = 0, ITEM, COMPOSITION, COLLECTION, RESOURCE, SUMMONWAIT, POPUP_COLLECTION, EXCHANGE, APPEARANCE };

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _bgSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelectCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _completeMark;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _summoned;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _checkIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _bgNotCollected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imagePreset;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelected();

public:
	virtual void ResetData() override { _data.Reset(); Super::ResetData(); }

	void SetData(TWeakPtr<FGsCostumeData> data, CreatureGenderType gender, int32 useCount) 
	{	
		SetData(data, gender);

		_useCount = useCount;
	};

	void SetCollectionData(CostumeCollectionItem& colData, CreatureGenderType gender = CreatureGenderType::ALL)
	{
		_data = colData._data;
		_complete = colData._complete;
		_isNewComplete = colData._isNew;
		_useGender = gender;
		_useCount = 0;

		Super::SetData(_data, gender);
	}

	virtual void SetData(TWeakPtr<IGsBasePortrait> data, CreatureGenderType gender = CreatureGenderType::ALL) override
	{
		_data = StaticCastSharedPtr<FGsCostumeData>(data.Pin());
		_useCount = 0;

		Super::SetData(data, gender);
	}

	virtual void RefreshUI() override;
	bool IsSelected() { return _isSelected; }
	void SetSelected(bool isSelected, bool isNotiSelect = true);
	void SetBlockSelected(bool selectBlock = true, float blockTime = 0.5f);
	CostumeId GetCostumeID();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void CommonRefreshUI();
	void CostumeDataRefreshUI();
	void CostumeDataComposeUI();
	void CostumeDataCollectionUI();
	void CostumeDataResourceUI();
	void CostumeDataSummonWaitUI();
	void CostumeDataPopupCollectionUI();
	void CostumeDataAppearanceUI();
	void CostumeDataExchangeUI();

public:
	UIItemType WidgetType{ UIItemType::NONE };
	FOnSelected OnItemSelected;

private:
	bool _isSelected{ false };
	bool _isBlocked{ false };

	float _blockTime{ 0.5f };

	bool _complete{ false };
	bool _isNewComplete{ false };
	int32 _useCount{ 0 };

	TWeakPtr<FGsCostumeData> _data{ nullptr };
};

