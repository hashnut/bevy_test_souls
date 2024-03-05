#include "GsActorSummonVeilCostume.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"


// 스케일 값 접근
float AGsActorSummonVeilCostume::GetObjectScaleValue()
{
	return GData()->GetGlobalData()->_summonVeilCostumeScale;
}


bool AGsActorSummonVeilCostume::IsHighGrade()
{
	return ((int)_grade >= (int)CostumeGrade::RARE) ? true : false;
}

// 등급 파티클 경로 가져오기
const FSoftObjectPath* AGsActorSummonVeilCostume::GetGradeParticlePath()
{
	// 레어 이상이면 고등급
	const FSoftObjectPath* path =
		IsHighGrade() ?
		&GData()->GetGlobalData()->_effectHighGradeSummonSpawnCostume :
		&GData()->GetGlobalData()->_effectNormalGradeSummonSpawnCostume;

	return path;
}

FName AGsActorSummonVeilCostume::GetMorphTargetName()
{
	return GData()->GetGlobalData()->_summonDragMorphTargetNameCostume;
}

// 스폰 위치 오프셋
FVector AGsActorSummonVeilCostume::GetSpawnPosOffset()
{
	return GData()->GetGlobalData()->_summonVeilCostumeOffset;
}

// 등급 이펙트 오프셋
FVector AGsActorSummonVeilCostume::GetGradeEffectOffset()
{
	return GData()->GetGlobalData()->_summonCubeGradeEffectOffset;
}