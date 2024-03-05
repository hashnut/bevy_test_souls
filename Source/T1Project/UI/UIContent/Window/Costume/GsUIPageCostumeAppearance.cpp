// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIPageCostumeAppearance.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsCostumeMessageParam.h"
#include "Item/GsItemManager.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Costume/GsUICostumeAppearanceDetail.h"
#include "UI/UIContent/Window/Costume/GsUICostumeItem.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UGsUIPageCostumeAppearance::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUICostumeItem>())
		{
			child->RemoveFromParent();
		}
	}

	_btnInit->OnClicked.AddDynamic(this, &UGsUIPageCostumeAppearance::OnInitializeAppearance);
	_btnChange->OnClicked.AddDynamic(this, &UGsUIPageCostumeAppearance::OnSetApparance);

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->SetEmptyListPanel(_emptyPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPageCostumeAppearance::OnCreateCostumeItemEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPageCostumeAppearance::OnRefreshCostumeItemEntry);

	TMap<CostumeGrade, UGsSwitcherButton*> gradeTabs;
	{
		gradeTabs.Add(CostumeGrade::MAX, _btnGradeAll);
		gradeTabs.Add(CostumeGrade::LEGEND, _btnGradeLegend);
		gradeTabs.Add(CostumeGrade::UNIQUE, _btnGradeUnique);
		gradeTabs.Add(CostumeGrade::EPIC, _btnGradeEpic);
		gradeTabs.Add(CostumeGrade::RARE, _btnGradeRare);
		gradeTabs.Add(CostumeGrade::MAGIC, _btnGradeMagic);
		gradeTabs.Add(CostumeGrade::NORMAL, _btnGradeNormal);
	}

	int32 index = 0;
	for (auto iter : gradeTabs)
	{
		_tabGradeIndex.Add(index, iter.Key);
		_toggleGroupGrade.AddToggleWidget(iter.Value);

		index++;
	}

	_gradeTab = -1;
	_selectedCostumeId = INVALID_COSTUME_ID;
	_toggleGroupGrade.OnSelectChanged.BindUObject(this, &UGsUIPageCostumeAppearance::OnGradeTabChanged);
}

void UGsUIPageCostumeAppearance::NativeConstruct()
{
	Super::NativeConstruct();

	//_btnGradeLegend->SetIsSelected(false);
	//_btnGradeLegend->SetSwitcherIndex(1);
	//_btnGradeLegend->SetIsEnabled(false);
}

void UGsUIPageCostumeAppearance::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPageCostumeAppearance::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIPageCostumeAppearance::InitializeTab(int32 tabIndex)
{
	IGsSelectedPanelInterface::InitializeTab(tabIndex);

	_gradeTab = -1;
	_selectedCostumeId = INVALID_COSTUME_ID;
	
	_hairAppearance->SetPanelInfo(CostumeParts::HAIR);
	_bodyAppearance->SetPanelInfo(CostumeParts::BODY);
	_equipCostume->SetPanelInfo(CostumeParts::FULLSET);

	_hairAppearance->OnNoApplyedFix.BindUObject(this, &UGsUIPageCostumeAppearance::OnFixHairCostume);
	_bodyAppearance->OnNoApplyedFix.BindUObject(this, &UGsUIPageCostumeAppearance::OnFixBodyCostume);

	if (auto userData = GGameData()->GetUserData())
	{
		_genderType = userData->mGender;
		if (const auto weaponInfo = UGsItemManager::GetEquipItemTableDataByTID(userData->mLookInfo.mWeapon))
		{
			_weaponType = weaponInfo->weaponType;
		}
	}

	_scrollBoxHelper->RefreshAll(0);
	_toggleGroupGrade.SetSelectedIndex(0, true);
}

void UGsUIPageCostumeAppearance::OnSelected()
{
	if (_selectedCostumeId == INVALID_COSTUME_ID)
	{
		InitializeTab(_tabIndex);
	}

	bool isHairOn = GGameData()->GetUserData()->mLookInfo.mViewHelmet;
	if (auto equip = GSCostume()->GetWearOnCostumeData().Pin())
	{
		_equipCostume->SetData(equip);

		FGsCostumeMsgParamWearOn param(equip->_id, isHairOn);
		GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_SELECTION, &param);
	}
	else
	{
		FGsCostumeMsgParamWearOn param(INVALID_COSTUME_ID, isHairOn);
		GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_SELECTION, &param);
	}

	FGsCostumeMsgParamAppearanceOn param(INVALID_COSTUME_ID, _hairAppearance->GetCostumePartsFixed() == false, _bodyAppearance->GetCostumePartsFixed() == false);
	GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_APPEARANCE, &param);

	if (_tabGradeIndex.Find(_gradeTab))
		SetRefreshCostumeItems(_tabGradeIndex[_gradeTab]);

	OnSelectedAppearanceItem(INVALID_COSTUME_ID);
}

void UGsUIPageCostumeAppearance::OnUnselected()
{

}

