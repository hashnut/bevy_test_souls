// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildCampBuffInfo.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Popup/Guild/GsUIGuildCampBuffInfoEntry.h"
#include "DataSchema/GameObject/Camp/GsSchemaCampData.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildHelper.h"


void UGsUIPopupGuildCampBuffInfo::BeginDestroy()
{
	_toggleGroupCampType.Clear();
	_toggleGroupLevel.Clear();

	if (_slotHelperLevel)
	{
		_slotHelperLevel->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupGuildCampBuffInfo::OnRefreshLevel);
	}
	_slotHelperLevel = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupGuildCampBuffInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupGuildCampBuffInfo::OnClickClose);

	_slotHelperLevel = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperLevel->Initialize(_levelEntryClass, _panelRootLevelTab);
	_slotHelperLevel->OnRefreshEntry.AddDynamic(this, &UGsUIPopupGuildCampBuffInfo::OnRefreshLevel);

	_toggleGroupCampType.OnSelectChanged.BindUObject(this, &UGsUIPopupGuildCampBuffInfo::OnSelectCampTypeTab);
	_toggleGroupCampType.AddToggleWidgetByParentPanel(_panelRootCampTypeTab);

	_toggleGroupLevel.OnSelectChanged.BindUObject(this, &UGsUIPopupGuildCampBuffInfo::OnSelectLevelTab);
}

void UGsUIPopupGuildCampBuffInfo::NativeDestruct()
{
	_toggleGroupLevel.Clear();
	_campTypeMap.Empty();
	_campLevel = 0;

	Super::NativeDestruct();
}

void UGsUIPopupGuildCampBuffInfo::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildCampBuffInfo::SetData(uint16 InCampLevel, int32 InTabIndex)
{
	_campLevel = InCampLevel;

	_campTypeMap.Empty();
	_constructableLevelMap.Empty();

	if (const UGsTable* table = FGsSchemaCampData::GetStaticTable())
	{
		TArray<const FGsSchemaCampData*> tempList;
		table->GetAllRows(tempList);

		for (const FGsSchemaCampData* iter : tempList)
		{
			// 테스트 데이터 생략
			if (0 > iter->infoUITabIndex)
			{
				continue;
			}

			_campTypeMap.Emplace(iter->infoUITabIndex, iter);

			int32 constructableCampLevel = FGsGuildHelper::GetConstructableCampLevel(iter->id);
			_constructableLevelMap.Emplace(iter->infoUITabIndex, constructableCampLevel);
		}
	}

	// 첫 탭을 선택
	int32 campTypeTabIndex = InTabIndex;
	if (0 > InTabIndex ||
		static_cast<int32>(EGsGuildCampType::MAX_CAMP_TYPE) <= InTabIndex)
	{
		campTypeTabIndex = 0;
	}

	_toggleGroupCampType.SetSelectedIndex(campTypeTabIndex, true);
}

void UGsUIPopupGuildCampBuffInfo::InvalidateLevel(uint16 InCampLevel)
{
	const FGsSchemaCampData* campData = GetSelectedCamp();
	if (nullptr == campData)
	{
		_toggleGroupLevel.Clear();
		_slotHelperLevel->RefreshAll(0);

		return;
	}

	// 레벨 리스트
	_toggleGroupLevel.Clear();
	_slotHelperLevel->RefreshAll(campData->constructionBuffList.Num());
	_scrollBoxLevel->ScrollToStart();

	// 해당 레벨이 선택되어 있도록 함 
	if (0 < InCampLevel &&
		campData->constructionBuffList.Num() >= InCampLevel)
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

void UGsUIPopupGuildCampBuffInfo::SetEmptyText(EGsGuildCampType InType)
{
	FText textMsg;
	switch (InType)
	{
	case EGsGuildCampType::CAMP_PVB:
		// TEXT: 필드보스 수호탑은 수호탑Lv.4부터사용할수있습니다.
		FText::FindText(TEXT("GuildText"), TEXT("Camp_Level_Lack_PVB"), textMsg);
		break;
	case EGsGuildCampType::CAMP_PVP:
		// TEXT: PVP 수호탑은수호탑Lv.7부터사용할수있습니다.
		FText::FindText(TEXT("GuildText"), TEXT("Camp_Level_Lack_PVP"), textMsg);
		break;
	}

	_textBlockEmpty->SetText(textMsg);
}

void UGsUIPopupGuildCampBuffInfo::OnSelectCampTypeTab(int32 InIndex)
{
	InvalidateLevel(_campLevel);
}

void UGsUIPopupGuildCampBuffInfo::OnSelectLevelTab(int32 InIndex)
{
	int32 constructableLevel = GetSelectedConstructableLevel();
	_campLevel = InIndex + 1;
	
	if (constructableLevel > _campLevel)
	{
		_uiCampInfo->SetVisibility(ESlateVisibility::Collapsed);
		_panelRootEmpty->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		SetEmptyText(static_cast<EGsGuildCampType>(_toggleGroupCampType.GetSelectedIndex()));
	}
	else
	{
		_uiCampInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
		_panelRootEmpty->SetVisibility(ESlateVisibility::Collapsed);

		// 인덱스이므로 레벨을 넘길 때는 +1을 해서 넘김에 유의
		_uiCampInfo->SetData(GetSelectedCamp(), _campLevel);
	}
}

void UGsUIPopupGuildCampBuffInfo::OnRefreshLevel(int32 InIndex, UWidget* InEntry)
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

void UGsUIPopupGuildCampBuffInfo::OnClickClose()
{
	OnInputCancel();
}

const FGsSchemaCampData* UGsUIPopupGuildCampBuffInfo::GetSelectedCamp() const
{
	int32 selectedIndex = _toggleGroupCampType.GetSelectedIndex();
	if (const FGsSchemaCampData*const* campPtr = _campTypeMap.Find(selectedIndex))
	{
		return *campPtr;
	}

	return nullptr;
}

int32 UGsUIPopupGuildCampBuffInfo::GetSelectedConstructableLevel() const
{
	int32 selectedIndex = _toggleGroupCampType.GetSelectedIndex();
	if (const int32* levelPtr = _constructableLevelMap.Find(selectedIndex))
	{
		return *levelPtr;
	}

	return -1;
}
