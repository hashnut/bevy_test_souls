// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SignificanceManager.h"
#include "GsSignificanceManager.generated.h"

/**
 * 파티클 시스템의 시그니피컨스 레벨을 옵션에 연동해서 처리하기로 협의
 * 하, 중, (상, 최상) 3단계로 분리하여 처리
 */
UCLASS()
class T1PROJECT_API UGsSignificanceManager : public USignificanceManager
{
	GENERATED_BODY()
public:
	UGsSignificanceManager();

public:
	UFUNCTION()
	void OnActivateParticle(class UParticleSystemComponent* inParticle, bool inActive);	
};
