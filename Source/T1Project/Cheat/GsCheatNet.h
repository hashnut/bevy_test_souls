// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GsCheatNet.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsCheatNet : public UObject
{
	GENERATED_BODY()

public:
	// 서버 치트키
	UFUNCTION(Exec, Category = ExecFunctions)
	void  NC(const FString& in_val);	

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void Despawn_npc(int In_Radius);
	
};
