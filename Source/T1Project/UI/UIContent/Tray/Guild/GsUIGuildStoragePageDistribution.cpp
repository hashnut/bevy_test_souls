//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsUIGuildStoragePageDistribution.h"
//#include "Components/WidgetSwitcher.h"
//#include "Components/TextBlock.h"
//#include "Components/Image.h"
//#include "Components/ListView.h"
//#include "Guild/GsGuildStorageData.h"
//#include "Guild/GsGuildHelper.h"
//#include "Management/ScopeGlobal/GsGuildManager.h"
//#include "Management/ScopeGlobal/GsUIManager.h"
//#include "Message/MessageParam/GsMessageParam.h"
//#include "Net/GsNetSendServiceGuild.h"
//#include "Item/GsItem.h"
//#include "Item/GsItemEquipment.h"
//#include "Shared/Client/SharedEnums/SharedItemEnum.h"
//#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
//#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
//#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
//#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
//#include "UI/UIContent/Tray/GsUITrayGuildStorage.h"
//#include "UI/UIContent/Tray/Guild/GsGuildStorageDistributionListItem.h"
//#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
//#include "UI/UIContent/Popup/GsUIPopupCommonItemAction.h"
//#include "UI/UIContent/Helper/GsUIColorHelper.h"
//#include "UI/UIContent/Helper/GsUIHelper.h"
//#include "UI/UIContent/Helper/GsUIStringHelper.h"
//#include "UI/UILib/Base/GsUIWidgetBase.h"
//
//
//void UGsUIGuildStoragePageDistribution::BeginDestroy()
//{
//	_filterList.Empty();
//
//	Super::BeginDestroy();
//}
//
//void UGsUIGuildStoragePageDistribution::NativeOnInitialized()
//{
//	Super::NativeOnInitialized();
//
//	_btnFilterLevel->OnClicked.AddDynamic(this, &UGsUIGuildStoragePageDistribution::OnClickFilterLevel);
//	_btnFilterName->OnClicked.AddDynamic(this, &UGsUIGuildStoragePageDistribution::OnClickFilterName);
//	_btnFilterMemberGrade->OnClicked.AddDynamic(this, &UGsUIGuildStoragePageDistribution::OnClickFilterMemberGrade);
//	_btnFilterContribution->OnClicked.AddDynamic(this, &UGsUIGuildStoragePageDistribution::OnClickFilterContribution);
//	_btnFilterAccumContribution->OnClicked.AddDynamic(this, &UGsUIGuildStoragePageDistribution::OnClickFilterAccumContribution);
//
//	_filterList.Emplace(_btnFilterLevel);
//	_filterList.Emplace(_btnFilterName);
//	_filterList.Emplace(_btnFilterMemberGrade);
//	_filterList.Emplace(_btnFilterContribution);
//	_filterList.Emplace(_btnFilterAccumContribution);
//
//	_btnMinus->OnClicked.AddDynamic(this, &UGsUIGuildStoragePageDistribution::OnClickedCalcMinus1);
//	_btnPlus->OnClicked.AddDynamic(this, &UGsUIGuildStoragePageDistribution::OnClickedCalcPlus1);
//	_btnCalcOpen->OnClicked.AddDynamic(this, &UGsUIGuildStoragePageDistribution::OnClickedCalcOpen);
//	_btnDistribution->OnClicked.AddDynamic(this, &UGsUIGuildStoragePageDistribution::OnClickDistribution);
//
//	_listView->OnItemClicked().AddUObject(this, &UGsUIGuildStoragePageDistribution::OnClickedMember);
//
//	_calculateHelper.SetMin(GUILD_STORAGE_DISTRIBUTION_AMOUNT_MIN);
//}
//
//void UGsUIGuildStoragePageDistribution::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	// 주의: 순서주의. 소팅함수 연결
//	_sortFuncList.Empty();
//	_sortFuncList.Emplace([this](int32 InState)
//		{
//			SortByLevel(InState);
//		});
//	_sortFuncList.Emplace([this](int32 InState)
//		{
//			SortByName(InState);
//		});
//	_sortFuncList.Emplace([this](int32 InState)
//		{
//			SortByMemberGrade(InState);
//		});
//	_sortFuncList.Emplace([this](int32 InState)
//		{
//			SortByContribution(InState);
//		});
//	_sortFuncList.Emplace([this](int32 InState)
//		{
//			SortByAccumContribution(InState);
//		});
//
//	_currFilter = EGsGuildStorageFilterType::AccumContribution;
//
//	_calculateHelper._onChanged.Unbind();
//	_calculateHelper.SetCurrent(GUILD_STORAGE_DISTRIBUTION_AMOUNT_MIN);
//	_calculateHelper._onChanged.BindUObject(this, &UGsUIGuildStoragePageDistribution::OnChangedCalculateHelper);
//}
//
//void UGsUIGuildStoragePageDistribution::NativeDestruct()
//{
//	_dataList.Empty();
//	_sortFuncList.Empty();
//	_listView->ClearListItems();
//
//	if (nullptr != _iconSelector)
//	{
//		_iconSelector->RemoveAllChildren();
//	}
//	_uiIcon = nullptr;
//
//	Super::NativeDestruct();
//}
//
//void UGsUIGuildStoragePageDistribution::SetStorageUI(UGsUITrayGuildStorage* InStorageUI)
//{
//	_storageUI = InStorageUI;
//}
//
//FGsGuildStorageMemberData* UGsUIGuildStoragePageDistribution::GetSelectedMemberData()
//{
//	if (_dataList.IsValidIndex(_selectedIndex) && true == _dataList[_selectedIndex]->GetIsSelected())
//	{
//		return _dataList[_selectedIndex]->GetData();
//	}
//
//	return nullptr;
//}
//
//void UGsUIGuildStoragePageDistribution::InvalidateAllInternal()
//{
//	// 0 은 선택되지 않은 상태
//	int32 index = 0;
//	if (_filterList.IsValidIndex(_currFilter))
//	{
//		index = _filterList[_currFilter]->GetSwitcherIndex();
//	}
//
//	// 0 상태는 없도록 한다
//	index = (0 == index) ? 1 : index;
//
//	SetFilter(_currFilter, index);
//}
//
//void UGsUIGuildStoragePageDistribution::InvalidateDistribution()
//{
//	// 기사단 창고에 대한 권한이 없습니다.
//	uint32 myAuthority = GGuild()->GetAuthority();
//	if (false == FGsGuildHelper::CanDistribute(myAuthority))
//	{
//		_switcherDistribution->SetActiveWidgetIndex(1);
//		_imageLine->SetVisibility(ESlateVisibility::Collapsed);
//		_btnDistribution->SetVisibility(ESlateVisibility::Collapsed);
//
//		FText findText;
//		FText::FindText(TEXT("GuildText"), TEXT("GuildSafe_UI_NoAuthority"), findText);
//		_textBlockDistributionError->SetText(findText);
//	}
//	else
//	{
//		// 선택된 아이템이 있다.
//		if (_storageUI->IsItemSelected())
//		{
//			_switcherDistribution->SetActiveWidgetIndex(0);
//			_imageLine->SetVisibility(ESlateVisibility::Visible);
//			_btnDistribution->SetVisibility(ESlateVisibility::Visible);
//			_btnDistribution->SetIsEnabled(true);
//
//			InvalidateDistributionItem();
//			InvalidateCalculator();
//		}
//		// 선택된 아이템이 없다.
//		else
//		{
//			_switcherDistribution->SetActiveWidgetIndex(1);
//			_imageLine->SetVisibility(ESlateVisibility::Visible);
//			_btnDistribution->SetVisibility(ESlateVisibility::Visible);
//			_btnDistribution->SetIsEnabled(false);
//
//			TMap<ItemDBId, FGsItem*>& itemList = GGuild()->GetMyGuildStorageData()->GetItemList();
//			// 분배 가능한 아이템이 존재하지 않습니다.
//			if (0 == itemList.Num())
//			{
//				FText findText;
//				FText::FindText(TEXT("GuildText"), TEXT("GuildSafe_UI_HaveNoItem"), findText);
//				_textBlockDistributionError->SetText(findText);
//			}
//			// 우측의 인벤토리에서 분배할 아이템을 선택해 주세요.
//			else
//			{
//				FText findText;
//				FText::FindText(TEXT("GuildText"), TEXT("GuildSafe_UI_ItemNotSelected"), findText);
//				_textBlockDistributionError->SetText(findText);
//			}
//		}
//	}
//}
//
//void UGsUIGuildStoragePageDistribution::InvalidateDistributionItem()
//{
//	if (nullptr == _uiIcon)
//	{
//		_uiIcon = Cast<UGsUIIconItemInventory>(_iconSelector->CreateIcon());
//		_iconSelector->AddChild(_uiIcon);
//	}
//
//	if (nullptr == _uiIcon)
//		return;
//
//	FGsItem* selectedItem = _storageUI->GetSelectedItem();
//	if (nullptr == selectedItem)
//		return;
//
//	if (ItemType::EQUIP == selectedItem->GetType())
//	{
//		const FGsItemEquipment* equipData = selectedItem->GetCastItem<FGsItemEquipment>();
//		if (nullptr == equipData)
//			return;
//
//		_uiIcon->SetNotOwnItem(equipData->GetTID(), equipData->GetAmount(), equipData->GetLevel(), equipData->GetEnchantShieldCount(), equipData->GetEnchantBonusIndex());
//	}
//	else
//	{
//		_uiIcon->SetNotOwnItem(selectedItem->GetTID(), selectedItem->GetAmount(), selectedItem->GetLevel());
//	}
//
//	_uiIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::NotUseDetailWndType);
//	_uiIcon->SetItemNameVisibility(false);
//
//	FGsItem* itemData = _uiIcon->GetItemIconData();
//	if (nullptr == itemData)
//		return;
//
//	itemData->SetDBID(selectedItem->GetDBID());
//
//	_textBlockItemName->SetText(selectedItem->GetName());
//	_textBlockItemName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(selectedItem->GetGradeType()));
//}
//
//void UGsUIGuildStoragePageDistribution::InvalidateCalculator()
//{
//	if (nullptr == _storageUI)
//		return;
//
//	FGsItem* selectedItem = _storageUI->GetSelectedItem();
//	if (nullptr == selectedItem)
//	{
//		InvalidateDistribution();
//		return;
//	}
//
//	const ItemAmount maxAmount = FMath::Min((selectedItem->GetAmount()), selectedItem->GetMaxStackCount());
//	_calculateHelper.SetMax(maxAmount);
//
//	_btnMinus->SetIsEnabled(!_calculateHelper.IsMin());
//	_btnPlus->SetIsEnabled(!_calculateHelper.IsMax());
//	_btnCalcOpen->SetIsEnabled(_calculateHelper.GetMin() != _calculateHelper.GetMax());
//
//	FText itemNameWithCount;
//	FGsUIStringHelper::GetTwoValueSlashedText(_calculateHelper.GetCurrent(), maxAmount, itemNameWithCount);
//	_textBlockAmount->SetText(itemNameWithCount);
//}
//
//void UGsUIGuildStoragePageDistribution::Show()
//{
//	_selectedIndex = 0;
//	SetSelectedIndex(0, false);
//
//	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
//
//	InvalidateAll();
//}
//
//void UGsUIGuildStoragePageDistribution::OpenPopupCalculator()
//{
//	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCalculator"));
//	if (false == widget.IsValid())
//		return;
//
//	UGsUIPopupCalculator* popupCalculator = Cast<UGsUIPopupCalculator>(widget.Get());
//	if (nullptr == popupCalculator)
//		return;
//
//	popupCalculator->SetDimmed(true);
//	popupCalculator->SetResetCount();
//	popupCalculator->SetDefaultCount(_calculateHelper.GetMin());
//	popupCalculator->SetMinimumCount(_calculateHelper.GetMin());
//	popupCalculator->SetMaximumCount(_calculateHelper.GetMax());
//	popupCalculator->InvalidateNumText();
//
//	if (!popupCalculator->OnFinishCalculateDelegate.IsBoundToObject(this))
//	{
//		popupCalculator->OnFinishCalculateDelegate.AddUObject(this, &UGsUIGuildStoragePageDistribution::OnFinishCalculatePopupCalculator);
//	}
//}
//
//void UGsUIGuildStoragePageDistribution::OnClickFilterLevel()
//{
//	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
//	int32 index = (1 == _btnFilterLevel->GetSwitcherIndex()) ? 2 : 1;
//	SetFilter(EGsGuildStorageFilterType::Level, index);
//}
//
//void UGsUIGuildStoragePageDistribution::OnClickFilterName()
//{
//	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
//	int32 index = (1 == _btnFilterName->GetSwitcherIndex()) ? 2 : 1;
//	SetFilter(EGsGuildStorageFilterType::Name, index);
//}
//
//void UGsUIGuildStoragePageDistribution::OnClickFilterMemberGrade()
//{
//	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
//	int32 index = (1 == _btnFilterMemberGrade->GetSwitcherIndex()) ? 2 : 1;
//	SetFilter(EGsGuildStorageFilterType::MemberGrade, index);
//}
//
//void UGsUIGuildStoragePageDistribution::OnClickFilterContribution()
//{
//	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
//	int32 index = (1 == _btnFilterContribution->GetSwitcherIndex()) ? 2 : 1;
//	SetFilter(EGsGuildStorageFilterType::Contribution, index);
//}
//
//void UGsUIGuildStoragePageDistribution::OnClickFilterAccumContribution()
//{
//	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
//	int32 index = (1 == _btnFilterAccumContribution->GetSwitcherIndex()) ? 2 : 1;
//	SetFilter(EGsGuildStorageFilterType::AccumContribution, index);
//}
//
//void UGsUIGuildStoragePageDistribution::OnClickedCalcMinus1()
//{
//	_calculateHelper.Add(-1);
//}
//
//void UGsUIGuildStoragePageDistribution::OnClickedCalcPlus1()
//{
//	_calculateHelper.Add(1);
//}
//
//void UGsUIGuildStoragePageDistribution::OnClickedCalcOpen()
//{
//	OpenPopupCalculator();
//}
//
//void UGsUIGuildStoragePageDistribution::OnChangedCalculateHelper(int64 InNumber)
//{
//	InvalidateCalculator();
//}
//
//void UGsUIGuildStoragePageDistribution::OnFinishCalculatePopupCalculator(int64 InValue)
//{
//	_calculateHelper.SetCurrent(InValue);
//}
//
//void UGsUIGuildStoragePageDistribution::OnClickDistribution()
//{
//	TWeakObjectPtr<UGsUIWidgetBase> popup = GUI()->OpenAndGetWidget(TEXT("PopupCommonItemAction"));
//	if (false == popup.IsValid())
//		return;
//
//	_distributionUI = Cast<UGsUIPopupCommonItemAction>(popup.Get());
//	if (nullptr == _distributionUI)
//		return;
//
//	if (nullptr == _uiIcon)
//		return;
//
//	FText textDesc;
//	FText::FindText(TEXT("GuildText"), TEXT("GuildSafe_Popup_CheckDistribution"), textDesc);
//	int64 amount = _calculateHelper.GetCurrent();
//	_uiIcon->GetItemIconData()->SetAmount(1);
//	_distributionUI->SetItem(_uiIcon->GetItemIconData(), amount, true);
//	_uiIcon->GetItemIconData()->SetAmount(_calculateHelper.GetMax());
//	_distributionUI->SetDescription(FText::Format(textDesc, FText::FromString(_storageUI->GetSelectedUserName())));
//	_distributionUI->SetCallBack([this, amount](bool state)
//		{
//			if (true == state)
//			{
//				FText errorText;
//				if (CheckDistribution(amount, errorText))
//				{
//					FGsItem* distributeItem = _uiIcon->GetItemIconData();
//					FGsNetSendServiceGuild::SendReqGuildStorageDistributeItem(_storageUI->GetSelectedUserDBId(),
//						distributeItem->GetTID(), distributeItem->GetDBID(), amount);
//				}
//				else
//				{
//					FGsUIHelper::PopupOK(errorText);
//				}
//
//				_storageUI->InvalidateInventory();
//			}
//		});
//	_distributionUI->SetCurrencyVisibility(false);
//}
//
//bool UGsUIGuildStoragePageDistribution::CheckDistribution(int64 InAmount, OUT FText& OutError)
//{
//	// 유저가 분배 시점에서 기사단원이 아닌 경우
//	bool selectedUserExist = false;
//	FGsGuildStorageData* myGuildStorageData = GGuild()->GetMyGuildStorageData();
//	TArray<FGsGuildStorageMemberData*>& memberList = myGuildStorageData->_memberList;
//	for (int32 i = 0; i < memberList.Num(); ++i)
//	{
//		if (nullptr == memberList[i])
//			continue;
//
//		if (_storageUI->GetSelectedUserDBId() != INVALID_USER_DB_ID &&
//			memberList[i]->_userDBId == _storageUI->GetSelectedUserDBId())
//		{
//			selectedUserExist = true;
//			break;
//		}
//	}
//	if (false == selectedUserExist)
//	{
//		// 해당 유저를 기사단원 목록에서 찾을 수 없습니다
//		FText::FindText(TEXT("GuildText"), TEXT("GuildSafe_Popup_NotInList"), OutError);
//		return false;
//	}
//
//	// 아이템이 존재하지 않거나 수량이 부족한 경우
//	FGsItem* distributeItem = _uiIcon->GetItemIconData();
//	FGsItem* selectedItem = _storageUI->GetSelectedItem();
//	if (nullptr == distributeItem ||
//		nullptr == selectedItem ||
//		distributeItem->GetDBID() != selectedItem->GetDBID() ||
//		distributeItem->GetAmount() < InAmount)
//	{
//		// 분배할 아이템의 수량이 부족합니다.
//		FText::FindText(TEXT("GuildText"), TEXT("GuildSafe_Popup_NotEnoughAmount"), OutError);
//		return false;
//	}
//
//	// 기사단 창고에 대한 권한이 없습니다.
//	uint32 myAuthority = GGuild()->GetAuthority();
//	if (false == FGsGuildHelper::CanDistribute(myAuthority))
//	{
//		FText::FindText(TEXT("GuildText"), TEXT("GuildSafe_Popup_NoAuthority"), OutError);
//		return false;
//	}
//
//	return true;
//}
//
//void UGsUIGuildStoragePageDistribution::SetFilter(EGsGuildStorageFilterType InType, int32 InState)
//{
//	_currFilter = InType;
//
//	SetUISelectFilter(InType, InState);
//
//	// 기사단원 정보에 변화가 있다면 데이터 갱신
//	FGsGuildStorageData* myGuildStorageData = GGuild()->GetMyGuildStorageData();
//	if (true == myGuildStorageData->GetDirtyMember() || 0 == _dataList.Num())
//	{
//		myGuildStorageData->SetDirtyMember(false);
//		_dataList.Empty();
//
//		TArray<FGsGuildStorageMemberData*>& memberList = myGuildStorageData->_memberList;
//		for (uint8 i = 0; i < memberList.Num(); ++i)
//		{
//			if (nullptr == memberList[i])
//				continue;
//
//			UGsGuildStorageDistributionListItem* member = NewObject<UGsGuildStorageDistributionListItem>(this);
//			member->SetData(memberList[i]);
//			_dataList.Emplace(member);
//		}
//	}
//
//	// 정렬
//	if (_sortFuncList.IsValidIndex(InType))
//	{
//		_sortFuncList[InType](InState);
//	}
//
//	// 변경된 정렬에 맞추어 선택 위치 갱신
//	_selectedIndex = 0;
//	for (uint8 i = 0; i < _dataList.Num(); ++i)
//	{
//		if (true == _dataList[i]->GetIsSelected())
//		{
//			_selectedIndex = i;
//			break;
//		}
//	}
//
//	// 설정
//	_listView->SetListItems(_dataList);
//	SetSelectedIndex(_selectedIndex, true);
//}
//
//void UGsUIGuildStoragePageDistribution::SortByLevel(int32 InState)
//{
//	// 1: 내림차순, 2: 오름차순
//	if (1 == InState)
//	{
//		_dataList.Sort([](UGsGuildStorageDistributionListItem& A, UGsGuildStorageDistributionListItem& B)
//			{
//				return (A.GetData()->_level >= B.GetData()->_level) ? true : false;
//			});
//	}
//	else
//	{
//		_dataList.Sort([](UGsGuildStorageDistributionListItem& A, UGsGuildStorageDistributionListItem& B)
//			{
//				return (A.GetData()->_level < B.GetData()->_level) ? true : false;
//			});
//	}
//}
//
//void UGsUIGuildStoragePageDistribution::SortByName(int32 InState)
//{
//	// 1: 내림차순, 2: 오름차순
//	if (1 == InState)
//	{
//		_dataList.Sort([](UGsGuildStorageDistributionListItem& A, UGsGuildStorageDistributionListItem& B)
//			{
//				return (A.GetData()->_userName >= B.GetData()->_userName) ? true : false;
//			});
//	}
//	else
//	{
//		_dataList.Sort([](UGsGuildStorageDistributionListItem& A, UGsGuildStorageDistributionListItem& B)
//			{
//				return (A.GetData()->_userName < B.GetData()->_userName) ? true : false;
//			});
//	}
//}
//
//void UGsUIGuildStoragePageDistribution::SortByMemberGrade(int32 InState)
//{
//	// 1: 내림차순, 2: 오름차순
//	if (1 == InState)
//	{
//		_dataList.Sort([](UGsGuildStorageDistributionListItem& A, UGsGuildStorageDistributionListItem& B)
//			{
//				return (A.GetData()->_memberGrade >= B.GetData()->_memberGrade) ? true : false;
//			});
//	}
//	else
//	{
//		_dataList.Sort([](UGsGuildStorageDistributionListItem& A, UGsGuildStorageDistributionListItem& B)
//			{
//				return (A.GetData()->_memberGrade < B.GetData()->_memberGrade) ? true : false;
//			});
//	}
//}
//
//void UGsUIGuildStoragePageDistribution::SortByContribution(int32 InState)
//{
//	// 1: 내림차순, 2: 오름차순
//	if (1 == InState)
//	{
//		_dataList.Sort([](UGsGuildStorageDistributionListItem& A, UGsGuildStorageDistributionListItem& B)
//			{
//				return (A.GetData()->_weeklyContribution >= B.GetData()->_weeklyContribution) ? true : false;
//			});
//	}
//	else
//	{
//		_dataList.Sort([](UGsGuildStorageDistributionListItem& A, UGsGuildStorageDistributionListItem& B)
//			{
//				return (A.GetData()->_weeklyContribution < B.GetData()->_weeklyContribution) ? true : false;
//			});
//	}
//}
//
//void UGsUIGuildStoragePageDistribution::SortByAccumContribution(int32 InState)
//{
//	// 1: 내림차순, 2: 오름차순
//	if (1 == InState)
//	{
//		_dataList.Sort([](UGsGuildStorageDistributionListItem& A, UGsGuildStorageDistributionListItem& B)
//			{
//				return (A.GetData()->_accumContribution >= B.GetData()->_accumContribution) ? true : false;
//			});
//	}
//	else
//	{
//		_dataList.Sort([](UGsGuildStorageDistributionListItem& A, UGsGuildStorageDistributionListItem& B)
//			{
//				return (A.GetData()->_accumContribution < B.GetData()->_accumContribution) ? true : false;
//			});
//	}
//}
//
//void UGsUIGuildStoragePageDistribution::SetUISelectFilter(EGsGuildStorageFilterType InType, int32 InState)
//{
//	int32 index = static_cast<int32>(InType);
//
//	for (int32 i = 0; i < _filterList.Num(); ++i)
//	{
//		UGsSwitcherButton* btn = _filterList[i];
//
//		if (i == index)
//		{
//			btn->SetSwitcherIndex(InState);
//		}
//		else
//		{
//			btn->SetSwitcherIndex(0);
//		}
//	}
//}
//
//void UGsUIGuildStoragePageDistribution::OnChangeMyAuthority()
//{
//	InvalidateDistribution();
//}
//
//void UGsUIGuildStoragePageDistribution::OnClickedMember(UObject* InListItem)
//{
//	UGsGuildStorageDistributionListItem* item = Cast<UGsGuildStorageDistributionListItem>(InListItem);
//	if (nullptr != item)
//	{
//		int32 index = _listView->GetIndexForItem(item);
//		if (-1 != index)
//		{
//			SetSelectedIndex(index, true);
//			return;
//		}
//	}
//
//	if (0 != _dataList.Num())
//	{
//		SetSelectedIndex(0, false);
//	}
//}
//
//void UGsUIGuildStoragePageDistribution::SetSelectedIndex(int32 InIndex, bool bInForced)
//{
//	int32 prevIndex = _selectedIndex;
//	_selectedIndex = InIndex;
//
//	// 항상 하나만 선택되도록 처리한다
//	for (int32 i = 0; i < _dataList.Num(); ++i)
//	{
//		UGsGuildStorageDistributionListItem* item = _dataList[i];
//		if (i == InIndex)
//		{
//			item->SetIsSelected(true);
//		}
//		else
//		{
//			item->SetIsSelected(false);
//		}
//	}
//	_listView->SetSelectedIndex(InIndex);
//
//	// 강제로 이벤트를 발생시킬 때
//	if (bInForced)
//	{
//		InvalidateDistribution();
//	}
//	else
//	{
//		// 달라졌을 때
//		if (prevIndex != _selectedIndex)
//		{
//			InvalidateDistribution();
//		}
//	}
//}
//
//void UGsUIGuildStoragePageDistribution::InitializeCalculator()
//{
//	_calculateHelper.SetCurrent(GUILD_STORAGE_DISTRIBUTION_AMOUNT_MIN);
//}
