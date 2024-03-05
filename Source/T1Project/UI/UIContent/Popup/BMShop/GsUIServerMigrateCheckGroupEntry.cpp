#include "GsUIServerMigrateCheckGroupEntry.h"
#include "GsUIServerMigrateCheckConditionEntry.h"
#include "DataSchema/BMShop/ServerMigrate/GsSchemaServerMigrateGroupData.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateHandler.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "T1Project.h"

void UGsUIServerMigrateCheckGroupEntry::BeginDestroy()
{
	if (nullptr != _conditionItemSlotHelper)
	{
		_conditionItemSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIServerMigrateCheckGroupEntry::ConditionItemRefresh);
		_conditionItemSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIServerMigrateCheckGroupEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _conditionItemSlotHelper)
	{
		_conditionItemSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_conditionItemSlotHelper->Initialize(_conditionItemWidgetClass, _verticlaBoxConditionSet);
		_conditionItemSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIServerMigrateCheckGroupEntry::ConditionItemRefresh);
	}
}

void UGsUIServerMigrateCheckGroupEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIServerMigrateCheckGroupEntry::ConditionItemRefresh(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIServerMigrateCheckConditionEntry> entry = Cast<UGsUIServerMigrateCheckConditionEntry>(InEntry);
	if (false == entry.IsValid() || !_data.IsValid())
	{
		GSLOG(Error, TEXT("false == entry.IsValid() || nullptr == _schemaGroupData"));
		return;
	}
	if (auto data = _data.Pin())
	{
		const auto& conditions = data->GetConditions();
		if (conditions.IsValidIndex(InIndex))
		{
			entry->SetData(data->GetMigrateType(), data->GetConditions()[InIndex]);
			entry->RefreshUI();
		}
	}
}

void UGsUIServerMigrateCheckGroupEntry::RefreshUI()
{
	if (auto data = _data.Pin())
	{
		_textGroupName->SetText(data->GetGroupTitle());
		_conditionItemSlotHelper->RefreshAll(data->GetConditions().Num());
	}
}
