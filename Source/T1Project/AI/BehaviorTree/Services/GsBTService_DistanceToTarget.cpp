#include "GsBTService_DistanceToTarget.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"

#include "Engine/Classes/GameFramework/Pawn.h"
#include "AIModule/Classes/AIController.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BTFunctionLibrary.h"


UGsBTService_DistanceToTarget::UGsBTService_DistanceToTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// tick 사용
	bNotifyTick = 1;
	// search start 사용
	bNotifyOnSearch = 1;
}

void UGsBTService_DistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AIOwner != nullptr)
	{
		CalledReceiveTickAI(OwnerComp,AIOwner, AIOwner->GetPawn(), DeltaSeconds);
	}

}
void UGsBTService_DistanceToTarget::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	if (AIOwner != nullptr)
	{
		UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
		CalledReceiveSearchStartAI(*OwnerComp, AIOwner, AIOwner->GetPawn());
	}
	
}
// 호출된 tick ai
void UGsBTService_DistanceToTarget::CalledReceiveTickAI(UBehaviorTreeComponent& OwnerComp, AAIController* OwnerController, APawn* ControlledPawn, float DeltaSeconds)
{
	CalcDistanceToTarget(OwnerComp, ControlledPawn);
}

// 호출된 start ai(확장 불가능 한 함수라 윗단 호출에서 처리)
void UGsBTService_DistanceToTarget::CalledReceiveSearchStartAI(UBehaviorTreeComponent& OwnerComp, AAIController* OwnerController, APawn* ControlledPawn)
{
	CalcDistanceToTarget(OwnerComp, ControlledPawn);
}

// 타겟과의 거리 갱신(나: 몬스터, 타겟: 로컬)
void UGsBTService_DistanceToTarget::CalcDistanceToTarget(UBehaviorTreeComponent& OwnerComp, APawn* ControlledPawn)
{
	if (nullptr == ControlledPawn)
	{
		return;
	}

	UGsGameObjectPlayableEventLocalPlayer* targetObj =
		LSLobbyCharacter()->GetPlayableEventLocalPlayer();

	if (nullptr == targetObj)
	{
		return;
	}

	float dist = ControlledPawn->GetDistanceTo(targetObj->GetActor());

	UBlackboardComponent* myBlackboard = OwnerComp.GetBlackboardComponent();

	if (myBlackboard == nullptr)
	{
		return;
	}

	float oldBBDist = myBlackboard->GetValueAsFloat(myBlackboardKey.SelectedKeyName);
	// 기존과 다르면 세팅
	if (oldBBDist != dist)
	{
		myBlackboard->SetValueAsFloat(myBlackboardKey.SelectedKeyName, dist);
	}
}