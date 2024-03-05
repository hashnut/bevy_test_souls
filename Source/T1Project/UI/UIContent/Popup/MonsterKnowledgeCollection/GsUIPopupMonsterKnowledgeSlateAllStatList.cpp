

#include "GsUIPopupMonsterKnowledgeSlateAllStatList.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Item/GsItemManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
//#include "Management/ScopeGame/GsMonsterKnowledgeCollectionManager.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMonsterKnowledgeCollection.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Popup/GsUIPopupCommonItemConsume.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
//#include "UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIMonsterKnowledgeSlateStatListEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/GsUITrayTickerSection.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "../Public/Components/WidgetSwitcher.h"
#include "../Runtime/UMG/Public/Components/TextBlock.h"
#include "../Runtime/UMG/Public/Components/ScrollBox.h"
#include "../Runtime/UMG/Public/Components/WrapBox.h"



#include "Net/GsNetSendServiceWorld.h"



void UGsUIPopupMonsterKnowledgeSlateAllStatList::NativeOnInitialized()
{
	UGsUIPopup::NativeOnInitialized();

	_btnOK->OnClicked.AddDynamic(this, &UGsUIPopupMonsterKnowledgeSlateAllStatList::OnClickedOK);

	_btnResetAll->OnClicked.AddDynamic(this, &UGsUIPopupMonsterKnowledgeSlateAllStatList::OnClickedReset);

	_slotHelperStatList = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperStatList->Initialize(_subClassOfStat, _wrapBoxStatList);
	_slotHelperStatList->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMonsterKnowledgeSlateAllStatList::OnRefreshEntryStat);
}

void UGsUIPopupMonsterKnowledgeSlateAllStatList::BeginDestroy()
{
	if (_btnOK)
	{
		_btnOK->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterKnowledgeSlateAllStatList::OnClickedOK);
		_btnOK = nullptr;
	}	

	if (_btnResetAll)
	{
		_btnResetAll->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterKnowledgeSlateAllStatList::OnClickedOK);
		_btnResetAll = nullptr;
	}

	if (_slotHelperStatList)
	{
		_slotHelperStatList->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMonsterKnowledgeSlateAllStatList::OnRefreshEntryStat);
		_slotHelperStatList = nullptr;
	}

	UGsUIPopup::BeginDestroy();
}

void UGsUIPopupMonsterKnowledgeSlateAllStatList::NativeConstruct()
{
	UGsUIPopup::NativeConstruct();

	FText textTitle;
	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slate_PassivityPopupTitle"), textTitle);
	_textTitle->SetText(textTitle);

	Refresh();
}

void UGsUIPopupMonsterKnowledgeSlateAllStatList::NativeDestruct()
{
	UGsUIPopup::NativeDestruct();
}

void UGsUIPopupMonsterKnowledgeSlateAllStatList::OnRefreshEntryStat(int32 InIndex, UWidget* InEntry)
{
	//TWeakObjectPtr<UGsUIMonsterKnowledgeSlateStatListEntry> statEntry
	//	= Cast<UGsUIMonsterKnowledgeSlateStatListEntry>(InEntry);
	//if (false == statEntry.IsValid())
	//{
	//	GSLOG(Error, TEXT("false == statEntry.IsValid()"));
	//	return;
	//}

	//if (false == _statListAll.IsValidIndex(InIndex))
	//{
	//	GSLOG(Error, TEXT("false == _statListAll.IsValidIndex(InIndex)"));
	//	return;
	//}

	//FGsMonsterKnowledgeCollectionManager* manager = GsMonsterKnowledgeCollection();
	//if (nullptr == manager)
	//{
	//	GSLOG(Error, TEXT("nullptr == manager"));
	//	return;
	//}

	//StatType statType = _statListAll[InIndex].Key;
	//const TArray<TPair<StatType, FText>>& statNames = manager->GetStatNames();
	//int32 findIndex = statNames.IndexOfByPredicate([statType](const TPair<StatType, FText>& pair)
	//	{
	//		return (pair.Key == statType);
	//	}
	//);
	//
	//if (INDEX_NONE == findIndex)
	//{
	//	GSLOG(Error, TEXT("INDEX_NONE == findIndex"));
	//	return;
	//}

	//FText statName = statNames[findIndex].Value;
	//StatValue statValue = _statListAll[InIndex].Value;

	//FText valueName;
	//FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statType, statValue, valueName);

	//statEntry->SetData(statName, valueName);
}

void UGsUIPopupMonsterKnowledgeSlateAllStatList::OnClickedOK()
{
	Close();
}

