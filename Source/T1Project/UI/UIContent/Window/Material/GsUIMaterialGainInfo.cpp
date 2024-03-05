
#include "GsUIMaterialGainInfo.h"
#include "Unlock/GsUnlockDefine.h"
#include "../Item/GsItem.h"
#include "../Item/GsItemManager.h"
#include "../Item/Craft/GsCraftTarget.h"
#include "../Item/Collection/EItemCollectionEnumData.h"
#include "../Item/Collection/GsItemCollectionField.h"
#include "../Item/Collection/GsItemCollectionCondition.h"

#include "../Management/GsScopeGame.h"
#include "../Management/GsScopeHolder.h"
#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGame/GsCraftManager.h"
#include "../Management/ScopeGame/GsSupplyManager.h"
#include "../Management/ScopeGame/GsItemCollectionManager.h"
#include "../Management/ScopeGame/GsUnlockManager.h"
#include "../Management/ScopeGlobal/GsUIManager.h"

#include "../Message/GsMessageContents.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/MessageParam/GsEnchantMessageParam.h"
#include "../Message/MessageParam/GsItemContentsMessageParam.h"

#include "../UI/UIContent/Window/Material/GsUIMaterialGainInfoMarketPrice.h"
#include "../UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropListContainer.h"
#include "../UI/UIContent/Window/Material/GsUIMaterialSetGainInfoCraftMaterialList.h"
#include "../UI/UIContent/Popup/ItemCollection/GsUIPopupItemCollectionAlternateList.h"

#include "../UI/UILib/Base/GsUIWidgetBase.h"
#include "../UI/UIContent/Common/Item/GsUIItemDefaultInfo.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "../UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "../UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "../../Popup/GsUIPopupCommonItemAction.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/RichTextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "SupplyHelper/GsDropSupplyHelper.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "Market/GsMarketHelper.h"

#include "../Net/GsNetSendServiceWorld.h"

#include "T1Project.h"



void UGsUIMaterialGainInfo::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIMaterialGainInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// 2021/08/02 PKT - 메인 탭
	_toggleGroupMainTabs.OnSelectChanged.BindUObject(this, &UGsUIMaterialGainInfo::OnChangedToogleMainTab);

	_btnGoEnchant->OnClicked.AddDynamic(this, &UGsUIMaterialGainInfo::OnGoEnchant);
	_btnGoItemCollection->OnClicked.AddDynamic(this, &UGsUIMaterialGainInfo::OnGoItemCollection);
	_btnGoMarket->OnClicked.AddDynamic(this, &UGsUIMaterialGainInfo::OnGoMarket);
	_btnGoCraft->OnClicked.AddDynamic(this, &UGsUIMaterialGainInfo::OnGoCraft);
	
	_btnItemCollectionSubstituteMaterials->OnClicked.AddDynamic(this, &UGsUIMaterialGainInfo::OnClickedItemCollectionSubstituteMaterials);
	_btnItemCollectionRegist->OnClicked.AddDynamic(this, &UGsUIMaterialGainInfo::OnClickedItemCollectionRegister);
}

void UGsUIMaterialGainInfo::NativeConstruct()
{
	Super::NativeConstruct();

	/**
	 * Init..
	 */
	_driveOwner = DriveOwner::None;
	_data = nullptr;
	_materialId = INVALID_ITEM_ID;
	_level = 0;
	
	_toggleGroupMainTabs.Clear();
	_toggleGroupMainTabs.AddToggleWidget(_btnMarketDetailTab);
	_toggleGroupMainTabs.AddToggleWidget(_btnCraftDetailTab);
	_toggleGroupMainTabs.AddToggleWidget(_btnDropNPCDetailTab);
}

void UGsUIMaterialGainInfo::InvalidateMarket()
{
	const FGsItemCollectionCondition* conditionData = reinterpret_cast<const FGsItemCollectionCondition*>(_data);
	if (nullptr == conditionData)
	{
		GSLOG(Error, TEXT("nullptr == conditionData"));
		return;
	}

	if (const FGsItemCollectionField* findField = GSItemCollection()->FindFieldByConditionId(conditionData->Id()))
	{
		if (UGsUIMaterialGainInfoMarketPrice* marketPage =
			Cast<UGsUIMaterialGainInfoMarketPrice>(_switcherDetailInfo->GetWidgetAtIndex(0)))
		{
			// 2023/1/18 PKT - Field / Material Id / Material Level
			marketPage->SetData(findField->Id(), conditionData->EntryMaterialInfo().Key, conditionData->EntryMaterialInfo().Value);
		}
	}
}

