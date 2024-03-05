// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPageCostume.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsCostumeMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Item/GsItemManager.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "Costume/GsCostumeData.h"
#include "GameObject/Stat/GsStatHelper.h"

#include "Summon/GsSummonDefine.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupSkillDetailInfo.h"
#include "UI/UIContent/Popup/GsUIPopupCostumeCollection.h"
#include "UI/UIContent/Popup/GsUIPopupFlavorText.h"
#include "UI/UIContent/Popup/GsUIPopupSummonMileage.h"
#include "UI/UIContent/Common/Menu/GsUIDropDownMenuCommon.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "GsUICostumeItem.h"
#include "GsUICostumeSkillIconItem.h"
#include "UI/UIContent/Window/Costume/GsUICostumeDetail.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PaperSprite.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"

#include "Net/GsNetSendServiceWorld.h"

void UGsUIPageCostume::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPageCostume::OnCreateCostumeItemEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPageCostume::OnRefreshCostumeItemEntry);
		_scrollBoxHelper = nullptr;
	}

	_tabGradeIndex.Reset();
	_toggleGroupGrade.Clear();

	Super::BeginDestroy();
}

void UGsUIPageCostume::NativeOnInitialized()
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

	_listCostume->OnUserScrolled.AddDynamic(this, &UGsUIPageCostume::OnScrollCostumeList);

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->SetEmptyListPanel(_emptyPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPageCostume::OnCreateCostumeItemEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPageCostume::OnRefreshCostumeItemEntry);

	_btnWear->OnClicked.AddDynamic(this, &UGsUIPageCostume::OnClickDressOn);
	_collectionButton->OnClicked.AddDynamic(this, &UGsUIPageCostume::OnOpenCollection);
	_flavorButton->OnClicked.AddDynamic(this, &UGsUIPageCostume::OnOpenFlavorText);
	_btnWearOnOff->OnClicked.AddDynamic(this, &UGsUIPageCostume::OnClickHairOn);
	_btnMenuTooltip->OnClicked.AddDynamic(this, &UGsUIPageCostume::OnClickMenuTooltip);

	FText UI_InfoIcon_RecommandFilter;
	FText::FindText(TEXT("CostumeText"), TEXT("UI_InfoIcon_RecommandFilter"), UI_InfoIcon_RecommandFilter);

	_tooltipWeaponMenu->SetDescText(UI_InfoIcon_RecommandFilter);
	_tooltipWeaponMenu->Close();

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
	_toggleGroupGrade.OnSelectChanged.BindUObject(this, &UGsUIPageCostume::OnGradeTabChanged);

	_btnOpenPopupSummonMileage->OnClicked.AddDynamic(this, &UGsUIPageCostume::OnClickOpenPopupSummonMileage);
}

void UGsUIPageCostume::NativeConstruct()
{
	_currentHairOn = GGameData()->GetUserData()->mLookInfo.mViewHelmet;
	_btnWearOnOff->SetIsSelected(_currentHairOn);
	_weaponSelectMenu->OnSelectedEntry.BindUObject(this, &UGsUIPageCostume::OnWeaponSelectMenuSelected);

	SetWeaponSelectMenu();
	SetMileageButtonState();

	_weaponSelectMenu->SetSelectMenuEntry(0);

	FGsMessageHolder* msg = GMessage();
	_handlerUserInfo = msg->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIPageCostume::OnUpdateCurrency);

	MUI& msgUI = msg->GetUI();
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::CHANGED_SUMMON_MILEAGE_DATA, this, &UGsUIPageCostume::OnChangedSummonMileageData));

	Super::NativeConstruct();
}

void UGsUIPageCostume::NativeDestruct()
{
	SetTutorialTarget(-1);
	OnTutorialSettingComplete.Unbind();
	_weaponSelectMenu->OnSelectedEntry.Unbind();
	_weaponSelectMenu->InitializeMenu(0);

	_selectedReservedCostumeId = INVALID_COSTUME_ID;
	OnInitData(_selectedCostumeId);

	if (GGameData()->GetUserData()->mLookInfo.mViewHelmet != _currentHairOn)
	{
		GOption()->SetValue(EGsOptionItem::CostumeHairOn, _currentHairOn ? 1.f : 0.f);
		GOption()->SaveAll();
	}

	// 메시지 해제
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		msgMgr->GetUserBasicInfo().Remove(_handlerUserInfo);

		if (0 != _uiMsgArray.Num())
		{
			for (auto iter : _uiMsgArray)
			{
				msgMgr->GetUI().Remove(iter);
			}
			_uiMsgArray.Empty();
		}
	}

	Super::NativeDestruct();
}

