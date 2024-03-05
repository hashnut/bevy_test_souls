// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotifyState_StaticMesh.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimNotify/GsMaterialParam.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveFloat.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

UAnimNotifyState_StaticMesh::UAnimNotifyState_StaticMesh()
	: Super()
{
	Attached = true;
}

void UAnimNotifyState_StaticMesh::BeginDestroy()
{
	DestoryStaticMeshActor();
	Super::BeginDestroy();
}

void UAnimNotifyState_StaticMesh::NotifyBegin(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_TotalDuration)
{
	CurrentTime = 0;
	NotifyTotalTime = in_TotalDuration;

	if (InitStaticMeshActor(in_MeshComp))
	{
		MeshActor->SetLifeSpan(in_TotalDuration + MeshRemainTime);

		ModifySkeletalMeshMaterial();
	}

	Super::NotifyBegin(in_MeshComp, in_Animation, in_TotalDuration);
}

void UAnimNotifyState_StaticMesh::NotifyTick(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_FrameDeltaTime)
{
	Super::NotifyTick(in_MeshComp, in_Animation, in_FrameDeltaTime);

	if (StaticMeshCompnent.IsValid())
	{
		CurrentTime += in_FrameDeltaTime;
		for (FGsMaterialParam& param : MaterialPrarams)
		{
			switch (param.ParamType)
			{
			case EGsSkeletalMeshMaterialParam::CurveScalar:
			{
				if (nullptr == param.MaterialParamCF)
					return;

				StaticMeshCompnent->SetScalarParameterValueOnMaterials(param.MaterialParameterName, param.MaterialParamCF->GetFloatValue(CurrentTime / NotifyTotalTime));
			}
			break;
			case EGsSkeletalMeshMaterialParam::CurveVector:
			{
				if (nullptr == param.MaterialParamCV)
					return;

				StaticMeshCompnent->SetVectorParameterValueOnMaterials(param.MaterialParameterName, param.MaterialParamCV->GetVectorValue(CurrentTime / NotifyTotalTime));
			}
			break;
			case EGsSkeletalMeshMaterialParam::Scalar:
				StaticMeshCompnent->SetScalarParameterValueOnMaterials(param.MaterialParameterName, param.MaterialParamF);
				break;
			case EGsSkeletalMeshMaterialParam::Vector:
				StaticMeshCompnent->SetVectorParameterValueOnMaterials(param.MaterialParameterName, param.MaterialParamV);
				break;
			}
		}
	}
}

void UAnimNotifyState_StaticMesh::NotifyEnd(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation)
{
	DestoryStaticMeshActor();

	Super::NotifyEnd(in_MeshComp, in_Animation);
}

void UAnimNotifyState_StaticMesh::DestoryStaticMeshActor()
{
	if (MeshActor.IsValid() && (MeshRemainTime <= 0.0f))
	{
		MeshActor->Destroy();
	}
	MeshActor.Reset();
	StaticMeshCompnent.Reset();
}

bool UAnimNotifyState_StaticMesh::InitStaticMeshActor(USkeletalMeshComponent* in_MeshComp)
{
	// SkeletalMeshActor 준비
	if (false == SpawnActorByStaticMeshSelect(in_MeshComp))
	{
		return false;
	}

	//이후 머터리얼 조작을 위해 백업
	StaticMeshCompnent = Cast<UStaticMeshComponent>(MeshActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	check(StaticMeshCompnent.IsValid());

	// 시전자 (Mesh)의 LightChannel을 설정
	StaticMeshCompnent->SetLightingChannels(in_MeshComp->LightingChannels.bChannel0, in_MeshComp->LightingChannels.bChannel1,
		in_MeshComp->LightingChannels.bChannel2);
	
	return true;
}

bool UAnimNotifyState_StaticMesh::SpawnActorByStaticMeshSelect(USkeletalMeshComponent* in_MeshComp)
{
	AActor* ownerActor = in_MeshComp->GetOwner();
	if (nullptr == ownerActor)
		return false;
	
	UClass* bpClass = nullptr;
	FString BPPath = BluePrint.ToString();
	if (BPPath.Len() != 0)
	{
		BPPath += TEXT("_C");
		bpClass = LoadObject<UClass>(nullptr, *BPPath);
	}

	if (nullptr == bpClass)
	{
		return false;
	}

	UWorld* World = in_MeshComp->GetWorld();
	if (nullptr == World)
		return false;

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	MeshActor = World->SpawnActor(bpClass, &LookAtBaseOffsetPos, &LookAtBaseOffsetRot, spawnParam);
	if (nullptr == MeshActor)
	{
		return false;
	}

	if (Attached)
	{
		USceneComponent* attachComponent = MeshActor->GetRootComponent();
		if (nullptr == attachComponent)
		{
			return false;
		}

		USceneComponent* parentComponent = nullptr;
		ACharacter* characterActor = Cast<ACharacter>(ownerActor);
		if (characterActor)
		{
			// 인게임 환경에서는 모델 정보를 찾아서 모델에 붙임
			parentComponent = characterActor->GetMesh();
		}
		else
		{
			parentComponent = ownerActor->GetDefaultAttachComponent();
		}

		if (nullptr == parentComponent)
		{
			return false;
		}
		
		attachComponent->AttachToComponent(parentComponent,
			FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	}
	else
	{
		UCharacterMovementComponent* moveComponent = Cast< UCharacterMovementComponent>(
			in_MeshComp->GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));

		FVector OwnerPos = nullptr != moveComponent ? moveComponent->GetActorFeetLocation() : in_MeshComp->GetComponentLocation();
		FRotator OwnerRot = in_MeshComp->GetOwner()->GetActorRotation();
		if (in_MeshComp->GetWorld()->IsGameWorld())
		{
			OwnerRot += FRotator(0, -90, 0);
		}

		MeshActor->SetActorLocationAndRotation(OwnerPos + OwnerRot.RotateVector(LookAtBaseOffsetPos), OwnerRot + LookAtBaseOffsetRot);
	}	
	MeshActor->SetActorScale3D(FVector(ActorScale, ActorScale, ActorScale));
	return true;
}

void UAnimNotifyState_StaticMesh::ModifySkeletalMeshMaterial()
{
	if (CustomMaterial && StaticMeshCompnent.IsValid())
	{
		StaticMeshCompnent->SetMaterial(TargetMaterialIndex, CustomMaterial);
	}
}