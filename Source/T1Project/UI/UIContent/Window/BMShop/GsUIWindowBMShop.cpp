// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIWindowBMShop.h"
#include "DataSchema/BMShop/ServerMigrate/GsSchemaServerMigrateConfigData.h"
#include "DataSchema/BMShop/ServerMigrate/GsSchemaServerMigrateGroupData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/item/GsSchemaItemCommon.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Currency/GsCurrencyHelper.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateData.h"
#include "UI/UIContent/Window/BMShop/GsUIBMShopStorageEntry.h"
#include "UI/UIContent/Window/BMShop/GsUIBMShopProductEntry.h"
#include "UI/UIContent/Window/BMShop/GsUIBMShopDetailProductEntry.h"
#include "UI/UIContent/Window/BMShop/GsUIBMShopSBProductEntry.h"
#include "UI/UIContent/Window/BMShop/GsUIBMShopLBProductEntry.h"
#include "UI/UIContent/Window/BMShop/GsUIBMShopStepUpProductEntry.h"
#include "UI/UIContent/Popup/BMShop/GsUIServerMigrateCheckGroupEntry.h"
#include "UI/UIContent/Popup/GsUIPopupMigrationReqConfirm.h"
#include "UI/UIContent/Popup/GsUIPopupMigrationUserInit.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/GridSlot.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"

#include "Net/GsNetSendServiceBMShop.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "T1Project.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"


void UGsUIWindowBMShop::BeginDestroy()
{
	if (nullptr != _mainTabListPanelSlotHelper)
	{
		_mainTabListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowBMShop::MainTabListRefresh);
		_mainTabListPanelSlotHelper = nullptr;
	}
	if (nullptr != _subTabListPanelSlotHelper)
	{
		_subTabListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowBMShop::SubTabListRefresh);
		_subTabListPanelSlotHelper = nullptr;
	}
	if (nullptr != _stepUpPanelSlotHelper)
	{
		_stepUpPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowBMShop::StepUpListRefresh);
		_stepUpPanelSlotHelper = nullptr;
	}
	if (nullptr != _serverMigratePanelSlotHelper)
	{
		_serverMigratePanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowBMShop::ServerMigrateGroupListEntryRefresh);
	}
	if (nullptr != _btnServerMigrateRefresh)
	{
		_btnServerMigrateRefresh->OnClicked.RemoveDynamic(this, &UGsUIWindowBMShop::OnClickServerMigrateDataRefresh);
	}

	for (int32 i = 0; i < _productListPanelSlotHelperList.Num(); ++i)
	{
		if (nullptr != _productListPanelSlotHelperList[i])
		{
			switch (BMShopProductDisplayType(i))
			{
			case BMShopProductDisplayType::DP_Normal:
				_productListPanelSlotHelperList[i]->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowBMShop::DefaultProductListRefresh);
				_productListPanelSlotHelperList[i] = nullptr;
				break;
			case BMShopProductDisplayType::DP_Detail:
				_productListPanelSlotHelperList[i]->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowBMShop::DetailProductListRefresh);
				_productListPanelSlotHelperList[i] = nullptr;
				break;
			}
		}
	}
	_productListPanelSlotHelperList.Empty();

	for (int32 i = 0; i < _bannerListPanelSlotHelperList.Num(); ++i)
	{
		if (nullptr != _bannerListPanelSlotHelperList[i])
		{
			switch (BMShopBannerDisplayType(i))
			{
			case BMShopBannerDisplayType::Banner_SmallType:
				_bannerListPanelSlotHelperList[i]->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowBMShop::SmallBannerProductListRefresh);
				_bannerListPanelSlotHelperList[i] = nullptr;
				break;
			case BMShopBannerDisplayType::Banner_LargeType:
				_bannerListPanelSlotHelperList[i]->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowBMShop::LargeBannerProductListRefresh);
				_bannerListPanelSlotHelperList[i] = nullptr;
				break;
			}
		}
	}
	_bannerListPanelSlotHelperList.Empty();

	if (nullptr != _storageListPanelSlotHelper)
	{
		_storageListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowBMShop::StorageListRefresh);
		_storageListPanelSlotHelper = nullptr;
	}

	_productRootPanelList.Empty();

	Super::BeginDestroy();
}

void UGsUIWindowBMShop::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowBMShop::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowBMShop::OnClickCloseAllStack);
	_btnAllAccept->OnClicked.AddDynamic(this, &UGsUIWindowBMShop::AllAcceptStorageItems);
	_btnProbabilityInfo->OnClicked.AddDynamic(this, &UGsUIWindowBMShop::OnProbabilityInfo);
	_btnBulkSale->OnClicked.AddDynamic(this, &UGsUIWindowBMShop::OnBulkSale);

	_btnInAppPurchaseWarnLink->OnClicked.AddDynamic(this, &UGsUIWindowBMShop::OnPurchaseWarn);
	_btnRestore->OnClicked.AddDynamic(this, &UGsUIWindowBMShop::OnRestore);

	_btnServerMigrateGuideLink->OnClicked.AddDynamic(this, &UGsUIWindowBMShop::OnClickServerMigrateGuide);

	_btnServerMigrateRefresh->OnClicked.AddDynamic(this, &UGsUIWindowBMShop::OnClickServerMigrateDataRefresh);
	_btnServerMigrateRefresh->SetOnClickLockSec(1.f);
	_btnServerMigrateCurrency->OnClickCurrencyButton.BindUObject(this, &UGsUIWindowBMShop::OnClickMigration);
	_btnImpossibleMigrate->OnClicked.AddDynamic(this, &UGsUIWindowBMShop::OnClickImpossibleMigration);

	_toggleGroupMainTab.OnSelectChanged.BindUObject(this, &UGsUIWindowBMShop::OnSelectChangedMainTab);
	_toggleGroupSubTab.OnSelectChanged.BindUObject(this, &UGsUIWindowBMShop::OnSelectChangedSubTab);

	if (nullptr == _mainTabListPanelSlotHelper)
	{
		_mainTabListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_mainTabListPanelSlotHelper->Initialize(_mainTabListEntryWidgetClass, _mainTabRootPanel);
		_mainTabListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowBMShop::MainTabListRefresh);
	}
	if (nullptr == _subTabListPanelSlotHelper)
	{
		_subTabListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_subTabListPanelSlotHelper->Initialize(_subTabListEntryWidgetClass, _subTabRootPanel);
		_subTabListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowBMShop::SubTabListRefresh);
	}
	if (nullptr == _storageListPanelSlotHelper)
	{
		_storageListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_storageListPanelSlotHelper->Initialize(_storageEntryWidgetClass, _storageListRootPanel);
		_storageListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowBMShop::StorageListRefresh);
	}
	if (nullptr == _stepUpPanelSlotHelper)
	{
		_stepUpPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_stepUpPanelSlotHelper->Initialize(_stepUpEntryWidgetClass, _stepUpRootPanel);
		_stepUpPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowBMShop::StepUpListRefresh);
	}
	if (nullptr == _serverMigratePanelSlotHelper)
	{
		_serverMigratePanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_serverMigratePanelSlotHelper->Initialize(_serverMigrateEntryWidgetClass, _serverMigrateRootPanel);
		_serverMigratePanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowBMShop::ServerMigrateGroupListEntryRefresh);
	}
	
	_productRootPanelList.Emplace(_defaultProductRootPanel);
	_productRootPanelList.Emplace(_detailProductRootPanel);

	InitProductPanel(BMShopProductDisplayType::DP_Normal);
	InitProductPanel(BMShopProductDisplayType::DP_Detail);

	InitBannerPanel(BMShopBannerDisplayType::Banner_SmallType);
	InitBannerPanel(BMShopBannerDisplayType::Banner_LargeType);

	_defaultTypeScrollbox->OnUserScrolled.AddDynamic(this, &UGsUIWindowBMShop::OnScrollDefaultType);

	float shopRestoreCooltimeSecondsOnWindows = 5.f;
	if (const UGsGlobalConstant* GlobalConstant = GData()->GetGlobalData())
	{
		shopRestoreCooltimeSecondsOnWindows = GlobalConstant->_shopRestoreCooltimeSecondsOnWindows;
	}
	_btnRestore->_clickLockSecond = shopRestoreCooltimeSecondsOnWindows;
}

void UGsUIWindowBMShop::NativeConstruct()
{
	Super::NativeConstruct();

	FGsBMShopManager* bmShopMgr = GBMShop();
	FGsBMShopManager::SendPopupShopProduct();
	bmShopMgr->GetAllStorageItemsDBID(_storageItemDBIDList);

	CreateMainTab();
	OnChangeSubTab();
	OnChangeProductList();

	_toggleGroupMainTab.SetSelectedIndex(0);
	_toggleGroupSubTab.SetSelectedIndex(0);

	_shopSwitcher->SetActiveWidgetIndex(BMShopDisplayType::Display_ProductListType);

	InvalidateExtraCurrency();
	RegisterMessage();

	CheckZpayModeUI(bmShopMgr);
}

