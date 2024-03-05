//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsUITrayGuildStorage.h"
//#include "Components/Image.h"
//#include "Components/WidgetSwitcher.h"
//
//#include "Guild/GsGuildStorageData.h"
//
//#include "Management/ScopeGlobal/GsGuildManager.h"
//#include "Management/GsMessageHolder.h"
//
//#include "Message/MessageParam/GsGuildMessageParam.h"
//
//#include "Net/GsNetSendServiceGuild.h"
//
//#include "Item/GsItem.h"
//#include "UI/UIControlLib/Define/GsIconDefine.h"
//#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
//#include "UI/UIContent/Helper/GsRedDotChecker.h"
//#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
//#include "UI/UIContent/Common/Inventory/GsUIInventoryCommon.h"
//#include "UI/UIContent/Common/Inventory/GsInventoryLogicGuildStorage.h"
//#include "UI/UIContent/Tray/Guild/GsUIGuildStoragePageDistribution.h"
//#include "UI/UIContent/Tray/Guild/GsUIGuildStoragePageAcquireDetail.h"
//#include "UI/UIContent/Tray/Guild/GsUIGuildStoragePageDistributionDetail.h"
//
//
//void UGsUITrayGuildStorage::BeginDestroy()
//{
//	delete _inventoryLogic;
//	_inventoryLogic = nullptr;
//
//	Super::BeginDestroy();
//}
//
//void UGsUITrayGuildStorage::NativeOnInitialized()
//{
//	Super::NativeOnInitialized();
//
//	_btnBack->OnClicked.AddDynamic(this, &UGsUITrayGuildStorage::OnClickBack);
//
//	_toggleGroup.AddToggleWidgetByParentPanel(_panelMainTab);
//	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUITrayGuildStorage::OnClickMainTab);
//
//	_pageList.Empty();
//	_pageList.Emplace(_pageDistribution);
//	_pageList.Emplace(_pageAcquireDetail);
//	_pageList.Emplace(_pageDistributionDetail);
//
//	// 레드닷 체크 함수 설정
//	_redDotCheckerList.Empty();
//	_redDotCheckerList.Emplace(FGsRedDotChecker(nullptr, EGsIconRedDotType::NORMAL, nullptr));
//	_redDotCheckerList.Emplace(FGsRedDotChecker(_uiRedDotAcquired, []()
//		{
//			if (FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData())
//			{
//				if (storageData->GetDirtyAcquired())
//				{
//					return true;
//				}
//
//				FGsItem* item = nullptr;
//				for (uint8 i = 0; i < storageData->_acquireDetailList.Num(); ++i)
//				{
//					item = storageData->_acquireDetailList[i]->_acquiredItem;
//					if (item->IsNew())
//					{
//						return true;
//					}
//				}
//			}
//			return false;
//		}));
//	_redDotCheckerList.Emplace(FGsRedDotChecker(_uiRedDotDistributed, []()
//		{
//			if (FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData())
//			{
//				if (storageData->GetDirtyDistributed())
//				{
//					return true;
//				}
//
//				FGsItem* item = nullptr;
//				for (uint8 i = 0; i < storageData->_distributionDetailList.Num(); ++i)
//				{
//					item = storageData->_distributionDetailList[i]->_distributedItem;
//					if (item->IsNew())
//					{
//						return true;
//					}
//				}
//			}
//			return false;
//		}));
//
//	_inventoryLogic = new FGsInventoryLogicGuildStorage();
//}
//
//void UGsUITrayGuildStorage::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	RegisterMessages();
//
//	UpdateRedDot(EGsGuildStorageMainTab::All);
//
//	_pageDistribution->SetStorageUI(this);
//	_toggleGroup.SetSelectedIndex(0, true);
//
//	_inventoryLogic->OnNativeConstruct();
//	_inventoryUI->SetProPerty(EInventoryCommonProperty::InventoryRedDot);
//	_inventoryUI->SetData(_inventoryLogic);
//	_inventoryUI->OnClickItem().BindUObject(this, &UGsUITrayGuildStorage::OnClickInventoryItem);
//
//	_tabPrev = EGsGuildStorageMainTab::Max;
//	_switcherPage->SetActiveWidgetIndex(static_cast<int32>(EGsGuildStorageMainTab::Distribution));
//
//	InvalidateAll();
//}
//
//void UGsUITrayGuildStorage::NativeDestruct()
//{
//	DeregisterMessages();	
//
//	_tabPrev = EGsGuildStorageMainTab::Max;
//
//	FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData();
//	storageData->ResetData();
//	storageData->UpdateInvenCheckTime();
//
//	// 레드닷 체크
//	GGuild()->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::GuildStorage);
//
//	// 레드닷 갱신
//	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true); // HUD 갱신
//	GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_CLOSE, nullptr); // Guild Window 갱신
//
//	// 아이템 상세정보 종료
//	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, nullptr);
//
//	Super::NativeDestruct();
//}
//
//void UGsUITrayGuildStorage::NativeTick(const FGeometry& InMyGeometry, float InDeltaTime)
//{
//	Super::NativeTick(InMyGeometry, InDeltaTime);
//
//	IGsInvalidateUIInterface::CheckInvalidate();
//
//	_inventoryLogic->CheckExpiredItem();
//}
//
//void UGsUITrayGuildStorage::InvalidateAllInternal()
//{
//	SetUICurrentPage(static_cast<int32>(GetCurrentTab()));
//}
//
//FReply UGsUITrayGuildStorage::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
//{
//	return FReply::Handled();
//}
//
//FReply UGsUITrayGuildStorage::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
//{
//	return FReply::Handled();
//}
//
//FReply UGsUITrayGuildStorage::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
//{
//	return FReply::Handled();
//}
//
//void UGsUITrayGuildStorage::OnClickBack()
//{
//	Close();
//}
//
//void UGsUITrayGuildStorage::RegisterMessages()
//{
//	MGuild& msgGuild = GMessage()->GetGuild();
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::NOTIFY_CHANGE_MEMBER_GRADE,
//		this, &UGsUITrayGuildStorage::OnNotifyChangeMemberGrade));
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::NOTIFY_JOIN_MEMBER,
//		this, &UGsUITrayGuildStorage::OnNotifyJoinMember));
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_REMOVE_MEMBER,
//		this, &UGsUITrayGuildStorage::OnReceiveRemoveMember));
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_CHANGE_MY_AUTHORITY,
//		this, &UGsUITrayGuildStorage::OnChangeMyAuthority));
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_NOTIFY_ACQUIRED,
//		this, &UGsUITrayGuildStorage::OnNotifyAcquireDetail));
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_NOTIFY_DISTRIBUTED,
//		this, &UGsUITrayGuildStorage::OnNotifyDistributeDetail));
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_EXTEND_SLOT,
//		this, &UGsUITrayGuildStorage::OnExtendSlot));
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_INVALIDATE_PAGE,
//		this, &UGsUITrayGuildStorage::OnInvalidatePage));
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_INVALIDATE_INVENTORY,
//		this, &UGsUITrayGuildStorage::OnInvalidateInventory));
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_INVALIDATE_DISTRIBUTION,
//		this, &UGsUITrayGuildStorage::OnInvalidateDistribution));
//	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_INVALIDATE_ALL_DATA,
//		this, &UGsUITrayGuildStorage::OnInvalidateAllData));
//
//}
//
//void UGsUITrayGuildStorage::DeregisterMessages()
//{
//	FGsMessageHolder* msgMgr = GMessage();
//
//	MGuild& msgGuild = msgMgr->GetGuild();
//	for (MsgGuildHandle& handle : _msgGuildHandleList)
//	{
//		msgGuild.Remove(handle);
//	}
//	_msgGuildHandleList.Empty();
//}
//
//void UGsUITrayGuildStorage::SetUICurrentPage(int32 InIndex)
//{
//	// 이전 페이지 나가기 처리
//	if (EGsGuildStorageMainTab::Max != _tabPrev)
//	{
//		if (_pageList.IsValidIndex(_tabPrev))
//		{
//			if (UGsUIGuildStoragePageBase* prevPage = _pageList[_tabPrev])
//			{
//				prevPage->Hide();
//			}
//		}
//
//		// 이전 Hide에서 레드닷 확인에 대한 처리를 진행함
//		// 확인이 되었으면 레드닷 제거를 위해 Update 호출 
//		if (_redDotCheckerList.IsValidIndex(_tabPrev))
//		{
//			_redDotCheckerList[_tabPrev].UpdateRedDot();
//		}
//	}
//
//	_switcherPage->SetActiveWidgetIndex(InIndex);
//
//	if (_pageList.IsValidIndex(InIndex))
//	{
//		if (UGsUIGuildStoragePageBase* page = _pageList[InIndex])
//		{
//			page->Show();
//
//			_tabPrev = static_cast<EGsGuildStorageMainTab>(InIndex);
//		}
//	}
//
//	//현재 Show의 레드닷 확인에 대한 처리를 진행함
//	// 확인이 되었으면 레드닷 제거를 위해 Update 호출 
//	if (_redDotCheckerList.IsValidIndex(_tabPrev))
//	{
//		_redDotCheckerList[_tabPrev].UpdateRedDot();
//	}
//}
//
//UGsUIGuildStoragePageBase* UGsUITrayGuildStorage::GetCurrentPage() const
//{
//	int32 index = _switcherPage->GetActiveWidgetIndex();
//	if (_pageList.IsValidIndex(index))
//	{
//		return _pageList[index];
//	}
//
//	return nullptr;
//}
//
//UGsUITrayGuildStorage::EGsGuildStorageMainTab UGsUITrayGuildStorage::GetCurrentTab() const
//{
//	return static_cast<EGsGuildStorageMainTab>(_switcherPage->GetActiveWidgetIndex());
//}
//
//void UGsUITrayGuildStorage::UpdateRedDot(EGsGuildStorageMainTab InType)
//{
//	// Max값이면 전부 업데이트
//	if (EGsGuildStorageMainTab::All == InType)
//	{
//		for (FGsRedDotChecker& iter : _redDotCheckerList)
//		{
//			iter.UpdateRedDot();
//		}
//
//		return;
//	}
//
//	int32 index = InType;
//	if (_redDotCheckerList.IsValidIndex(index))
//	{
//		_redDotCheckerList[InType].UpdateRedDot();
//	}
//}
//
//void UGsUITrayGuildStorage::OnClickMainTab(int32 InIndex)
//{
//	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, nullptr);
//	SetUICurrentPage(InIndex);
//}
//
//void UGsUITrayGuildStorage::OnClickInventoryItem(ItemDBId InItemDBId)
//{
//	if (EGsGuildStorageMainTab::Distribution == GetCurrentTab())
//	{
//		_pageDistribution->InitializeCalculator();
//		_pageDistribution->InvalidateDistribution();
//	}
//	else
//	{
//		_toggleGroup.SetSelectedIndex(static_cast<int32>(EGsGuildStorageMainTab::Distribution), true);
//	}
//
//	_inventoryUI->InvalidateInvenTabRedDot();
//}
//
//void UGsUITrayGuildStorage::OnNotifyChangeMemberGrade(const IGsMessageParam* InParam)
//{
//	FGsNetSendServiceGuild::SendReqGuildStorageMemberList();
//}
//
//void UGsUITrayGuildStorage::OnNotifyJoinMember(const IGsMessageParam* InParam)
//{
//	FGsNetSendServiceGuild::SendReqGuildStorageMemberList();
//}
//
//void UGsUITrayGuildStorage::OnReceiveRemoveMember(const IGsMessageParam* InParam)
//{
//	FGsNetSendServiceGuild::SendReqGuildStorageMemberList();
//}
//
//void UGsUITrayGuildStorage::OnChangeMyAuthority(const IGsMessageParam* InParam)
//{
//	if (EGsGuildStorageMainTab::Distribution == GetCurrentTab())
//	{
//		_pageDistribution->OnChangeMyAuthority();
//	}
//}
//
//void UGsUITrayGuildStorage::OnNotifyAcquireDetail(const IGsMessageParam* InParam)
//{
//	FGsNetSendServiceGuild::SendReqGuildStorageAcquiredHistory();
//	FGsNetSendServiceGuild::SendReqGuildStorageItemList();
//	
//	UpdateRedDot(EGsGuildStorageMainTab::AcquireDetail);
//}
//
//void UGsUITrayGuildStorage::OnNotifyDistributeDetail(const IGsMessageParam* InParam)
//{
//	FGsNetSendServiceGuild::SendReqGuildStorageDistributedHistory();
//	FGsNetSendServiceGuild::SendReqGuildStorageItemList();
//
//	UpdateRedDot(EGsGuildStorageMainTab::DistributionDetail);
//}
//
//void UGsUITrayGuildStorage::OnExtendSlot(const IGsMessageParam* InParam)
//{
//	if (const FGsGuildMsgParamStorageExtendSlot* param = InParam->Cast<const FGsGuildMsgParamStorageExtendSlot>())
//	{
//		_inventoryUI->ChangeInvenSlotCount(param->_maxSlotCount, param->_slotExtendedCount);
//	}
//}
//
//void UGsUITrayGuildStorage::OnInvalidatePage(const IGsMessageParam* InParam)
//{
//	if (UGsUIGuildStoragePageBase* page = GetCurrentPage())
//	{
//		page->InvalidateAll();
//	}
//
//	UpdateRedDot(GetCurrentTab());
//}
//
//void UGsUITrayGuildStorage::OnInvalidateInventory(const IGsMessageParam* InParam)
//{
//	_inventoryUI->InvalidateData();
//	_inventoryLogic->UpdateSelectedItem();
//}
//
//void UGsUITrayGuildStorage::OnInvalidateDistribution(const IGsMessageParam* InParam)
//{
//	_inventoryLogic->UpdateSelectedItem();
//
//	if (EGsGuildStorageMainTab::Distribution == GetCurrentTab())
//	{
//		_pageDistribution->InvalidateDistribution();
//	}
//}
//
//void UGsUITrayGuildStorage::OnInvalidateAllData(const IGsMessageParam* InParam)
//{
//	FGsNetSendServiceGuild::SendReqGuildStorageDistributedHistory();
//	FGsNetSendServiceGuild::SendReqGuildStorageItemList();
//	FGsNetSendServiceGuild::SendReqGuildStorageMemberList();
//}
//
//void UGsUITrayGuildStorage::InvalidateInventory()
//{
//	OnInvalidateInventory(nullptr);
//}
//
//bool UGsUITrayGuildStorage::IsItemSelected()
//{
//	return (INVALID_ITEM_DB_ID != _inventoryLogic->GetSelectedItemDBId()) ? true : false;
//}
//
//FGsItem* UGsUITrayGuildStorage::GetSelectedItem()
//{
//	ItemDBId selectedItemDBId = _inventoryLogic->GetSelectedItemDBId();
//	int32 selectedItemIndex = _inventoryLogic->GetDataIndex(selectedItemDBId);
//	return _inventoryLogic->GetDataByIndex(selectedItemIndex);
//}
//
//FString UGsUITrayGuildStorage::GetSelectedUserName()
//{
//	if (EGsGuildStorageMainTab::Distribution == GetCurrentTab())
//	{
//		FGsGuildStorageMemberData* memberData = _pageDistribution->GetSelectedMemberData();
//		if (nullptr != memberData)
//		{
//			return memberData->_userName;
//		}
//	}
//
//	return TEXT("");
//}
//
//UserDBId UGsUITrayGuildStorage::GetSelectedUserDBId()
//{
//	if (EGsGuildStorageMainTab::Distribution == GetCurrentTab())
//	{
//		FGsGuildStorageMemberData* memberData = _pageDistribution->GetSelectedMemberData();
//		if (nullptr != memberData)
//		{
//			return memberData->_userDBId;
//		}
//	}
//
//	return INVALID_USER_DB_ID;
//}
//
//void UGsUITrayGuildStorage::OnInputCancel()
//{
//	Close();
//}
