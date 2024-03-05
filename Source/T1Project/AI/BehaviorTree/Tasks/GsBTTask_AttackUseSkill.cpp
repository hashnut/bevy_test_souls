#include "GsBTTask_AttackUseSkill.h"

#include "ActorComponentEx/GsPlayableEventNonPlayerComponent.h"

#include "AIModule/Classes/AIController.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h"

#include "Engine/Classes/GameFramework/Pawn.h"

UGsBTTask_AttackUseSkill::UGsBTTask_AttackUseSkill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// tick 사용
	bNotifyTick = 1;

	bNotifyTaskFinished = true;
	bShowPropertyDetails = true;
	bIsAborting = false;

	bCreateNodeInstance = true;
}

EBTNodeResult::Type UGsBTTask_AttackUseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// fail when task doesn't react to execution (start or tick)
	CurrentCallResult = EBTNodeResult::InProgress;
	bIsAborting = false;

	
	bStoreFinishResult = true;

	CalledReceiveExecuteAI(AIOwner, AIOwner->GetPawn());
	
	bStoreFinishResult = false;
	

	return CurrentCallResult;
}

// 호출된 executeAI(확장 불가능 한 함수라 윗단 호출에서 처리)
void UGsBTTask_AttackUseSkill::CalledReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
	// 공격 처리
	if (nullptr == OwnerController)
	{
		return;
	}
	APawn* pawn = OwnerController->GetPawn();

	if (nullptr == pawn)
	{
		return;
	}
	UGsPlayableEventNonPlayerComponent* playableEventNonPlayerComponent = pawn->FindComponentByClass<UGsPlayableEventNonPlayerComponent>();
	if (nullptr == playableEventNonPlayerComponent)
	{
		return;
	}
	playableEventNonPlayerComponent->UseSkill([this](bool In_isSuccess) {
	
		CalledFinishExecute(In_isSuccess);
		});


}

// 호출된 finish execute(확장 불가능 한 함수라 윗단 호출에서 처리)
void UGsBTTask_AttackUseSkill::CalledFinishExecute(bool bSuccess)
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	EBTNodeResult::Type NodeResult(bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);

	if (bStoreFinishResult)
	{
		CurrentCallResult = NodeResult;
	}
	else if (OwnerComp && !bIsAborting)
	{
		FinishLatentTask(*OwnerComp, NodeResult);
	}
}