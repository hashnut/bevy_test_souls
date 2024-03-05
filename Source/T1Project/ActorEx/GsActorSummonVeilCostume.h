#pragma once
#include "CoreMinimal.h"

#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

#include "ActorEx/GsActorSummonVeilObject.h"

#include "GsActorSummonVeilCostume.generated.h"
/*
	뽑기전 코스튬 bp
*/

UCLASS()
class T1PROJECT_API AGsActorSummonVeilCostume : public AGsActorSummonVeilObject
{
	GENERATED_BODY()

	// 멤버 변수
private:
	// 등급
	CostumeGrade _grade = CostumeGrade::NONE;
	// 가상 함수
protected:
	// 스케일 값 접근
	virtual float GetObjectScaleValue() override;
	// 등급 파티클 경로 가져오기
	virtual const FSoftObjectPath* GetGradeParticlePath() override;

	// 가상 함수(public)
public:
	virtual bool IsHighGrade() override;
	virtual FName GetMorphTargetName() override;
	// 스폰 위치 오프셋
	virtual FVector GetSpawnPosOffset() override;

	// 등급 이펙트 오프셋
	virtual FVector GetGradeEffectOffset() override;
	// get, set
public:
	void SetGrade(CostumeGrade In_grade)
	{
		_grade = In_grade;
	}
	CostumeGrade GetGrade()
	{
		return _grade;
	}
};