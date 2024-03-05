#include "GsUISealLevelStatListEntry.h"

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "GsUISealStatListEntry.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UILib/Base/GsUIVFX.h"

void UGsUISealLevelStatListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_statListEntries.Append({
		_statListEntry0,
		_statListEntry1,
		_statListEntry2
	});
}

void UGsUISealLevelStatListEntry::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUISealLevelStatListEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(inListItemObject);

	Invalidate(inListItemObject);
}

void UGsUISealLevelStatListEntry::Invalidate(const UObject* InListItemObject)
{
	const UGsUISealLevelStatListItem* ListItem = Cast<UGsUISealLevelStatListItem>(InListItemObject);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	const FText LevelText = FGsUIStringHelper::MakeFormatedText("Lv.{0}", ListItem->_level);

	const bool IsActivated = ListItem->_activated;
	if (IsActivated)
	{
		if (nullptr != _activationSwitcher)
		{
			_activationSwitcher->SetActiveWidgetIndex(1);
		}

		if (nullptr != _activationTxt)
		{
			_activationTxt->SetText(LevelText);
		}

		if (nullptr != _activationVfx)
		{
			_activationVfx->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (nullptr != _activationOnceVfx && ListItem->_justActivated)
		{
			_activationOnceVfx->ShowVFX();
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
			_inactivationTxt->SetText(LevelText);
		}

		if (nullptr != _activationVfx)
		{
			_activationVfx->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (nullptr != _activationOnceVfx)
		{
			_activationOnceVfx->HideVFX();
		}
	}

	// 연출 플래그 리셋
	if (ListItem->_justActivated)
	{
		ListItem->_justActivated = false;
	}

	for (int32 i = 0; i < _statListEntries.Num(); i++)
	{
		UGsUISealStatListItem* StatListItem = (i < _statListitems.Num()) ? _statListitems[i] : NewObject<UGsUISealStatListItem>();
		if (nullptr == StatListItem)
		{
			GSLOG(Error, TEXT("nullptr == StatListItem"));
			continue;
		}

		UGsUISealStatListEntry* ListEntry = _statListEntries[i];

		if (ListItem->_statInfos.IsValidIndex(i))
		{
			// 스탯 유효 및 출력
			const TPair<StatType, StatValue>& StatInfo = ListItem->_statInfos[i];
			StatListItem->_activated = IsActivated;
			StatListItem->_statType = StatInfo.Key;
			StatListItem->_statValue = StatInfo.Value;
			ListEntry->Invalidate(StatListItem);
			ListEntry->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			// 스탯 무효 및 숨김
			ListEntry->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
