// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

UENUM()
enum class EGsAutoConnectType : uint8
{
	CONNECT_LOGIN = 0,
	CONNECT_SERVER,
	CONNECT_CHARACTER,
};

/**
 * 
 */
class UGsGameUserSettings;
class T1PROJECT_API FGsNetAutoLogin
{
public:
	virtual ~FGsNetAutoLogin() {}

private:
	UGsGameUserSettings* gameUserSettings = nullptr;
	bool isAutoLogin = false;

	TMap<EGsAutoConnectType, TFunction<bool()>> stageTypeFuncs;

public:
	void OnInitialize();

public:	
	bool Enter(EGsAutoConnectType in_StageType);
	void SetAutoLogin(bool in_value);

private:
	bool EnterLogin();
	bool EnterServerSelect();
	bool EnterCharacterSelect();

public:
	bool IsAutoLogin();

public:
	static const FString GetAppID();
	static const Did GetDID();
	static const FString GetDIDText();
	static const FString GetDeviceName();
	static const FString GetHiveLanguage();	
	static const FString GetHiveCountry();
};