void UGsUIWindowBMShop::NativeDestruct()
{
	_isExcuteFocusingProduct = false;
	_entryWidgetWidthSize = 0.f;

	_toggleGroupMainTab.Clear();
	_toggleGroupSubTab.Clear();

	_storageItemDBIDList.Empty();
	_productIDList.Empty();
	_mainTabDataList.Empty();
	_subTabDataList.Empty();

	_productDisplayType = EGsBMShopDisPlayType::None;

	RemoveMessage();

	_tutorialTabId = 0;
	_tutorialProductId = INVALID_BMPRODUCT_ID;
	OnTutorialListEvent.Unbind();	

	FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr;
	if (handler)
	{
		handler->ClearConditionInfo();
	}

	Super::NativeDestruct();
}

void UGsUIWindowBMShop::InitProductPanel(BMShopProductDisplayType InDPtype)
{
	if (0 >= _productListPanelSlotHelperList.Num())
	{
		_productListPanelSlotHelperList.SetNum((uint8)BMShopProductDisplayType::DP_Max);
	}

	if (_productListPanelSlotHelperList.IsValidIndex((uint8)InDPtype) && _productListEntryWidgetClassList.IsValidIndex((uint8)InDPtype))
	{
		if (nullptr == _productListPanelSlotHelperList[(uint8)InDPtype])
		{
			_productListPanelSlotHelperList[(uint8)InDPtype] = NewObject<UGsDynamicPanelSlotHelper>(this);
			switch (InDPtype)
			{
			case BMShopProductDisplayType::DP_Normal:
				_productListPanelSlotHelperList[(uint8)InDPtype]->Initialize(_productListEntryWidgetClassList[(uint8)InDPtype], _defaultProductRootPanel);
				_productListPanelSlotHelperList[(uint8)InDPtype]->OnRefreshEntry.AddDynamic(this, &UGsUIWindowBMShop::DefaultProductListRefresh);
				break;
			case BMShopProductDisplayType::DP_Detail:
				_productListPanelSlotHelperList[(uint8)InDPtype]->Initialize(_productListEntryWidgetClassList[(uint8)InDPtype], _detailProductRootPanel);
				_productListPanelSlotHelperList[(uint8)InDPtype]->OnRefreshEntry.AddDynamic(this, &UGsUIWindowBMShop::DetailProductListRefresh);
				break;
			}
		}
	}
}

void UGsUIWindowBMShop::InitBannerPanel(BMShopBannerDisplayType InType)
{
	if (0 >= _bannerListPanelSlotHelperList.Num())
	{
		_bannerListPanelSlotHelperList.SetNum((uint8)BMShopBannerDisplayType::Banner_Max);
	}

	if (_bannerListPanelSlotHelperList.IsValidIndex((uint8)InType) && _bannerListEntryWidgetClassList.IsValidIndex((uint8)InType))
	{
		if (nullptr == _bannerListPanelSlotHelperList[(uint8)InType])
		{
			_bannerListPanelSlotHelperList[(uint8)InType] = NewObject<UGsDynamicPanelSlotHelper>(this);
			switch (InType)
			{
			case BMShopBannerDisplayType::Banner_SmallType:
				_bannerListPanelSlotHelperList[(uint8)InType]->Initialize(_bannerListEntryWidgetClassList[(uint8)InType], _smallBannerProductRootPanel);
				_bannerListPanelSlotHelperList[(uint8)InType]->OnRefreshEntry.AddDynamic(this, &UGsUIWindowBMShop::SmallBannerProductListRefresh);
				break;
			case BMShopBannerDisplayType::Banner_LargeType:
				_bannerListPanelSlotHelperList[(uint8)InType]->Initialize(_bannerListEntryWidgetClassList[(uint8)InType], _largeBannerRootPanel);
				_bannerListPanelSlotHelperList[(uint8)InType]->OnRefreshEntry.AddDynamic(this, &UGsUIWindowBMShop::LargeBannerProductListRefresh);
				break;
			}
		}
	}
}

void UGsUIWindowBMShop::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isExcuteFocusingProduct)
	{
		float offsetEnd = 0.f;
		switch (_productDisplayType)
		{
		case EGsBMShopDisPlayType::DefaultType:
			offsetEnd = _defaultTypeScrollbox->GetScrollOffsetOfEnd();
			break;
		case EGsBMShopDisPlayType::DetailType:
			offsetEnd = _detailTypeScrollbox->GetScrollOffsetOfEnd();
			break;

		default:
			_isExcuteFocusingProduct = false;
			break;
		}

		if (1.f < offsetEnd)
		{
			OnFocusingProduct();
		}
	}
	
	//GSLOG(Warning, TEXT("_defaultTypeScrollbox->GetScrollOffset : %f"), _defaultTypeScrollbox->GetScrollOffset());
	//GSLOG(Warning, TEXT("_detailTypeScrollbox->GetScrollOffsetOfEnd : %f"), _detailTypeScrollbox->GetScrollOffsetOfEnd());
}

void UGsUIWindowBMShop::CheckZpayModeUI(FGsBMShopManager* InBMShopMgr)
{
	_btnClose->SetVisibility(ESlateVisibility::Visible);
	_loginUserInfoRootpanel->SetVisibility(ESlateVisibility::Collapsed);

	FGsNetSendServiceBMShop::SendBMShopProductList();

	GMessage()->GetBMShop().SendMessage(FGsBMShopPurchaseRestoreCheck::MESSAGE_ID, FGsSharedParam(nullptr));
}

void UGsUIWindowBMShop::OnClickBack()
{
	Super::OnClickBack();
}

void UGsUIWindowBMShop::DetailProductListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopDetailProductEntry* entry = Cast<UGsUIBMShopDetailProductEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _productIDList.IsValidIndex(InIndex))
		return;

	entry->SetProductID(_productIDList[InIndex]);
	_entryWidgetWidthSize = entry->GetWidthSize();

	const TArray<UPanelSlot*>& slots = _detailProductRootPanel->GetSlots();
	if (UPanelSlot* slot = slots[InIndex])
	{
		if (UGridSlot* gridSlot = Cast<UGridSlot>(slot))
		{
			gridSlot->SetRow(0);
			gridSlot->SetColumn(InIndex);
		}
	}
}

void UGsUIWindowBMShop::DefaultProductListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopProductEntry* entry = Cast<UGsUIBMShopProductEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _productIDList.IsValidIndex(InIndex))
		return;

	entry->SetProductID(_productIDList[InIndex]);
	_entryWidgetWidthSize = entry->GetWidthSize();

	// 두줄 배치
	// 0, 2, 4, ...
	// 1, 3, 5, ...
	const TArray<UPanelSlot*>& slots = _defaultProductRootPanel->GetSlots();
	if (UPanelSlot* slot = slots[InIndex])
	{
		if (UGridSlot* gridSlot = Cast<UGridSlot>(slot))
		{
			gridSlot->SetRow(InIndex % 2);
			gridSlot->SetColumn(FMath::FloorToInt(InIndex / 2));
		}
	}
}

void UGsUIWindowBMShop::SmallBannerProductListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopSBProductEntry* entry = Cast<UGsUIBMShopSBProductEntry>(InEntry);
	if (nullptr == entry)
		return;

	entry->ResetBannerChild();
	entry->SetProduct(_productIDList, _focusingProductID);
	_focusingProductID = 0;
}

void UGsUIWindowBMShop::LargeBannerProductListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopLBProductEntry* entry = Cast<UGsUIBMShopLBProductEntry>(InEntry);
	if (nullptr == entry)
		return;

	entry->ResetChild();
	entry->SetProduct(_productIDList, _focusingProductID);
	_focusingProductID = 0;
}

void UGsUIWindowBMShop::StepUpListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopStepUpProductEntry* entry = Cast<UGsUIBMShopStepUpProductEntry>(InEntry);
	if (nullptr == entry)
		return;

	TArray<FGsBMShopProductData*> stepUpList;
	GBMShop()->FindStepUpProduct(_productIDList, stepUpList);
	entry->SetProduct(stepUpList);
}

void UGsUIWindowBMShop::ServerMigrateGroupListEntryRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIServerMigrateCheckGroupEntry* entry = Cast<UGsUIServerMigrateCheckGroupEntry>(InEntry);
	if (nullptr == entry)
	{
		return;
	}

	if (_migrateData.IsValidIndex(InIndex))
	{
		entry->SetData(_migrateData[InIndex]); 
		entry->RefreshUI();
	}
}

bool UGsUIWindowBMShop::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsBMShop))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);
		return true;
	}

	return false;
}

bool UGsUIWindowBMShop::OnBack()
{
	// 소환 중 Back키 막음. 소환중 닫히며 네비게이션 바가 꼬이는 문제 수정 위함
	if (UGsSummonManager* summonMgr = GSSummon())
	{
		if (FGsSummonHandler* summonHandler = summonMgr->GetSummonHandler())
		{
			if (summonHandler->GetIsSummonPlay())
			{
				return true;
			}
		}
	}

	return Super::OnBack();
}

