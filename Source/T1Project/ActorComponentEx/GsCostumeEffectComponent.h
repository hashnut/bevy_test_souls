// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "GsCostumeEffectComponent.generated.h"


/**
* 코스튬 이펙트 캐릭터 부착 및 위치 설정용 컴포넌트
*/
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsCostumeEffectComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FName _socketName;

	UPROPERTY(EditAnywhere)
	FTransform _socketOffsetTransform;

public:	
	UGsCostumeEffectComponent();

public:
	FName GetSocketName() { return _socketName; }
	FTransform GetSocketOffsetTransform() { return _socketOffsetTransform; }
	
};
