// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trigger/BaseTriggerActor.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "QuestTriggerActor.generated.h"

class UGsEditorWidget;
class UWidgetComponentEx;
class UCapsuleComponent;

UCLASS(BlueprintType)
class T1PROJECT_API AQuestTriggerActor : public ABaseTriggerActor
{
	GENERATED_BODY()

public:
	//Collision
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	UCapsuleComponent* CapsulCollision;

	// FIX: PT버전에서 필요했던 데이터. 필요없을 시 제거
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	int32 QuestID;
	
	// FIX: PT버전에서 필요했던 데이터. 필요없을 시 제거
	// 액터 타입. (목표좌표용, NPC스폰용)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	EGsQuestActorType QuestActorType = EGsQuestActorType::QUEST_ACTOR_GOAL_POS;
	
	// FIX: PT버전에서 필요했던 데이터. 필요없을 시 제거
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	int32 DataIndex = 0;

	// FIX: PT버전에서 필요했던 데이터. 필요없을 시 제거
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	int32 LevelKey;	

public:
	// Sets default values for this actor's properties
	AQuestTriggerActor();

#if WITH_EDITOR
protected:
	virtual void RefreshWidgetText() override;
#endif

public:
	UFUNCTION()
	void OnQuestStart(
		UPrimitiveComponent* in_overlappedComponent,
		AActor* in_otherActor,
		UPrimitiveComponent* in_otherComp,
		int32 in_otherBodyIndex,
		bool in_bFromSweep,
		const FHitResult& in_SweepResult
	);
};
