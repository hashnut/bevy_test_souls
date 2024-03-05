#pragma once

#include "Classes/GsPoolUObject.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

#include "GsUIItemDropListContainer.generated.h"

class FGsGameStateRegionMap;
class UGsDynamicPanelSlotHelper;
class UGsPoolUObject;
class UGsUICraftShortCutListItem;
class UGsUIIconBase;
class UGsUIItemDropElementListItem;
class UListView;
class UScrollBox;
class UWidget;
class UWidgetSwitcher;
struct FGsSchemaWorldMapMonsterElementDataRow;

UCLASS(BlueprintType)
class T1PROJECT_API UGsUIItemDropListContainer final : public UGsUIWidgetBase
{
	GENERATED_BODY()

public:
	enum class EGsListItemType : uint8
	{
		Drop,
		Craft,
		EmptyListItem,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _widgetSwitcher;

	/** 드랍 숏컷 리스트뷰 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _dropListView;

	/** 제작 숏컷 리스트뷰 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _craftListView;

	/** 타입별 리스트아이템풀 */
	UPROPERTY()
	TArray<UGsPoolUObject*> _listItemPools;

	/** 타입별 출력용 리스트아이템 */
	TMap<EGsListItemType, TArray<UObject*>> _listItemMap;

	/** 타입별 리스트뷰 */
	TMap<EGsListItemType, UListView*> _listViewMap;

	/** 리스트엔트리 클릭 시, 이 이벤트를 발생시킴 */
	FSimpleDelegate _onClickListEntry;

	FText _levelFormat;

protected:
	void NativeOnInitialized() final;

public:
	void SetParameters(FSimpleDelegate InOnClickListEntry, TArray<TPair<MapId, CreatureId>> InDropMapCreatures);
	
	void SetParameters(FSimpleDelegate InOnClickListEntry, const TArray<FGsSchemaWorldMapMonsterElementDataRow>& InDropMapCreatures);

	void SetParameters(FSimpleDelegate InOnClickListEntry, const TArray<CraftId>& InCraftIds);

private:
	template<typename TListItem>
	TListItem* ClaimListItem(const EGsListItemType ListItemType)
	{
		const uint8 ListViewIndex = StaticCast<uint8>(ListItemType);
		if (!_listItemPools.IsValidIndex(ListViewIndex))
		{
			GSLOG(Error, TEXT("!_listItemPoolMap.IsValidIndex(%d)"), ListViewIndex);
			return nullptr;
		}

		UGsPoolUObject* PoolObject = _listItemPools[ListViewIndex];
		if (nullptr == PoolObject)
		{
			GSLOG(Error, TEXT("nullptr == PoolObject, ListViewIndex == %d"), ListViewIndex);
			return nullptr;
		}

		return PoolObject->Claim<TListItem>();
	}

private:
	void OnClickedCraftShortCutMoveBtn(const UGsUICraftShortCutListItem& InListItem);

	void OnClickedDropElementMoveBtn(const UGsUIItemDropElementListItem& InListItem);

public:
	static FGsGameStateRegionMap* GetGameStateRegionMap();
	static void TryToNavigate(const MapId InMapId, const CreatureId InCreatureId, const Level InLevel);

public:
	// TODO: 숏컷매니저로 이동 필요
	static bool TryToNavigate(const MapId InMapId);

};
