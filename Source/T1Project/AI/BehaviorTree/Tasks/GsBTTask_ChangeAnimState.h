#pragma once

#include "AIModule/Classes/BehaviorTree/BTTaskNode.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h"

#include "GameObject/Define/GsGameObjectDefine.h"

#include "GsBTTask_ChangeAnimState.generated.h"

/*
* bt 태스크 노드: 이동 및 애니 처리
*/

UCLASS()
class T1PROJECT_API UGsBTTask_ChangeAnimState : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

	// 변경될 상태
	UPROPERTY(Category = ChgangeAnimState, EditAnywhere)
	EGsStateBase changeState;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};