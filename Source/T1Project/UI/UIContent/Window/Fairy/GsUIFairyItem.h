#pragma once
#include "CoreMinimal.h"
#include "GsUISummonPortraitBase.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Fairy/GsFairyCollection.h"
#include "Fairy/GsFairyData.h"
#include "GsUIFairyItem.generated.h"

class UImage;
class UTextBlock;
class UGsButton;
class UCanvasPanel;
class UWidgetSwitcher;
class UPaperSprite;

UCLASS()
class T1PROJECT_API UGsUIFairyItem : public UGsUISummonPortraitBase, public IGsEntryInterface
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnSelected, FairyId);

	enum class UIItemType : uint8 { NONE = 0, ITEM, ENCHANT, COMPOSITION, COLLECTION, RESOURCE, SUMMONWAIT, ENCHANT_RES, POPUP_COLLECTION };

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textFairyAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textEnchantCount;

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
	UImage* _bgNotCollectedFairy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imagePreset;

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelected();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelectedCollection();

public:
	virtual void ResetData() override { _data.Reset(); Super::ResetData(); }
	virtual void SetData(TWeakPtr<IGsBasePortrait> data, CreatureGenderType gender = CreatureGenderType::ALL) override
	{
		_data = StaticCastSharedPtr<FGsFairyData>(data.Pin());
		_useCount = 0;

		Super::SetData(_data);
	}

	void SetData(TWeakPtr<FGsFairyData> data, int32 useCount) 
	{
		_data = data; 
		_useCount = useCount;

		Super::SetData(_data);
	}

	void SetCollectionData(FairyCollectionItem& colData)
	{
		_data = colData._data;
		_complete = colData._complete;
		_isNewComplete = colData._isNew;
		_needLv = colData._needLv;

		Super::SetData(_data);
	}

	virtual void RefreshUI() override;
	bool IsSelected() { return _isSelected; }
	void SetSelected(bool isSelected, bool isNotiSelect = true);

	void SetBlockSelected(bool selectBlock = true, float blockTime = 0.5f);

	FairyId GetFairyId();
	uint64 GetSoundId() { return _soundId; }

	virtual void OnHide() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void CommonRefreshUI();
	void FairyDataRefreshUI();
	void FairyDataEnchantUI();
	void FairyDataComposeUI();
	void FairyDataCollectionUI();
	void FairyDataResourceUI();
	void FairyDataSummonWaitUI();
	void FairyDataEnchantResUI();
	void FairyDataPopupCollectionUI();

public:
	UIItemType WidgetType{ UIItemType::NONE };
	FOnSelected OnItemSelected;

private:
	bool _isSelected{ false };
	bool _isBlocked{ false };

	float _blockTime{ 0.5f };

	bool _complete{ false }; 
	bool _isNewComplete{ false };
	Level _needLv{ 0 };
	int32 _useCount{ 0 };

	uint64 _soundId{ 0 };

	TWeakPtr<FGsFairyData> _data{ nullptr};
};

