#pragma once
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "Unlock/GsUnlockDefine.h"
#include "GsUIMaterialSetGainInfo.generated.h"

class UGsButton;
class UTextBlock;
class URichTextBlock;
class UGsToggleButton;
class UWidgetSwitcher;
class UGsUIMaterialSetGainInfoCraftTargetList;
class UGsUIMaterialSetGainInfoMarketPrice;
struct IGsMessageParam;

UCLASS()
class T1PROJECT_API UGsUIMaterialSetGainInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	enum class TabIndex { Market, Craft};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleBtnMarket;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _toggleBtnCraft;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGoEnchant;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMarketRefresh;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMarketQuickBuy;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDetailInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMaterialSetGainInfoMarketPrice* _uiMarketPrice;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIMaterialSetGainInfoCraftTargetList* _uiCraftTarget;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	URichTextBlock* _textLockReason;
	
	/*
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxCraft;*/

	const class FGsItemCollectionField* _data = nullptr;
	bool _prevBuildMarket = false;
	bool _prevBuildCraft = false;

	FGsToggleGroup _toggleGroupBtn;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	void OnChangedToogleMainTab(int32 InIndex);

	void BuildMarket(const class FGsItemCollectionField* InData);
	void BuildCraft(const class FGsItemCollectionField* InData);
	void BuildAfterContentsCheck(TabIndex InTab);

protected:
	UFUNCTION()
	void OnClickedGoEnchant();

	UFUNCTION()
	void OnClickedGoExit();

	UFUNCTION()
	void OnClickedMarketRefresh();

	UFUNCTION()
	void OnClickedMarketQuickBuy();

public:
	void OnReceiveCollectionPriceList(const IGsMessageParam* InParam);
	void OnRequestRefreshPrice(const IGsMessageParam*);

public:
	void SetData(const class FGsItemCollectionField* InData, const int32 InTabIndex = -1);
	void Invalidate();
	void ContentsUnlock(EGsUnlockContentsType InType);
	void Capture(int32& OutTabIndex);
};