void UGsUIWindowBMShop::RegisterMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	_listBmShopDelegate.Emplace(msg->GetBMShop().AddUObject(FGsBMShopSalesTimeOver::MESSAGE_ID, this, &UGsUIWindowBMShop::OnFinishedRemainTime));
	_listBmShopDelegate.Emplace(msg->GetBMShop().AddUObject(FGsBMShopInvalidateProduct::MESSAGE_ID, this, &UGsUIWindowBMShop::InvalidateProductItem));
	_listBmShopDelegate.Emplace(msg->GetBMShop().AddUObject(FGsBMShopInvalidateProductList::MESSAGE_ID, this, &UGsUIWindowBMShop::InvalidateProductListItem));
	_listBmShopDelegate.Emplace(msg->GetBMShop().AddUObject(FGsBMShopInvalidateStorage::MESSAGE_ID, this, &UGsUIWindowBMShop::InvalidateStorageList));
	_listBmShopDelegate.Emplace(msg->GetBMShop().AddUObject(FGsBMShopChangeTab::MESSAGE_ID, this, &UGsUIWindowBMShop::MoveToTab));
	_listBmShopDelegate.Emplace(msg->GetBMShop().AddUObject(FGsBMShopChangeTabFromId::MESSAGE_ID, this, &UGsUIWindowBMShop::MoveToTabByTabID));
	_listBmShopDelegate.Emplace(msg->GetBMShop().AddUObject(FGsBMShopChangeFocusingProduct::MESSAGE_ID, this, &UGsUIWindowBMShop::FocusingProduct));
	_listBmShopDelegate.Emplace(msg->GetBMShop().AddUObject(FGsBMShopPopupProductTimeOver::MESSAGE_ID, this, &UGsUIWindowBMShop::PopupShopTimeOverProduct));

	//_listBmShopDelegate.Emplace(msg->GetBMShop().AddUObject(MessageContentStoreBM::INVALIDATE_POPUP_SHOP_PRODUCT,
		//this, &UGsUIWindowBMShop::PopupShopInvalidateProduct));

	//_listItemActionDelegates.Emplace(msg->GetItem().AddUObject(MessageItem::ADDITEM, this, &UGsUIWindowBMShop::OnAddItem));
	//_listItemActionDelegates.Emplace(msg->GetItem().AddUObject(MessageItem::UPDATEITEM, this, &UGsUIWindowBMShop::OnUpdateItem));
	//_listItemActionDelegates.Emplace(msg->GetItem().AddUObject(MessageItem::REMOVEITEM, this, &UGsUIWindowBMShop::OnRemoveItem));

	_userInfoMsgHandler = msg->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIWindowBMShop::OnUpdateCurrency);

	_uiMsgHandler = msg->GetUI().AddUObject(MessageUI::LEVEL_UP,
		this, &UGsUIWindowBMShop::OnUpdateUserLevel);
}

void UGsUIWindowBMShop::RemoveMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (0 < _listBmShopDelegate.Num())
	{
		for (TPair<MessageContentStoreBM, FDelegateHandle>& el : _listBmShopDelegate)
		{
			msg->GetBMShop().Remove(el);
		}
		_listBmShopDelegate.Empty();
	}
	/*
	if (0 < _listItemActionDelegates.Num())
	{
		for (TPair<MessageItem, FDelegateHandle>& el : _listItemActionDelegates)
		{
			msg->GetItem().Remove(el);
		}
		_listItemActionDelegates.Empty();
	}
	*/

	msg->GetUserBasicInfo().Remove(_userInfoMsgHandler);
	msg->GetUI().Remove(_uiMsgHandler);
}

void UGsUIWindowBMShop::CreateMainTab()
{
	FGsBMShopManager* shopMgr =  GBMShop();
	shopMgr->FindMainTabData(false, _mainTabDataList);
	if (0 >= _mainTabDataList.Num())
	{
		GSLOG(Warning, TEXT("[UGsUIWindowBMShop::NativeConstruct()] - _mainTabDataList is empty !!"));
		Close();
	}

	_mainTabListPanelSlotHelper->RefreshAll(_mainTabDataList.Num());

	// 튜토리얼 체크
	CheckTutorialTab();
}

