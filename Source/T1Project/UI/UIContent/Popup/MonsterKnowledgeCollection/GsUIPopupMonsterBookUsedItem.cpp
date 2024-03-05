#include "GsUIPopupMonsterBookUsedItem.h"
#include "../Currency/GsCostPackage.h"
#include "../Item/GsItemManager.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../UI/UIContent/Common/GsUICheckBoxCommonText.h"
#include "../UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "../UI/UIContent/Window/Option/GsUIOptionButtonGroup.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "T1Project.h"




void UGsUIPopupMonsterBookUsedItem::BeginDestroy()
{
	if (_panelBtnCheckBoxGroupHelper)
	{
		_panelBtnCheckBoxGroupHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMonsterBookUsedItem::OnRefreshCheckBox);
		_panelBtnCheckBoxGroupHelper = nullptr;
	}

	if (_panelBtnSlotHelper)
	{
		_panelBtnSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMonsterBookUsedItem::OnRefreshTabButton);
		_panelBtnSlotHelper = nullptr;
	}
	
	if (_btnCancel)
	{
		_btnCancel->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookUsedItem::OnCancel);
	}
	_btnCancel = nullptr;

	if (_btnApply)
	{
		_btnApply->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookUsedItem::OnApply);
	}
	_btnApply = nullptr;

	if (_btnTooltip)
	{
		_btnTooltip->OnClicked.RemoveDynamic(this, &UGsUIPopupMonsterBookUsedItem::OnClickedTooltip);
	}
	_btnTooltip = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMonsterBookUsedItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnCancel)
	{
		_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookUsedItem::OnCancel);
	}

	if (_btnApply)
	{
		_btnApply->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookUsedItem::OnApply);
	}

	if (_btnTooltip)
	{
		_btnTooltip->OnClicked.AddDynamic(this, &UGsUIPopupMonsterBookUsedItem::OnClickedTooltip);
	}
	
	// 2021/12/24 PKT - 우선 순위
	_panelBtnSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_panelBtnSlotHelper->Initialize(_btnSlotEntryClass, _btnHorizontalBox);
	_panelBtnSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMonsterBookUsedItem::OnRefreshTabButton);

	// 2021/12/24 PKT - 등급별 체크 박스
	_panelBtnCheckBoxGroupHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_panelBtnCheckBoxGroupHelper->Initialize(_checkBoxEntryClass, _checkGroupBox);
	_panelBtnCheckBoxGroupHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMonsterBookUsedItem::OnRefreshCheckBox);
}

void UGsUIPopupMonsterBookUsedItem::NativeConstruct()
{
	Super::NativeConstruct();

	const int32 maxGradeCount = StaticCast<int32>(MonsterKnowledgeBookGrade::MAX) - 1;
	_panelBtnCheckBoxGroupHelper->RefreshAll(maxGradeCount);

	const static int32 BUTTON_PRIORITY_COUNT = 2;
	_panelBtnSlotHelper->RefreshAll(BUTTON_PRIORITY_COUNT);

	_btnTogglePriority.Clear();
	_btnTogglePriority.AddToggleWidgetByParentPanel(_btnHorizontalBox);
	
	FText textTip;
	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("monsterBook_ItemAutoUsePopupTooltip"), textTip);
	_tooltipWithLifeTime->SetDescText(textTip);
	_tooltipWithLifeTime->Close();
}

void UGsUIPopupMonsterBookUsedItem::NativeDestruct()
{
	_tooltipWithLifeTime->Close();

	Super::NativeDestruct();
}

void UGsUIPopupMonsterBookUsedItem::ImportOption(uint64 InParam)
{
	bool isActive = bool(InParam & uint64(OPTION_FLAG::PRIORITY));
	_btnTogglePriority.SetSelectedIndex(isActive ? 1 : 0);

	const int32 maxCount = _panelBtnCheckBoxGroupHelper->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUICheckBoxCommonText> item = Cast<UGsUICheckBoxCommonText>(_panelBtnCheckBoxGroupHelper->GetEntry(idx));
		if (true == item.IsValid())
		{	// 2023/2/10 PKT - 각 등급 별 Bit 이동으로 옵션 값 체크
			isActive = bool(InParam & (uint64(OPTION_FLAG::BOOKGRADE) << idx));
			item->SetIsSelected(isActive);
		}
	}
}

