// Fill out your copyright notice in the Description page of Project Settings.


#include "GsLocalizationConstant.h"


UGsLocalizationConstant::UGsLocalizationConstant()
{

}

int32 UGsLocalizationConstant::GetAssetPackageNum(const FString& InCulture) const
{
	FName cultureName(InCulture);
	if (const FGsLocalizationPackageData* data = GetLocPackageData(cultureName))
	{
		return data->assetPackageNum;
	}

	return -1;
}

int32 UGsLocalizationConstant::GetVoicePackageNum(const FString& InVoiceCulture) const
{
	FName cultureName(InVoiceCulture);
	if (const FGsLocalizationPackageData* data = GetLocPackageData(cultureName))
	{
		return data->voicePackageNum;
	}

	return -1;
}

const FGsLocalizationPackageData* UGsLocalizationConstant::GetLocPackageData(const FName& InCulture) const
{
	for (const FGsLocalizationPackageData& data : _locPackageDataList)
	{
		if (data.culture == InCulture)
		{
			return &data;
		}
	}

	return nullptr;
}