void UGsUIWindowBMShop::OnChangeSubTab()
{
	_subTabDataList.Empty();
	_toggleGroupSubTab.Clear();

	FindSubTabList();
	_subTabStateRootPanel->SetVisibility(0 >= _subTabDataList.Num() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	if (0 >= _subTabDataList.Num()) // sub tab 이 없는 탭
	{
		_subTabListPanelSlotHelper->RefreshAll(0);
		return;
	}

	_subTabListPanelSlotHelper->RefreshAll(_subTabDataList.Num());
	_toggleGroupSubTab.SetSelectedIndex(0);

	// 튜토리얼 체크
	CheckTutorialTab();
}

void UGsUIWindowBMShop::OnChangeProductList()
{
	const int32 mainTabIndex = _toggleGroupMainTab.GetSelectedIndex();
	if (false == _mainTabDataList.IsValidIndex(mainTabIndex))
	{
		GSLOG(Warning, TEXT("[UGsUIWindowBMShop::OnChangeProductList()] is not Valid _mainTabDataList Index"));
		_productIDList.Empty();
		return;
	}

	FGsBMShopManager* mgr = GBMShop();
	if (0 < _subTabDataList.Num())
	{
		const int32 subTabIndex = _toggleGroupSubTab.GetSelectedIndex();
		if (true == _subTabDataList.IsValidIndex(subTabIndex))
		{
			mgr->FindProductIDListByTabInfo(_mainTabDataList[mainTabIndex]->id, _subTabDataList[subTabIndex]->id, _productIDList);
			_productDisplayType = mgr->GetProductDisplayType(_mainTabDataList[mainTabIndex]->id, _subTabDataList[subTabIndex]->id);
		}
	}
	else
	{
		mgr->FindProductIDListByTabInfo(_mainTabDataList[mainTabIndex]->id, _productIDList);
		_productDisplayType = mgr->GetProductDisplayType(_mainTabDataList[mainTabIndex]->id, 0);
	}

	_btnBulkSale->SetVisibility((mgr->HasBulkSaleProduct(_productIDList)) && (_productDisplayType != EGsBMShopDisPlayType::StepUpType) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_switcherProductDP->SetActiveWidgetIndex((uint8)GetConvertProductDisplayType());

	// 튜토리얼일 경우, 튜토리얼 타겟을 제일 앞으로
	if (IsTutorialProduct())
	{
		if (_productIDList.Contains(_tutorialProductId))
		{
			_productIDList.Remove(_tutorialProductId);
		}
		_productIDList.Insert(_tutorialProductId, 0);
	}

	OnRefreshProductList();
}

void UGsUIWindowBMShop::OnFocusingProduct()
{
	bool isFind = false;
	int32 index = 1;
	for (BMProductId iter : _productIDList)
	{
		if (_focusingProductID != iter)
		{
			++index;
			continue;
		}
		isFind = true;
		break;
	}

	if (isFind)
	{
		FVector2D sizeVec = EGsBMShopDisPlayType::DefaultType == _productDisplayType ? 
			_defaultTypeScrollbox->GetDesiredSize() : _detailTypeScrollbox->GetDesiredSize();

		float width = sizeVec.X;
		int32 fractionSize = _entryWidgetWidthSize * 5;
		int32 fraction = width - fractionSize;
		if (0 >= fraction) 
		{
			return;
		}

		int32 resultCalc = EGsBMShopDisPlayType::DefaultType == _productDisplayType ? 
			(ceilf((float)index / 2.f)) * _entryWidgetWidthSize : index * _entryWidgetWidthSize;
		float per = ((float)resultCalc * 100.f) / width;
		int32 roundFloat = ceilf(per);
		float finalPer = ceilf(((float)fraction / 100.f) * (float)roundFloat);

		if (EGsBMShopDisPlayType::DefaultType == _productDisplayType)
		{
			_defaultTypeScrollbox->SetScrollOffset(finalPer);
		}
		else
		{
			_detailTypeScrollbox->SetScrollOffset(finalPer);
		}

		/*
#if WITH_EDITOR
		GSLOG(Warning, TEXT("resultCalc : %d"), resultCalc);
		GSLOG(Warning, TEXT("roundFloat : %d"), roundFloat);
		GSLOG(Warning, TEXT("fraction : %d"), fraction);
		GSLOG(Warning, TEXT("finalPer : %f"), finalPer);
		GSLOG(Warning, TEXT("fractionSize : %d"), fractionSize);
		GSLOG(Warning, TEXT("sizeVec.X : %f"), width);
#endif
*/
	}

	_focusingProductID = 0;
	_isExcuteFocusingProduct = false;
}

UGsUIWindowBMShop::BMShopProductDisplayType UGsUIWindowBMShop::GetConvertProductDisplayType()
{
	switch (_productDisplayType)
	{
	case EGsBMShopDisPlayType::None:
	case EGsBMShopDisPlayType::DefaultType:
		return BMShopProductDisplayType::DP_Normal;
	case EGsBMShopDisPlayType::DetailType:
		return BMShopProductDisplayType::DP_Detail;
	case EGsBMShopDisPlayType::SmallBannerType:
		return BMShopProductDisplayType::DP_SmallBanner;
	case EGsBMShopDisPlayType::LargeBannerType:
		return BMShopProductDisplayType::DP_LargeBanner;
	case EGsBMShopDisPlayType::StepUpType:
		return BMShopProductDisplayType::DP_StepUpBanner;
	case EGsBMShopDisPlayType::ServerMigrate:
		return BMShopProductDisplayType::DP_ServerMigrate;
	}

	return BMShopProductDisplayType::DP_Normal;
}

void UGsUIWindowBMShop::OnRefreshProductList()
{
#if !WITH_EDITOR
	GBMShop()->ValidCheckCashProduct(_productIDList);
#endif

	switch (_productDisplayType)
	{
	case EGsBMShopDisPlayType::None:
	case EGsBMShopDisPlayType::DefaultType:
		if (_productListPanelSlotHelperList.IsValidIndex((uint8)BMShopProductDisplayType::DP_Normal))
		{
			_productListPanelSlotHelperList[(uint8)BMShopProductDisplayType::DP_Normal]->RefreshAll(_productIDList.Num());
			//GSLOG(Warning, TEXT("[NativeTick] - _normalTypeScrollbox->GetScrollOffsetOfEnd : %f"), _normalTypeScrollbox->GetScrollOffsetOfEnd());
		}
		break;
	case EGsBMShopDisPlayType::DetailType:
		if (_productListPanelSlotHelperList.IsValidIndex((uint8)BMShopProductDisplayType::DP_Detail))
		{
			_productListPanelSlotHelperList[(uint8)BMShopProductDisplayType::DP_Detail]->RefreshAll(_productIDList.Num());
			//_longTypeScrollbox->ScrollToEnd();
			//GSLOG(Warning, TEXT("[OnRefreshProductList] - _longTypeScrollbox->GetScrollOffsetOfEnd : %f"), _longTypeScrollbox->GetScrollOffsetOfEnd());
		}
		break;
	case EGsBMShopDisPlayType::SmallBannerType:
		if (_bannerListPanelSlotHelperList.IsValidIndex((uint8)BMShopBannerDisplayType::Banner_SmallType))
		{
			_bannerListPanelSlotHelperList[(uint8)BMShopBannerDisplayType::Banner_SmallType]->RefreshAll(1);
		}
		break;
	case EGsBMShopDisPlayType::LargeBannerType:
		if (_bannerListPanelSlotHelperList.IsValidIndex((uint8)BMShopBannerDisplayType::Banner_LargeType))
		{
			_bannerListPanelSlotHelperList[(uint8)BMShopBannerDisplayType::Banner_LargeType]->RefreshAll(1);
		}
		break;
	case EGsBMShopDisPlayType::StepUpType:
		_stepUpPanelSlotHelper->RefreshAll(1);
		break;
	case EGsBMShopDisPlayType::ServerMigrate:
		BuildViewServerMigrate();
		break;
	}

	CheckTutorialProduct();
}

void UGsUIWindowBMShop::OnInvalidate()
{
	switch (_productDisplayType)
	{
	case EGsBMShopDisPlayType::None:
	case EGsBMShopDisPlayType::DefaultType:
	{
		if (_productListPanelSlotHelperList.IsValidIndex((uint8)BMShopProductDisplayType::DP_Normal))
		{
			UGsDynamicPanelSlotHelper* helper = _productListPanelSlotHelperList[(uint8)BMShopProductDisplayType::DP_Normal];
			if (nullptr == helper)
				return;

			int32 childNum = helper->GetChildrenCount();
			for (int32 i = 0; i < childNum; ++i)
			{
				UGsUIBMShopProductEntry* entry = Cast<UGsUIBMShopProductEntry>(helper->GetEntry(i));
				if (entry)
				{
					if (ESlateVisibility::Collapsed == entry->GetVisibility() || ESlateVisibility::Hidden == entry->GetVisibility())
						continue;

					entry->InvalidateProduct();
				}
			}
		}
		break;
	}
	case EGsBMShopDisPlayType::DetailType:
	{
		if (_productListPanelSlotHelperList.IsValidIndex((uint8)BMShopProductDisplayType::DP_Detail))
		{
			UGsDynamicPanelSlotHelper* helper = _productListPanelSlotHelperList[(uint8)BMShopProductDisplayType::DP_Detail];
			if (nullptr == helper)
				return;

			int32 childNum = helper->GetChildrenCount();
			for (int32 i = 0; i < childNum; ++i)
			{
				UGsUIBMShopDetailProductEntry* entry = Cast<UGsUIBMShopDetailProductEntry>(helper->GetEntry(i));
				if (entry)
				{
					if(ESlateVisibility::Collapsed == entry->GetVisibility() || ESlateVisibility::Hidden == entry->GetVisibility())
						continue;
					
					entry->InvalidateProduct();
				}
			}
		}
		break;
	}
	case EGsBMShopDisPlayType::SmallBannerType:
	{
		if (_bannerListPanelSlotHelperList.IsValidIndex((uint8)BMShopBannerDisplayType::Banner_SmallType))
		{
			UGsDynamicPanelSlotHelper* helper = _bannerListPanelSlotHelperList[(uint8)BMShopBannerDisplayType::Banner_SmallType];
			if (nullptr == helper)
				return;

			int32 childNum = helper->GetChildrenCount();
			for (int32 i = 0; i < childNum; ++i)
			{
				UGsUIBMShopSBProductEntry* entry = Cast<UGsUIBMShopSBProductEntry>(helper->GetEntry(i));
				if (entry)
				{
					entry->InvalidateProduct();
				}
			}
		}
		break;
	}
	case EGsBMShopDisPlayType::LargeBannerType:
	{
		if (_bannerListPanelSlotHelperList.IsValidIndex((uint8)BMShopBannerDisplayType::Banner_LargeType))
		{
			UGsDynamicPanelSlotHelper* helper = _bannerListPanelSlotHelperList[(uint8)BMShopBannerDisplayType::Banner_LargeType];
			if (nullptr == helper)
				return;

			int32 childNum = helper->GetChildrenCount();
			for (int32 i = 0; i < childNum; ++i)
			{
				UGsUIBMShopLBProductEntry* entry = Cast<UGsUIBMShopLBProductEntry>(helper->GetEntry(i));
				if (entry)
				{
					entry->InvalidateProduct();
				}
			}
		}
		break;
	}
	case EGsBMShopDisPlayType::StepUpType:
		_stepUpPanelSlotHelper->RefreshAll(1);
		break;
	case EGsBMShopDisPlayType::ServerMigrate:
		BuildViewServerMigrate(false);
		break;
	}
}

void UGsUIWindowBMShop::BuildViewServerMigrate(bool InIsResetScroll /*= true*/)
{
	FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr;
	if (nullptr == handler)
	{
		GSLOG(Error, TEXT("nullptr == handler"));
		return;
	}

	ServerMigrateType migrateType = handler->ConvertIntToServerMigrateType(_toggleGroupSubTab.GetSelectedIndex());
	if (ServerMigrateType::NONE == migrateType)
	{
		GSLOG(Warning, TEXT("not find Migrate Type"));
		return;
	}

	const static int32 SHOW_CATEGORY_INDOEX = 0;
	const static int32 HIDDEN_CATEGORY_INDOEX = 1;

	// 2023/11/3 PKT - Reset
	_textServerMigrateCount->SetVisibility(ESlateVisibility::Collapsed);			// 2023/11/3 PKT - 계정 통합 카운트
	_btnServerMigrateCurrency->SetVisibility(ESlateVisibility::Collapsed);			// 2023/11/3 PKT - 이주하기 버튼
	_btnImpossibleMigrate->SetVisibility(ESlateVisibility::Collapsed);
	_btnImpossibleMigrate->SetIsEnabled(false);

	// 2023/11/3 PKT - 이주 날짜가 아니거나, 서버에 응답을 기다리고 있는 상태
	_switcherServerMigrateInfo->SetActiveWidgetIndex(HIDDEN_CATEGORY_INDOEX);
	_panelServerMigrateRemainingTime->SetVisibility(ESlateVisibility::HitTestInvisible);

	{	// 2023/10/31 PKT - 이것 저것 text 뽑을게 많아서 지역변수가 늘어남. 쓰고 지우기 위해 중괄호 
		// 2023/11/3 PKT - 현재 계정 서버 이전 기간이 아닙니다.
		FText textDesc;
		FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_UiNotice_Expired"), textDesc);
		FText textTimeMsg = FText::Format(textDesc, handler->GetTextRemainingTime());
		// 2023/11/3 PKT - Message 를 조합해서 출력
		//FText textTimeMsg = FText::FromString(FString::Format(TEXT("{0} {1}"), { *textDesc.ToString(), *handler->GetTextRemainingTime().ToString() }));
		_textServerMigrateRemainingTime->SetText(textTimeMsg);
	}
	
	// 2023/11/3 PKT - 조건 상태를 체크해서 셋팅
	FGsServerMigrateHandler::ETryReason reason = handler->IsCheckedMigrateStateAll(migrateType);

	switch (reason)
	{
	case FGsServerMigrateHandler::ETryReason::Valid:
		break;
	case FGsServerMigrateHandler::ETryReason::InvalidServerList:
		break;
	case FGsServerMigrateHandler::ETryReason::InvalidWaitRequest:
	{
		_panelServerMigrateRemainingTime->SetVisibility(ESlateVisibility::Collapsed);   // 이주 가능한 시간 표기(숨김)
		handler->ReqServerMigrateConditionState();
		return;
	}
	break;
	case FGsServerMigrateHandler::ETryReason::InvalidDate:
	{
		_panelServerMigrateRemainingTime->SetVisibility(ESlateVisibility::Collapsed);   // 이주 가능한 시간 표기(숨김)

		// 2023/10/30 PKT - 현재 계정 서버 이전 기간이 아닙니다. or 현재 기사단 서버 이전기간이 아닙니다.
		FText::FindText(TEXT("ServerMigrateText")
			, (ServerMigrateType::ACCOUNT_ONLY == migrateType) ? TEXT("ServerMigrate_UiNotice_Expired_Account") : TEXT("ServerMigrate_UiNotice_Expired_Guild")
			, _textEmptyList);
		return;
	}
	break;
	case FGsServerMigrateHandler::ETryReason::InvalidCondition:
		break;
	case FGsServerMigrateHandler::ETryReason::InvalidGuildMarster:
	{
		// 2023/10/30 PKT - 기사단장만 이용 할 수 있습니다.
		FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_UiNotice_NotGuildMaster"), _textEmptyList);
		return;
	}
	break;
	case FGsServerMigrateHandler::ETryReason::InvalidOverCount:
		break;
	default:
		break;
	}

	_switcherServerMigrateInfo->SetActiveWidgetIndex(SHOW_CATEGORY_INDOEX);

	// 2023/11/3 PKT - Build View
	_migrateData.Reset();
	int32 maxCount = handler->GetCheckGroups(migrateType, _migrateData);
	_serverMigratePanelSlotHelper->RefreshAll(maxCount);
	// 2023/11/6 PKT - Scroll Reset
	if (InIsResetScroll)
	{
		_serverMigrateRootPanel->SetScrollOffset(0.f);
	}

	{	// 2023/10/31 PKT - 이것 저것 text 뽑을게 많아서 지역변수가 늘어남. 쓰고 지우기 위해 중괄호 

		FText textFormat;
		// 2023/10/30 PKT - 계정당 통합 {0} / {1}
		FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_UiNotice_Buyable_Count"), textFormat);

		FString textMessage = FString::Format(*textFormat.ToString(), { handler->CountInfo().Key, handler->CountInfo().Value });
		_textServerMigrateCount->SetText(FText::FromString(textMessage));
		_textServerMigrateCount->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	/**
		* Set Currency Button
		*/

	FText textButtonName;
	if (ServerMigrateType::ACCOUNT_ONLY == migrateType)
	{	// 2023/11/3 PKT - 계정 서버 이전
		FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Button_Account"), textButtonName);
	}
	else
	{	// 2023/11/3 PKT - 기사단 서버 이전
		FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Button_Guild"), textButtonName);
	}

	const static float LOCK_SEC = 2.f;
	// 2023/11/3 PKT - 현재 필요한 재화량과 재화량을 충분히 소지 하고 있는가?
	const auto& currencyPair = handler->GetCurrencyInfo(migrateType);
	bool isNotEnoughCurrency = (currencyPair.mCurrency > FGsCurrencyHelper::GetCurrencyAmount(currencyPair.mCurrencyType));

	_btnServerMigrateCurrency->SetIsEnabled(FGsServerMigrateHandler::ETryReason::Valid == reason);
	_btnServerMigrateCurrency->SetButtonEnable(FGsServerMigrateHandler::ETryReason::Valid == reason);
	_btnServerMigrateCurrency->SetButtonName(textButtonName);
	_btnServerMigrateCurrency->SetOnClickLockTime(LOCK_SEC);
	_btnServerMigrateCurrency->SetData(currencyPair.mCurrencyType, currencyPair.mCurrency);
	_btnServerMigrateCurrency->SetAmountTextColor((true == isNotEnoughCurrency) ? EGsUIColorType::DEFAULT_INVALID : EGsUIColorType::DEFAULT_VALID);
	_btnServerMigrateCurrency->SetVisibility(ESlateVisibility::Visible);

	if (_btnServerMigrateCurrency->GetIsEnabled() == false)
	{
		_btnImpossibleMigrate->SetVisibility(ESlateVisibility::Visible);
		_btnImpossibleMigrate->SetIsEnabled(true);
	}
}

void UGsUIWindowBMShop::FindSubTabList()
{
	const int32 SelecteIndex = _toggleGroupMainTab.GetSelectedIndex();
	if (true == _mainTabDataList.IsValidIndex(SelecteIndex))
	{
		GBMShop()->FindSubTabDataByMainID(_mainTabDataList[SelecteIndex]->id, _subTabDataList);
	}
}

void UGsUIWindowBMShop::MainTabListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUITabButtonCommon* entry = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _mainTabDataList.IsValidIndex(InIndex))
		return;

	entry->SetTitleText(_mainTabDataList[InIndex]->displayTabNameText);
	entry->SetIsRedDot(EGsIconRedDotType::NORMAL, (true == _mainTabDataList[InIndex]->isStorageTab) && (0 < _storageItemDBIDList.Num()));
	_toggleGroupMainTab.AddToggleWidget(entry->GetToggleGroupEntry());
}

void UGsUIWindowBMShop::SubTabListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUITabButtonCommon* entry = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _subTabDataList.IsValidIndex(InIndex))
		return;

	entry->SetTitleText(_subTabDataList[InIndex]->displayTabNameText);
	_toggleGroupSubTab.AddToggleWidget(entry->GetToggleGroupEntry());
}

