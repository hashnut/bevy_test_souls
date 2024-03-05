// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupSanctumNexusBuffInfo.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Popup/Sanctum/GsUISanctumNexusInfoByLevel.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "DataSchema/Sanctum/GsSchemaSanctumNexusData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildHelper.h"


void UGsUIPopupSanctumNexusBuffInfo::BeginDestroy()
{
	_toggleGroupTop.Clear();
	_toggleGroupLevel.Clear();

	if (_slotHelperLevel)
	{
		_slotHelperLevel->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupSanctumNexusBuffInfo::OnRefreshLevel);
	}
	_slotHelperLevel = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupSanctumNexusBuffInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupSanctumNexusBuffInfo::OnClickClose);

	_slotHelperLevel = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperLevel->Initialize(_levelEntryClass, _panelRootLevelTab);
	_slotHelperLevel->OnRefreshEntry.AddDynamic(this, &UGsUIPopupSanctumNexusBuffInfo::OnRefreshLevel);

	_toggleGroupTop.OnSelectChanged.BindUObject(this, &UGsUIPopupSanctumNexusBuffInfo::OnSelectTopTab);
	_toggleGroupTop.AddToggleWidgetByParentPanel(_panelRootTopTab);

	_toggleGroupLevel.OnSelectChanged.BindUObject(this, &UGsUIPopupSanctumNexusBuffInfo::OnSelectLevelTab);
}

void UGsUIPopupSanctumNexusBuffInfo::NativeDestruct()
{
	_toggleGroupLevel.Clear();
	_nexusLevel = 0;

	Super::NativeDestruct();
}

void UGsUIPopupSanctumNexusBuffInfo::OnInputCancel()
{
	Close();
}

void UGsUIPopupSanctumNexusBuffInfo::SetData(uint16 InNexusLevel, int32 InTabIndex)
{
	_panelRootTopTab->SetVisibility(ESlateVisibility::Collapsed);

	_nexusLevel = InNexusLevel;	

	// 첫 탭을 선택
	_toggleGroupTop.SetSelectedIndex(0, true);
}

void UGsUIPopupSanctumNexusBuffInfo::InvalidateLevel(uint16 InCampLevel)
{
	const FGsSchemaSanctumNexusData* nexusData = GetNexusData();
	if (nullptr == nexusData)
	{
		_toggleGroupLevel.Clear();
		_slotHelperLevel->RefreshAll(0);

		return;
	}

	// 레벨 리스트
	_toggleGroupLevel.Clear();
	_slotHelperLevel->RefreshAll(nexusData->constructionBuffList.Num());
	_scrollBoxLevel->ScrollToStart();

	// 해당 레벨이 선택되어 있도록 함 
	if (0 < InCampLevel &&
		nexusData->constructionBuffList.Num() >= InCampLevel)
	{
		// 인덱스이므로 -1
		_toggleGroupLevel.SetSelectedIndex(InCampLevel - 1, true);
	}
	else
	{
		_toggleGroupLevel.SetSelectedIndex(0, true);
	}

	_scrollBox->ScrollToStart();
}

void UGsUIPopupSanctumNexusBuffInfo::OnSelectTopTab(int32 InIndex)
{
	InvalidateLevel(_nexusLevel);
}

void UGsUIPopupSanctumNexusBuffInfo::OnSelectLevelTab(int32 InIndex)
{
	// 수호탑 빈페이지 출력할 일이 없어서 constructableLevel를 0으로 설정
	int32 constructableLevel = 0;
	_nexusLevel = InIndex + 1;
	
	if (constructableLevel > _nexusLevel)
	{
		_nexusInfoUI->SetVisibility(ESlateVisibility::Collapsed);
		_panelRootEmpty->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		// 과거 요구레벨 텍스트를 찍는 부분이었지만 빈 페이지 보일 일 없음
		_textBlockEmpty->SetText(FText::GetEmpty());
	}
	else
	{
		_nexusInfoUI->SetVisibility(ESlateVisibility::HitTestInvisible);
		_panelRootEmpty->SetVisibility(ESlateVisibility::Collapsed);

		// 인덱스이므로 레벨을 넘길 때는 +1을 해서 넘김에 유의
		_nexusInfoUI->SetData(GetNexusData(), _nexusLevel);
	}
}

void UGsUIPopupSanctumNexusBuffInfo::OnRefreshLevel(int32 InIndex, UWidget* InEntry)
{
	if (UGsUITabButtonCommon* entry = Cast<UGsUITabButtonCommon>(InEntry))
	{
		// 레벨 표시라 인덱스에 +1을 함
		entry->SetTitleText(FText::AsNumber(InIndex + 1));
		entry->SetSubText(FText::GetEmpty());
		entry->SetLockedText(FText::GetEmpty());

		_toggleGroupLevel.AddToggleWidget(entry->GetToggleGroupEntry());
	}
}

void UGsUIPopupSanctumNexusBuffInfo::OnClickClose()
{
	OnInputCancel();
}

const FGsSchemaSanctumNexusData* UGsUIPopupSanctumNexusBuffInfo::GetNexusData() const
{
	// 개선: 가져오는 방법 개선 필요

	if (const UGsTable* table = FGsSchemaSanctumNexusData::GetStaticTable())
	{
		TArray<const FGsSchemaSanctumNexusData*> rows;
		table->GetAllRows(rows);

		for (const FGsSchemaSanctumNexusData* iter : rows)
		{
			if (iter->npcDataId.IsValidIndex(0))
			{
				if (const FGsSchemaNpcData* npcData = iter->npcDataId[0].GetRow())
				{
					if (NpcFunctionType::SANCTUM_NEXUS == npcData->npcFunctionType)
					{
						return iter;
					}
				}
			}
		}
	}

	return nullptr;
}
