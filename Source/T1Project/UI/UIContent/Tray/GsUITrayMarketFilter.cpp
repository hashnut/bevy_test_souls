// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayMarketFilter.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsUITwoSideSlider.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUICheckBoxCommonText.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMarketMessageParam.h"
#include "DataSchema/Market/GsSchemaMarketFilterSetting.h"
#include "DataSchema/Stat/GsSchemaStatFormatingData.h"
#include "DataSchema/Item/GsSchemaItemEquipRefineOptionStatBalance.h"
#include "Market/GsMarketHelper.h"
#include "Item/GsItemHelper.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "T1Project.h"


void UGsUITrayMarketFilter::BeginDestroy()
{
	if (_slotHelperEnchantProp)
	{
		_slotHelperEnchantProp->OnRefreshEntry.RemoveDynamic(this, &UGsUITrayMarketFilter::OnRefreshEntryEnchantProp);
	}
	_slotHelperEnchantProp = nullptr;
	
	if (_slotHelperRefine)
	{
		_slotHelperRefine->OnRefreshEntry.RemoveDynamic(this, &UGsUITrayMarketFilter::OnRefreshEntryRefine);
	}
	_slotHelperRefine = nullptr;
	
	Super::BeginDestroy();
}

void UGsUITrayMarketFilter::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_sliderEnchat->SetMinStepValue(0.f);
	_sliderEnchat->SetMaxStepValue(static_cast<float>(MAX_ENCHANT_LEVEL));
	_sliderEnchat->SetStepSize(1.f);
	_sliderEnchat->OnEndSlideValue().BindUObject(this, &UGsUITrayMarketFilter::OnSlideEnchant);

	_sliderShield->SetMinStepValue(0.f);
	_sliderShield->SetMaxStepValue(static_cast<float>(MAX_SHIELD_COUNT));
	_sliderShield->SetStepSize(1.f);
	_sliderShield->OnEndSlideValue().BindUObject(this, &UGsUITrayMarketFilter::OnSlideShield);
	
	_btnOk->OnClicked.AddDynamic(this, &UGsUITrayMarketFilter::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUITrayMarketFilter::OnClickCancel);

	// 등급
	_slotHelperItemGrade = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperItemGrade->Initialize(_entryWidgetClass, _panelGradeListRoot);
	_slotHelperItemGrade->OnCreateEntry.AddDynamic(this, &UGsUITrayMarketFilter::OnCreateEntryItemGrade);
	_slotHelperItemGrade->OnRefreshEntry.AddDynamic(this, &UGsUITrayMarketFilter::OnRefreshEntryItemGrade);
	
	// 특성
	_slotHelperEnchantProp = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperEnchantProp->Initialize(_entryWidgetClass, _panelEnchantPropListRoot);
	_slotHelperEnchantProp->OnCreateEntry.AddDynamic(this, &UGsUITrayMarketFilter::OnCreateEntryEnchantProp);
	_slotHelperEnchantProp->OnRefreshEntry.AddDynamic(this, &UGsUITrayMarketFilter::OnRefreshEntryEnchantProp);
	
	// 제련옵션
	_slotHelperRefine = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperRefine->Initialize(_entryWidgetClass, _panelRefineListRoot);
	_slotHelperRefine->OnCreateEntry.AddDynamic(this, &UGsUITrayMarketFilter::OnCreateEntryRefine);
	_slotHelperRefine->OnRefreshEntry.AddDynamic(this, &UGsUITrayMarketFilter::OnRefreshEntryRefine);
	
	// 전체 버튼들
	_checkBoxGradeAll->OnClickCheckBox().BindUObject(this, &UGsUITrayMarketFilter::OnCheckGradeAll);
	_checkBoxEnchantPropAll->OnClickCheckBox().BindUObject(this, &UGsUITrayMarketFilter::OnCheckEnchantPropAll);
	_checkBoxRefineAll->OnClickCheckBox().BindUObject(this, &UGsUITrayMarketFilter::OnCheckRefineAll);

	// 제련 리스트 만들어놓기
	InitRefineStatList();

	// 숫자 세팅
	TArray<UTextBlock*> enchantNumList;
	RecursiveFindTextBlock(_panelRootEnchantNum, enchantNumList);
	for (int i = 0; i < enchantNumList.Num(); ++i)
	{
		enchantNumList[i]->SetText(FText::AsNumber(i)); 
	}

	TArray<UTextBlock*> sheildNumList;
	RecursiveFindTextBlock(_panelRootShieldNum, sheildNumList);
	for (int i = 0; i < sheildNumList.Num(); ++i)
	{
		sheildNumList[i]->SetText(FText::AsNumber(i));
	}
}