void UGsUIPopupMonsterKnowledgeSlateAllStatList::OnClickedReset()
{
	//FGsMonsterKnowledgeCollectionManager* manager = GsMonsterKnowledgeCollection();
	//if (nullptr == manager)
	//{
	//	GSLOG(Error, TEXT("nullptr == manager"));
	//	return;
	//}

	//const TMap<StatType, StatValue>& statValueList = manager->GetStatEffects();

	//if (0 >= statValueList.Num())
	//{	// 2021/12/28 PKT - 지식의 석판에 활성화 된 슬롯이 없습니다.
	//	FText errorMsg;
	//	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("ticker_NoSlotToReset"), errorMsg);
	//	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorMsg);
	//	return;
	//}


	//UGsItemManager* itemManager = GItem();
	//if (nullptr == itemManager)
	//{
	//	GSLOG(Error, TEXT("nullptr == ItemManager"));
	//	return;
	//}

	//const TArray<ItemId>& itemList = manager->GetSlateStatEraseItem();
	//if (0 >= itemList.Num())
	//{
	//	GSLOG(Error, TEXT("0 >= itemList.Num()"));
	//	return;
	//}

	//// 2021/12/28 PKT - 스탯 초기화 아이템은 한개만 존재 한다.
	//ItemId clearItemID = itemList[0];

	//ItemDBId clearItemDBId = INVALID_ITEM_DB_ID;
	//	
	//for (ItemId itemId : itemList)
	//{
	//	const TArray<TWeakPtr<FGsItem>> Items = itemManager->FindByTID(itemId);
	//	for (TWeakPtr<const FGsItem> item : Items)
	//	{
	//		if(false == item.IsValid())
	//			continue;

	//		// 창고 아이템 제외
	//		if (item.Pin()->IsDepotItem())
	//		{
	//			continue;
	//		}

	//		// 잠김 장비 제외
	//		if (item.Pin()->GetLock())
	//		{
	//			continue;
	//		}

	//		clearItemDBId = item.Pin()->GetDBID();
	//		break;
	//	}

	//	if (INVALID_ITEM_DB_ID != clearItemDBId)
	//	{
	//		break;
	//	}
	//}

	//UGsUIManager* uiManager = GUI();
	//TWeakObjectPtr<UGsUIPopupCommonItemConsume> itemConsumePopup
	//	= Cast<UGsUIPopupCommonItemConsume>(GUI()->OpenAndGetWidget(TEXT("PopupCommonItemConsume")).Get());
	//if (itemConsumePopup.IsValid())
	//{
	//	const static int32 USE_ITEM_AMOUNT = 1;

	//	FText textContent;
	//	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slate_ResetPopupTitle"), textContent);

	//	FText textDesc;
	//	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slate_ResetPopupDesc"), textDesc);
	//	FText textMsg = FText::Format(textDesc, USE_ITEM_AMOUNT);

	//	itemConsumePopup->SetDataNotCheckItem(textContent
	//		, textMsg
	//		, clearItemID
	//		, USE_ITEM_AMOUNT
	//		, [clearItemID, clearItemDBId](bool InIsOk)
	//		{
	//			if (InIsOk && INVALID_ITEM_DB_ID != clearItemDBId)
	//			{
	//				const FGsMessageMonsterKnowledgeSlateUseResetItem message(clearItemID, clearItemDBId);
	//				GMessage()->GetMonsterKnowledgeCollection().SendMessage(MessageContentMonsterKnowledge::USE_ITEM_SLATE_RESET, &message);
	//			}
	//		}
	//	);
	//}
}

void UGsUIPopupMonsterKnowledgeSlateAllStatList::Refresh()
{
	//FGsMonsterKnowledgeCollectionManager* manager = GsMonsterKnowledgeCollection();
	//if (nullptr == manager)
	//{
	//	GSLOG(Error, TEXT("nullptr == manager"));
	//	return;
	//}

	//_statListAll.Empty(manager->GetStatEffects().Num());

	//for (const auto& pair : manager->GetStatEffects())
	//{
	//	_statListAll.Emplace(pair);
	//}
	//// 스탯을 순서대로 정렬 하기..
	//_statListAll.Sort([](const TPair<StatType, StatValue>& lhr, const TPair<StatType, StatValue>& rhr)
	//	{
	//		return lhr.Key < rhr.Key;
	//	}
	//);

	//const int32  maxCount = _statListAll.Num();
	//
	//if (0 < maxCount)
	//{
	//	_switcherStatList->SetActiveWidgetIndex(0);
	//	_slotHelperStatList->RefreshAll(maxCount);
	//}
	//else
	//{
	//	_switcherStatList->SetActiveWidgetIndex(1);

	//	FText emptyDesc;
	//	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slate_PassivityPopupEmpty"), emptyDesc);
	//	_textEmpty->SetText(emptyDesc);
	//}
}

void UGsUIPopupMonsterKnowledgeSlateAllStatList::Invalid()
{
	Refresh();
}