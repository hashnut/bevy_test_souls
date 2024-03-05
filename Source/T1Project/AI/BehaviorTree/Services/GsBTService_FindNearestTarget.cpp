#include "GsBTService_FindNearestTarget.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"

#include "Engine/Classes/GameFramework/Pawn.h"
#include "AIModule/Classes/AIController.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BTFunctionLibrary.h"


UGsBTService_FindNearestTarget::UGsBTService_FindNearestTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// tick 사용
	bNotifyTick = 1;
}

void UGsBTService_FindNearestTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AIOwner != nullptr)
	{
		CalledReceiveTickAI(OwnerComp,AIOwner, AIOwner->GetPawn(), DeltaSeconds);
	}

}

// 호출된 tick ai
void UGsBTService_FindNearestTarget::CalledReceiveTickAI(UBehaviorTreeComponent& OwnerComp, AAIController* OwnerController, APawn* ControlledPawn, float DeltaSeconds)
{
	UGsGameObjectPlayableEventLocalPlayer* targetObj = LSLobbyCharacter()->GetPlayableEventLocalPlayer();

	if (targetObj == nullptr)
	{
		return;
	}
	UBlackboardComponent* myBlackboard = OwnerComp.GetBlackboardComponent();

	if (myBlackboard == nullptr)
	{
		return;
	}


	UObject* bbObject = myBlackboard->GetValueAsObject(myBlackboardKey.SelectedKeyName);

	// 비어있으면 넣는다
	if (bbObject == nullptr)
	{
		myBlackboard->SetValueAsObject(myBlackboardKey.SelectedKeyName, targetObj->GetActor());
	}

}

