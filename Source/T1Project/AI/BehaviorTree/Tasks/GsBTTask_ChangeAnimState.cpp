#include "GsBTTask_ChangeAnimState.h"

#include "ActorEx/GsCharacterBase.h"

#include "Animation/GsAnimInstanceState.h"

#include "AIModule/Classes/AIController.h"

#include "Engine/Classes/GameFramework/Pawn.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"

UGsBTTask_ChangeAnimState::UGsBTTask_ChangeAnimState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

EBTNodeResult::Type UGsBTTask_ChangeAnimState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type nodeResult = EBTNodeResult::Succeeded;

	AAIController* MyController = OwnerComp.GetAIOwner();

	if (MyController == nullptr)
	{
		return nodeResult;
	}
	APawn* pawn = MyController->GetPawn();

	if (nullptr == pawn)
	{
		return nodeResult;
	}

	AGsCharacterBase* charBase = Cast<AGsCharacterBase>(pawn);

	if (nullptr == charBase)
	{
		return nodeResult;
	}
	USkeletalMeshComponent* meshPtr = charBase->GetMesh();

	if (nullptr == meshPtr)
	{
		return nodeResult;
	}

	UGsAnimInstanceState* anim = Cast<UGsAnimInstanceState>(meshPtr->GetAnimInstance());

	if (anim == nullptr)
	{
		return nodeResult;
	}

	anim->ChangeState((int)(changeState), 0);

	return nodeResult;
}