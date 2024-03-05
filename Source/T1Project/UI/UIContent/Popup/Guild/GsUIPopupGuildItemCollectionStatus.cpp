
#include "GsUIPopupGuildItemCollectionStatus.h"
#include "Guild/GsGuildItemCollectionData.h"
#include "Classes/Collection/GsCollectionData.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/ItemCollection/GsUIItemCollectionApplyStatus.h"
#include "UI/UIContent/Window/ItemCollection/GsUIItemCollectionApplyStatus.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "T1Project/T1Project.h"


void UGsUIPopupGuildItemCollectionStatus::BeginDestroy()
{
	if (_slotHelperApplyStat)
	{
		_slotHelperApplyStat->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupGuildItemCollectionStatus::OnRefreshEntryApplyStat);
		_slotHelperApplyStat = nullptr;
	}

	if (_btnOk)
	{
		_btnOk->OnClicked.RemoveDynamic(this, &UGsUIPopupGuildItemCollectionStatus::OnClickOk);
		_btnOk = nullptr;
	}

	UGsUIPopup::BeginDestroy();
}


void UGsUIPopupGuildItemCollectionStatus::NativeOnInitialized()
{
	UGsUIPopup::NativeOnInitialized();

	_slotHelperApplyStat = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperApplyStat->Initialize(_subClassOfApplyStat, _panelApplyStatList);
	_slotHelperApplyStat->OnRefreshEntry.AddDynamic(this, &UGsUIPopupGuildItemCollectionStatus::OnRefreshEntryApplyStat);


	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupGuildItemCollectionStatus::OnClickOk);
}


void UGsUIPopupGuildItemCollectionStatus::NativeConstruct()
{
	UGsUIPopup::NativeConstruct();

	InvalidateAll();
}


void UGsUIPopupGuildItemCollectionStatus::NativeDestruct()
{
	UGsUIPopup::NativeDestruct();
}


void UGsUIPopupGuildItemCollectionStatus::OnClickOk()
{
	UGsUIPopup::Close();
}


void UGsUIPopupGuildItemCollectionStatus::OnRefreshEntryApplyStat(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIItemCollectionApplyStatus> widget = Cast<UGsUIItemCollectionApplyStatus>(InEntry);
	if (false == widget.IsValid())
	{
		GSLOG(Warning, TEXT("false == widget.IsValid()"));
		return;
	}

	if (false == _applyEffectTextList.IsValidIndex(InIndex))
	{
		GSLOG(Warning, TEXT("false == _applyEffectTextList.IsValidIndex(InIndex) : invaild InIndex %d"), InIndex);
		return;
	}

	const TPair<FText, FText >& _EffectText = _applyEffectTextList[InIndex];

	const FText textEffect = FText::FromString(FString::Format(TEXT("{0} {1}"), { *_EffectText.Key.ToString(), *_EffectText.Value.ToString() }));
	widget->SetData(textEffect);
}


void UGsUIPopupGuildItemCollectionStatus::InvalidateAll()
{
	const FGsGuildItemCollectionData* guildItemCollection = GGuildItemCollection();
	if (nullptr == guildItemCollection)
	{
		GSLOG(Warning, TEXT("nullptr == guildItemCollection"));
		return;
	}

	_applyEffectTextList.Empty();

	const IGsCollectionData* collectionData = guildItemCollection->GetCollectionData();
	if (nullptr != collectionData)
	{
		FGsEffectStringHelper::GetCollectionEffectStringList(collectionData, _applyEffectTextList);
	}

	const static int32 EMPTY_COUNT = 0;
	int32 maxCount = _applyEffectTextList.Num();
	if (EMPTY_COUNT < maxCount)
	{
		_slotHelperApplyStat->RefreshAll(maxCount);
	}

	
	const static int32 FILL_LIST_SWITCHER_INDEX = 0;
	const static int32 EMPTY_LIST_SWITCHER_INDEX = 1;
	_switcherApplyStatList->SetActiveWidgetIndex((0 >= maxCount) ? EMPTY_LIST_SWITCHER_INDEX : FILL_LIST_SWITCHER_INDEX);
}