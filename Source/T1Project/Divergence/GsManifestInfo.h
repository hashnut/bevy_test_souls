#pragma once

#include "CoreMinimal.h"
#include "GsManifestInfo.generated.h"

// Manifest ASTC
// parse(FJsonObjectConverter::JsonArrayStringToUStruct)하기 위해서 
// 멤버 이름이 json 파일과 이름이 같아야 함

USTRUCT()
struct FGsManifestFileInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString filename;

	UPROPERTY()
	FString uniqueFilename;

	UPROPERTY()
	FString length;

	UPROPERTY()
	FString URL;

	UPROPERTY()
	FString hash;

	UPROPERTY()
	FString hash256;

	int32 PatchVersion;

public:
};

USTRUCT()
struct FGsManifestInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString ClientVersion;

	UPROPERTY()
	FString BuildUrl;

	UPROPERTY()
	TArray<FGsManifestFileInfo> files;
};