#include "GsActorSummonFairy.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/Summon/GsSchemaSummonFairyGradeRes.h"

#include "UTIL/GsTableUtil.h"

#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"

#include "Engine/AssetManager.h"

// 스케일 초기화
void AGsActorSummonFairy::InitScale()
{
	float scale = GData()->GetGlobalData()->_summonFairyScale;

	if (scale > 0.f)
	{
		SetActorRelativeScale3D(FVector(scale, scale, scale));
	}
}
bool AGsActorSummonFairy::IsHighGrade()
{
	return ((int)_fairyGrade >= (int)FairyGrade::RARE) ? true : false;
}

// 등급 이펙트 붙이기
void AGsActorSummonFairy::AttachGradeParticle()
{
	const FGsSchemaSummonFairyGradeRes* findTbl =
		UGsTableUtil::FindRowById<UGsTableSummonFairyGradeRes, FGsSchemaSummonFairyGradeRes>(_fairyGrade);

	if (findTbl == nullptr)
	{
		return;
	}

	const FSoftObjectPath* path = &findTbl->summonFairyGradeEffect;
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
void AGsActorSummonFairy::ChangeAniState(EGsStateSummonFairy In_state)
{
	USkeletalMeshComponent* myBodyMesh = GetPartsSkeletalMeshComponentBody();
	if (myBodyMesh == nullptr)
	{
		return;
	}

	UGsSummonFairyAnimInstance* animInstance =
		Cast<UGsSummonFairyAnimInstance>(myBodyMesh->GetAnimInstance());

	if (animInstance == nullptr)
	{
		return;
	}

	animInstance->ChangeState(In_state);
}

// 스폰 애니 출력
void AGsActorSummonFairy::PlaySpawnAni()
{
	ChangeAniState(EGsStateSummonFairy::Spawn);

	StartSpawnAni();
}

// 스폰 위치 오프셋
FVector AGsActorSummonFairy::GetSpawnPosOffset()
{
	return GData()->GetGlobalData()->_summonFairyOffset;
}