void UGsUIPageCostume::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_gradeTab < 0)
	{
		_toggleGroupGrade.SetSelectedIndex(0, true);
	}
}

#pragma region IGsSelectedPanelInterface
void UGsUIPageCostume::InitializeTab(int32 tabIndex)
{
	IGsSelectedPanelInterface::InitializeTab(tabIndex);

	_gradeTab = -1;
	_selectedCostumeId = INVALID_COSTUME_ID;

	if (auto userData = GGameData()->GetUserData())
	{
		_genderType = userData->mGender;
		if (const auto weaponInfo = UGsItemManager::GetEquipItemTableDataByTID(userData->mLookInfo.mWeapon))
		{
			_weaponType = weaponInfo->weaponType;
		}
	}

	_costumeDetail->SetSelectedCostumeDetail(nullptr);

	_btnWear->SetIsEnabled(false);

	_costumeName->SetText(FText::GetEmpty());
	_costumeGrade->SetText(FText::GetEmpty());

	_scrollBoxHelper->RefreshAll(0);
	_toggleGroupGrade.SetSelectedIndex(0, true);
}

void UGsUIPageCostume::ReleasedTab()
{
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFlavorText")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupCostumeCollection")).Get())
		popup->Close();
}

void UGsUIPageCostume::OnSelected()
{
	if (_selectedCostumeId == INVALID_COSTUME_ID)
	{
		InitializeTab(_tabIndex);
	}

	if(_tabGradeIndex.Find(_gradeTab))
		SetRefreshCostumeItems(_tabGradeIndex[_gradeTab], _selectedWeapon);

	FGsCostumeMsgParamWearOn param(_selectedCostumeId, _currentHairOn);
	GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_SELECTION, &param);

	if (_initData != INVALID_COSTUME_ID)
	{
		_selectedCostumeId = INVALID_COSTUME_ID;
		_selectedReservedCostumeId = _initData;

		OnInitData(INVALID_COSTUME_ID);
	}
	
	if (_selectedCostumeId != INVALID_COSTUME_ID)
	{
		OnSelectedCostumeItem(_selectedCostumeId);
	}

	SetReserveCostumeData();
	UpdateRedDot();
}

void UGsUIPageCostume::OnUnselected()
{
	const auto& dictionary = GSCostume()->GetCostumeDictionary();
	for (auto e : dictionary)
	{
		e->removeNewMark();
	}

	GSCostume()->DisableCostumeRedDot();

	_tooltipWeaponMenu->Close();
	_weaponSelectMenu->CloseMenuPopup();
}

void UGsUIPageCostume::OnUpdateItemList(TArray<CostumeId>& arr)
{
	auto selectTab = _tabGradeIndex[_gradeTab];
	bool isRefresh = selectTab == CostumeGrade::MAX;
	if (!isRefresh)
	{
		for (auto e : arr)
		{
			if (auto data = GSCostume()->GetCostumeData(e).Pin())
			{
				isRefresh = data->GetCostumeGrade() == selectTab && data->IsRecommendedWeaponType(_selectedWeapon);
				if (isRefresh) break;
			}
		}
	}
	if (isRefresh) SetRefreshCostumeItems(selectTab, _selectedWeapon);

	UpdateRedDot();
}
#pragma endregion

void UGsUIPageCostume::OnGradeTabChanged(int32 index)
{
	if (_gradeTab == index) return;

	_gradeTab = index;

	auto selectTab = _tabGradeIndex[index];
	SetRefreshCostumeItems(selectTab, _selectedWeapon);
}

void UGsUIPageCostume::OnClickDressOn()
{
	if (_selectedCostumeId == INVALID_COSTUME_ID)
		return;

	auto equipCostume = GSCostume()->GetWearOnCostumeData();
	if (equipCostume.IsValid() && equipCostume.Pin()->_id == _selectedCostumeId)
		return;

	TWeakPtr<FGsCostumeData> weakData = GSCostume()->GetCostumeData(_selectedCostumeId);
	if (weakData.IsValid() && weakData.Pin()->isCollectedCostume())
		FGsNetSendServiceWorld::SendReqCostumeEquip(_selectedCostumeId);
}

void UGsUIPageCostume::OnClickHairOn()
{
	_currentHairOn = !_currentHairOn;
	_btnWearOnOff->SetIsSelected(_currentHairOn);

	FGsCostumeMsgParamWearOn param(_selectedCostumeId, _currentHairOn);
	GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_SELECTION, &param);

	FText findText;
	if(_currentHairOn)
		 FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_HairOn"), findText);
	else FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_HairOff"), findText);

	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

