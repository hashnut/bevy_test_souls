// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GsCampBuildableQueryComponent.generated.h"

/*
* GsSphereNode 를 처리하기 위해 추가된 컴포넌트
* 캠프가 설치가능한지 여부를 쿼리하기 위함( 로컬 플레이어용 )
*/
class FGsBuildAbleQueryCallback;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsCampBuildableQueryComponent : public UActorComponent
{
	GENERATED_BODY()

	TSharedPtr< FGsBuildAbleQueryCallback > m_pQuery;

public:
	UPROPERTY(EditAnywhere)
	bool	_activeCheck = true;

	UPROPERTY(EditAnywhere)
	FSoftObjectPath BluePrint;

	UPROPERTY(EditAnywhere)
	float	_HeightGap = 450.0f;

	UPROPERTY(EditAnywhere)
	float	_DistanceGap = 1.2f;

	UPROPERTY(EditAnywhere)
	int		_DebugRender = 0;

private:
	// 스폰된 메쉬 액터
	TWeakObjectPtr<AActor> MeshActor;

	// 머터리얼 제저용 스테틱 메쉬 컴포넌트
	TWeakObjectPtr<UStaticMeshComponent>	StaticMeshCompnent;

public:	
	// Sets default values for this component's properties
	UGsCampBuildableQueryComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool CheckCampBuildAble(const FVector& inPos, float inRadius);

		
};