void UGsUIWindowBMShop::OnChangeStorageTab()
{
	GBMShop()->GetAllStorageItemsDBID(_storageItemDBIDList);
	_storageListPanelSlotHelper->RefreshAll(_storageItemDBIDList.Num());

	int32 count = _storageItemDBIDList.Num();
	_btnAllAccept->SetIsEnabled(0 < count);
	_switcherStorage->SetActiveWidgetIndex(0 < count ? 0 : 1);
	_storageCountRoot->SetVisibility(0 < count ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (0 < count)
	{
		FText resultText;
		FText findText;
		if (true == FText::FindText(TEXT("BMShopText"), TEXT("storageItemCount"), findText))
		{
			resultText = findText.Format(findText, _storageItemDBIDList.Num());
			_storageCountText->SetText(resultText);
		}
	}

	UpdateStorageTabRedDot();
}

void UGsUIWindowBMShop::StorageListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopStorageEntry* entry = Cast<UGsUIBMShopStorageEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _storageItemDBIDList.IsValidIndex(InIndex))
		return;

	entry->SetData(_storageItemDBIDList[InIndex]);
}

void UGsUIWindowBMShop::UpdateStorageTabRedDot()
{
	uint8 index = 0;
	for (const FGsSchemaBMShopTabInfo* tabInfo : _mainTabDataList)
	{
		if(nullptr == tabInfo)
			continue;

		if (true == tabInfo->isStorageTab)
		{
			_mainTabListPanelSlotHelper->RefreshByIndex(index);
			break;
		}
		++index;
	}
}

void UGsUIWindowBMShop::OnSelectChangedMainTab(int32 InIndex)
{
	const int32 SelecteIndex = _toggleGroupMainTab.GetSelectedIndex();
	if (true == _mainTabDataList.IsValidIndex(SelecteIndex))
	{
		if (true == _mainTabDataList[SelecteIndex]->isStorageTab)
		{
			// 0 : productList , 1 : storage
			_shopSwitcher->SetActiveWidgetIndex(BMShopDisplayType::Display_StorageType);
			OnChangeStorageTab();
		}
		else
		{
			_shopSwitcher->SetActiveWidgetIndex(BMShopDisplayType::Display_ProductListType);
			OnChangeSubTab();
			OnChangeProductList();
		}
	}

	InvalidateExtraCurrency();
}

void UGsUIWindowBMShop::OnSelectChangedSubTab(int32 InIndex)
{
	OnChangeProductList();
	InvalidateExtraCurrency();

	if (EGsBMShopDisPlayType::DefaultType == _productDisplayType)
	{
		_defaultTypeScrollbox->SetScrollOffset(0);
	}
	else
	{
		_detailTypeScrollbox->SetScrollOffset(0);
	}
}