void UGsUIMaterialGainInfo::InvalidateCraft()
{
	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	TSharedPtr<const FGsCostPackage> CostPackage;
	if (INVALID_ITEM_ID == _materialId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_ID == _materialId"));
	}
	else
	{
		const FGsCraftTarget* CraftTarget = CraftManager->FindAvaialbleCraftTarget(_materialId);
		if (nullptr != CraftTarget)
		{
			CostPackage = CraftTarget->GetCostPackage();
		}
	}

	_detailInfoCraft->SetParameters({
		CostPackage
	});
}

void UGsUIMaterialGainInfo::InvalidateDropNPC()
{
	if (INVALID_ITEM_ID == _materialId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_ID == _materialId"));
		return;
	}

	TArray<FGsSchemaWorldMapMonsterElementDataRow> NavigationList;
	FGsDropSupplyHelper::MakeNavigationList(NavigationList, _materialId);

	FSimpleDelegate ClickAnyNavigationButtonDelegate = FSimpleDelegate::CreateUObject(this, &UGsUIMaterialGainInfo::OnClickItemDropNavigateButton);
	_detailInfoDropNPC->SetParameters(MoveTemp(ClickAnyNavigationButtonDelegate), MoveTemp(NavigationList));
}

void UGsUIMaterialGainInfo::SetController(DriveOwner InOwner)
{
	if (DriveOwner::None != InOwner)
	{
		_switcherController->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		switch (InOwner)
		{
		case DriveOwner::ItemCollection:
		{
			static const int32 SWITCHER_ITEM_COLLECTION_INDEX = 0;
			_switcherController->SetActiveWidgetIndex(SWITCHER_ITEM_COLLECTION_INDEX);
		}	break;
		}
	}
	else
	{
		// 2023/1/18 PKT - Controller가 설정되어 있지 않다면 버튼을 숨긴다. ( 영역은 차지 )
		_switcherController->SetVisibility(ESlateVisibility::Hidden);
	}

	Invalidate();
}

void UGsUIMaterialGainInfo::InvalidateItemCollectionController()
{	
	_btnItemCollectionSubstituteMaterials->SetIsEnabled(false);
	_btnItemCollectionRegist->SetIsEnabled(false);

	bool isSubstituteMaterialsEnable = false;
	bool isRegistEnable = false;

	do 
	{
		/**
		 * 초기값 버튼 비활성화
		 */
		const FGsItemCollectionCondition* conditionData = reinterpret_cast<const FGsItemCollectionCondition*>(_data);
		if (nullptr == conditionData)
		{
			GSLOG(Error, TEXT("nullptr == conditionData"));
			break;
		}

		if (ECollectionMaterialActionType::Completed == conditionData->ActionType())
		{	// 2023/1/18 PKT - 이미 완료 상태라면 활성화 하지 않는다.
			break;
		}

		const FGsItemCollectionField* findField = GSItemCollection()->FindFieldByConditionId(conditionData->Id());
		if (nullptr == findField)
		{
			GSLOG(Error, TEXT("nullptr == findField"));
			break;
		}

		if (1 < conditionData->MaterialAll().Num())
		{
			// 2023/1/11 PKT - 대체 아이템이 1개 이상인 경우 대처 버튼을 활성화
			isSubstituteMaterialsEnable = true;
		}

		// 2023/1/11 PKT - 유효 기간이면서..
		if (findField->IsEnableFlag())
		{
			const ItemId materialId = _materialId;
			TArray<TWeakPtr<FGsItem>> hasMaterials;
			if (true == conditionData->HasCanCollectMaterials(hasMaterials) && hasMaterials.FindByPredicate([materialId](const TWeakPtr<FGsItem>& InData)
				{
					return (InData.IsValid() && InData.Pin().Get()->GetTID() == materialId);
				}
			))
			{
				// 2023/1/11 PKT - 해당 재료 정보로 즉시 등록 가능하다면 활성화..
				isRegistEnable = true;
			}
		}

	} while (0);

	_btnItemCollectionRegist->SetIsEnabled(isRegistEnable);
	_btnItemCollectionSubstituteMaterials->SetIsEnabled(isSubstituteMaterialsEnable);
}

