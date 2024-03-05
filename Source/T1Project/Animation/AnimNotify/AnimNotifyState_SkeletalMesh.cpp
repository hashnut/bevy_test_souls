// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_SkeletalMesh.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimSequenceBase.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Runtime/Engine/Classes/Curves/CurveVector.h"
#include "Animation/AnimNotify/GsMaterialParam.h"
#include "Animation/AnimNotify/EGsSkeletalMeshMaterialParam.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimNotify_PlayMeshDissolveShader.h"
#include "ActorEx/Interface/GsMeshShaderInterface.h"
#include "Data/GsDataContainManager.h"


//UTIL
static AActor* BPClass(UWorld* World, UClass* Class, const FVector& SpawnLocation, const FRotator& SpawnRotation, FName Name = NAME_None)
{
	if (nullptr == World)
		return nullptr;

	if (nullptr == Class)
		return nullptr;

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (Name != NAME_None)
	{
		spawnParam.Name = Name;
	}

	return World->SpawnActor(Class, &SpawnLocation, &SpawnRotation, spawnParam);
}

// [10/21/2021 bak1210]
UAnimNotifyState_SkeletalMesh::UAnimNotifyState_SkeletalMesh(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UAnimNotifyState_SkeletalMesh::BeginDestroy()
{
	DestorySkeletalMeshActor();

	Super::BeginDestroy();
}

FString UAnimNotifyState_SkeletalMesh::GetNotifyName_Implementation() const
{	
	return Super::GetNotifyName_Implementation();
}

void UAnimNotifyState_SkeletalMesh::NotifyBegin(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_TotalDuration)
{
	CurrentTime = 0;
	NotifyTotalTime = in_TotalDuration;

	if (InitSkeletalMeshActor(in_MeshComp))
	{
		SkeletalMeshActor->SetLifeSpan(in_TotalDuration + MeshRemainTime);

		ModifySkeletalMeshMaterial();
	}

	// 디졸브 처리가 있는 Notify조사
	if (IsBranch(EGsGameClientBranchType::MATERIAL_PARAM_RESTORE_PROCESS))
	{
		if (in_Animation)
		{
			for (FAnimNotifyEvent& el : in_Animation->Notifies)
			{
				if (el.Notify && el.Notify->IsA<UAnimNotify_PlayMeshDissolveShader>())
				{
					UseMeshDissolveNotify = true;
					break;
				}
			}
		}
	}
}

void UAnimNotifyState_SkeletalMesh::NotifyTick(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_FrameDeltaTime)
{
	// 머터리얼 조작을 진행하는 타입이라면
	if (EGsSkeletalMeshMaterial::NONE != MaterialControlType)
	{
		CurrentTime += in_FrameDeltaTime;

		if (false == SkeletalMeshCompnent.IsValid())
			return;

		if (0 == MaterialPrarams.Num())
			return;
					

		auto fnInsertParam = [this](FGsMaterialParam& param, float currentTime)->void
		{
			switch (param.ParamType)
			{
			case EGsSkeletalMeshMaterialParam::CurveScalar:
			{
				if (nullptr == param.MaterialParamCF)
					return;

				SkeletalMeshCompnent->SetScalarParameterValueOnMaterials(param.MaterialParameterName, param.MaterialParamCF->GetFloatValue(currentTime / NotifyTotalTime));
			}
			break;
			case EGsSkeletalMeshMaterialParam::CurveVector:
			{
				if (nullptr == param.MaterialParamCV)
					return;

				SkeletalMeshCompnent->SetVectorParameterValueOnMaterials(param.MaterialParameterName, param.MaterialParamCV->GetVectorValue(currentTime / NotifyTotalTime));
			}
			break;
			case EGsSkeletalMeshMaterialParam::Scalar:
				SkeletalMeshCompnent->SetScalarParameterValueOnMaterials(param.MaterialParameterName, param.MaterialParamF);
				break;
			case EGsSkeletalMeshMaterialParam::Vector:
				SkeletalMeshCompnent->SetVectorParameterValueOnMaterials(param.MaterialParameterName, param.MaterialParamV);
				break;
			}
		};

		for (int i = 0; i < MaterialPrarams.Num(); ++i)
		{
			fnInsertParam(MaterialPrarams[i], CurrentTime);
		}
	}
}

void UAnimNotifyState_SkeletalMesh::NotifyEnd(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation)
{
	if (IsBranch(EGsGameClientBranchType::MATERIAL_PARAM_RESTORE_PROCESS))
	{
		if (UseMeshDissolveNotify)
		{
			if (IGsMeshShaderInterface* meshShader = Cast<IGsMeshShaderInterface>(in_MeshComp->GetOwner()))
			{
				meshShader->StopMeshShader();
			}
		}
	}

	DestorySkeletalMeshActor();
}

void UAnimNotifyState_SkeletalMesh::DestorySkeletalMeshActor()
{
	if (SkeletalMeshActor.IsValid() && (MeshRemainTime <= 0.0f) )
	{
		SkeletalMeshActor->Destroy();
	}
	SkeletalMeshActor.Reset();
	SkeletalMeshCompnent.Reset();
}

// 스켈레탈 메쉬 초기화
bool UAnimNotifyState_SkeletalMesh::InitSkeletalMeshActor(USkeletalMeshComponent* in_MeshComp)
{
	// SkeletalMeshActor 준비
	if (false == SpawnActorBySkeletalMeshSelect(in_MeshComp))
	{
		return false;
	}

	//이후 머터리얼 조작을 위해 백업
	SkeletalMeshCompnent = Cast<USkeletalMeshComponent>(SkeletalMeshActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	check(SkeletalMeshCompnent.IsValid());

	// 시전자 (Mesh)의 LightChannel을 설정
	SkeletalMeshCompnent->SetLightingChannels(in_MeshComp->LightingChannels.bChannel0, in_MeshComp->LightingChannels.bChannel1,
		in_MeshComp->LightingChannels.bChannel2);

	// 외부조합형일때에는 TemplateBP를 로드한 상태임으로 초기세팅 및 입력된 데이터를 지정한다.
	if (EGsSelectSkeletalMesh::COMBINE == SkeletalMeshSelect)
	{
			
		SkeletalMeshCompnent->SetAnimationMode(EAnimationMode::AnimationSingleNode);

		//SkeletalMeshPath 
		if (false == SkeletalMeshPath.IsValid())
		{
			return false;
		}

		TSharedPtr<FStreamableHandle> meshHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(SkeletalMeshPath);
		if (meshHandle->HasLoadCompleted())
		{
			SkeletalMeshCompnent->SetSkeletalMesh(Cast<USkeletalMesh>(meshHandle->GetLoadedAsset()));
		}

		if(false == AniPath.IsValid())
		{
			return false;
		}

		TSharedPtr<FStreamableHandle> aniHandle = UAssetManager::GetStreamableManager().RequestSyncLoad(AniPath);
		if (aniHandle->HasLoadCompleted())
		{
			SkeletalMeshCompnent->PlayAnimation(Cast<UAnimationAsset>(aniHandle->GetLoadedAsset()), AniLoop);
			if (ApplyAniPlayRate)
			{
				// 현재 재생중인 몽타주 데이터를 확인
				ACharacter* actor = Cast<ACharacter>(in_MeshComp->GetOwner());
				if (nullptr == actor)
				{
					return false;
				}

				USkeletalMeshComponent* meshComponent = actor->GetMesh();
				if (nullptr == meshComponent)
				{
					return false;
				}

				UAnimInstance* animInstance = meshComponent->GetAnimInstance();
				if (nullptr == animInstance)
				{
					return false;
				}

				FAnimMontageInstance* playMontageInstance = animInstance->GetActiveMontageInstance();
				if (nullptr == playMontageInstance)
				{
					return false;
				}

				SkeletalMeshCompnent->SetPlayRate(playMontageInstance->GetPlayRate());
			}
		}
	}
	return true;
}

// EGsSelectSkeletalMesh 타입에 따라 스켈레탈 메쉬 액터를 스폰한다.
bool UAnimNotifyState_SkeletalMesh::SpawnActorBySkeletalMeshSelect(USkeletalMeshComponent* in_MeshComp)
{
	if (nullptr == in_MeshComp->GetOwner())
		return false;

	auto caleOffsetPos = [this](FVector OwnerPos, FRotator OwnerRot) -> FVector
	{
		return OwnerPos + OwnerRot.RotateVector(this->LookAtBaseOffsetPos);
	};

	auto caleOffsetRot = [this](FRotator OwnerRot) -> FRotator
	{
		return OwnerRot + this->LookAtBaseOffsetRot;
	};

	UClass* bpClass = nullptr;
	if (EGsSelectSkeletalMesh::BLUEPRINT == SkeletalMeshSelect)
	{
		//BluePrint 타입일때에는 해당 BP를 로드한후에 SkeletalMeshComponent를 가지고 있는지 검사 필수		
		FString BPPath = BluePrint.ToString();
		if (BPPath.Len() != 0)
		{
			BPPath += TEXT("_C");
			bpClass = LoadObject<UClass>(nullptr, *BPPath);
		}		
	}
	else // EGsSelectSkeletalMesh::COMBINE
	{		
		//AGsSkeletalMeshActor(작업용)를 로드한후 데이터 세팅처리
		bpClass = LoadObject<UClass>(nullptr, TEXT("/Game/Blueprint/VFX/SkeletalMesh.SkeletalMesh_C"));
	}
	UCharacterMovementComponent* moveComponent = Cast< UCharacterMovementComponent>(
		in_MeshComp->GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));

	FVector OwnerPos = nullptr != moveComponent ? moveComponent->GetActorFeetLocation() : in_MeshComp->GetComponentLocation();
	FRotator OwnerRot = in_MeshComp->GetOwner()->GetActorRotation();
	if (in_MeshComp->GetWorld()->IsGameWorld())
	{
		OwnerRot += FRotator(0, -90, 0);
	}
	FVector spawnPos = caleOffsetPos(OwnerPos, OwnerRot);
	FRotator spawnRot = caleOffsetRot(OwnerRot);
	SkeletalMeshActor = BPClass(in_MeshComp->GetWorld(), bpClass, spawnPos, spawnRot /*, name*/);

	if (nullptr != SkeletalMeshActor)
	{
		SkeletalMeshActor->SetActorScale3D(FVector(ActorScale, ActorScale, ActorScale));
		return true;
	}
	return false;
}

// EGsSkeletalMeshMaterial 타입에 따라 머터리얼을 수정한다.
void UAnimNotifyState_SkeletalMesh::ModifySkeletalMeshMaterial()
{
	if ((EGsSkeletalMeshMaterial::CUSTOM == MaterialControlType) && nullptr != CustomMaterial)
	{
		// 내부에서 인덱스 검사수행
		// 기존 머터리얼을 복구할 필요는 없어보임
		SkeletalMeshCompnent->SetMaterial(TargetMaterialIndex, CustomMaterial);
	}
}