void UGsUIWindowBMShop::OnFinishedRemainTime(FGsSharedParam& InParamData)
{
	const FGsBMShopSalesTimeOver* param = InParamData.GetParam<const FGsBMShopSalesTimeOver>();
	if (nullptr == param)
		return;

	OnRefreshProductList();

	/*
	for (uint8 i = 0; i < _productIDList.Num(); ++i)
	{
		if (_productIDList[i] == param->_productID)
		{
			UGsUIBMShopProductEntry* entry = Cast<UGsUIBMShopProductEntry>(_productListPanelSlotHelper->GetEntry(i));
			if (nullptr == entry)
				continue;

			entry->InvalidateProduct();
		}
	}
	*/
}

void UGsUIWindowBMShop::CheckExtraCurrency()
{
	InvalidateExtraCurrency();
}

void UGsUIWindowBMShop::InvalidateExtraCurrency()
{
	bool findExtraCurrencyType = false;
	const int32 mainIndex = _toggleGroupMainTab.GetSelectedIndex();
	if (true == _mainTabDataList.IsValidIndex(mainIndex))
	{
		if (CurrencyType::NONE != _mainTabDataList[mainIndex]->extraCurrencyType)
		{
			FGsUIMsgParamInt param(static_cast<int32>(_mainTabDataList[mainIndex]->extraCurrencyType));
			GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_CURRENCY, &param);

			findExtraCurrencyType = true;
			return;
		}
		else if(0 < _mainTabDataList[mainIndex]->extraCurrencyItemId)
		{
			FGsUIMsgParamInt param(static_cast<int32>(_mainTabDataList[mainIndex]->extraCurrencyItemId));
			GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_ITEM, &param);

			findExtraCurrencyType = true;
			return;
		}

		if (_mainTabDataList[mainIndex]->isStorageTab)
		{
			FGsUIMsgParamInt param(static_cast<int32>(CurrencyType::NONE));
			GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_CURRENCY, &param);
			return;
		}
	}
	
	const int32 subIndex = _toggleGroupSubTab.GetSelectedIndex();
	if (true == _subTabDataList.IsValidIndex(subIndex))
	{
		if (CurrencyType::NONE != _subTabDataList[subIndex]->extraCurrencyType)
		{
			FGsUIMsgParamInt param(static_cast<int32>(_subTabDataList[subIndex]->extraCurrencyType));
			GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_CURRENCY, &param);

			findExtraCurrencyType = true;
			return;
		}
		else if (0 < _subTabDataList[subIndex]->extraCurrencyItemId)
		{
			FGsUIMsgParamInt param(static_cast<int32>(_subTabDataList[subIndex]->extraCurrencyItemId));
			GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_ITEM, &param);

			findExtraCurrencyType = true;
			return;
		}
	}

	FGsUIMsgParamInt param(static_cast<int32>(CurrencyType::NONE));
	GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_CURRENCY, &param);
}

void UGsUIWindowBMShop::AllAcceptStorageItems()
{
	// 패킷보낼때 0 으로 보내면 보관함에 보유중인 모든 아이템 한번에 찾는걸로 하기로 서버팀과 얘기함
	MakeShared<FGsBMShopAcceptStorage>(0)->Send();
}

void UGsUIWindowBMShop::InvalidateStorageList(FGsSharedParam& InParamData)
{
	OnChangeStorageTab();
}

void UGsUIWindowBMShop::InvalidateProductItem(FGsSharedParam& InParamData)
{
	/*
	const FGsBMShopInvalidate* param = InParamData->Cast<const FGsBMShopInvalidate>();
	if (nullptr == param)
		return;

	OnRefreshProductList();
	*/

	FGsBMShopManager* mgr = GBMShop();
	const int32 mainTabIndex = _toggleGroupMainTab.GetSelectedIndex();
	if (false == _mainTabDataList.IsValidIndex(mainTabIndex))
	{
		GSLOG(Warning, TEXT("[UGsUIWindowBMShop::InvalidateProductListItem()] is not Valid _mainTabDataList Index"));
		return;
	}
	if (0 < _subTabDataList.Num())
	{
		const int32 subTabIndex = _toggleGroupSubTab.GetSelectedIndex();
		if (true == _subTabDataList.IsValidIndex(subTabIndex))
		{
			mgr->FindProductIDListByTabInfo(_mainTabDataList[mainTabIndex]->id, _subTabDataList[subTabIndex]->id, _productIDList);
		}
	}
	else
	{
		mgr->FindProductIDListByTabInfo(_mainTabDataList[mainTabIndex]->id, _productIDList);
	}
	OnRefreshProductList();


	/*
	int32 index = 0;
	for (uint16 id : _productIDList)
	{
		if (id == param->_productID)
		{
			if (_productListPanelSlotHelperList.IsValidIndex((uint8)GetConvertProductDisplayType()))
			{
				_productListPanelSlotHelperList[(uint8)GetConvertProductDisplayType()]->RefreshByIndex(index);
			}
			break;
		}
		++index;
	}
	*/
}

void UGsUIWindowBMShop::InvalidateProductListItem(FGsSharedParam& InParamData)
{
	//GSLOG(Error, TEXT("UGsUIWindowBMShop::InvalidateProductListItem"));

	/*
	const FGsBMShopInvalidateList* param = InParamData->Cast<const FGsBMShopInvalidateList>();
	if (nullptr == param)
		return;
		*/

	FGsBMShopManager* mgr = GBMShop();
	const int32 mainTabIndex = _toggleGroupMainTab.GetSelectedIndex();
	if (false == _mainTabDataList.IsValidIndex(mainTabIndex))
	{
		GSLOG(Warning, TEXT("[UGsUIWindowBMShop::InvalidateProductListItem()] is not Valid _mainTabDataList Index"));
		return;
	}
	if (0 < _subTabDataList.Num())
	{
		const int32 subTabIndex = _toggleGroupSubTab.GetSelectedIndex();
		if (true == _subTabDataList.IsValidIndex(subTabIndex))
		{
			mgr->FindProductIDListByTabInfo(_mainTabDataList[mainTabIndex]->id, _subTabDataList[subTabIndex]->id, _productIDList);
		}
	}
	else
	{
		mgr->FindProductIDListByTabInfo(_mainTabDataList[mainTabIndex]->id, _productIDList);
	}
	OnRefreshProductList();
}

void UGsUIWindowBMShop::MoveToTabByTabID(FGsSharedParam& InParamData)
{
	const FGsBMShopChangeTabFromId* param = InParamData.GetParam<const FGsBMShopChangeTabFromId>();
	if (nullptr == param)
		return;

	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;

	int32 mainTabIndex = 0;
	int32 subTabIndex = 0;
	int32 index = 0;
	int32 targetTabID = param->_tabID;
	if (true == bmShopMgr->IsMainTabID(targetTabID))
	{
		for (const FGsSchemaBMShopTabInfo* mainTabInfo : _mainTabDataList)
		{
			if (nullptr == mainTabInfo)
				continue;

			if (targetTabID == mainTabInfo->id)
			{
				mainTabIndex = index;
				break;
			}
			++index;
		}
		_toggleGroupMainTab.SetSelectedIndex(mainTabIndex);

		if (true == bmShopMgr->hasChildTab(targetTabID))
		{
			_toggleGroupSubTab.SetSelectedIndex(0);
		}
	}
	else
	{
		const FGsSchemaBMShopTabInfo* targetTabData = bmShopMgr->FindSubTabInfoByID(targetTabID);
		if (nullptr == targetTabData)
			return;

		for (const FGsSchemaBMShopTabInfo* mainTabInfo : _mainTabDataList)
		{
			if (nullptr == mainTabInfo)
				continue;

			if (targetTabData->parentTabid == mainTabInfo->id)
			{
				mainTabIndex = index;
				break;
			}
			++index;
		}
		_toggleGroupMainTab.SetSelectedIndex(mainTabIndex);

		index = 0;
		TArray<const FGsSchemaBMShopTabInfo*> subTabInfoList;
		bmShopMgr->FindSubTabDataByMainID(targetTabData->parentTabid, subTabInfoList);
		for (const FGsSchemaBMShopTabInfo* findSubTabInfo : subTabInfoList)
		{
			if (nullptr == findSubTabInfo)
				continue;

			if (targetTabData->id == findSubTabInfo->id)
			{
				subTabIndex = index;
				break;
			}
			++index;
		}
		_toggleGroupSubTab.SetSelectedIndex(subTabIndex);
	}
}

