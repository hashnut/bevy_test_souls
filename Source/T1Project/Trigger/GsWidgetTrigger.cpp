// Fill out your copyright notice in the Description page of Project Settings.


#include "GsWidgetTrigger.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Kismet/GameplayStatics.h"
#include "T1Project/ActorComponentEx/WidgetComponentEx.h"
#include "Components/TextBlock.h"
#include "Editor/Widget/GsEditorWidget.h"

AGsWidgetTrigger::AGsWidgetTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	if (_boxCollision)
	{
		_boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGsWidgetTrigger::OnShowWidget);

		_boxCollision->SetCollisionProfileName(TRIGGER_COLLISION_PROFILE);
		_boxCollision->SetCanEverAffectNavigation(false);
		_boxCollision->AreaClass = nullptr;
		_boxCollision->bApplyImpulseOnDamage = false;

#if WITH_EDITOR
		_boxCollision->SetVisibility(true);
		_boxCollision->bHiddenInGame = false;
#else
		_boxCollision->SetVisibility(false);
		_boxCollision->bHiddenInGame = true;
#endif

		RootComponent = _boxCollision;
	}
}

void AGsWidgetTrigger::OnShowWidget(UPrimitiveComponent* inOverlappedComponent, AActor* inOtherActor, UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex, bool inIsFromSweep, const FHitResult& inSweepResult)
{
	if (nullptr == inOverlappedComponent)
	{
		return;
	}

	if (false == IsPlayer(inOtherActor))
	{
		return;
	}

	UGsUIManager* uiManager = GUI();

	if (nullptr == uiManager)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(*_uiName);
	if (widget.IsValid())
	{
		ActiveAllWidgetTrigger(GetWorld(), false, _uiName);

		/*if (UGsUITrayMovie * movieWidget = Cast<UGsUIWidgetBase>(widget))
		{
			if (movieWidget->Play(_movieName, false))
			{
				_playMovieEvent.Broadcast();

				if (_stopMovieHandle.Value.IsValid())
				{
					GMessage()->GetUIAction().Remove(_stopMovieHandle);
				}
				_stopMovieHandle = GMessage()->GetUIAction().AddUObject(MessageUI::Action::StopMovie, this, &AGsMovieTrigger::OnStopMovie);
			}
		}*/
	}

}


void AGsWidgetTrigger::ActiveAllWidgetTrigger(UWorld* inWorld, bool inTrigger /*= false*/, FString inWidgetName /*= ""*/)
{
	if (nullptr == inWorld)
	{
		return;
	}

	TArray<AActor*> movieTriggers;

	UGameplayStatics::GetAllActorsOfClass(inWorld, AGsWidgetTrigger::StaticClass(), movieTriggers);

	for (AActor* actor : movieTriggers)
	{
		if (nullptr == actor)
		{
			continue;
		}

		if (AGsWidgetTrigger* widget = Cast<AGsWidgetTrigger>(actor))
		{
			if (inWidgetName != "")
			{
				if (false == widget->_uiName.Equals(inWidgetName))
				{
					continue;
				}
			}

			if (inTrigger)
			{
				widget->_boxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
			else
			{
				widget->_boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

#if WITH_EDITOR
void AGsWidgetTrigger::RefreshWidgetText()
{
	if (EditorWidgetComp)
	{
		if (UUserWidget* widget = EditorWidgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget* editorWidget = Cast<UGsEditorWidget>(widget))
			{
				FString log = "Widget : " + _uiName;
				editorWidget->DescriptionText->SetText(FText::FromString(log));
			}
		}
	}
}

void AGsWidgetTrigger::SetVisible(bool inVisible)
{
	if (_boxCollision)
	{
		_boxCollision->SetVisibility(inVisible);
		EditorWidgetComp->SetVisibility(inVisible);
	}
}

#endif