// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGrayscaleImage.h"
#include "PaperSprite.h"
#include "Materials/MaterialInstanceDynamic.h"


void UGsGrayscaleImage::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (nullptr != _sprite)
	{
		SetPaperSprite(_sprite);
		SetGrayscale(_bIsGray);
	}
}

void UGsGrayscaleImage::SetPaperSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		if (UMaterialInstanceDynamic* mat = GetDynamicMaterial())
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

void UGsGrayscaleImage::SetGrayscale(bool bInIsGray)
{
	_bIsGray = bInIsGray;

	FLinearColor darkness = (bInIsGray) ?
		FLinearColor(_grayDarkness, _grayDarkness, _grayDarkness) : FLinearColor::White;

	SetColorAndOpacity(darkness);

	if (UMaterialInstanceDynamic* mat = GetDynamicMaterial())
	{
		mat->SetScalarParameterValue(TEXT("GrayDensity"), (true == bInIsGray) ? _grayDensity : 0.f);
		mat->SetScalarParameterValue(TEXT("Opacity"), (true == bInIsGray) ? _grayOpacity : 1.f);
	}
}

void UGsGrayscaleImage::SetDissolveIntensity(float InValue)
{
	if (UMaterialInstanceDynamic* mat = GetDynamicMaterial())
	{
		mat->SetScalarParameterValue(TEXT("Dissolve Intensity"), InValue);
	}
}