void UGsUITrayMarketFilter::NativeDestruct()
{
	_data.Clear();

	Super::NativeDestruct();
}

void UGsUITrayMarketFilter::SetData(bool bIsWorldMarket)
{
	_bIsWorldMarket = bIsWorldMarket;

	FGsMarketFilterData& filterData = GSMarket()->GetFilterData();
	_data.Set(filterData);

	// 슬라이더 세팅
	if (_data.IsFilterLevel())
	{
		_sliderEnchat->SetCurrentValue(_data._enchantLevelMin, _data._enchantLevelMax, false);
	}
	else
	{
		_sliderEnchat->SetCurrentValue(0, MAX_ENCHANT_LEVEL, false);
	}

	if (_data.IsFilterShield())
	{
		_sliderShield->SetCurrentValue(_data._shieldMin, _data._shieldMax, false);
	}
	else
	{
		_sliderShield->SetCurrentValue(0, MAX_SHIELD_COUNT, false);
	}

	// 필터 리스트 세팅
	_gradeList.Empty();
	_enchantPropList.Empty();

	_worldMinimumGrade = FGsMarketHelper::GetWorldMarketMinimumItemGrade();

	if (const FGsSchemaMarketFilterSetting* setting = FGsMarketHelper::GetMarketFilterSetting())
	{
		_gradeList.Append(setting->itemGradeList);
		_enchantPropList.Append(setting->enchantBonusList);
	}

	// 전체 버튼들 이름설정, 체크처리(_data의 값대로 처리)
	FText textAll;
	FText::FindText(TEXT("MarketText"), TEXT("Filter_All"), textAll);
	_checkBoxGradeAll->SetText(textAll);
	_checkBoxEnchantPropAll->SetText(textAll);
	_checkBoxRefineAll->SetText(textAll);

	// 전체 버튼들 체크처리
	_checkBoxGradeAll->SetIsSelected((0 == _data._gradeList.Num()) ? true : false);
	_checkBoxEnchantPropAll->SetIsSelected((0 == _data._enchantPropList.Num()) ? true : false);
	_checkBoxRefineAll->SetIsSelected((0 == _data._refineStatList.Num()) ? true : false);

	// 주의: RefreshAll 전에 _data 정보가 차있어야 한다.
	_slotHelperItemGrade->RefreshAll(_gradeList.Num());
	_slotHelperEnchantProp->RefreshAll(_enchantPropList.Num());
	_slotHelperRefine->RefreshAll(_refineStatList.Num());
}

void UGsUITrayMarketFilter::InitRefineStatList()
{
	_refineStatList.Empty();

	const UGsTableStatFormatingData* statTable = nullptr;
	if (const UGsTable* formatTable = FGsSchemaStatFormatingData::GetStaticTable())
	{
		statTable = Cast<UGsTableStatFormatingData>(formatTable);
	}

	if (nullptr == statTable)
	{
		return;
	}

	TArray<const FGsSchemaItemEquipRefineOptionStatBalance*> rowList;
	if (const UGsTable* table = FGsSchemaItemEquipRefineOptionStatBalance::GetStaticTable())
	{
		table->GetAllRows(rowList);
	}

	TArray<StatType> statList;
	for (const FGsSchemaItemEquipRefineOptionStatBalance* row : rowList)
	{
		statList.AddUnique(row->stat);
	}

	for (int i = 0, maxCount = statList.Num(); i < maxCount; ++i)
	{
		const FGsSchemaStatFormatingData* statRow = nullptr;
		if (statTable->FindRowById(statList[i], statRow))
		{
			_refineStatList.Emplace(statRow);
		}
	}

	// uiOrderNum 오름차순으로 소팅
	_refineStatList.Sort([](const FGsSchemaStatFormatingData& A, const FGsSchemaStatFormatingData& B)
		{
			return (A.uiOrderNum <= B.uiOrderNum) ? true : false;
		});
}

void UGsUITrayMarketFilter::OnCreateEntryItemGrade(UWidget* InEntry)
{
	if (UGsUICheckBoxCommonText* entry = Cast<UGsUICheckBoxCommonText>(InEntry))
	{
		entry->OnClickCheckBox().BindUObject(this, &UGsUITrayMarketFilter::OnCheckGrade);
	}
}

void UGsUITrayMarketFilter::OnCreateEntryEnchantProp(UWidget* InEntry)
{
	if (UGsUICheckBoxCommonText* entry = Cast<UGsUICheckBoxCommonText>(InEntry))
	{
		entry->OnClickCheckBox().BindUObject(this, &UGsUITrayMarketFilter::OnCheckEnchantProp);
	}
}