void UGsUIPageCostumeAppearance::OnGradeTabChanged(int32 index)
{
	if (_gradeTab == index) return;

	_gradeTab = index;
	auto selectTab = _tabGradeIndex[index];
	SetRefreshCostumeItems(selectTab);
}

void UGsUIPageCostumeAppearance::OnCreateCostumeItemEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeItem>(InEntry))
	{
		entry->WidgetType = UGsUICostumeItem::UIItemType::ITEM;
		entry->OnItemSelected.BindUObject(this, &UGsUIPageCostumeAppearance::OnSelectedAppearanceItem);
	}
}

void UGsUIPageCostumeAppearance::OnRefreshCostumeItemEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeItem>(InEntry))
	{
		if (_costumeGradeList.IsValidIndex(index))
		{
			entry->SetData(_costumeGradeList[index], _genderType);
			entry->SetSelected(false);
			if (auto data = _costumeGradeList[index].Pin())
			{
				entry->SetSelected(data->_id == _selectedCostumeId);
			}
			entry->RefreshUI();
		}
	}
}

void UGsUIPageCostumeAppearance::OnInitializeAppearance()
{
	OnSelectedAppearanceItem(INVALID_COSTUME_ID);
}

void UGsUIPageCostumeAppearance::OnSetApparance()
{
	//
}

void UGsUIPageCostumeAppearance::OnSelectedAppearanceItem(CostumeId id)
{
	if (_selectedCostumeId == id)
	{
		return;
	}
	UpdateItemSelection(id);

	if (auto appearance = GSCostume()->GetCostumeData(id).Pin())
	{
		if (appearance->IsEnableAppearceParts(CostumeParts::FULLSET) &&
			(_hairAppearance->GetCostumePartsFixed() || _bodyAppearance->GetCostumePartsFixed()))
		{
			//set로만 적용되는 아이템인데 세트를 적용시킬 수 없을 때 - 있을 수 있음
		}
		else if (appearance->IsEnableAppearceParts(CostumeParts::HAIR) == false && _bodyAppearance->GetCostumePartsFixed())
		{
			//Body만 적용되는 아이템에 바디 적용되지 못할 때 - 이건 없을 듯
		}
		else if (appearance->IsEnableAppearceParts(CostumeParts::BODY) == false && _hairAppearance->GetCostumePartsFixed())
		{
			//Hair 만 적용되는 아이템에 Hair 적용되지 못할 때 - 이것도 없을 듯
		}

		_hairAppearance->SetData(appearance);
		_bodyAppearance->SetData(appearance);

		_btnChange->SetIsEnabled(appearance->isCollectedCostume());
	}
	else
	{
		_hairAppearance->SetData(nullptr);
		_bodyAppearance->SetData(nullptr);
		_btnChange->SetIsEnabled(false);
	}

	_hairAppearance->RefreshUI();
	_bodyAppearance->RefreshUI();

	FGsCostumeMsgParamAppearanceOn param(id, _hairAppearance->GetCostumePartsFixed() == false, _bodyAppearance->GetCostumePartsFixed() == false);
	GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_APPEARANCE, &param);
}

void UGsUIPageCostumeAppearance::OnFixHairCostume(bool set)
{
	FGsCostumeMsgParamAppearanceOn param(_selectedCostumeId, set == false, _bodyAppearance->GetCostumePartsFixed() == false);
	GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_APPEARANCE, &param);
}

void UGsUIPageCostumeAppearance::OnFixBodyCostume(bool set)
{
	FGsCostumeMsgParamAppearanceOn param(_selectedCostumeId, _hairAppearance->GetCostumePartsFixed() == false, set == false);
	GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_APPEARANCE, &param);
}

void UGsUIPageCostumeAppearance::SetRefreshCostumeItems(CostumeGrade grade)
{
	_costumeGradeList.Reset();
	int32 count = GSCostume()->GetGradeAppearenceCostume(grade, _costumeGradeList);
	_scrollBoxHelper->RefreshAll(count);

	UpdateItemSelection(INVALID_COSTUME_ID);
}

void UGsUIPageCostumeAppearance::OnUpdateItemList(TArray<CostumeId>& arr)
{

}

void UGsUIPageCostumeAppearance::UpdateItemSelection(CostumeId selectId)
{
	_selectedCostumeId = selectId;

	int32 itemCount = _scrollBoxHelper->GetItemCount();
	for (int i = 0; i < itemCount; ++i)
	{
		UWidget* child = _listPanel->GetChildAt(i);

		if (auto item = Cast<UGsUICostumeItem>(child))
		{
			if (selectId == INVALID_COSTUME_ID)
			{
				if (item->IsSelected())
				{
					item->SetSelected(false);
					item->RefreshUI();
				}
				else item->SetSelected(false);
				
			}
			else
			{
				if (item->IsSelected())
				{
					item->SetSelected(item->GetCostumeID() == selectId, false);
					item->RefreshUI();
				}
				else
				{
					item->SetSelected(item->GetCostumeID() == selectId, false);
					if (item->IsSelected())
						item->RefreshUI();
				}
			}
		}
	}
}