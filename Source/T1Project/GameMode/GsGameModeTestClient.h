// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeWorld.h"
#include "GsGameModeWorldBase.h"
#include "GsGameModeTestClient.generated.h"

/**
 * 
 */
struct FGsNetUserData;
struct FPathFollowingResult;
class FGsSummonHandler;

UCLASS()
class T1PROJECT_API AGsGameModeTestClient : public AGsGameModeWorldBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	CreatureGenderType _genderType = CreatureGenderType::MALE;
	FGsNetUserData* NetUserData = nullptr;;
	//World composition streaming level

	UPROPERTY()
	UClass* PlayerClass = nullptr;
private:
	// 테스트 클라이언트 모드에서 아트팀 테스트 지원용
	FGsSummonHandler* _summonHandler;

	// 테스트 클라이언트에서 네비게이션 TEST를 처리하기위한 목적지
	TArray<AActor*>	_navDestList;
	float _movementBlockReleaseTime = 0.0f;


public:
	AGsGameModeTestClient();
	virtual	~AGsGameModeTestClient();
	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginDestroy() override;
	//Worldcomposition
	//You must check loading terrain after player controller possess player
public:	
	virtual void TerrianLoadFinish();
	// 현재 모드 타입 구해오기`
	virtual EGsGameModeType GetGameModeType() override { return EGsGameModeType::TestClient; }	

	FGsSummonHandler* GetTestClientSummonHandler()
	{
		return _summonHandler;
	}

private:
	int GetCurrentLevelId();
public:
	bool OnCheckUnloadStreamingLevel(const FString& inLevel);

	//네비게이션 TEST를 처리하기위한 함수
	TArray<AActor*>& GetNavDestActor() { return _navDestList; }
	void OnTestClientAutoMove(const struct IGsMessageParam* InParam);
	void OnTestClientWarp(const struct IGsMessageParam* InParam);
	void CallbackMoveToPosEnd();
	void CallbackMoveToPosFailed(const FPathFollowingResult& Result);

	// 케릭터움직임제어
	void LocalPlayerMovementBlock(bool inFlag);
	void RefreshNavDest();
};
