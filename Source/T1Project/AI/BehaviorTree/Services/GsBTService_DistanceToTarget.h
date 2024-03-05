#pragma once

#include "AIModule/Classes/BehaviorTree/Services/BTService_BlueprintBase.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h"
#include "GsBTService_DistanceToTarget.generated.h"

class AAIController;
class APawn;

/*
* bt 서비스 노드: 타겟과 거리 계산 처리
*/

UCLASS()
class T1PROJECT_API UGsBTService_DistanceToTarget : public UBTService_BlueprintBase
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector myBlackboardKey;

	// 가상 함수
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	
public:
	// 호출된 tick ai(확장 불가능 한 함수라 윗단 호출에서 처리)
	void CalledReceiveTickAI(UBehaviorTreeComponent& OwnerComp, AAIController* OwnerController, APawn* ControlledPawn, float DeltaSeconds);
	// 호출된 start ai(확장 불가능 한 함수라 윗단 호출에서 처리)
	void CalledReceiveSearchStartAI(UBehaviorTreeComponent& OwnerComp, AAIController* OwnerController, APawn* ControlledPawn);
	// 타겟과의 거리 갱신(나: 몬스터, 타겟: 로컬)
	void CalcDistanceToTarget(UBehaviorTreeComponent& OwnerComp, APawn* ControlledPawn);
};