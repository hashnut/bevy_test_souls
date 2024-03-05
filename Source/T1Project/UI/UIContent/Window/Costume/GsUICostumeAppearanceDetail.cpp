// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUICostumeAppearanceDetail.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "../../Helper/GsUIColorHelper.h"

void UGsUICostumeAppearanceDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	_btnNoApplyFix->OnClicked.AddDynamic(this, &UGsUICostumeAppearanceDetail::OnClickNoAppliedOn);
}

void UGsUICostumeAppearanceDetail::NativeConstruct()
{
	Super::NativeConstruct();

	_btnNoApplyFix->SetIsSelected(false);
	RefreshUI();
}

void UGsUICostumeAppearanceDetail::OnClickNoAppliedOn()
{
	bool set = !_btnNoApplyFix->GetIsSelected();
	_btnNoApplyFix->SetIsSelected(set);

	_costumeInfo->SetVisibility(set ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	OnNoApplyedFix.ExecuteIfBound(set);
}

void UGsUICostumeAppearanceDetail::SetPanelInfo(CostumeParts parts)
{
	_panelType = parts;
	_fixPanel->SetVisibility(_panelType == CostumeParts::FULLSET ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UGsUICostumeAppearanceDetail::SetData(TWeakPtr<FGsCostumeData> data)
{
	_data = data;

	if (auto appearance = data.Pin())
	{
		if (appearance->IsEnableAppearceParts(CostumeParts::FULLSET) && _btnNoApplyFix->GetIsSelected())
			_btnNoApplyFix->SetIsSelected(false);
	}

	RefreshUI();
}

void UGsUICostumeAppearanceDetail::RefreshUI()
{
	if (auto data = _data.Pin())
	{
		if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(data->GetCostumePortraitPath()))
		{
			_portraitImg->Brush.SetResourceObject(loadObject);
		}

		FLinearColor gradeColor = FGsUIColorHelper::GetColorGradeWidthItemGrade(data->GetCostumeGrade());

		_costumeName->SetText(data->GetCostumeName());
		_costumeName->SetColorAndOpacity(gradeColor);

		_portraitImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_costumeName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		_bgNotCollected->SetVisibility(data->isCollectedCostume() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_portraitImg->SetVisibility(ESlateVisibility::Collapsed);
		_costumeName->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UGsUICostumeAppearanceDetail::GetCostumePartsFixed()
{
	return _btnNoApplyFix->GetIsSelected();
}
