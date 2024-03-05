#pragma once
#include "CoreMinimal.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "ActorEx/GsActorSummonUnveilObject.h"

#include "Animation/GsSummonFairyAnimInstance.h"

#include "GsActorSummonFairy.generated.h"

/*
	소환된 페어리 bp
*/

UCLASS()
class T1PROJECT_API AGsActorSummonFairy : public AGsActorSummonUnveilObject
{
	GENERATED_BODY()


private:
	// 등급
	FairyGrade _fairyGrade = FairyGrade::NONE;
	
	// 가상 함수
public:
	// 스케일 초기화
	virtual void InitScale() override;

	virtual bool IsHighGrade() override;
	// 등급 이펙트 붙이기
	virtual void AttachGradeParticle() override;
	// 스폰 애니 출력
	virtual void PlaySpawnAni() override;
	// 스폰 위치 오프셋
	virtual FVector GetSpawnPosOffset() override;
	
	// bp call function
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "SummonActor")
	void StartSpawnAni();

	// 로직 함수
public:

	// ani 상태 변경
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "SummonActor")
	void ChangeAniState(EGsStateSummonFairy In_state);

	// get, set
public:
	void SetGrade(FairyGrade In_grade)
	{
		_fairyGrade = In_grade;
	}
	FairyGrade GetGrade()
	{
		return _fairyGrade;
	}
};