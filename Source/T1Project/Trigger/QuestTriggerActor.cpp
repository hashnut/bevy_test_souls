// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTriggerActor.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "T1Project/ActorComponentEx/WidgetComponentEx.h"
#include "T1Project/Editor/Widget/GsEditorWidget.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsQuestMessageParam.h"


// Sets default values
AQuestTriggerActor::AQuestTriggerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsulCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));

	if (CapsulCollision)
	{
		CapsulCollision->OnComponentBeginOverlap.AddDynamic(this, &AQuestTriggerActor::OnQuestStart);

		CapsulCollision->SetCollisionProfileName(TRIGGER_COLLISION_PROFILE);
		CapsulCollision->SetCanEverAffectNavigation(false);
		CapsulCollision->AreaClass = nullptr;
		CapsulCollision->bApplyImpulseOnDamage = false;

#if WITH_EDITOR
		CapsulCollision->SetVisibility(true);
		CapsulCollision->bHiddenInGame = false;
#else
		CapsulCollision->SetVisibility(false);
		CapsulCollision->bHiddenInGame = true;
#endif
	}
}

void AQuestTriggerActor::OnQuestStart(UPrimitiveComponent* in_overlappedComponent,
	AActor* in_otherActor,
	UPrimitiveComponent* in_otherComp,
	int32 in_otherBodyIndex,
	bool in_bFromSweep,
	const FHitResult& in_SweepResult)
{
	if (this == in_otherActor)
	{
		return;
	}

	if (CheckOnlyPlayer)
	{
		if (false == IsPlayer(in_otherActor))
		{
			return;
		}
	}

	if (EGsQuestActorType::QUEST_ACTOR_GOAL_POS == QuestActorType)
	{
		FGsQuestParam param(QuestID);
		/*GMessage()->GetQuest().SendMessage(MessageContentQuest::EnterQuestActorCollision, &param);*/

#if WITH_EDITOR
		FString log = "Player get quest id : " + FString::FromInt(QuestID);
		GSLOG(Log, TEXT("%s"), *log);
#endif

	}
}

#if WITH_EDITOR
void AQuestTriggerActor::RefreshWidgetText()
{
	if (EditorWidgetComp)
	{
		if (UUserWidget * widget = EditorWidgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget * editorWidget = Cast<UGsEditorWidget>(widget))
			{
				if (editorWidget->DescriptionText)
				{
					FString log = TEXT("QuestID: ") + FString::FromInt(QuestID);

					if (EGsQuestActorType::QUEST_ACTOR_NPC_SPAWN_POS == QuestActorType)
					{
						log.Append(*FString::Format(TEXT("\nIndex: {0}"), { DataIndex }));
					}

					editorWidget->DescriptionText->SetText(FText::FromString(log));
				}
			}
		}
	}
}
#endif