#include "GsActorSummonCostume.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Customize/GsCustomizeFunc.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "DataSchema/Summon/GsSchemaSummonCostumeGradeRes.h"

#include "UTIL/GsTableUtil.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/SceneComponent.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Engine/AssetManager.h"

const FName NameSummonCostumeParticleSystemComponent = TEXT("Particle");

AGsActorSummonCostume::AGsActorSummonCostume()
	:Super()
{
	bCanBeInCluster = false;

	_particleGrade = CreateDefaultSubobject<UParticleSystemComponent>(NameSummonCostumeParticleSystemComponent);

	USkeletalMeshComponent* costumeMesh = GetMesh();
	_particleGrade->SetupAttachment(costumeMesh);
}

void AGsActorSummonCostume::BeginPlay()
{
	Super::BeginPlay();
}

// 스케일 초기화
void AGsActorSummonCostume::InitScale()
{
	float scale = GData()->GetGlobalData()->_summonCostumeScale;

	if (scale > 0.f)
	{
		SetActorRelativeScale3D(FVector(scale, scale, scale));
	}
}
bool AGsActorSummonCostume::IsHighGrade()
{
	return ((int)_grade >= (int)CostumeGrade::RARE) ? true : false;
}
// hide 처리
void AGsActorSummonCostume::SetHide(bool In_isHide)
{
	SetActorHiddenInGame(In_isHide);

	if (In_isHide == false)
	{
		_particleGrade->Activate(true);
	}
}

void AGsActorSummonCostume::Initialize(FGsGameObjectDataLocalPlayer* In_localData,
	CostumeId In_costumeId, CostumeGrade In_grade)
{
	SetCostumeId(In_costumeId);
	SetGrade(In_grade);


	_customizeHandler = new FGsCustomizeHandler();
	_customizeHandler->InitializeActor(this);

	if (In_localData != nullptr)
	{
		FGsCustomizeFunc::SetCustomizeByLocalData(_customizeHandler, In_localData, _costumeId, true, true, false, true );
	}

	// movement component tick에서 CheckStillInWorld 체크후 삭제 시도 있어서 막아본다
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	// beginplay에서 스케일시 무기 크기가 2배되어서
	// 여기서 해보자
	InitScale();
}
// 정리
void AGsActorSummonCostume::Finalize()
{
	if (_customizeHandler != nullptr)
	{
		delete _customizeHandler;
		_customizeHandler = nullptr;
	}	
}

void AGsActorSummonCostume::DestroySelf()
{
	Destroy();
}

// 등급 이펙트 붙이기
void AGsActorSummonCostume::AttachGradeParticle()
{
	const FGsSchemaSummonCostumeGradeRes* findTbl =
		UGsTableUtil::FindRowById<UGsTableSummonCostumeGradeRes, FGsSchemaSummonCostumeGradeRes>(_grade);

	if (findTbl == nullptr)
	{
		return;
	}

	const FSoftObjectPath* path = &findTbl->summonCostumeGradeEffect;
	if (path == nullptr ||
		path->IsValid() == false)
	{
		return;
	}

	if (UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(*path))
	{
		if (UParticleSystem* particleSystem = Cast<UParticleSystem>(load))
		{
			_particleGrade->SetTemplate(particleSystem);
		}
	}
}

// ani 상태 변경
void AGsActorSummonCostume::ChangeAniState(EGsStateSummonCostume In_state)
{
	USkeletalMeshComponent* costumeMesh = GetMesh();
	if (costumeMesh == nullptr)
	{
		return;
	}

	UGsSummonCostumeAnimInstance* animInstance =
		Cast<UGsSummonCostumeAnimInstance>(costumeMesh->GetAnimInstance());

	if (animInstance == nullptr)
	{
		return;
	}

	animInstance->ChangeState(In_state);
}

// 스폰 애니 출력
void AGsActorSummonCostume::PlaySpawnAni()
{
	ChangeAniState(EGsStateSummonCostume::Spawn);

	StartSpawnAni();
}

// 스폰 위치 오프셋
FVector AGsActorSummonCostume::GetSpawnPosOffset()
{
	return GData()->GetGlobalData()->_summonCostumeOffset;
}