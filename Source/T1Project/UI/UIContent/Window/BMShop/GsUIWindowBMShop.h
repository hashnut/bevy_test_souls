// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageUI.h"
#include "DataSchema/BMShop/GsSchemaBMShopTabInfo.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIWindowBMShop.generated.h"

class UWidget;
class UTextBlock;
class UScrollBox;
class UGsButton;
class UPanelWidget;
class UVerticalBox;
class UWidgetSwitcher;
class UGsUICurrencyButton;
class FGsBMShopProductData;
class FGsBMShopManager;
class UGsDynamicPanelSlotHelper;

struct MigrateGroupItem;
/**
 * BMShop main ui class
 */
UCLASS()
class T1PROJECT_API UGsUIWindowBMShop : public UGsUIWindow
{
	GENERATED_BODY()

private:
	enum BMShopDisplayType : uint8 
	{
		Display_ProductListType = 0,
		Display_StorageType,
	};

private:
	// enum 순서 변경 ㄴㄴ
	enum BMShopProductDisplayType : uint8
	{
		DP_Normal = 0,
		DP_Detail,
		DP_SmallBanner,
		DP_LargeBanner,
		DP_StepUpBanner,
		DP_ServerMigrate,	// 2023/10/27 PKT - 서버 이전

		DP_Max,
	};
	enum BMShopBannerDisplayType : uint8
	{
		Banner_SmallType = 0,
		Banner_LargeType,

		Banner_Max,
	};


protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBulkSale;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _mainTabRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _subTabRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _subTabStateRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _defaultTypeScrollbox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _detailTypeScrollbox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _storageListRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _storageCountText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAllAccept;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _shopSwitcher; // 0 : productList , 1 : storage
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnProbabilityInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherProductDP; // 0 : ProductNormal, 1 : DetailDPtype, 2 : SmallBanner, 3 : LargeBanner
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _loginUserInfoText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _loginUserInfoRootpanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherStorage; // 0 : Enable Storage  1 : Empty Storage
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInAppPurchaseWarnLink;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRestore;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _storageCountRoot;
	


protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _defaultProductRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _detailProductRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _stepUpRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _normalTypePanel;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _smallBannerProductRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _largeBannerRootPanel;
	

protected:
	// main tab 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _mainTabListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _mainTabListPanelSlotHelper;

protected:
	// sub tab 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subTabListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _subTabListPanelSlotHelper;

protected:
	// Storage List
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _storageEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _storageListPanelSlotHelper;

protected:
	// product List
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TArray<TSubclassOf<UUserWidget>> _productListEntryWidgetClassList;
	UPROPERTY()
	TArray<UGsDynamicPanelSlotHelper*> _productListPanelSlotHelperList;

protected:
	// banner list
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TArray<TSubclassOf<UUserWidget>> _bannerListEntryWidgetClassList;
	UPROPERTY()
	TArray<UGsDynamicPanelSlotHelper*> _bannerListPanelSlotHelperList;

protected:
	// StepUp
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _stepUpEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _stepUpPanelSlotHelper;

	/**
	 * 2023/10/30 PKT - Server Migrate
	 */
protected:
	// StepUp
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _serverMigrateEntryWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _serverMigratePanelSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _serverMigrateRootPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _panelServerMigrateRemainingTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textServerMigrateRemainingTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherServerMigrateInfo;

	UPROPERTY(BlueprintReadOnly)
	FText _textEmptyList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnServerMigrateGuideLink;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textServerMigrateCount;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnServerMigrateCurrency;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnImpossibleMigrate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnServerMigrateRefresh;

protected:
	FGsToggleGroup _toggleGroupMainTab;
	FGsToggleGroup _toggleGroupSubTab;

private:
	TArray<TPair<MessageContentStoreBM, FDelegateHandle>>	_listBmShopDelegate;
	TPair<MessageUserInfo, FDelegateHandle> _userInfoMsgHandler;
	TPair<MessageUI, FDelegateHandle> _uiMsgHandler;

private:
	UPROPERTY()
	TArray<UPanelWidget*> _productRootPanelList;

private:
	bool _isExcuteFocusingProduct = false;
	BMProductId _focusingProductID = 0;
	float _entryWidgetWidthSize = 0.f;
	TArray<const FGsSchemaBMShopTabInfo*> _mainTabDataList;
	TArray<const FGsSchemaBMShopTabInfo*> _subTabDataList;
	TArray<BMProductId> _productIDList;
	TArray<uint64> _storageItemDBIDList;
	EGsBMShopDisPlayType _productDisplayType = EGsBMShopDisPlayType::None;

private:
	TArray<TWeakPtr<MigrateGroupItem>> _migrateData;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual bool SyncContentState() override;
	virtual bool OnBack() override;

private:
	void CreateMainTab();
	void OnChangeSubTab();
	void OnChangeProductList();
	void OnChangeStorageTab();
	void FindSubTabList();
	void RegisterMessage();
	void RemoveMessage();
	void CheckZpayModeUI(FGsBMShopManager* InBMShopMgr);

private:
	void InitProductPanel(BMShopProductDisplayType InDPtype);
	void InitBannerPanel(BMShopBannerDisplayType InType);
	void OnRefreshProductList();
	BMShopProductDisplayType GetConvertProductDisplayType();
	void OnFocusingProduct();
	void OnInvalidate();