void UGsUITrayMarketFilter::OnCreateEntryRefine(UWidget* InEntry)
{
	if (UGsUICheckBoxCommonText* entry = Cast<UGsUICheckBoxCommonText>(InEntry))
	{
		entry->OnClickCheckBox().BindUObject(this, &UGsUITrayMarketFilter::OnCheckRefine);
	}
}

void UGsUITrayMarketFilter::OnRefreshEntryItemGrade(int32 InIndex, class UWidget* InEntry)
{
	if (_gradeList.IsValidIndex(InIndex))
	{
		if (UGsUICheckBoxCommonText* entry = Cast<UGsUICheckBoxCommonText>(InEntry))
		{
			ItemGrade grade = _gradeList[InIndex];

			FText textName;
			FGsItemHelper::GetItemGradeText(grade, textName);
			entry->SetText(textName);
			entry->SetIndexInGroup(InIndex);
			entry->SetIsSelected(_data._gradeList.Contains(grade));
			
			if (_bIsWorldMarket)
			{
				entry->SetIsEnabled((_worldMinimumGrade <= grade) ? true : false);
			}
			else
			{
				entry->SetIsEnabled(true);
			}
		}
	}
}

void UGsUITrayMarketFilter::OnRefreshEntryEnchantProp(int32 InIndex, UWidget* InEntry)
{
	if (_enchantPropList.IsValidIndex(InIndex))
	{
		if (UGsUICheckBoxCommonText* entry = Cast<UGsUICheckBoxCommonText>(InEntry))
		{
			EGsItemEnchantBonusType enchantBonusType = _enchantPropList[InIndex];

			FText textName;
			FGsItemHelper::GetEnchantBonusText(enchantBonusType, textName);
			entry->SetText(textName);
			entry->SetIndexInGroup(InIndex);
			entry->SetIsSelected(_data._enchantPropList.Contains(enchantBonusType));
		}
	}
}

void UGsUITrayMarketFilter::OnRefreshEntryRefine(int32 InIndex, UWidget* InEntry)
{
	if (_refineStatList.IsValidIndex(InIndex))
	{
		if (UGsUICheckBoxCommonText* entry = Cast<UGsUICheckBoxCommonText>(InEntry))
		{
			if (const FGsSchemaStatFormatingData* statData = _refineStatList[InIndex])
			{
				StatType statType = statData->statType;

				FText textName;
				FGsStatHelper::GetDisPlayStatName(statType, textName);
				entry->SetText(textName);
				entry->SetIndexInGroup(InIndex);
				entry->SetIsSelected(_data._refineStatList.Contains(statType));
			}
		}
	}
}

void UGsUITrayMarketFilter::OnClickOk()
{
	FGsMarketManager* marketMgr = GSMarket();
	if (nullptr == marketMgr)
	{
		return;
	}

	// 필터 저장 후, 리스트를 갱신시킴(필터링됨)
	FGsMarketFilterData& filterData = marketMgr->GetFilterData();

	if (0 == _data._enchantLevelMin && MAX_ENCHANT_LEVEL == _data._enchantLevelMax)
	{
		_data.ClearEnchantLevel();
	}

	if (0 == _data._shieldMin && MAX_SHIELD_COUNT == _data._shieldMax)
	{
		_data.ClearShield();
	}

	bool bIsGradeChanged = false;
	if (0 < filterData._gradeList.Num())
	{
		if (filterData._gradeList.Num() == _data._gradeList.Num())
		{
			for (int i = 0; i < filterData._gradeList.Num(); ++i)
			{
				if (false == _data._gradeList.Contains(filterData._gradeList[i]))
				{
					bIsGradeChanged = true;
					break;
				}
			}
		}
		else
		{
			bIsGradeChanged = true;
		}
	}
	else
	{
		if (0 < _data._gradeList.Num())
		{
			bIsGradeChanged = true;
		}
	}

	marketMgr->SetFilterData(_data);
	
	FGsMarketMsgParamId param(bIsGradeChanged ? 1 : 0);	
	GMessage()->GetMarket().SendMessage(MessageContentMarket::UPDATE_FILTER, &param);

	Close();
}

void UGsUITrayMarketFilter::OnClickCancel()
{
	Close();
}

void UGsUITrayMarketFilter::OnSlideEnchant(float InCurrMinValue, float InCurrMaxValue)
{
	_data._enchantLevelMin = static_cast<int32>(FMath::FloorToInt(InCurrMinValue));
	_data._enchantLevelMax = static_cast<int32>(FMath::FloorToInt(InCurrMaxValue));
}

void UGsUITrayMarketFilter::OnSlideShield(float InCurrMinValue, float InCurrMaxValue)
{
	_data._shieldMin = static_cast<int32>(FMath::FloorToInt(InCurrMinValue));
	_data._shieldMax = static_cast<int32>(FMath::FloorToInt(InCurrMaxValue));
}