void UGsUIPopupMonsterBookUsedItem::ExportOption(uint64& OutParam)
{
	// 2023/2/8 PKT - Clear
	OutParam = 0;
	// 2023/2/8 PKT - Priority
	OutParam = (1 == _btnTogglePriority.GetSelectedIndex()) ? uint64(OPTION_FLAG::PRIORITY) : 0;
	// 2023/2/8 PKT - Garde
	for (int32 idx = 0; idx < _panelBtnCheckBoxGroupHelper->GetItemCount(); ++idx)
	{
		TWeakObjectPtr<UGsUICheckBoxCommonText> item = Cast<UGsUICheckBoxCommonText>(_panelBtnCheckBoxGroupHelper->GetEntry(idx));
		if (true == item.IsValid())
		{
			OutParam |= uint64(item->GetIsSelected() ? (uint64(OPTION_FLAG::BOOKGRADE) << idx) : 0);
		}
	}
}

bool UGsUIPopupMonsterBookUsedItem::IsSelectedCheckBox()
{
	const int32 maxCount = _panelBtnCheckBoxGroupHelper->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUICheckBoxCommonText> item = Cast<UGsUICheckBoxCommonText>(_panelBtnCheckBoxGroupHelper->GetEntry(idx));
		if (item.IsValid() && true == item->GetIsSelected())
		{
			return true;
		}
	}

	return false;
}

uint64 UGsUIPopupMonsterBookUsedItem::SetDefaultCheckBox()
{
	uint64 outOption = 0;
	outOption = outOption | (_btnTogglePriority.GetSelectedIndex()) ? 1 : 0;
	outOption = outOption | (uint64(OPTION_FLAG::BOOKGRADE) << int32(0));
	return outOption;
}

void UGsUIPopupMonsterBookUsedItem::OnCancel() 
{
	const static int32 CLEAN_OPTION = 0;
	if (OnSavedOptionDelegate.IsBound())
	{
		OnSavedOptionDelegate.Execute(CLEAN_OPTION);
	}

	Close();
}

void UGsUIPopupMonsterBookUsedItem::OnApply() 
{
	if (false == GsMonsterCollection()->BookExpCostSet().IsValidIndex(_selectedIndex))
	{
		GSLOG(Error, TEXT("false == GsMonsterCollection()->BookExpCostSet().IsValidIndex(InIndex) : [%d]"), _selectedIndex);
		return;
	}

	const auto& costItem = GsMonsterCollection()->BookExpCostSet()[_selectedIndex];
	const FGsCostElement* costElement = costItem.GetFirstCostElement();
	if (nullptr == costElement)
	{
		GSLOG(Error, TEXT("nullptr == costElement"));
		return;
	}

	ItemDBId itemDBId = INVALID_ITEM_DB_ID;
	if (costElement->PlayerHasAmountMax())
	{
		const auto& findSet = GItem()->FindByTID(costElement->GetItemId(), ItemType::INGREDIENT);
		if (0 < findSet.Num() && findSet[0].IsValid())
		{
			itemDBId = findSet[0].Pin().Get()->GetDBID();
		}
	}

	uint64 savedOption = 0;
	ExportOption(savedOption);

	// 2023/2/9 PKT - 현재 설정을 저장한다.
	if (OnSavedOptionDelegate.IsBound())
	{
		OnSavedOptionDelegate.Execute(savedOption);
	}

	// 2023/2/6 PKT - 서버에 패킷을 보낸다.
	bool isPriority = bool(savedOption & uint64(OPTION_FLAG::PRIORITY));

	TArray<MonsterKnowledgeBookGrade> bookGradeSet;
	const int32 maxCount = StaticCast<int32>(MonsterKnowledgeBookGrade::MAX);
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		if (savedOption & (uint64(OPTION_FLAG::BOOKGRADE) << idx))
		{
			int32 gradeIndex = idx + 1;
			bookGradeSet.Emplace(StaticCast<MonsterKnowledgeBookGrade>(gradeIndex));
		}
	}

	FGsNetSendServiceWorld::Send_MonsterKnowledgeExpGainItemUseAll(itemDBId, _amountUsed, bookGradeSet, isPriority);
}

