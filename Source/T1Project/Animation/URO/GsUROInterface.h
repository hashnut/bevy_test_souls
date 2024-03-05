#pragma once

#include "CoreMinimal.h"

struct FAnimUpdateRateParameters;

class USkeletalMeshComponent;

class IGsUROInterface
{
public:
	virtual ~IGsUROInterface() = default;


public:
	void InitSkeletalMeshURO();
	
	// get func
public:
	virtual void GetSkeletalMeshs(TArray<USkeletalMeshComponent*>& Out_skeletalMesh) {}
};