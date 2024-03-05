// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GsSphereNodeActorComponent.generated.h"

/*
* GsSphereNode 를 처리하기 위해 추가된 컴포넌트
* 몬스터, 유저등 동적으로 이동하는 정보를 처리하기 위함
*/
class FGsSphereNode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsSphereNodeActorComponent : public UActorComponent
{
	GENERATED_BODY()

	FGsSphereNode* _pNodeData;

public:	
	// Sets default values for this component's properties
	UGsSphereNodeActorComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