void UGsUIMaterialGainInfo::OnChangedToogleMainTab(int32 InIndex)
{
	const static int32 MARKET_TAB_INDEX = 0;
	const static int32 CRAFT_TAB_INDEX = 1;
	const static int32 DROP_NPC_TAB_INDEX = 2;
	const static int32 LOCK_CONTENTS_INDEX = 3;

	EGsUnlockContentsType selectedType 
		= (InIndex == MARKET_TAB_INDEX) ? EGsUnlockContentsType::MARKET 
		: (InIndex == CRAFT_TAB_INDEX) ? EGsUnlockContentsType::CRAFT : EGsUnlockContentsType::WORLDMAP;

	bool isUnLock = GSUnlock()->IsContentsUnlock(selectedType);

	int32 switcherIndex = InIndex;
	if (false == isUnLock)
	{
		// 2023/2/23 PKT - 컨텐츠 잠김 상태
		FText message;
		GSUnlock()->GetUnLockMessage(selectedType, message);

		_textLockReason->SetText(message);

		switcherIndex = LOCK_CONTENTS_INDEX;
	}

	_switcherDetailInfo->SetActiveWidgetIndex(switcherIndex);

	switch (switcherIndex)
	{
	case MARKET_TAB_INDEX:
		InvalidateMarket();
		break;
	case CRAFT_TAB_INDEX:
		InvalidateCraft();
		break;
	case DROP_NPC_TAB_INDEX:
		InvalidateDropNPC();
		break;
	}
}

void UGsUIMaterialGainInfo::OnGoMarket()
{
	if (_data)
	{
		if (const FGsItemCollectionCondition* conditionData = reinterpret_cast<const FGsItemCollectionCondition*>(_data))
		{
			if (const FGsItemCollectionField* findField = GSItemCollection()->FindFieldByConditionId(conditionData->Id()))
			{
				FGsMarketHelper::OpenMarketFromCollection(findField->Id(), _materialId);
			}
		}
	}
}

void UGsUIMaterialGainInfo::OnGoCraft()
{
	if (false == GSSupply()->TryToNavigateItemId(EGsUnlockContentsType::CRAFT, _materialId))
	{
		GSLOG(Error, TEXT("!SupplyManager->TryToNavigate(EGsUnlockContentsType::Craft, %d)"), _materialId);
	}
}

void UGsUIMaterialGainInfo::OnGoItemCollection()
{
	if (DriveOwner::ItemCollection == _driveOwner)
	{	// 2023/1/13 PKT - 이미 Window는 활성화 되어 있으므로.. 검색만 진행
		const FGsPrimitiveInt32 param(_materialId);
		GMessage()->GetItemCollection().SendMessage(MessageContentItemCollection::SEARCH_MATERIAL, &param);
	}
	else
	{
		if (GSItemCollection()->IsValidMaterial(_materialId))
		{
			// 2023/1/12 PKT - 윈도우 활성화
			GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_ITEM_COLLECTION);

			const FGsPrimitiveInt32 param(_materialId);
			GMessage()->GetItemCollection().SendMessage(MessageContentItemCollection::SEARCH_MATERIAL, &param);
		}
	}
}

void UGsUIMaterialGainInfo::OnGoEnchant()
{
	TArray<TWeakPtr<FGsItem>> hasMaterial = GItem()->FindByTID(_materialId);
	if (0 < hasMaterial.Num())
	{
		hasMaterial.Sort([](const TWeakPtr<FGsItem>& lhr, const TWeakPtr<FGsItem>& rhr)
			{
				return lhr.Pin().Get()->GetDBID() < rhr.Pin().Get()->GetDBID();
			}
		);

		// 2023/5/10 PKT - 복수개의 아이템 중 장착 or 잠금 되어 있는 아이템은 제외 시킨다.
		ItemDBId entryItemDBId = INVALID_ITEM_DB_ID;
		for (auto& material : hasMaterial)
		{
			if(false == material.IsValid())
				continue;

			if (const FGsItem* itemData = material.Pin().Get())
			{
				if (false == itemData->IsEquipped() && false == itemData->GetLock() && false == itemData->IsPresetEquipped())
				{
					entryItemDBId = itemData->GetDBID();
					break;
				}
			}			
		}

		if (INVALID_ITEM_DB_ID != entryItemDBId)
		{
			FGsOpenItemEnchant paramData(entryItemDBId, true, true /*DriveOwner::ItemCollection == _driveOwner*/);
			GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::OPEN, &paramData);
		}		
	}
}

