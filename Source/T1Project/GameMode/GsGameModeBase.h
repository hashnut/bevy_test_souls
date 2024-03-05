// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GsGameModeDefine.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsGameModeBase.generated.h"

class UGsScopeHolder;
class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;
/**
 * 게임의 틀을 잡아주는 역활
   1. 플레이어 점검
   2. 입장한 플레이어가 조종할 컨트롤러 할당
 */
UCLASS()
class T1PROJECT_API AGsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	UGsScopeHolder*	_scopeHolder = nullptr;

	UPROPERTY(Transient)
	FTransform _playerSpawnTransform;

	//Global ambient 
private:
	UPROPERTY(Transient)
	UMaterialParameterCollectionInstance* _materialParamterCollectionIntacne;

public:
	AGsGameModeBase();
	virtual ~AGsGameModeBase();

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	// 플레이어가 게임에 입장하는 순간(로그인), Editor에서 Play
	virtual void PostLogin(APlayerController* newPlayer) override;

	virtual void StartPlay() override;

	//~ Begin AActor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void BeginDestroy() override;
	// 현재 모드 타입 구해오기
	virtual EGsGameModeType GetGameModeType() { return EGsGameModeType::None; }

public:
	void InitSignificanceManager();

public:
	static const DevicePlatformType GetPlatformType();
};