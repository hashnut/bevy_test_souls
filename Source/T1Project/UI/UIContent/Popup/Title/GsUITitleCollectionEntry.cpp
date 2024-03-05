
#include "GsUITitleCollectionEntry.h"
#include "Title/ETitleEnum.h"
#include "Title/GsTitleRewardData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/Title/GsUITitleCollectionStatItemEntry.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "../Runtime/UMG/Public/Components/TextBlock.h"
#include "../Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "../Runtime/UMG/Public/Components/VerticalBox.h"
#include "../../../../T1Project.h"



void UGsUITitleCollectionEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperStatData = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperStatData->Initialize(_subClassOfStatEntry, _verticalBoxStat);
	_slotHelperStatData->OnRefreshEntry.AddDynamic(this, &UGsUITitleCollectionEntry::OnRefreshStatEntry);
}

void UGsUITitleCollectionEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_vfxChangedEffect->HideVFX();
	_vfxChangedEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUITitleCollectionEntry::FinishedVFX);	
}

void UGsUITitleCollectionEntry::NativeDestruct()
{
	Super::NativeDestruct();

	_vfxChangedEffect->HideVFX();
}

void UGsUITitleCollectionEntry::FinishedVFX()
{
	_OnFinishedVFX.ExecuteIfBound();
}

void UGsUITitleCollectionEntry::SetActivate(bool InIsActive)
{
	if (InIsActive)
	{
		_switcherSymbol->SetActiveWidgetIndex(ACTIVE_WIDGET_INDEX);
	}
	else
	{
		_switcherSymbol->SetActiveWidgetIndex(INACTIVE_WIDGET_INDEX);
	}
}

void UGsUITitleCollectionEntry::OnRefreshStatEntry(int32 InIndex, class UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITitleCollectionStatItemEntry> item = Cast<UGsUITitleCollectionStatItemEntry>(InEntry);
	if (false == item.IsValid())
	{
		GSLOG(Error, TEXT("false == item.IsValid()"));
		return;
	}

	TPair<StatType, int32> statData;
	if (false == _data->FindStatData(InIndex, statData))
	{
		GSLOG(Error, TEXT("false == _data->FindStatData(InIndex, statData)"));
		return;
	}

	FText statName;
	FGsStatHelper::GetDisPlayStatName(statData.Key, statName);

	FText valueName;
	FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statData.Key, statData.Value, valueName);

	item->SetData(statName, valueName);
}

void UGsUITitleCollectionEntry::SetData(const class FGsTitleRewardData* InData, bool InIsConfirm)
{
	_data = InData;

	// 2022/01/26 PKT - Text Count
	_textStatCount->SetText(FText::AsNumber(_data->GetAcquisitionCount()));

	const int32 maxCount = _data->GetStatCount();
	_slotHelperStatData->RefreshAll(maxCount);

	bool isActivate = (ETitleRewardState::Activate == InData->GetState());
	if (false == InIsConfirm)
	{
		isActivate = (false == isActivate);
	}
	SetActivate(isActivate);

	//_redDotNormal->SetRedDot(false == InIsConfirm && ETitleRewardState::Activate == _data->GetState());
}

void UGsUITitleCollectionEntry::PlayEffect(bool InIsActive)
{
	_vfxChangedEffect->ShowVFX();

	SetActivate(InIsActive);
}


const FGsTitleRewardData* UGsUITitleCollectionEntry::GetData() const
{
	return _data;
}

UGsUITitleCollectionEntry::FOnFinishedVFX& UGsUITitleCollectionEntry::OnFinishedVFX()
{
	return _OnFinishedVFX;
}