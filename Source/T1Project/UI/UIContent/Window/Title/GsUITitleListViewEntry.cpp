
#include "GsUITitleListViewEntry.h"
#include "Title/GsTitleData.h"
#include "Title/GsTitleHelper.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "UI/UIContent/Window/Title/GsUITitleStatItemEntry.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"
#include "../Runtime/UMG/Public/Components/RichTextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "../Runtime/UMG/Public/Components/VerticalBox.h"
#include "../Runtime/UMG/Public/Components/TextBlock.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "../Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "../Runtime/UMG/Public/Components/Image.h"
#include "T1Project.h"



void UGsUITitleListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnEquip->OnClicked.AddDynamic(this, &UGsUITitleListViewEntry::OnClickedBtnEquip);

	_btnUnEquip->OnClicked.AddDynamic(this, &UGsUITitleListViewEntry::OnClickedBtnUnEquip);
	//_btnUnEquip->_pressSound

	// 2021/08/02 PKT - 메인 탭
	_slotHelperStat = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperStat->Initialize(_subClassOfStat, _verticalBoxStatList);
	_slotHelperStat->OnRefreshEntry.AddDynamic(this, &UGsUITitleListViewEntry::OnRefreshStatList);
}

void UGsUITitleListViewEntry::BeginDestroy()
{
	if (_btnEquip)
	{
		_btnEquip->OnClicked.RemoveDynamic(this, &UGsUITitleListViewEntry::OnClickedBtnEquip);
		_btnEquip = nullptr;
	}

	if (_btnUnEquip)
	{
		_btnUnEquip->OnClicked.RemoveDynamic(this, &UGsUITitleListViewEntry::OnClickedBtnUnEquip);
		_btnUnEquip = nullptr;
	}

	if (_slotHelperStat)
	{
		_slotHelperStat->OnRefreshEntry.RemoveDynamic(this, &UGsUITitleListViewEntry::OnRefreshStatList);
	}

	Super::BeginDestroy();
}

void UGsUITitleListViewEntry::NativeDestruct()
{
	_titleData = nullptr;

	Super::NativeDestruct(); 
}

void UGsUITitleListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsTitleListViewEntryData* entryData
		= Cast<UGsTitleListViewEntryData>(inListItemObject);

	_titleData = entryData->_titleData;

	OnClickedTitleEquipChanged = entryData->OnClickedTitleEquipChanged;

	/**
	 * title text / Color
	 */
	_textTitle->SetText(_titleData->GetTitle());
	_textTitle->SetColorAndOpacity(_titleData->GetTitleColor());

	const static int32 EMPTY_STAT = 0;
	const int32 maxCount = _titleData->GetStatCount();
	if (EMPTY_STAT >= maxCount)
	{	
		const static int32 EMPTY_STAT_SWITCHER_INDEX = 1;
		_swithcerStatDisplay->SetActiveWidgetIndex(EMPTY_STAT_SWITCHER_INDEX);

		FText message = _titleData->GetPassivitySubDesc();
		if (message.IsEmpty())
		{
			FGsTitleHelper::FindText(TEXT("UI_Passivity_Empty"), message);
		}

		_textNotifyEmptyStat->SetText(message);
	}
	else
	{
		const static int32 NOT_EMPTY_STAT_SWITCHER_INDEX = 0;
		_swithcerStatDisplay->SetActiveWidgetIndex(NOT_EMPTY_STAT_SWITCHER_INDEX);

		_slotHelperStat->RefreshAll(maxCount);
	}

	Refresh();
}

void UGsUITitleListViewEntry::OnRefreshStatList(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITitleStatItemEntry> item = Cast<UGsUITitleStatItemEntry>(InEntry);
	if (false == item.IsValid())
	{
		GSLOG(Error, TEXT("false == item.IsValid()"));
		return;
	}

	if (nullptr == _titleData)
	{
		GSLOG(Error, TEXT("nullptr == _titleData"));
		return;
	}

	StatType statType = _titleData->GetStatType(InIndex);
	int32 statValue = _titleData->GetStatValue(InIndex);

	FText valueName;
	FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statType, statValue, valueName);

	item->SetData(_titleData->GetStatName(InIndex), valueName);
}

void UGsUITitleListViewEntry::OnClickedBtnEquip()
{
	if (nullptr == _titleData)
	{
		GSLOG(Error, TEXT("nullptr == _titleData"));
		return;
	}

	OnClickedTitleEquipChanged.ExecuteIfBound(StaticCast<int32>(_titleData->GetId()), true);
}

void UGsUITitleListViewEntry::OnClickedBtnUnEquip()
{
	if (nullptr == _titleData)
	{
		GSLOG(Error, TEXT("nullptr == _titleData"));
		return;
	}

	OnClickedTitleEquipChanged.ExecuteIfBound(StaticCast<int32>(_titleData->GetId()), false);
}


void UGsUITitleListViewEntry::Refresh()
{
	if (nullptr == _titleData)
	{
		GSLOG(Error, TEXT("nullptr == _titleData"));
		return;
	}

	_textcondition->SetText(_titleData->GetDesc());

	const static int32 EQUIP_SWITCHER_INDEX = 0;
	const static int32 UNEQUIP_SWITCHER_INDEX = 1;
	const static int32 DISABLE_SWITCHER_INDEX = 2;

	if (false == _titleData->IsAcquire())
	{
		_swithcerChangedStateBtn->SetActiveWidgetIndex(DISABLE_SWITCHER_INDEX);
	}
	else
	{
		if (true == _titleData->IsEquip())
		{	// 2022/01/20 PKT - 장착 중이라면..
			_swithcerChangedStateBtn->SetActiveWidgetIndex(UNEQUIP_SWITCHER_INDEX);
		}
		else
		{	// 2022/01/20 PKT - 장착 중이 아니라면..
			_swithcerChangedStateBtn->SetActiveWidgetIndex(EQUIP_SWITCHER_INDEX);
		}
	}

	_imageEquipEffectMarker->SetVisibility(_titleData->IsEquip() ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	// 2022/01/24 PKT - RedDot..
	bool isNewTitle = _titleData->IsNewAcquire();
	_redDotNormal->SetRedDot(isNewTitle);
}


void UGsUITitleListViewEntry::Invalidate()
{
	if (nullptr == _titleData)
	{
		GSLOG(Error, TEXT("nullptr == _titleData"));
		return;
	}

	Refresh();
}


const FGsTitleData* UGsUITitleListViewEntry::GetData() const
{
	return _titleData;
}