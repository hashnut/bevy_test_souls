// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICoolTimeEffectMaterial.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

const FName MaterialParamName = FName(TEXT("Percent"));


void UGsUICoolTimeEffectMaterial::NativeConstruct()
{
	Super::NativeConstruct();

	SetProgress(0.f);
}

void UGsUICoolTimeEffectMaterial::SetProgress(float InRate)
{
	if (UMaterialInstanceDynamic* material = _imgProgress->GetDynamicMaterial())
	{
		material->SetScalarParameterValue(MaterialParamName, InRate);
	}
}