void UGsUIPageCostume::OnClickMenuTooltip()
{
	_tooltipWeaponMenu->ToggleOpenClose();
}

void UGsUIPageCostume::OnOpenCollection()
{
	if (auto PopupCollection = Cast<UGsUIPopupCostumeCollection>(GUI()->OpenAndGetWidget(TEXT("PopupCostumeCollection"))))
	{
		CreatureGenderType genderType{ CreatureGenderType::ALL };
		if (auto userData = GGameData()->GetUserData())
			genderType = userData->mGender;

		UGsUIPopupCostumeCollection::PopupInitData param = { _selectedCostumeId, genderType };
		PopupCollection->InitializeData(&param);
	}
}

void UGsUIPageCostume::OnOpenFlavorText()
{
	if (auto costumeData = GSCostume()->GetCostumeData(_selectedCostumeId).Pin())
	{
		if (auto popup = Cast<UGsUIPopupFlavorText>(GUI()->OpenAndGetWidget(TEXT("PopupFlavorText")).Get()))
		{
			UGsUIPopupFlavorText::PopupInitData param{ costumeData->GetCostumeName(), costumeData->GetFlavorText() };
			popup->InitializeData(&param);
		}
	}
}

void UGsUIPageCostume::OnCreateCostumeItemEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUICostumeItem>(InEntry))
	{
		entry->WidgetType = UGsUICostumeItem::UIItemType::ITEM;
		entry->OnItemSelected.BindUObject(this, &UGsUIPageCostume::OnSelectedCostumeItem);
	}
}

void UGsUIPageCostume::OnRefreshCostumeItemEntry(int32 index, UWidget* InEntry)
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

void UGsUIPageCostume::SetRefreshCostumeItems(CostumeGrade grade, CreatureWeaponType selectedWeapon /*=CreatureWeaponType::NONE*/)
{
	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (auto item = Cast<UGsUICostumeItem>(child))
		{
			item->SetSelected(false);
		}
	}

	_costumeGradeList.Empty(0);
	_scrollBoxHelper->RefreshAll(0);

	int32 count = GSCostume()->GetGradeCostumeDictionary(grade, selectedWeapon, _costumeGradeList, true);

	_scrollBoxHelper->RefreshAll(count);

	if (_selectedCostumeId == 0)
	{
		if (auto equipCostume = GSCostume()->GetWearOnCostumeData().Pin())
		{
			int32 itemCount = _scrollBoxHelper->GetItemCount();
			for (int i = 0 ; i < itemCount; ++i)
			{
				UWidget* child = _listPanel->GetChildAt(i);

				if (auto item = Cast<UGsUICostumeItem>(child))
				{
					item->SetSelected(item->GetCostumeID() == equipCostume->_id);
				}
			}
		}
	}
	else
	{
		if (auto costumeData = GSCostume()->GetCostumeData(_selectedCostumeId).Pin())
		{
			if (costumeData->isCollectedCostume())
			{
				if (auto equipCostume = GSCostume()->GetWearOnCostumeData().Pin())
					 _btnWear->SetIsEnabled(equipCostume->_id != _selectedCostumeId);
				else _btnWear->SetIsEnabled(true);
			}
		}
	}

	if (IsTutorial() &&
		_tutorialTargetIndex < _costumeGradeList.Num())
	{
		// 1번슬롯으로 고정했놨으므로 1번항목 리턴
		OnTutorialSettingComplete.ExecuteIfBound(_listPanel->GetChildAt(_tutorialTargetIndex));
	}
}

