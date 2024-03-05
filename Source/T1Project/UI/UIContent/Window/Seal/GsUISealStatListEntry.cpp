#include "GsUISealStatListEntry.h"

#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "GameObject/Stat/GsStatHelper.h"
#include "T1Project.h"

void UGsUISealStatListEntry::NativeOnListItemObjectSet(UObject* InListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(InListItemObject);

	Invalidate(InListItemObject);
}

void UGsUISealStatListEntry::Invalidate(const UObject* InListItemObject)
{
	const UGsUISealStatListItem* ListItem = Cast<UGsUISealStatListItem>(InListItemObject);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	TPair<FText, FText> StatTextPair;
	if (!FGsStatHelper::GetDesplayStatNameAndValue(ListItem->_statType, ListItem->_statValue, StatTextPair, true))
	{
		GSLOG(Error, TEXT("!FGsStatHelper::GetDesplayStatNameAndValue(ListItem->_statType, ListItem->_statValue, StatTextPair, true)"));
		return;
	}

	const FText StatText = FText::Join(FText::GetEmpty(), StatTextPair.Key, StatTextPair.Value);

	if (ListItem->_activated)
	{
		if (nullptr != _activationSwitcher)
		{
			_activationSwitcher->SetActiveWidgetIndex(1);
		}
		if (nullptr != _activationTxt)
		{
			_activationTxt->SetText(StatText);
		}
	}
	else
	{
		if (nullptr != _activationSwitcher)
		{
			_activationSwitcher->SetActiveWidgetIndex(0);
		}
		if (nullptr != _inactivationTxt)
		{
			_inactivationTxt->SetText(StatText);
		}
	}
}
