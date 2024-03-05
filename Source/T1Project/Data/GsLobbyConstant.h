// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "NoExportTypes.h"
#include "GsLobbyConstant.generated.h"

USTRUCT(BlueprintType)
struct FGsLobbyUIData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyUIData")
	FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyUIData")
	FSoftObjectPath Path;
};

class UMediaSource;
class USoundBase;
class UCurveFloat;
/**
 * 전역에서 쓰이는 로비 설정데이터를 입력받고 처리하기 위한 데이터 에셋
 */
UCLASS(BlueprintType, Category = "LobbyConstant")
class T1PROJECT_API UGsLobbyConstant : public UDataAsset
{
	GENERATED_BODY()
public:
	UGsLobbyConstant();

public:
	// window ui
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathWIndowTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathWindowServerList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathWindowNetAddressSelect;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathWindowIDPassword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathWIndowDownLoad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathWindowIntro;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathPopupDownload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathPopupLoginWaiting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathPopupServerSelect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathPopupLobbyOption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathPopupResolutionList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathPopupResolutionYesNo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathTrayBlockUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathTrayFadeEffectUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathTrayTickerUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathTrayDimmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathPopupSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathPopupSystemYesNo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FGsLobbyUIData _pathPopupAccountDelete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FSoftObjectPath _touchVfx;

	// sound bgm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FSoftObjectPath _pathBgmIntro;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FSoftObjectPath _pathBgmTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	FSoftObjectPath _pathBgmDownLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyConstant")
	UCurveFloat* _audioVolumeScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UMediaSource> _introMovie;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UMediaSource> _downloadMovie;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UMediaSource> _titleMovie;
};