void UGsUIPageCostume::OnSelectedCostumeItem(CostumeId id)
{
	if (id == INVALID_COSTUME_ID) return;

	auto UpdateItemSelection = [this](CostumeId id) {
		int32 itemCount = _scrollBoxHelper->GetItemCount();
		for (int i = 0; i < itemCount; ++i)
		{
			UWidget* child = _listPanel->GetChildAt(i);

			if (auto item = Cast<UGsUICostumeItem>(child))
			{
				if (item->IsSelected())
				{
					item->SetSelected(item->GetCostumeID() == id, false);
					item->RefreshUI();
				}
				else
				{
					item->SetSelected(item->GetCostumeID() == id, false);
					if (item->IsSelected())
						item->RefreshUI();
				}
				item->SetBlockSelected();
			}
		}
	};

	UpdateItemSelection(id);

	_selectedCostumeId = id;

	_btnWear->SetIsEnabled(false);
	_costumeName->SetText(FText::GetEmpty());
	_costumeGrade->SetText(FText::GetEmpty());
	_costumeDetail->SetSelectedCostumeDetail(nullptr);

	if (auto costumeData = GSCostume()->GetCostumeData(id).Pin())
	{
		FLinearColor gradeColor = FGsUIColorHelper::GetColorGradeWidthItemGrade(costumeData->GetCostumeGrade());
		FText gradeText;
		switch (costumeData->GetCostumeGrade())
		{
		case CostumeGrade::NORMAL:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Normal"), gradeText);	break;
		case CostumeGrade::MAGIC:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Magic"), gradeText);	break;
		case CostumeGrade::RARE:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Rare"), gradeText);		break;
		case CostumeGrade::EPIC:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Epic"), gradeText);		break;
		case CostumeGrade::UNIQUE:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Unique"), gradeText);	break;
		case CostumeGrade::LEGEND:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Legend"), gradeText);	break;
		default: break;
		}

		_selectGrade->SetActiveWidgetIndex(static_cast<int>(costumeData->GetCostumeGrade()) - 1);
		_costumeName->SetText(costumeData->GetCostumeName());
		_costumeGrade->SetText(gradeText);
		_costumeName->SetColorAndOpacity(gradeColor);
		_costumeGrade->SetColorAndOpacity(gradeColor);

		_costumeDetail->SetSelectedCostumeDetail(costumeData);

		if (costumeData->isCollectedCostume())
		{
			if (auto equipCostume = GSCostume()->GetWearOnCostumeData().Pin())
				 _btnWear->SetIsEnabled(equipCostume->_id != id);
			else _btnWear->SetIsEnabled(true);
		}

		auto flavorText = costumeData->GetFlavorText();
		_flavorButton->SetIsEnabled(flavorText.IsEmpty() == false);

		FGsCostumeMsgParamWearOn param(id, _currentHairOn);
		GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_SELECTION, &param);
	}
}

void UGsUIPageCostume::OnChangeDressUp()
{
	auto selectTab = _tabGradeIndex[_gradeTab];
	SetRefreshCostumeItems(selectTab, _selectedWeapon); //순서변경

	auto preSelect = _selectedCostumeId;
	_selectedCostumeId = INVALID_COSTUME_ID;

	int32 itemCount = _scrollBoxHelper->GetItemCount();
	for (int i = 0; i < itemCount; ++i)
	{
		UWidget* child = _listPanel->GetChildAt(i);

		if (auto item = Cast<UGsUICostumeItem>(child))
		{
			item->SetSelected(item->GetCostumeID() == preSelect, true);
		}
	}
}

void UGsUIPageCostume::OnSetScrollToSelectedCostumeItem(CostumeId id)
{
	if (IsSelectedTab() == false)
	{
		_gradeTab = 0;
		_selectedCostumeId = INVALID_COSTUME_ID;
		_selectedReservedCostumeId = id;
	}
}

void UGsUIPageCostume::SetReserveCostumeData()
{
	if (_selectedReservedCostumeId != INVALID_COSTUME_ID)
	{
		OnSelectedCostumeItem(_selectedReservedCostumeId);

		UWidget* selectedItem = nullptr;
		int32 itemCount = _scrollBoxHelper->GetItemCount();
		for (int32 i = 0; i < itemCount; ++i)
		{
			UWidget* child = _listPanel->GetChildAt(i);
			if (auto item = Cast<UGsUICostumeItem>(child))
			{
				if (item->GetCostumeID() == _selectedReservedCostumeId)
				{
					selectedItem = child;
					break;
				}
			}
		}

		if (selectedItem)
		{
			_listCostume->ScrollWidgetIntoView(selectedItem, true, EDescendantScrollDestination::Center);
		}

		_selectedReservedCostumeId = INVALID_COSTUME_ID;
	}
}

