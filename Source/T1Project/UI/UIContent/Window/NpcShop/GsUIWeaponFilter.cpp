// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWeaponFilter.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

#include "Management/ScopeGame/GsNpcShopManager.h"

#include "DataSchema/UI/GsSchemaWeaponTypeMenu.h"


void UGsUIWeaponFilter::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSave->OnClicked.AddDynamic(this, &UGsUIWeaponFilter::OnClickSave);

	// 무기 추가 시 이곳에 추가만 하면 된다.
	_buttonTabTypeList.Empty();
	_buttonTabTypeList.Emplace(ENpcShopProductListTab::ProductList_Sword, _btnSword);
	_buttonTabTypeList.Emplace(ENpcShopProductListTab::ProductList_Dagger, _btnDagger);
	_buttonTabTypeList.Emplace(ENpcShopProductListTab::ProductList_Bow, _btnBow);
	_buttonTabTypeList.Emplace(ENpcShopProductListTab::ProductList_SHOTGUN, _btnShotgun);
	_buttonTabTypeList.Emplace(ENpcShopProductListTab::ProductList_Wand, _btnWand);
	_buttonTabTypeList.Emplace(ENpcShopProductListTab::ProductList_Orb, _btnOrb);
	
	for (int32 i = 0;i < _buttonTabTypeList.Num();++i)
	{
		if (UGsSwitcherButton* switcherButton = _buttonTabTypeList[i].Value)
		{
			switcherButton->SetIndexInGroup(i);
			switcherButton->SetToggleCallback([this](int32 InIndex, bool InIsSelected)
				{
					OnClickWeapon(InIndex, InIsSelected);
				});
		}
	}
}

void UGsUIWeaponFilter::NativeConstruct()
{
	Super::NativeConstruct();

	if (const UGsTableWeaponTypeMenu* table = Cast<UGsTableWeaponTypeMenu>(FGsSchemaWeaponTypeMenu::GetStaticTable()))
	{
		// 표시되는 무기 타입 목록
		TSet<CreatureWeaponType> weaponTypeSet;
		TArray<const FGsSchemaWeaponTypeMenu*> weaponTypeMenuList;
		if (table->GetAllRows(weaponTypeMenuList))
		{
			for (const FGsSchemaWeaponTypeMenu* weaponTypeMenuData : weaponTypeMenuList)
			{
				if (false == weaponTypeMenuData->useMenu)
					continue;

				if (CreatureWeaponType::NONE == weaponTypeMenuData->type)
					continue;

				weaponTypeSet.Emplace(weaponTypeMenuData->type);
			}
		}

		for (int32 i = 0; i < _buttonTabTypeList.Num(); ++i)
		{
			ENpcShopProductListTab tabType = _buttonTabTypeList[i].Key;
			UGsSwitcherButton* btnWeapon = _buttonTabTypeList[i].Value;
			if (nullptr == btnWeapon)
				continue;

			btnWeapon->SetVisibility(weaponTypeSet.Contains(GetWeaponType(tabType)) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}

	Hide();
}

void UGsUIWeaponFilter::NativeDestruct()
{

	Super::NativeDestruct();
}

void UGsUIWeaponFilter::ShowOrHide()
{
	ESlateVisibility visibility = GetVisibility();
	if (visibility == ESlateVisibility::Collapsed ||
		visibility == ESlateVisibility::Hidden)
	{
		Show();
	}
	else
	{
		Hide();
	}
}

void UGsUIWeaponFilter::Show()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return;

	for (int32 i = 0;i < _buttonTabTypeList.Num();++i)
	{
		ENpcShopProductListTab tabType = _buttonTabTypeList[i].Key;
		UGsSwitcherButton* btnWeapon = _buttonTabTypeList[i].Value;
		if (nullptr == btnWeapon)
			continue;

		btnWeapon->SetIsSelected(npcShopManager->IsInFilter(tabType));
	}
}

void UGsUIWeaponFilter::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIWeaponFilter::OnClickSave()
{
	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return;

	for (int32 i = 0;i < _buttonTabTypeList.Num();++i)
	{
		ENpcShopProductListTab tabType = _buttonTabTypeList[i].Key;
		UGsSwitcherButton* btnWeapon = _buttonTabTypeList[i].Value;
		if (nullptr == btnWeapon)
			continue;

		bool isSelected = btnWeapon->GetIsSelected();
		if (isSelected)
		{
			npcShopManager->AddFilter(tabType);
		}
		else
		{
			npcShopManager->RemoveFilter(tabType);
		}
	}

	OnWeaponFilterSave.ExecuteIfBound();
	Hide();
}

void UGsUIWeaponFilter::OnClickWeapon(int32 InIndex, bool InIsSelected)
{
	if (false == _buttonTabTypeList.IsValidIndex(InIndex))
		return;

	ENpcShopProductListTab tabType = _buttonTabTypeList[InIndex].Key;
	UGsSwitcherButton* btnWeapon = _buttonTabTypeList[InIndex].Value;
	if (nullptr == btnWeapon)
		return;

	btnWeapon->SetIsSelected(InIsSelected);
}

CreatureWeaponType UGsUIWeaponFilter::GetWeaponType(ENpcShopProductListTab InTabType)
{
	switch (InTabType)
	{
	case ENpcShopProductListTab::ProductList_Sword:
		return CreatureWeaponType::SWORD;
	case ENpcShopProductListTab::ProductList_Dagger:
		return CreatureWeaponType::DAGGER;
	case ENpcShopProductListTab::ProductList_Bow:
		return CreatureWeaponType::BOW;
	case ENpcShopProductListTab::ProductList_SHOTGUN:
		return CreatureWeaponType::SHOTGUN;
	case ENpcShopProductListTab::ProductList_Wand:
		return CreatureWeaponType::WAND;
	case ENpcShopProductListTab::ProductList_Orb:
		return CreatureWeaponType::ORB;
	default:
		return CreatureWeaponType::NONE;
	}
	return CreatureWeaponType::NONE;
}