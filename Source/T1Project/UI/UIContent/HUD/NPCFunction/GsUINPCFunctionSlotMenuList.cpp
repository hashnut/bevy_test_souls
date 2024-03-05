#include "GsUINPCFunctionSlotMenuList.h"

#include "UMG/Public/Components/PanelWidget.h"

#include "UI/UIContent/HUD/NPCFunction/GsUINPCFunctionSlotMenuItem.h"

void UGsUINPCFunctionSlotMenuList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_arrayItem.Empty();

	for (int32 i = 0, maxCount = _panelRootListItem->GetChildrenCount(); i < maxCount; ++i)
	{
		if (UGsUINPCFunctionSlotMenuItem* item = Cast<UGsUINPCFunctionSlotMenuItem>(_panelRootListItem->GetChildAt(i)))
		{
			_arrayItem.Emplace(item);
		}
	}
}

void UGsUINPCFunctionSlotMenuList::SetMenuListData(TArray<int> In_arrayNpcTblId)
{
	for (int i=0; _arrayItem.Num() > i; ++i )
	{
		TWeakObjectPtr<UGsUINPCFunctionSlotMenuItem> item = _arrayItem[i];
		if (false == item.IsValid())
		{
			continue;
		}

		// data smaller : hide item
		if (In_arrayNpcTblId.Num() - 1 < i)
		{
			item->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}

		int tblId = In_arrayNpcTblId[i];

		item->SetSlotMenuItemData(tblId, i+1);
		item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}