// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIContent/Window/Fairy/GsUISummonPortraitBase.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "Engine/AssetManager.h"

#include "PaperSprite.h"
#include "Image.h"
#include "CanvasPanel.h"
#include "WidgetSwitcher.h"
#include "Classes/PaperSprite.h"
#include "Materials/MaterialInstanceDynamic.h"

void UGsUISummonPortraitBase::RefreshUI()
{
	if (auto baseData = _baseData.Pin())
	{
		SetPortraiteImage(baseData->GetPortraitPath(_useGender));
		SetGrade(baseData->GetGradeByItemBase());
	}
}

void UGsUISummonPortraitBase::SetPortraiteImage(const FSoftObjectPath& path, bool masked)
{
	if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(path))
	{
		if (masked)
		{
			SetIconImageSprite(Cast<UPaperSprite>(loadObject));
		}
		else
		{
			_portraitImg->Brush.SetResourceObject(loadObject);
		}
	}

}

void UGsUISummonPortraitBase::SetGrade(ItemGrade grade)
{
	_gradeSwitcher->SetVisibility(grade == ItemGrade::NONE ?
		ESlateVisibility::Hidden : ESlateVisibility::Visible);
	_gradeBack->SetVisibility(grade == ItemGrade::NONE ?
		ESlateVisibility::Hidden : ESlateVisibility::Visible);

	static TMap<ItemGrade, uint8> gradeSelect = {
		{ ItemGrade::NONE, 0 },
		{ ItemGrade::NORMAL, 0 },
		{ ItemGrade::MAGIC, 1 },
		{ ItemGrade::RARE, 2 },
		{ ItemGrade::EPIC, 3 },
		{ ItemGrade::UNIQUE, 4 },
		{ ItemGrade::LEGEND, 5 },
	};

	_gradeSwitcher->SetActiveWidgetIndex(gradeSelect[grade]);
	_gradeBack->SetActiveWidgetIndex(gradeSelect[grade]);
}

void UGsUISummonPortraitBase::SetIconImageSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		if (UMaterialInstanceDynamic* mat = _portraitImg->GetDynamicMaterial())
		{
			FSlateAtlasData atlasData = InSprite->GetSlateAtlasData();
			mat->SetTextureParameterValue(TEXT("Texture"), atlasData.AtlasTexture);
			mat->SetScalarParameterValue(TEXT("OffsetU"), atlasData.StartUV.X);
			mat->SetScalarParameterValue(TEXT("OffsetV"), atlasData.StartUV.Y);
			mat->SetScalarParameterValue(TEXT("TilingU"), atlasData.SizeUV.X);
			mat->SetScalarParameterValue(TEXT("TilingV"), atlasData.SizeUV.Y);
		}
	}
}