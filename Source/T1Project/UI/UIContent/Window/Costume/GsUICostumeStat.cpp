// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICostumeStat.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Engine/AssetManager.h"

void UGsUICostumeStat::SetData(StatType type, int32 value, bool isLock /*= false*/)
{
	_isLock->SetVisibility(isLock ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (type != StatType::NONE)
	{
		FGsStatHelper::GetDisPlayStatName(type, _txtStatName);
		FGsStatHelper::GetDisPlayStatValueWithMarkFormat(type, value, _txtStatValue);

		FString iconImagePath;
		if (FGsStatHelper::GetStatIconImagePath(type, iconImagePath))
		{
			if (iconImagePath.IsEmpty()) return;

			if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(iconImagePath))
			{
				_statIconBrush.SetResourceObject(loadObject);
				_statIconBrush.SetImageSize(FVector2D(56, 56));
			}
		}
	}
}
