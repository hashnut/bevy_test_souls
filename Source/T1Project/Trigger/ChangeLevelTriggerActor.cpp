// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeLevelTriggerActor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "T1Project/GSGameInstance.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "T1Project/Level/GsLevelDefine.h"
#include "T1Project/Editor/Widget/GsEditorWidget.h"
#include "T1Project/ActorEx/GsCharacterLocalPlayer.h"
#include "T1Project/ActorComponentEx/WidgetComponentEx.h"
#include "T1Project/GameObject/ObjectClass/GsGameObjectPlayer.h"

// Sets default values
AChangeLevelTriggerActor::AChangeLevelTriggerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AChangeLevelTriggerActor::OnChangeLevel);

		BoxCollision->SetCollisionProfileName(TRIGGER_COLLISION_PROFILE);
		BoxCollision->SetCanEverAffectNavigation(false);
		BoxCollision->AreaClass = nullptr;
		BoxCollision->bApplyImpulseOnDamage = false;

#if WITH_EDITOR
		BoxCollision->SetVisibility(true);
		BoxCollision->bHiddenInGame = false;
#else
		BoxCollision->SetVisibility(false);
		BoxCollision->bHiddenInGame = true;
#endif

	}
}

void AChangeLevelTriggerActor::OnChangeLevel(UPrimitiveComponent* in_overlappedComponent,
	AActor* in_otherActor,
	UPrimitiveComponent* in_otherComp,
	int32 in_otherBodyIndex,
	bool in_bFromSweep,
	const FHitResult& in_SweepResult)
{
	if (in_otherActor)
	{
		if (in_otherActor->IsA(AGsCharacterLocalPlayer::StaticClass()))
		{
			
			if (UGsLevelManager* level = GLevel())
			{
				if (false == level->LoadLevelById(GetWorld(), LevelKey))
				{
					GSLOG(Error, TEXT("Invaild Level ID[%d] Access!"), LevelKey);
				}
			}
		}
	}
}

#if WITH_EDITOR
void AChangeLevelTriggerActor::RefreshWidgetText()
{
	if (EditorWidgetComp)
	{
		if (UUserWidget* widget = EditorWidgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget* editorWidget = Cast<UGsEditorWidget>(widget))
			{
				if (UGsLevelManager* level = GLevel())
				{
					FString levelName;

					if (level->TryGetLevelName(levelName, LevelKey))
					{
						if (editorWidget->DescriptionText)
						{
							editorWidget->DescriptionText->SetText(FText::FromString(levelName));
						}
					}
				}
			}
		}
	}
}
#endif