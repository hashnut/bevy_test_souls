// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Helper/GsRedDotChecker.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsIntervalChecker.h"
#include "GsUIWindowMarket.generated.h"

class UWidget;
class UGsButton;
class UImage;
class UPanelWidget;
class UWidgetSwitcher;
struct IGsMessageParam;
class UGsUIMarketPageBase;
class UGsSwitcherButton;

/**
 * 거레소 창
 */
UCLASS()
class UGsUIWindowMarket : public UGsUIWindow
{
	GENERATED_BODY()

public:
	using MsgHandleMarket = TPair<MessageContentMarket, FDelegateHandle>;
	using MsgHandleUnlock = TPair<MessageContentUnlock, FDelegateHandle>;

	enum class EGsMarketServerTab
	{
		Normal = 0,
		World,
		MaxType
	};

	enum EGsMarketMainTab
	{
		Search = 0,
		Sell,
		Settlement,
		Log,
		MaxType
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelWorldBackground;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnWorld;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnTooltip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUITooltipWorldMarketServerList* _tooltipWorldMarketServerList;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMainTab;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherPage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIMarketPageSearch* _pageSearch;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIMarketPageSell* _pageSell;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIMarketPageSettlement* _pageSettlement;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIMarketPageLog* _pageLog;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIRedDotBase* _uiRedDot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIRedDotBase* _uiRedDotWorld;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIRedDotBase* _uiRedDotNormal;

protected:
	FGsToggleGroup _toggleGroupServer;
	FGsToggleGroup _toggleGroupMainTab;
	EGsMarketServerTab _currServerTab = EGsMarketServerTab::MaxType;
	EGsMarketMainTab _currMainTab = EGsMarketMainTab::MaxType;
	TArray<UGsUIMarketPageBase*> _pageList;
	TArray<MsgHandleMarket> _msgHandleMarketList;
	TArray<MsgHandleUnlock> _msgHandleUnlockList;
	bool _bRedDotDirty = false;

	FGsIntervalChecker _coolTimeServerTab;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual bool OnBack() override;
	virtual bool SyncContentState() override;

protected:
	void BindMessages();
	void UnbindMessages();

protected:
	void OnClickServer(int32 InIndex, bool bIsSelected);
	void OnClickBlockedWorldTab(int32 InIndex);

	void OnSelectMainTab(int32 InIndex);
	void OnClickBlockedTab(int32 InIndex);

protected:
	UFUNCTION()
	void OnPressTooltipWorldServerList();

protected:
	void SetUIServer(EGsMarketServerTab InType);
	void SetUIPage(EGsMarketMainTab InTabType);

	void CheckUnlockWorld(bool InIsFirstCheck);
	void CheckUnlock(bool InIsFirstCheck);
	bool CheckChangeServerMaintainStatus(EGsMarketServerTab InNextTab);

protected:
	// Market
	void OnUpdateItemList(const IGsMessageParam*);
	void OnUpdateBookmark(const IGsMessageParam* InParam);
	void OnRcvSellRegisterdItem(const IGsMessageParam* InParam);
	void OnRcvSellCancelItem(const IGsMessageParam* InParam);
	void OnRcvBuySuccess(const IGsMessageParam* InParam);
	void OnUpdateSearch(const IGsMessageParam* InParam);
	void OnUpdateFilter(const IGsMessageParam* InParam);
	void OnUpdateRedDot(const IGsMessageParam* InParam);
	void OnRcvItemDetailInfo(const IGsMessageParam* InParam);
	void OnRcvSearchResult(const IGsMessageParam* InParam);
	void OnReqRefresh(const IGsMessageParam*);
	void OnRcvBookmarkmList(const IGsMessageParam*);
	void OnRcvBuyResult(const IGsMessageParam* InParam);
	void OnAddToPurchaseList(const IGsMessageParam* InParam);
	void OnRemoveFromPurchaseList(const IGsMessageParam* InParam);

	// Unlock
	void OnContentsUnlock(const IGsMessageParam* InParam);

protected:	
	UGsUIMarketPageBase* GetCurrentPage() const;
	void SetRedDotDirty(bool bIsDirty) { _bRedDotDirty = bIsDirty; }
	bool IsWorldMarket() const;
};
