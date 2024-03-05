// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/AnimNotify/GsMaterialParam.h"
#include "AnimNotifyState_StaticMesh.generated.h"

UCLASS()
class T1PROJECT_API UAnimNotifyState_StaticMesh : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_StaticMesh();

	UPROPERTY(EditAnywhere)
	FVector LookAtBaseOffsetPos;

	UPROPERTY(EditAnywhere)
	FRotator LookAtBaseOffsetRot;

	UPROPERTY(EditAnywhere)
	float ActorScale = 1.0f;

	UPROPERTY(EditAnywhere)
	FSoftObjectPath BluePrint;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface*	CustomMaterial;

	UPROPERTY(EditAnywhere)
	int TargetMaterialIndex = 0;

	UPROPERTY(EditAnywhere)
	TArray<FGsMaterialParam> MaterialPrarams;

	UPROPERTY(EditAnywhere)
	float MeshRemainTime = 0.0f;

	// 스폰된 메쉬 액터
	TWeakObjectPtr<AActor> MeshActor;

	// 메쉬 컴포넌트
	TWeakObjectPtr<UStaticMeshComponent> StaticMeshCompnent;

	// Should attach to the bone/socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 Attached:1; 	//~ Does not follow coding standard due to redirection from BP

	// SocketName to attach to
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

private:
	float CurrentTime;
	float NotifyTotalTime;

public:
	//UObject
	virtual void BeginDestroy() override;
	virtual void NotifyBegin(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation) override;

private:
	// 스켈레탈 메쉬액터 소멸
	void DestoryStaticMeshActor();

	// 스켈레탈 메쉬액터 초기화
	bool InitStaticMeshActor(USkeletalMeshComponent* in_MeshComp);

private:
	// EGsSelectSkeletalMesh 타입에 따라 스켈레탈 메쉬 액터를 스폰한다.
	bool SpawnActorByStaticMeshSelect(USkeletalMeshComponent* in_MeshComp);

	// EGsSkeletalMeshMaterial 타입에 따라 머터리얼을 수정한다.
	void ModifySkeletalMeshMaterial();

};