void UGsUIWindowBMShop::FocusingProduct(FGsSharedParam& InParamData)
{
	const FGsBMShopChangeFocusingProduct* param = InParamData.GetParam<const FGsBMShopChangeFocusingProduct>();
	if (nullptr == param)
		return;

	if (0 >= param->_productID)
	{
		_toggleGroupMainTab.SetSelectedIndex(0);
		if (true == GBMShop()->hasChildTab(param->_mainTabID))
		{
			_toggleGroupSubTab.SetSelectedIndex(0);
		}

		return;
	}

	bool isSameProductID = _focusingProductID == param->_productID;
	_focusingProductID = param->_productID;
	int32 index = 0;
	for (const FGsSchemaBMShopTabInfo* mainTabInfo : _mainTabDataList)
	{
		if (nullptr == mainTabInfo)
			continue;

		if (param->_mainTabID == mainTabInfo->id)
		{
			_toggleGroupMainTab.SetSelectedIndex(index);
			break;
		}
		++index;
	}

	if (0 < param->_subTabID)
	{
		index = 0;
		TArray<const FGsSchemaBMShopTabInfo*> subTabInfoList;
		GBMShop()->FindSubTabDataByMainID(param->_mainTabID, subTabInfoList);
		for (const FGsSchemaBMShopTabInfo* findSubTabInfo : subTabInfoList)
		{
			if (nullptr == findSubTabInfo)
				continue;

			if (param->_subTabID == findSubTabInfo->id)
			{
				_toggleGroupSubTab.SetSelectedIndex(index);
				break;
			}
			++index;
		}
	}
	else
	{
		_toggleGroupSubTab.SetSelectedIndex(0);
	}

	if (_productDisplayType == EGsBMShopDisPlayType::DefaultType || _productDisplayType == EGsBMShopDisPlayType::DetailType)
	{
		_entryWidgetWidthSize = 0.f;
		_isExcuteFocusingProduct = true;
	}
}

void UGsUIWindowBMShop::MoveToTab(FGsSharedParam& InParamData)
{
	const FGsBMShopChangeTab* param = InParamData.GetParam<const FGsBMShopChangeTab>();
	if (nullptr == param)
		return;

	if (EGsBMShopSpecialTab::None == param->_tabType)
		return;

	const FGsSchemaBMShopTabInfo* tabInfo = GBMShop()->FindSpecialTabInfo(param->_tabType);
	if (nullptr == tabInfo)
		return;

	int32 mainTabIndex = 0;
	int32 subTabIndex = 0;
	int32 index = 0;
	if (0 >= tabInfo->parentTabid)
	{
		for (const FGsSchemaBMShopTabInfo* mainTabInfo : _mainTabDataList)
		{
			if (nullptr == mainTabInfo)
				continue;

			if (tabInfo->id == mainTabInfo->id)
			{
				mainTabIndex = index;
				break;
			}
			++index;
		}

		_toggleGroupMainTab.SetSelectedIndex(mainTabIndex);
		_toggleGroupSubTab.SetSelectedIndex(0);
	}
	else
	{
		TArray<const FGsSchemaBMShopTabInfo*> subTabInfoList;
		GBMShop()->FindSubTabDataByMainID(tabInfo->parentTabid, subTabInfoList);
		for (const FGsSchemaBMShopTabInfo* mainTabInfo : _mainTabDataList)
		{
			if (nullptr == mainTabInfo)
				continue;

			if (tabInfo->parentTabid == mainTabInfo->id)
			{
				mainTabIndex = index;
				break;
			}
			++index;
		}
		_toggleGroupMainTab.SetSelectedIndex(mainTabIndex);

		index = 0;
		for (const FGsSchemaBMShopTabInfo* findSubTabInfo : subTabInfoList)
		{
			if (nullptr == findSubTabInfo)
				continue;

			if (param->_tabType == findSubTabInfo->specialTabType)
			{
				subTabIndex = index;
				break;
			}
			++index;
		}
		_toggleGroupSubTab.SetSelectedIndex(subTabIndex);
	}
}

void UGsUIWindowBMShop::OnBulkSale()
{
	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;

	TArray<BMProductIdAmountPair> bulkSaleList;
	for (BMProductId iter : _productIDList)
	{
		FGsBMShopProductData* data = bmShopMgr->FindProduct(iter);
		if(nullptr == data)
			continue;

		if (data->IsBulkSale() && false == data->IsCashProductItem())
		{
			BMProductIdAmountPair pair(iter, bmShopMgr->GetBulkSaleAmount(iter));
			bulkSaleList.Emplace(pair);
		}
	}

	MakeShared<FGsBMShopOpenBulkSale>(bulkSaleList)->Send();
	
	/*
	productDataList.Sort([](FGsBMShopProductData& dataA, FGsBMShopProductData& dataB)
		{
			return dataA.GetBannerDisplayOrder() > dataB.GetBannerDisplayOrder();
		}
	);
	*/
}

void UGsUIWindowBMShop::OnAddItem(struct FGsItemMessageParamBase& InParam)
{
	const FGsAddItem* Param = StaticCast<const FGsAddItem*>(&InParam);
	if (nullptr != Param)
	{
		TWeakPtr<FGsItem> findItem = GItem()->FindItem(Param->_itemDBID);
		if (findItem.IsValid())
		{
			if (findItem.Pin()->IsCurrency())
			{
				OnInvalidate();
				//OnRefreshProductList();
			}
		}
	}
}

void UGsUIWindowBMShop::OnUpdateItem(struct FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	if (nullptr != Param)
	{
		TWeakPtr<FGsItem> findItem = GItem()->FindItem(Param->_itemDBID);
		if (findItem.IsValid())
		{
			if (findItem.Pin()->IsCurrency())
			{
				OnInvalidate();
				//OnRefreshProductList();
			}
		}
	}
}

void UGsUIWindowBMShop::OnRemoveItem(struct FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	if (nullptr != Param)
	{
		const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(Param->_itemTID);
		if (itemData && ItemType::CURRENCY == itemData->type)
		{
			OnInvalidate();
			//OnRefreshProductList();
		}
	}
}

void UGsUIWindowBMShop::OnUpdateCurrency(uint64 InData)
{
	OnInvalidate();
	//OnRefreshProductList();
}

void UGsUIWindowBMShop::OnUpdateUserLevel(const struct IGsMessageParam* inParam)
{
	OnInvalidate();
	//OnRefreshProductList();
}

void UGsUIWindowBMShop::PopupShopTimeOverProduct(FGsSharedParam& InMessageParam)
{
	/*
	const FGsBMShopPopupProductTimeOver* param = InMessageParam->Cast<const FGsBMShopPopupProductTimeOver>();
	if (nullptr == param)
		return;
		*/

	FGsBMShopManager* mgr = GBMShop();
	const int32 mainTabIndex = _toggleGroupMainTab.GetSelectedIndex();
	if (false == _mainTabDataList.IsValidIndex(mainTabIndex))
	{
		GSLOG(Warning, TEXT("[UGsUIWindowBMShop::PopupShopInvalidateProduct()] is not Valid _mainTabDataList Index"));
		return;
	}
	if (0 < _subTabDataList.Num())
	{
		const int32 subTabIndex = _toggleGroupSubTab.GetSelectedIndex();
		if (true == _subTabDataList.IsValidIndex(subTabIndex))
		{
			mgr->FindProductIDListByTabInfo(_mainTabDataList[mainTabIndex]->id, _subTabDataList[subTabIndex]->id, _productIDList);
		}
	}
	else
	{
		mgr->FindProductIDListByTabInfo(_mainTabDataList[mainTabIndex]->id, _productIDList);
	}
	OnRefreshProductList();
}

void UGsUIWindowBMShop::PopupShopInvalidateProduct(FGsSharedParam& InMessageParam)
{
	FGsBMShopManager* mgr = GBMShop();
	const int32 mainTabIndex = _toggleGroupMainTab.GetSelectedIndex();
	if (false == _mainTabDataList.IsValidIndex(mainTabIndex))
	{
		GSLOG(Warning, TEXT("[UGsUIWindowBMShop::PopupShopInvalidateProduct()] is not Valid _mainTabDataList Index"));
		return;
	}
	if (0 < _subTabDataList.Num())
	{
		const int32 subTabIndex = _toggleGroupSubTab.GetSelectedIndex();
		if (true == _subTabDataList.IsValidIndex(subTabIndex))
		{
			mgr->FindProductIDListByTabInfo(_mainTabDataList[mainTabIndex]->id, _subTabDataList[subTabIndex]->id, _productIDList);
		}
	}
	else
	{
		mgr->FindProductIDListByTabInfo(_mainTabDataList[mainTabIndex]->id, _productIDList);
	}
	OnRefreshProductList();
}

void UGsUIWindowBMShop::OnProbabilityInfo()
{
	if (!GHive()->IsSignIn()) 
		return;

	// 확률 정보 표시
	FGsUIHelper::OpenHelpPage(EGsHelpContents::BMShopProbability);

	/*
	FString contentsKey = TEXT("300203");	
	Promotion::showCustomContents(PromotionCustomType::VIEW, TCHAR_TO_UTF8(*contentsKey), [](ResultAPI result, PromotionEventType promotionEventType) {

			if (result.isSuccess())
			{
				switch (promotionEventType) 
				{
				case PromotionEventType::OPEN:   break; //block 필요하다면 처리
				case PromotionEventType::CLOSE:  break; //block 해지 필요하다면 처리
				}
			}
			else
			{

			}

		});
	//FPlatformProcess::LaunchURL(TEXT("http://terms.withhive.com/terms/policy/view/M31"), nullptr, nullptr);
	*/
}

void UGsUIWindowBMShop::SetTutorialTab(int32 InTabId)
{
	_tutorialTabId = InTabId;

	if (0 == _tutorialTabId)
	{
		OnTutorialListEvent.Unbind();
	}
	else
	{
		// 리스트 찾기
		CheckTutorialTab();
	}
}

