
#include "GsUIMaterialSetGainInfo.h"

#include "../Item/Collection/GsItemCollectionField.h"
#include "../Item/Collection/GsItemCollectionCondition.h"
#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGame/GsUnlockManager.h"
#include "../Message/GsMessageContents.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/MessageParam/GsEnchantMessageParam.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Material/GsUIMaterialSetGainInfoMarketPrice.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/RichTextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Currency/GsCostPackage.h"
#include "Item/Craft/GsCraftTarget.h"
#include "GsUIMaterialSetGainInfoCraftTargetList.h"
#include "GsUIMaterialSetGainInfoCraftTargetListEntry.h"
#include "T1Project.h"



void UGsUIMaterialSetGainInfo::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIMaterialSetGainInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (false == _btnClose->OnClicked.IsBound())
	{
		_btnClose->OnClicked.AddDynamic(this, &UGsUIMaterialSetGainInfo::OnClickedGoExit);
	}

	if (false == _btnGoEnchant->OnClicked.IsBound())
	{
		_btnGoEnchant->OnClicked.AddDynamic(this, &UGsUIMaterialSetGainInfo::OnClickedGoEnchant);
	}

	if (false == _btnMarketRefresh->OnClicked.IsBound())
	{
		_btnMarketRefresh->OnClicked.AddDynamic(this, &UGsUIMaterialSetGainInfo::OnClickedMarketRefresh);
	}

	if (false == _btnMarketQuickBuy->OnClicked.IsBound())
	{
		_btnMarketQuickBuy->OnClicked.AddDynamic(this, &UGsUIMaterialSetGainInfo::OnClickedMarketQuickBuy);
	}

	_toggleGroupBtn.OnSelectChanged.BindUObject(this, &UGsUIMaterialSetGainInfo::OnChangedToogleMainTab);
}

void UGsUIMaterialSetGainInfo::NativeConstruct()
{
	Super::NativeConstruct();

	_toggleGroupBtn.Clear();
	_toggleGroupBtn.AddToggleWidget(_toggleBtnMarket);
	_toggleGroupBtn.AddToggleWidget(_toggleBtnCraft);
}

void UGsUIMaterialSetGainInfo::OnChangedToogleMainTab(int32 InIndex)
{	
	const TabIndex tabIndex = StaticCast<TabIndex>(InIndex);
	
	EGsUnlockContentsType selectedType = tabIndex == TabIndex::Market ? EGsUnlockContentsType::MARKET : EGsUnlockContentsType::CRAFT;
	bool isUnLock = GSUnlock()->IsContentsUnlock(selectedType);

	if (TabIndex::Market == tabIndex)
	{
		_btnMarketQuickBuy->SetIsEnabled(isUnLock);
		_btnMarketRefresh->SetIsEnabled(isUnLock);
	}
	
	int32 switcherIndex = InIndex;
	if (false == isUnLock)
	{
		// 2023/2/23 PKT - 컨텐츠 잠김 상태
		FText message;
		GSUnlock()->GetUnLockMessage(selectedType, message);

		_textLockReason->SetText(message);
		
		const static int32 SWITCHER_LOCK_PANEL_INDEX = 2;
		switcherIndex = SWITCHER_LOCK_PANEL_INDEX;
	}
	else
	{
		// 2023/2/28 PKT - Build..
		BuildAfterContentsCheck(StaticCast<TabIndex>(switcherIndex));
	}

	_switcherDetailInfo->SetActiveWidgetIndex(switcherIndex);
	
	Invalidate();
}

void UGsUIMaterialSetGainInfo::BuildMarket(const FGsItemCollectionField* InData)
{
	_uiMarketPrice->SetData(InData);
}

void UGsUIMaterialSetGainInfo::BuildCraft(const FGsItemCollectionField* InData)
{
	/**
	 * Craft Data List
	 */

	 //const TArray<TPair<ItemId, Level>> materials;
	 //for (const auto& item : InData->Conditions())
	 //{	
	 //	// 2023/1/13 PKT - 각 조건에 대한 대표 재료 정보를 얻어온다.( 대체 아이템으로 대표 재료 정보가 바뀔 수 있음. )
	 //	materials.Emplace(item.EntryMaterialInfo());
	 //	// 2023/1/13 PKT - 조건 충족 갯수
	 //	item.ConditionCount();
	 //	// 2023/1/13 PKT - 현재까지 등록한 갯수
	 //	item.RegisterCount();
	 //	// 2023/1/13 PKT - 필요 갯수
	 //	item.ConditionCount() - item.RegisterCount();
	 //}

	if (nullptr == _uiCraftTarget)
	{
		return;
	}

	// 매개변수 생명 주기를 확정할 수 없어, 사용 권한 제한함.
	TSharedPtr<UGsUIMaterialSetGainInfoCraftTargetList::Parameters> Parameters = MakeShared<UGsUIMaterialSetGainInfoCraftTargetList::Parameters>();
	if (nullptr == InData)
	{
		GSLOG(Warning, TEXT("nullptr == InData. Is this error?"));
	}
	else
	{
		Parameters->_itemCollectionConditions = &InData->Conditions();
	}
	
	//for (const FGsItemCollectionCondition& ItemCollectionCondition : InData->Conditions())
	//{
	//	const TPair<ItemId, Level>& EntryMaterialInfo = ItemCollectionCondition.EntryMaterialInfo();

	//	// 제작 대상 정보
	//	CraftId CurrentCraftId = INVALID_CRAFT_ID;
	//	TSet<CraftId> CraftIds;
	//	if (UGsCraftManager::FindCraftIds(EntryMaterialInfo.Key, CraftIds) && 0 < CraftIds.Num())
	//	{
	//		CurrentCraftId = *CraftIds.CreateConstIterator();
	//	}

	//	// UI 파라미터에 추가
	//	Parameters._collectionSlotInfos.Emplace(UGsUIMaterialSetGainInfoCraftTargetListEntry::Parameters {
	//		CurrentCraftId,
	//		EntryMaterialInfo.Key,
	//		EntryMaterialInfo.Value,
	//		(ItemCollectionCondition.ConditionCount() <= ItemCollectionCondition.RegisterCount())
	//	});
	//}

	_uiCraftTarget->SetParameters(Parameters);
}

