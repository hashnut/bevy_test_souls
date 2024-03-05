#include "GsActorSummonFairyStone.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"


// 스케일 값 접근
float AGsActorSummonFairyStone::GetObjectScaleValue()
{
	return GData()->GetGlobalData()->_summonFairyStoneScale;
}


bool AGsActorSummonFairyStone::IsHighGrade()
{
	return ((int)_stoneGrade >= (int)FairyGrade::RARE) ? true : false;
}

// 등급 파티클 경로 가져오기
const FSoftObjectPath* AGsActorSummonFairyStone::GetGradeParticlePath()
{
	// 레어 이상이면 고등급
	const FSoftObjectPath* path =
		IsHighGrade() ?
		&GData()->GetGlobalData()->_effectHighGradeSummonSpawnFairy :
		&GData()->GetGlobalData()->_effectNormalGradeSummonSpawnFairy;

	return path;
}

FName AGsActorSummonFairyStone::GetMorphTargetName()
{
	return GData()->GetGlobalData()->_summonDragMorphTargetName;
}
// 스폰 위치 오프셋
FVector AGsActorSummonFairyStone::GetSpawnPosOffset()
{
	return GData()->GetGlobalData()->_summonFairyStoneOffset;
}

// 등급 이펙트 오프셋
FVector AGsActorSummonFairyStone::GetGradeEffectOffset()
{
	return GData()->GetGlobalData()->_summonStoneGradeEffectOffset;
}