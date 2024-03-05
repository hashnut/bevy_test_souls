#include "GsUIRegionMonsterDropItemList.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Window/RegionMap/Monster/GsUIRegionMonsterDropItem.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"

#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"

#include "Map/WorldMap/GsRegionInfo.h"
#include "Map/WorldMap/GsRegionMonsterInfo.h"

#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/Widget.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "Core/Public/Delegates/Delegate.h"

void UGsUIRegionMonsterDropItemList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIRegionMonsterDropItemList::OnRefreshEntry);

}

void UGsUIRegionMonsterDropItemList::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	FGsRegionMonsterInfo* monsterInfo =
		GSWorldMap()->GetCurrentRegionMonsterInfoByIndex(_monsterIndex);
	if (nullptr == monsterInfo)
	{
		return;
	}

	EGsRegionItemInfoSetType nowMapType = GetNowMapItemInfoSetType(monsterInfo);
	TArray<FGsRegionMonsterItemInfo*> arrayItems;
	//monsterInfo->GetDropItem(nowMapType, _dropItemType, arrayItems);
	

	if (arrayItems.Num() <= InIndex)
	{
		return;
	}
	FGsRegionMonsterItemInfo* itemInfo = arrayItems[InIndex];

	if (nullptr == itemInfo)
	{
		return;
	}
	const FGsSchemaItemCommon* itemTbl = itemInfo->_itemTbl;

	if (itemTbl == nullptr)
	{
		return;
	}

	UGsUIRegionMonsterDropItem* item = Cast<UGsUIRegionMonsterDropItem>(InEntry);
	if (item == nullptr)
	{
		return;
	}
	item->SetItemTblId(itemTbl->id, itemTbl->name, itemTbl->grade);

}

void UGsUIRegionMonsterDropItemList::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIRegionMonsterDropItemList::OnRefreshEntry);
	}
	_scrollBoxHelper = nullptr;
	Super::BeginDestroy();
}

// 리스트 초기화
void UGsUIRegionMonsterDropItemList::InitList(EGsRegionMonsterDropItemType In_type, int In_monsterIndex)
{
	_dropItemType = In_type;
	_monsterIndex = In_monsterIndex;


	FGsRegionMonsterInfo* monsterInfo = 
		GSWorldMap()->GetCurrentRegionMonsterInfoByIndex(_monsterIndex);
	if (nullptr == monsterInfo)
	{
		return;
	}

	TArray<FGsRegionMonsterItemInfo*> arrayItems;
	EGsRegionItemInfoSetType nowMapType = GetNowMapItemInfoSetType(monsterInfo);
	//monsterInfo->GetDropItem(nowMapType, _dropItemType, arrayItems);
	


	_scrollBoxHelper->RefreshAll(arrayItems.Num());
	_scrollBox->SetScrollOffset(0.0f);
}

EGsRegionItemInfoSetType UGsUIRegionMonsterDropItemList::GetNowMapItemInfoSetType(FGsRegionMonsterInfo* In_monsterInfo)
{
	if (In_monsterInfo == nullptr ||
		In_monsterInfo->_npcData == nullptr)
	{
		return EGsRegionItemInfoSetType::Normal;
	}

	// field boss only use normal
	if (In_monsterInfo->_npcData->npcFunctionType == NpcFunctionType::FIELD_BOSS)
	{
		return EGsRegionItemInfoSetType::Normal;
	}

	// 1. check is invade
	if (true == GGameData()->IsInvadeWorld())
	{
		return EGsRegionItemInfoSetType::Invade_Attack;
	}	
	
	
	// 2. check is defense map
	// 3. etc = normal
	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	bool isDefense = GSInterServer()->IsValidInvasionMap(showRegionMapId);
				
	
	return (isDefense == true) ? EGsRegionItemInfoSetType::Invade_Defense : EGsRegionItemInfoSetType::Normal;	
}