	void BuildViewServerMigrate(bool InIsResetScroll = true);

private:
	void OnFinishedRemainTime(FGsSharedParam& InParamData);
	void InvalidateProductItem(FGsSharedParam& InParamData);
	void InvalidateProductListItem(FGsSharedParam& InParamData);
	void InvalidateStorageList(FGsSharedParam& InParamData);
	void MoveToTab(FGsSharedParam& InParamData);
	void MoveToTabByTabID(FGsSharedParam& InParamData);
	void FocusingProduct(FGsSharedParam& InParamData);
	void UpdateStorageTabRedDot();
	void InvalidateExtraCurrency();
	void OnUpdateCurrency(uint64 InData);
	void OnUpdateUserLevel(const struct IGsMessageParam* inParam);
	void PopupShopTimeOverProduct(FGsSharedParam& InMessageParam);
	void PopupShopInvalidateProduct(FGsSharedParam& InMessageParam);

private:
	void OnAddItem(struct FGsItemMessageParamBase& InParam);
	void OnUpdateItem(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItem(struct FGsItemMessageParamBase& InParam);

public:
	void CheckExtraCurrency();

protected:
	void OnSelectChangedMainTab(int32 InIndex);
	void OnSelectChangedSubTab(int32 InIndex);

protected:
	UFUNCTION()
	void MainTabListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void SubTabListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void DefaultProductListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void DetailProductListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void SmallBannerProductListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void LargeBannerProductListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void StorageListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void StepUpListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void ServerMigrateGroupListEntryRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void AllAcceptStorageItems();
	UFUNCTION()
	void OnProbabilityInfo();
	UFUNCTION()
	void OnBulkSale();
	UFUNCTION()
	void OnPurchaseWarn();
	UFUNCTION()
	void OnRestore();
	UFUNCTION()
	void OnClickServerMigrateGuide();
	UFUNCTION()
	void OnClickServerMigrateDataRefresh();
	UFUNCTION()
	void OnClickMigration();
	UFUNCTION()
	void OnClickImpossibleMigration();
	
protected:
	virtual void OnClickBack() override;

// 튜토리얼
protected:
	int32 _tutorialTabId = 0;
	BMProductId _tutorialProductId = INVALID_BMPRODUCT_ID;

public:
	DECLARE_DELEGATE_OneParam(FOnTutorialListEvent, UWidget*);
	FOnTutorialListEvent OnTutorialListEvent;

public:
	void SetTutorialTab(int32 InTabId);
	void SetTutorialProduct(BMProductId InProductId);

protected:
	bool IsTutorialTab() const { return (0 < _tutorialTabId) ? true : false; }
	bool IsTutorialProduct() const { return (INVALID_BMPRODUCT_ID != _tutorialProductId) ? true : false; }
	void CheckTutorialTab();
	void CheckTutorialProduct();

	UFUNCTION()
	void OnScrollDefaultType(float InCurrentOffset);

	/**
	 * 2023/10/31 PKT - 서버 이주 관련
	 */
protected:
	// 2023/11/1 PKT - 서버 선택시 처리를 위한 콜백
	void OnCallBackServerMigrateSelectedServer(WorldId InWorldId);
public:
	// 2023/11/1 PKT - 서버 이주 관련 UI 갱신
	void ServerMigrateViewRefresh();

	void ServerMigrateConfirm(ServerMigrateType type);
};