void UGsUIPopupMonsterBookUsedItem::OnRefreshCheckBox(int32 InIndex, UWidget* InEntry)
{
	//TWeakObjectPtr<UGsUICheckBoxCommonText> checkBoxEntry = Cast<UGsUICheckBoxCommonText>(InEntry);
	//if (false == checkBoxEntry.IsValid())
	//{
	//	GSLOG(Error, TEXT("false == checkBoxEntry.IsValid()"));
	//	return;
	//}

	//// 2023/2/8 PKT - Enum 등급 순
	//int32 bookGradeIndex = (InIndex + 1);
	//MonsterKnowledgeBookGrade bookGrade = StaticCast<MonsterKnowledgeBookGrade>(bookGradeIndex);
	//const auto& schemaDetailInfo = GsMonsterCollection()->BookGradeDetailInfo(bookGrade);
	//if (nullptr == schemaDetailInfo)
	//{
	//	GSLOG(Error, TEXT("nullptr == schemaDetailInfo"));
	//	return;
	//}

	//checkBoxEntry->SetText(schemaDetailInfo->gardeName);
	//checkBoxEntry->_onClickSlot.BindUObject(this, &UGsUIPopupMonsterBookUsedItem::OnClickedCheckBox);
}

void UGsUIPopupMonsterBookUsedItem::OnRefreshTabButton(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIOptionButtonGroup> btnEntry = Cast<UGsUIOptionButtonGroup>(InEntry);
	if (false == btnEntry.IsValid())
	{
		GSLOG(Error, TEXT("false == btnEntry.IsValid()"));
		return;
	}

	const static int32 PRIORITY_LOW_INDEX = 0;
	const static int32 PRIORITY_HIGHT_INDEX = 1;

	FText format;
	if (PRIORITY_LOW_INDEX == InIndex)
	{
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("monsterBook_ItemAutoUse_PriorityLow"), format);
	}
	else if (PRIORITY_HIGHT_INDEX == InIndex)
	{
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("monsterBook_ItemAutoUse_PriorityHigh"), format);
	}

	// 2021/12/28 PKT - Set Text
	btnEntry.Get()->SetData(format);
}

void UGsUIPopupMonsterBookUsedItem::OnClickedCheckBox(bool bIsChecked, int32 InIndexInGroup)
{
	if (false == bIsChecked && false == IsSelectedCheckBox())
	{	// 2023/2/8 PKT - 모든 체크 박스가 꺼져 있다면..
		uint64 defautl_option = SetDefaultCheckBox();
		ImportOption(defautl_option);
	}
}

void UGsUIPopupMonsterBookUsedItem::OnClickedTooltip()
{
	_tooltipWithLifeTime->ToggleOpenClose();
}

void UGsUIPopupMonsterBookUsedItem::SetData(uint64 InParam /*= 0*/) 
{	
	ImportOption(InParam);

	Invalidate();
}

void UGsUIPopupMonsterBookUsedItem::Invalidate()
{
	Super::Invalidate();

	if (UNSELECTED_INDEX < _selectedIndex)
	{	// 2023/2/8 PKT - 선택한 아이템이 있다면 체크 박스를 하나라도 기본으로 선택해 준다.
		if (false == IsSelectedCheckBox())
		{
			uint64 defautl_option = SetDefaultCheckBox();
			ImportOption(defautl_option);
		}
	}
	else
	{	// 2023/2/8 PKT - 모든 Option Off
		const static int32 CLEAN_OPTION = 0;
		ImportOption(CLEAN_OPTION);		
	}

	// 2023/2/8 PKT - 아이템이 없다면 굳이 버튼을 활성화 하지 않는다.
	_btnApply->SetIsEnabled(UNSELECTED_INDEX < _selectedIndex ? true : false);
	
	for (int32 idx = 0; idx < _panelBtnCheckBoxGroupHelper->GetItemCount(); ++idx)
	{
		TWeakObjectPtr<UGsUICheckBoxCommonText> item = Cast<UGsUICheckBoxCommonText>(_panelBtnCheckBoxGroupHelper->GetEntry(idx));
		if (item.IsValid())
		{
			item->SetIsEnabled(UNSELECTED_INDEX < _selectedIndex ? true : false);
		}
	}

	for (int32 idx = 0; idx < _panelBtnSlotHelper->GetItemCount(); ++idx)
	{
		TWeakObjectPtr<UGsUIOptionButtonGroup> item = Cast<UGsUIOptionButtonGroup>(_panelBtnSlotHelper->GetEntry(idx));
		if (item.IsValid())
		{
			item->SetIsEnabled(UNSELECTED_INDEX < _selectedIndex ? true : false);
		}
	}
}

void UGsUIPopupMonsterBookUsedItem::OnInputCancel()
{
	OnCancel();
}