void UGsUIMaterialGainInfo::OnClickedItemCollectionSubstituteMaterials()
{
	if (const FGsItemCollectionCondition* conditionData = reinterpret_cast<const FGsItemCollectionCondition*>(_data))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupItemCollectionAlternateList"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemCollectionAlternateList* popup = Cast<UGsUIPopupItemCollectionAlternateList>(widget.Get()))
			{
				popup->SetData(conditionData);
			}
		}
	}
}

void UGsUIMaterialGainInfo::OnClickedItemCollectionRegister()
{
	if (const FGsItemCollectionCondition* conditionData = reinterpret_cast<const FGsItemCollectionCondition*>(_data))
	{
		const ItemId materialId = _materialId;
		TArray<TWeakPtr<FGsItem>> hasMaterials;
		conditionData->HasCanCollectMaterials(hasMaterials);

		const auto& material = hasMaterials.FindByPredicate([materialId](const TWeakPtr<FGsItem>& InData)
			{
				return (InData.IsValid() && InData.Pin().Get()->GetTID() == materialId);
			}
		);

		if (material && material->IsValid())
		{	// 2023/1/11 PKT - 등록.
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCommonItemAction"));
			if (widget.IsValid())
			{
				if (UGsUIPopupCommonItemAction* popup = Cast<UGsUIPopupCommonItemAction>(widget.Get()))
				{
					FText textMsg1;
					FText::FindText(TEXT("ItemUIText"), TEXT("UI_Popup_ConfirmRegist"), textMsg1);
					
					const int32 DEFAULT_COUNT = 1;
					popup->SetItemEx(material->Pin().Get(), DEFAULT_COUNT, true);
					popup->SetDescription(textMsg1);
					popup->SetCurrencyVisibility(false);					

					CollectionId collectionId = conditionData->Id();
					ItemDBId targetItemId = material->Pin().Get()->GetDBID();
					popup->SetCallBack([collectionId, targetItemId](bool InIsOk)
						{
							if (InIsOk)
							{
								FGsNetSendServiceWorld::SendItemCollectionRegister(collectionId, targetItemId);
							}
						}
					);
				}
			}			
		}
	}
}

void UGsUIMaterialGainInfo::OnClickItemDropNavigateButton()
{
	// "획득처" 목록의 이동하기 버튼 클릭 시 이 이벤트를 항상 발생시킴.
}

void UGsUIMaterialGainInfo::OnReceiveItemPriceInfo(const IGsMessageParam*)
{
	if (UWidget* activeWidget = _switcherDetailInfo->GetActiveWidget())
	{
		if (activeWidget->IsA<UGsUIMaterialGainInfoMarketPrice>())
		{
			if (UGsUIMaterialGainInfoMarketPrice* activePage = Cast<UGsUIMaterialGainInfoMarketPrice>(activeWidget))
			{
				activePage->OnReceiveItemPriceInfo(nullptr);
			}
		}
	}
}

void UGsUIMaterialGainInfo::OnReceiveCollectionPriceList(const IGsMessageParam* InParam)
{
	if (UWidget* activeWidget = _switcherDetailInfo->GetActiveWidget())
	{
		if (activeWidget->IsA<UGsUIMaterialGainInfoMarketPrice>())
		{
			if (UGsUIMaterialGainInfoMarketPrice* activePage = Cast<UGsUIMaterialGainInfoMarketPrice>(activeWidget))
			{
				activePage->OnReceiveCollectionPriceList(InParam);
			}
		}
	}
}

void UGsUIMaterialGainInfo::OnRequestRefreshPrice(const IGsMessageParam*)
{
	if (UWidget* activeWidget = _switcherDetailInfo->GetActiveWidget())
	{
		if (activeWidget->IsA<UGsUIMaterialGainInfoMarketPrice>())
		{
			if (UGsUIMaterialGainInfoMarketPrice* activePage = Cast<UGsUIMaterialGainInfoMarketPrice>(activeWidget))
			{
				activePage->OnRequestRefreshPrice(nullptr);
			}
		}
	}
}

