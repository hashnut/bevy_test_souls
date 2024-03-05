// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconRegionMapInvasion.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIIconRegionMapInvasion::SetIconImageColor(const FLinearColor& InColor)
{
	//Super::SetIconImageColor(InColor);

	_textTime->SetColorAndOpacity(InColor);
}

void UGsUIIconRegionMapInvasion::SetTimeText(FText inTextTime)
{
	_textTime->SetText(inTextTime);
}

void UGsUIIconRegionMapInvasion::SetActiveSelect(int set /*= 1*/)
{
	_selectActive->SetActiveWidgetIndex(set);
}
void UGsUIIconRegionMapInvasion::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnIcon->OnClicked.AddDynamic(this, &UGsUIIconRegionMapInvasion::OnClickButton);
}

void UGsUIIconRegionMapInvasion::OnClickButton()
{
	if (_callbackClick != nullptr)
	{
		_callbackClick();
	}
}