void UGsUIWindowBMShop::SetTutorialProduct(BMProductId InProductId)
{
	_tutorialProductId = InProductId;

	if (INVALID_BMPRODUCT_ID == _tutorialProductId)
	{
		OnTutorialListEvent.Unbind();

		_defaultTypeScrollbox->SetAllowOverscroll(true);
	}
	else
	{
		_defaultTypeScrollbox->SetAllowOverscroll(false);
		_defaultTypeScrollbox->ScrollToStart();

		OnChangeProductList();
		//CheckTutorialProduct();
	}
}

void UGsUIWindowBMShop::CheckTutorialTab()
{
	if (false == IsTutorialTab())
	{
		return;
	}

	// id에 해당하는 탭 찾기
	int targetIndex = -1;

	for (int i = 0; i < _mainTabDataList.Num(); ++i)
	{
		if (_mainTabDataList[i]->id == _tutorialTabId)
		{
			targetIndex = i;
			break;
		}
	}

	if (0 < targetIndex)
	{
		if (UWidget* entry = _mainTabListPanelSlotHelper->GetEntry(targetIndex))
		{
			OnTutorialListEvent.ExecuteIfBound(entry);
			OnTutorialListEvent.Unbind();
		}

		return;
	}

	for (int i = 0; i < _subTabDataList.Num(); ++i)
	{
		if (_subTabDataList[i]->id == _tutorialTabId)
		{
			targetIndex = i;
			break;
		}
	}

	if (0 < targetIndex)
	{
		if (UWidget* entry = _subTabListPanelSlotHelper->GetEntry(targetIndex))
		{
			OnTutorialListEvent.ExecuteIfBound(entry);
			OnTutorialListEvent.Unbind();
		}
	}
}

void UGsUIWindowBMShop::CheckTutorialProduct()
{
	if (false == IsTutorialProduct())
	{
		return;
	}

	int targetIndex = -1;
	for (int i = 0; i < _productIDList.Num(); ++i)
	{
		if (_productIDList[i] == _tutorialProductId)
		{
			targetIndex = i;
			break;
		}
	}

	if (-1 == targetIndex)
	{
		return;
	}

	if (EGsBMShopDisPlayType::DefaultType != _productDisplayType)
	{
		return;
	}

	UGsDynamicPanelSlotHelper* slotHelper = nullptr;
	uint8 helerIndex = static_cast<uint8>(BMShopProductDisplayType::DP_Normal);
	if (_productListPanelSlotHelperList.IsValidIndex(helerIndex))
	{
		slotHelper = _productListPanelSlotHelperList[helerIndex];
	}

	if (nullptr == slotHelper)
	{
		return;
	}

	if (UWidget* entry = slotHelper->GetEntry(targetIndex))
	{
		OnTutorialListEvent.ExecuteIfBound(entry);
		OnTutorialListEvent.Unbind();
	}
}

void UGsUIWindowBMShop::OnScrollDefaultType(float InCurrentOffset)
{
	if (IsTutorialProduct())
	{
		_defaultTypeScrollbox->SetScrollOffset(0);
	}
}

void UGsUIWindowBMShop::OnPurchaseWarn()
{
	if (!GHive()->IsSignIn()) 
		return;

	FGsUIHelper::OpenHelpPage(EGsHelpContents::BMShopPurchaseWarn);

	/*
	FString contentsKey = TEXT("300145"); // 샌박 콘솔 - 게임 인덱스 1047 커스텀뷰 Id
	if (Configuration::getZone() == ZoneType::REAL)
	{
		contentsKey = TEXT("300156");
	}

	Promotion::showCustomContents(PromotionCustomType::VIEW, TCHAR_TO_UTF8(*contentsKey), [](ResultAPI result, PromotionEventType promotionEventType) {

		if (result.isSuccess())
		{
			switch (promotionEventType)
			{
			case PromotionEventType::OPEN:   break; //block 필요하다면 처리
			case PromotionEventType::CLOSE:  break; //block 해지 필요하다면 처리
			}
		}
		else
		{
		}

		});
	//FPlatformProcess::LaunchURL(TEXT("https://terms.withhive.com/terms/policy/view/M31"), nullptr, nullptr);
	*/
}

void UGsUIWindowBMShop::OnRestore()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	// restore 
	MakeShared<FGsBMShopUserCheckRestore>()->Send();
#elif PLATFORM_WINDOWS
	FGsBMShopManager* BMShopManager = GBMShop();
	if (nullptr == BMShopManager || !BMShopManager->OnClickUserPgComplete(true))
	{
		GSLOG(Error, TEXT("nullptr == BMShopManager || !BMShopManager->OnClickUserPgComplete(true)"));
		return;
	}
#endif
}

void UGsUIWindowBMShop::OnClickServerMigrateGuide()
{
	if (EGsBMShopDisPlayType::ServerMigrate != _productDisplayType)
	{
		GSLOG(Error, TEXT("EGsBMShopDisPlayType::ServerMigrate != _productDisplayType"));
		return;
	}

	const FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr;
	if (nullptr == handler)
	{
		return;
	}

	ServerMigrateType subCategoryType = handler->ConvertIntToServerMigrateType(_toggleGroupSubTab.GetSelectedIndex());
	if (ServerMigrateType::NONE == subCategoryType)
	{
		return;
	}

	EGsHelpContents helpContents = handler->FindHelpContetns(subCategoryType);
	if (EGsHelpContents::None != helpContents)
	{
		FGsUIHelper::OpenHelpPage(helpContents);
	}
}

void UGsUIWindowBMShop::OnClickServerMigrateDataRefresh()
{
	if (EGsBMShopDisPlayType::ServerMigrate != _productDisplayType)
	{
		GSLOG(Error, TEXT("EGsBMShopDisPlayType::ServerMigrate != _productDisplayType"));
		return;
	}

	FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr;
	if (nullptr == handler)
	{
		return;
	}

	handler->ReqServerMigrateConditionState();
}

void UGsUIWindowBMShop::OnClickMigration()
{
	if (FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr)
	{
		ServerMigrateType migrateType = (_migrateData.IsValidIndex(0) && _migrateData[0].IsValid()) ? 
			_migrateData[0].Pin()->_type : ServerMigrateType::NONE ;

		if (migrateType == ServerMigrateType::NONE) return;

		auto reason = handler->IsCheckedMigrateStateAll(migrateType);
		if (reason == FGsServerMigrateHandler::ETryReason::Valid)
		{
			if (auto server = handler->GetSelectedServer().Pin())
			{
				if (server->_userCount > 0)
				{
					if (auto popup = Cast<UGsUIPopupMigrationUserInit>(GUI()->OpenAndGetWidget(TEXT("PopupMigrationUserInit"))))
					{
						UGsUIPopupMigrationUserInit::InitParam param = { server->_userCount };
						TWeakObjectPtr<UGsUIWindowBMShop> thiz = this;
						popup->SetInit(&param, [thiz, migrateType]() {
								if(thiz.IsValid()) thiz->ServerMigrateConfirm(migrateType);
							});
					}
				}
				else
				{
					ServerMigrateConfirm(migrateType);
				}
			}
		}
		else if (reason == FGsServerMigrateHandler::ETryReason::InvalidOverCount)
		{
			FText message;
			FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Notice_NotEnoughCount"), message);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
		}
	}
}

void UGsUIWindowBMShop::OnClickImpossibleMigration()
{
	if (FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr)
	{
		ServerMigrateType migrateType = handler->ConvertIntToServerMigrateType(_toggleGroupSubTab.GetSelectedIndex());
		FGsServerMigrateHandler::ETryReason reason = handler->IsCheckedMigrateStateAll(migrateType);

		if (reason != FGsServerMigrateHandler::ETryReason::Valid)
		{
			if (reason == FGsServerMigrateHandler::ETryReason::InvalidOverCount)
			{
				FText message;
				FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Notice_NotEnoughCount"), message);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
			}
			else
			{
				FText message;
				FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Notice_NotEnoughCheckList"), message);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
			}
		}
	}
}

void UGsUIWindowBMShop::ServerMigrateConfirm(ServerMigrateType type)
{
	if (FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr)
	{
		if (auto popup = Cast<UGsUIPopupMigrationReqConfirm>(GUI()->OpenAndGetWidget(TEXT("PopupMigrationReqConfirm"))))
		{
			UGsUIPopupMigrationReqConfirm::InitParam param = { type };
			TWeakObjectPtr<UGsUIWindowBMShop> thiz = this;
			popup->SetInit(&param, [type]() {
				if (FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr)
				{
					handler->SendServerMigrate(type);
				}
			});
		}

	}
}

void UGsUIWindowBMShop::ServerMigrateViewRefresh()
{
	if (EGsBMShopDisPlayType::ServerMigrate != _productDisplayType)
	{
		GSLOG(Error, TEXT("EGsBMShopDisPlayType::ServerMigrate != _productDisplayType"));
		return;
	}

	// 2023/10/31 PKT - 화면 갱신
	BuildViewServerMigrate(false);
}