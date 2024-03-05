// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GsGameObjectBlueprintLibrary.generated.h"

class UGsGameObjectBase;
/**
 * GameObject 관련 BP 제공 유틸함수 모음
 */
UCLASS()
class T1PROJECT_API UGsGameObjectBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
		//로컬 캐릭터 스폰
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "GameObject")
	static void SpawnPlayer(UClass* ActorClass, int64 InGameId, const FVector& StartPos, const FVector& Offset);
	//지형위에 스폰되는 함수 제공
	//ACharacter 클래스에는 UCharacterMovementComponent 가 제공해주고 있지만 APawn계열엔 없다.
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "GameObject")
	static UGsGameObjectBase* SpawnNonPlayer(UClass* ActorClass, int64 InGameId, const FVector& StartPos, const FVector& Offset);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "GameObject")
	static UGsGameObjectBase* SpawnVehicle(UClass* ActorClass, int64 InGameId, const FVector& StartPos, const FVector& Offset);

	UFUNCTION(BlueprintPure, meta = (Tooltip = ""), Category = "GameObject")
	static UGsGameObjectLocalPlayer* GetCurrentObjectLocalPlayer();
	
public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = "WorldComposition Streaming In/Out Falling Charater Holder "), Category = "GameObject")
	static bool HoldStreamingLevelbyLocation(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "WorldComposition Streaming In/Out Falling Charater Holder "), Category = "GameObject")
	static bool HoldStreamingLevelbyLocalPlayer();

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "WorldComposition Streaming In/Out Falling Charater Holder "), Category = "GameObject")
	static void UnHoldStreamingLevel();

public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Change Camera Mode"), Category = "GameObject")
	static void ChangeCameraMode();
};
