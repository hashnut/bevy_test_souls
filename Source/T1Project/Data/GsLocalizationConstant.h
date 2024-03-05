// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GsLocalizationConstant.generated.h"


USTRUCT(BlueprintType)
struct T1PROJECT_API FGsLocalizationPackageData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocalizationPackageData")
	FName culture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocalizationPackageData")
	int32 assetPackageNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocalizationPackageData")
	int32 voicePackageNum;
};

/**
 * 로컬라이징 패키지 번호 정보
 */
UCLASS(BlueprintType, Category = "LocalizationConstant")
class T1PROJECT_API UGsLocalizationConstant : public UDataAsset
{
	GENERATED_BODY()

public:
	UGsLocalizationConstant();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
	TArray<FGsLocalizationPackageData> _locPackageDataList;

public:
	// InCulture: GLocalization()->GetCurrentCultureName() 값
	int32 GetAssetPackageNum(const FString& InCulture) const;
	// InVoiceCulture: GLocalization()->GetVoiceCultureName() 값
	int32 GetVoicePackageNum(const FString& InVoiceCulture) const;

private:
	const FGsLocalizationPackageData* GetLocPackageData(const FName& InCulture) const;
};