void UGsUIMaterialSetGainInfo::BuildAfterContentsCheck(TabIndex InTab)
{
	if (TabIndex::Market == InTab && GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET) && false == _prevBuildMarket )
	{
		BuildMarket(_data);
		
		_prevBuildMarket = true;
	}
	else if (TabIndex::Craft == InTab && GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::CRAFT) && false == _prevBuildCraft)
	{
		BuildCraft(_data);

		_prevBuildCraft = true;
	}
}

void UGsUIMaterialSetGainInfo::OnClickedGoEnchant()
{
	FGsOpenItemEnchant paramData(INVALID_ITEM_DB_ID, true, false);
	GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::OPEN, &paramData);
}

void UGsUIMaterialSetGainInfo::OnClickedGoExit()
{
	// 2023/1/13 PKT - send message
	GMessage()->GetMaterial().SendMessage(MessageContentMaterial::EXIT, nullptr);
}

void UGsUIMaterialSetGainInfo::OnClickedMarketRefresh()
{
	_uiMarketPrice->OnClickRefresh();
}

void UGsUIMaterialSetGainInfo::OnClickedMarketQuickBuy()
{
	_uiMarketPrice->OnClickPurchase();
}

void UGsUIMaterialSetGainInfo::OnReceiveCollectionPriceList(const IGsMessageParam* InParam)
{
	_uiMarketPrice->OnReceiveMarketPriceList(InParam);
	_btnMarketQuickBuy->SetIsEnabled(_uiMarketPrice->IsExistMarketData());
}

void UGsUIMaterialSetGainInfo::OnRequestRefreshPrice(const IGsMessageParam*)
{
	_uiMarketPrice->OnRequestRefreshPrice(nullptr);
}

void UGsUIMaterialSetGainInfo::SetData(const class FGsItemCollectionField* InData, const int32 InTabIndex /* = -1 */)
{
	_data = InData;
	_prevBuildMarket = false;
	_prevBuildCraft = false;

	int32 prevTabIndex = _toggleGroupBtn.GetSelectedIndex();
	int32 currTabIndex = (0 <= InTabIndex) ? InTabIndex : prevTabIndex;

	_toggleGroupBtn.SetSelectedIndex(currTabIndex, true);
}

void UGsUIMaterialSetGainInfo::Invalidate()
{
	const TabIndex tabIndex = StaticCast<TabIndex>(_toggleGroupBtn.GetSelectedIndex());
	switch (tabIndex)
	{
	case TabIndex::Market:
	{
		_btnMarketRefresh->SetVisibility(ESlateVisibility::Visible);
		_btnMarketQuickBuy->SetVisibility(ESlateVisibility::Visible);

		_btnMarketQuickBuy->SetIsEnabled(_uiMarketPrice->IsExistMarketData());
	}
	break;
	case TabIndex::Craft:
	{
		_btnMarketRefresh->SetVisibility(ESlateVisibility::Collapsed);
		_btnMarketQuickBuy->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	}

	bool InIsUnlock = GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ITEM_ENCHANT);
	_btnGoEnchant->SetIsEnabled(true == InIsUnlock);
}

void UGsUIMaterialSetGainInfo::ContentsUnlock(EGsUnlockContentsType InType)
{
	const static int32 SWITCHER_LOCK_PANEL_INDEX = 2;
	if (SWITCHER_LOCK_PANEL_INDEX != _switcherDetailInfo->GetActiveWidgetIndex())
	{
		return;
	}

	const TabIndex currentTab = StaticCast<TabIndex>(_toggleGroupBtn.GetSelectedIndex());
	EGsUnlockContentsType currentType = (currentTab == TabIndex::Market) ? EGsUnlockContentsType::MARKET : EGsUnlockContentsType::CRAFT;

	if (currentType == InType)
	{
		if (GSUnlock()->IsContentsUnlock(currentType))
		{
			_toggleGroupBtn.SetSelectedIndex(StaticCast<int32>(currentTab), true);
		}
	}
}

void UGsUIMaterialSetGainInfo::Capture(int32& OutTabIndex)
{
	OutTabIndex = _toggleGroupBtn.GetSelectedIndex();
}