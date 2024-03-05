// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayEnchantFairyResult.h"
#include "Fairy/GsFairyData.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyItem.h"

void UGsUITrayEnchantFairyResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_ItemFairyPortrait->WidgetType = UGsUIFairyItem::UIItemType::ENCHANT_RES;
}

void UGsUITrayEnchantFairyResult::NativeConstruct()
{
	Super::NativeConstruct();

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayEnchantFairyResult::OnclickConfirm);
	_btnConfirm->SetIsEnabled(false);
	_lockMaxTime = 2.f;
}

void UGsUITrayEnchantFairyResult::NativeDestruct()
{
	_btnConfirm->OnClicked.RemoveDynamic(this, &UGsUITrayEnchantFairyResult::OnclickConfirm);
	Super::NativeDestruct();
}

void UGsUITrayEnchantFairyResult::OnclickConfirm()
{
	_isFinished = true;
}

void UGsUITrayEnchantFairyResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (_lockMaxTime > 0.f)
	{
		_lockMaxTime -= InDeltaTime;

		if (_lockMaxTime <= 0.f)
		{
			_lockMaxTime = 0.f;
			_btnConfirm->SetIsEnabled(true);
		}
	}
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUITrayEnchantFairyResult::InitializeData(EffectInitData* initParam)
{
	_btnConfirm->SetIsEnabled(false);
	_lockMaxTime = 1.3f;

	if (auto resParam = StaticCast<EnchantResultEffectInitData*>(initParam))
	{
		if (resParam->_data.IsValid())
		{
			_ItemFairyPortrait->SetData(resParam->_data);
			_ItemFairyPortrait->RefreshUI();
		}
		if (resParam->_title.IsEmpty() == false)
			_title = resParam->_title;
		if (resParam->_desc.IsEmpty() == false)
			_desc = resParam->_desc;

		_detail->SetVisibility(resParam->_useDetail ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	Super::InitializeData(initParam);
}