void UGsUIMaterialGainInfo::SetData(const FGsItemCollectionCondition* InData, const int32 InTabIndex /*= -1*/)
{
	/**
	 * Item Collection Only
	 */
	_data = InData;
	_materialId = InData->EntryMaterialInfo().Key;
	_level = InData->EntryMaterialInfo().Value;

	_driveOwner = DriveOwner::ItemCollection;

	{
		FGsItem* notOwnerItem = FGsItem::Create(_materialId);
		if (nullptr == notOwnerItem)
		{
			GSLOG(Error, TEXT("nullptr == notOwnerItem"));
			return;
		}
		notOwnerItem->SetLevel(_level);

		// 2023/1/11 PKT - 아이템 정보
		_itemDefaultInfo->SetData(notOwnerItem, true);

		// 2023/1/18 PKT - bool -> Switcher Index
		auto activeWidgetIndex = [](bool InFlag) -> int32
		{
			return (InFlag == true) ? 0 : 1;
		};

		/**
		* 바로 가기 설정..(마켓/컬렉션/제작/강화)
		*/
		// 2023/1/11 PKT - 마켓 
		_switcherMarketBtn->SetActiveWidgetIndex(activeWidgetIndex(notOwnerItem->IsTradable() && GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET)));
		// 2023/1/11 PKT - 아이템 컬렉션
		_switcherItemCollectionBtn->SetActiveWidgetIndex(activeWidgetIndex(GSItemCollection()->IsValidMaterial(_materialId) && GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ITEM_COLLECTION)));
		// 2023/1/11 PKT - 제작
		{
			const bool HasNavigateToCraft = UGsSupplyManager::EGsNavigationStatus::Movable == GSSupply()->GetNavigationStatus(EGsUnlockContentsType::CRAFT, _materialId);
			_switcherCraftBtn->SetActiveWidgetIndex(activeWidgetIndex(HasNavigateToCraft && GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::CRAFT)));
		}
		// 2023/1/11 PKT - 인챈트
		{
			// 2023/1/12 PKT - 조건 : 선택한 아이템과 무관하게 강화 UI 팝업(단일 강화 탭) / 미보유한 경우 비활성화
			TArray<TWeakPtr<FGsItem>> hasMaterial = GItem()->FindByTID(_materialId);
			_switcherEnchantBtn->SetActiveWidgetIndex(activeWidgetIndex(0 < hasMaterial.Num() && notOwnerItem->IsEquipment() && GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ITEM_ENCHANT)));
		}

		// 반드시 OnChangedToogleMainTab 을 한 번 부르도록 true 설정
		int32 prevTabIndex = _toggleGroupMainTabs.GetSelectedIndex();
		int32 currTabIndex = (0 <= InTabIndex) ? InTabIndex : prevTabIndex;
		_toggleGroupMainTabs.SetSelectedIndex(currTabIndex, true);

		delete notOwnerItem;
	}

	// 2023/1/18 PKT - Set Bottom Button's
	SetController(_driveOwner);
}

void UGsUIMaterialGainInfo::Invalidate() 
{
	switch (_driveOwner)
	{
	case DriveOwner::ItemCollection:
		InvalidateItemCollectionController();
		break;
	/**
	* case DriveOwnder::GuildItemCollection:
	* break;
	*/
	}
}

void UGsUIMaterialGainInfo::ContentsUnlock(EGsUnlockContentsType InType)
{
	const static int32 SWITCHER_LOCK_PANEL_INDEX = 3;
	if (SWITCHER_LOCK_PANEL_INDEX != _switcherDetailInfo->GetActiveWidgetIndex())
	{
		return;
	}

	const static int32 MARKET_TAB_INDEX = 0;
	const static int32 CRAFT_TAB_INDEX = 1;
	const static int32 DROP_NPC_TAB_INDEX = 2;

	const int32 currentTab = _toggleGroupMainTabs.GetSelectedIndex();
	EGsUnlockContentsType currentType
		= (currentTab == MARKET_TAB_INDEX) ? EGsUnlockContentsType::MARKET
		: (currentTab == CRAFT_TAB_INDEX) ? EGsUnlockContentsType::CRAFT : EGsUnlockContentsType::WORLDMAP;

	if (currentType == InType)
	{
		if (GSUnlock()->IsContentsUnlock(currentType))
		{
			_toggleGroupMainTabs.SetSelectedIndex(currentTab, true);
		}
	}
}


void UGsUIMaterialGainInfo::Capture(int32& OutTabIndex)
{
	OutTabIndex = _toggleGroupMainTabs.GetSelectedIndex();
}