void UGsUITrayMarketFilter::OnCheckGradeAll(bool bIsChecked, int32 InIndex)
{
	// 비어있으면 전체 대상이 된다.
	_data._gradeList.Empty();

	// 전체버튼이 선택되면, 모두 선택 해제
	if (bIsChecked)
	{
		for (int32 i = 0, maxCount = _slotHelperItemGrade->GetItemCount(); i < maxCount; ++i)
		{
			if (UGsUICheckBoxCommonText* entry = Cast<UGsUICheckBoxCommonText>(_slotHelperItemGrade->GetEntry(i)))
			{
				entry->SetIsSelected(false);
			}
		}
	}
}	

void UGsUITrayMarketFilter::OnCheckGrade(bool bIsChecked, int32 InIndex)
{
	if (false == _gradeList.IsValidIndex(InIndex))
	{
		return;
	}

	ItemGrade grade = _gradeList[InIndex];
	if (bIsChecked)
	{
		_data._gradeList.AddUnique(grade);
		
		// 다른 버튼이 클릭되면 전체 버튼은 체크 해제한다
		_checkBoxGradeAll->SetIsSelected(false);
	}
	else
	{
		_data._gradeList.Remove(grade);
	}
}

void UGsUITrayMarketFilter::OnCheckEnchantPropAll(bool bIsChecked, int32 InIndex)
{
	// 비어있으면 전체 대상이 된다.
	_data._enchantPropList.Empty();

	// 전체버튼이 선택되면, 모두 선택 해제
	if (bIsChecked)
	{
		for (int32 i = 0, maxCount = _slotHelperEnchantProp->GetItemCount(); i < maxCount; ++i)
		{
			if (UGsUICheckBoxCommonText* entry = Cast<UGsUICheckBoxCommonText>(_slotHelperEnchantProp->GetEntry(i)))
			{
				entry->SetIsSelected(false);
			}
		}
	}
}

void UGsUITrayMarketFilter::OnCheckEnchantProp(bool bIsChecked, int32 InIndex)
{
	if (false == _enchantPropList.IsValidIndex(InIndex))
	{
		return;
	}

	EGsItemEnchantBonusType enchantPropType = _enchantPropList[InIndex];
	if (bIsChecked)
	{
		_data._enchantPropList.AddUnique(enchantPropType);

		// 다른 버튼이 클릭되면 전체 버튼은 체크 해제한다
		_checkBoxEnchantPropAll->SetIsSelected(false);
	}
	else
	{
		_data._enchantPropList.Remove(enchantPropType);
	}
}

void UGsUITrayMarketFilter::OnCheckRefineAll(bool bIsChecked, int32 InIndex)
{
	// 비어있으면 전체 대상이 된다.
	_data._refineStatList.Empty();

	// 전체버튼이 선택되면, 모두 선택 해제
	if (bIsChecked)
	{
		for (int32 i = 0, maxCount = _slotHelperRefine->GetItemCount(); i < maxCount; ++i)
		{
			if (UGsUICheckBoxCommonText* entry = Cast<UGsUICheckBoxCommonText>(_slotHelperRefine->GetEntry(i)))
			{
				entry->SetIsSelected(false);
			}
		}
	}
}

void UGsUITrayMarketFilter::OnCheckRefine(bool bIsChecked, int32 InIndex)
{
	if (false == _refineStatList.IsValidIndex(InIndex))
	{
		return;
	}

	const FGsSchemaStatFormatingData* statData = _refineStatList[InIndex];
	if (nullptr == statData)
	{
		return;
	}

	StatType statType = statData->statType;
	if (bIsChecked)
	{
		_data._refineStatList.AddUnique(statType);

		// 다른 버튼이 클릭되면 전체 버튼은 체크 해제한다
		_checkBoxRefineAll->SetIsSelected(false);
	}
	else
	{
		_data._refineStatList.Remove(statType);
	}
}

void UGsUITrayMarketFilter::RecursiveFindTextBlock(UPanelWidget* InPanel, OUT TArray<UTextBlock*>& OutTextBlock)
{
	int32 count = InPanel->GetChildrenCount();
	for (int32 i = 0; i < count; ++i)
	{
		UWidget* child = InPanel->GetChildAt(i);
		if (child->IsA<UTextBlock>())
		{
			OutTextBlock.Emplace(Cast<UTextBlock>(child));
		}
		else
		{
			if (child->IsA<UPanelWidget>())
			{
				RecursiveFindTextBlock(Cast<UPanelWidget>(child), OutTextBlock);
			}
		}
	}
}
