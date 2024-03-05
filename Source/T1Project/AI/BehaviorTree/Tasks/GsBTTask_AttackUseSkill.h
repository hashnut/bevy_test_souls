#pragma once

#include "AIModule/Classes/BehaviorTree/Tasks/BTTask_BlueprintBase.h"

#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"

#include "GsBTTask_AttackUseSkill.generated.h"

class AAIController;
class APawn;

UCLASS()
class T1PROJECT_API UGsBTTask_AttackUseSkill : public UBTTask_BlueprintBase
{
	GENERATED_UCLASS_BODY()


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	// 호출된 executeAI(확장 불가능 한 함수라 윗단 호출에서 처리)
	void CalledReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);
	// 호출된 finish execute(확장 불가능 한 함수라 윗단 호출에서 처리)
	void CalledFinishExecute(bool bSuccess);
};