void UGsUIPageCostume::UpdateRedDot()
{
	bool reddot = GSCostume()->IsCostumeRedDot();

	TArray<TWeakPtr<FGsCostumeData>> gradelist;
	for (auto e : { TPair<CostumeGrade, UImage*>{ CostumeGrade::MAX, _redDotAll },
					TPair<CostumeGrade, UImage*>{ CostumeGrade::LEGEND, _redDotLegend },
					TPair<CostumeGrade, UImage*>{ CostumeGrade::UNIQUE, _redDotUnique },
					TPair<CostumeGrade, UImage*>{ CostumeGrade::EPIC, _redDotEpic },
					TPair<CostumeGrade, UImage*>{ CostumeGrade::RARE, _redDotRare },
					TPair<CostumeGrade, UImage*>{ CostumeGrade::MAGIC, _redDotMagic },
					TPair<CostumeGrade, UImage*>{ CostumeGrade::NORMAL, _redDotNormal } })
	{
		if (reddot)
		{
			gradelist.Empty(0);
			GSCostume()->GetGradeCostumeDictionary(e.Key, gradelist);

			e.Value->SetVisibility(gradelist.FindByPredicate([](TWeakPtr<FGsCostumeData> data) {
				return data.IsValid() && data.Pin()->_isNew;
				}) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
		else e.Value->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIPageCostume::SetWeaponSelectMenu()
{
	if (_weaponSelectMenu->GetMenuEntryDataList().Num() > 0)
	{
		return;
	}

	TArray<FGsCostumeWeaponMenuEntry> menuData = GSCostume()->GetCostumeSelectMenuData();
	_weaponSelectMenu->InitializeMenu(menuData.Num());

	int index = 0;
	for (auto cIter = menuData.CreateConstIterator(); cIter; cIter++, index++)
	{
		_weaponSelectMenu->SetMenuEntryName(index, (*cIter)._menuName);
		_weaponSelectMenu->SetMenuEntryIconPath(index, (*cIter)._iconPath, (*cIter)._iconImgSize, (*cIter)._iconTintColor);
		_weaponSelectMenu->SetMenuEntryOptionData(index, MakeShared<WeaponTypeMenuData>((*cIter)._menuType));
	}
	_weaponSelectMenu->RefreshUI();
	_weaponSelectMenu->SetSelectMenuEntry(0);
}

CostumeId UGsUIPageCostume::GetRecoverCostumeId()
{
	return _costumeDetail->GetRecoverCostumeId();
}

void UGsUIPageCostume::SetTutorialTarget(int32 InIndex)
{
	_tutorialTargetIndex = InIndex;

	if (-1 == InIndex)
	{
		OnTutorialSettingComplete.Unbind();
		_listCostume->SetAllowOverscroll(true);
	}
	else
	{		
		_listCostume->SetAllowOverscroll(false);
	}
}

void UGsUIPageCostume::OnScrollCostumeList(float InCurrentOffset)
{
	// 튜토리얼 중 스크롤 못하게 막음
	if (IsTutorial())
	{
		_listCostume->SetScrollOffset(0.f);
	}
}

void UGsUIPageCostume::OnClickOpenPopupSummonMileage()
{
	TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->OpenAndGetWidget(TEXT("PopupSummonMileage"));

	if (wigdet.IsValid() == false)
	{
		return;
	}
	

	TWeakObjectPtr<UGsUIPopupSummonMileage> popupSummonMileage = Cast<UGsUIPopupSummonMileage>(wigdet);
	popupSummonMileage->InitSummonMileage(EGsSummonType::SUMMON_COSTUME);
}

bool UGsUIPageCostume::OnWeaponSelectMenuSelected(int32 index, bool forced /*= false*/)
{
	CreatureWeaponType weaponType = CreatureWeaponType::NONE;
	if (auto weaponData = StaticCastSharedPtr<WeaponTypeMenuData>(_weaponSelectMenu->GetMenuEntryOptionData(index).Pin()))
	{
		weaponType = weaponData->_weaponType;
	}

	if (_selectedWeapon != weaponType)
	{
		_selectedWeapon = weaponType;
		SetRefreshCostumeItems(_tabGradeIndex[_gradeTab], weaponType);
	}

	return true;
}

// 재화 갱신 메시지
void UGsUIPageCostume::OnUpdateCurrency(uint64 InType)
{
	CurrencyType targetCurrency = static_cast<CurrencyType>(InType);

	// 유니크, 일반 체크
	if (targetCurrency != CurrencyType::SUMMON_COSTUME_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_FAIRY_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_UNIQUE_COSTUME_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_UNIQUE_FAIRY_MILEAGE
		)
	{
		return;
	}

	SetMileageButtonState();
}
// 소환 마일리지 데이터 갱신
void UGsUIPageCostume::OnChangedSummonMileageData(const  IGsMessageParam* inParam)
{
	SetMileageButtonState();
}
// 마일리지 버튼 상태 갱신
void UGsUIPageCostume::SetMileageButtonState()
{
	// 소환 마일리지 보상 받을것이 있는가
	bool isExistSummonMileageReward = GSSummon()->IsExistSummonMileageRewardByType(EGsSummonType::SUMMON_COSTUME);

	ESlateVisibility rewardEffectVisibility =
		(isExistSummonMileageReward == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_mileageRewardEffectWidget->SetVisibility(rewardEffectVisibility);
}