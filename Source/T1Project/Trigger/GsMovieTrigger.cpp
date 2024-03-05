// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMovieTrigger.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "T1Project/ActorEx/GsCharacterLocalPlayer.h"
#include "T1Project/ActorComponentEx/WidgetComponentEx.h"
#include "T1Project/Editor/Widget/GsEditorWidget.h"
#include "UI/UIContent/Tray/GsUITrayMovie.h"
#include "DataSchema/Movie/GsSchemaMovieData.h"
#include "Components/BoxComponent.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"

#include "Editor/Widget/GsEditorWidget.h"
#include "Kismet/GameplayStatics.h"

#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Components/TextBlock.h"

AGsMovieTrigger::AGsMovieTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	if (_boxCollision)
	{
		_boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGsMovieTrigger::OnPlayMovie);

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

void AGsMovieTrigger::EndPlay(EEndPlayReason::Type inReason)
{
	if (_stopMovieHandle.Value.IsValid())
	{
		GMessage()->GetUI().Remove(_stopMovieHandle);
	}	

	_usedMessageStop = false;

	Super::EndPlay(inReason);
}

void AGsMovieTrigger::OnPlayMovie(UPrimitiveComponent* inOverlappedComponent, AActor* inOtherActor, UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex, bool inIsFromSweep, const FHitResult& inSweepResult)
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

	const UGsTableManager& mgr = UGsTableManager::GetInstance();

	const UGsTable* table = mgr.GetTable(FGsSchemaMovieData::StaticStruct());

	if (nullptr == table)
	{
		return;
	}

	const FGsSchemaMovieData* movie = nullptr;

	if (false == table->FindRow(FName(*_movieName), movie))
	{
		GSLOG(Error, TEXT("Can't' find movie : %s"), *_movieName);

		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayMovie"));
	if (widget.IsValid())
	{
		if (UGsUITrayMovie * movieWidget = Cast<UGsUITrayMovie>(widget.Get()))
		{	
			if (movieWidget->Play(_movieName, true))
			{
				_usedMessageStop = false;

				ActiveAllMovieTrigger(GetWorld(), false, _movieName);
				_playMovieEvent.Broadcast();

				if (_stopMovieHandle.Value.IsValid())
				{
					GMessage()->GetUI().Remove(_stopMovieHandle);
				}				
				_stopMovieHandle = GMessage()->GetUI().AddUObject(MessageUI::STOP_MOVIE, this, &AGsMovieTrigger::OnStopMovie);
			}
		}
	}
}

void AGsMovieTrigger::OnStopMovie(const IGsMessageParam*)
{
	if (_usedMessageStop) return;

	_stopMovieEvent.Broadcast();

	if (_isDeactiveSameMovie) 
	{		
		_usedMessageStop = true;
		//if (_stopMovieHandle.Value.IsValid())
		//{
		//	GMessage()->GetUI().Remove(_stopMovieHandle); //MessageUI::STOP_MOVIE Broadcasting 완료 전, 삭제를 하면 다음 추가된 delegate 호출 시, SIGBUS Crash 가능  
		//}
	}
}

void AGsMovieTrigger::ActiveAllMovieTrigger(UWorld* inWorld, bool inTrigger, FString inMovieName)
{
	if (nullptr == inWorld)
	{
		return;
	}

	TArray<AActor*> movieTriggers;

	UGameplayStatics::GetAllActorsOfClass(inWorld, AGsMovieTrigger::StaticClass(), movieTriggers);

	for (AActor* actor : movieTriggers)
	{
		if (nullptr == actor)
		{
			continue;
		}			
			
		if (AGsMovieTrigger* movie = Cast<AGsMovieTrigger>(actor))
		{
			if (inMovieName != "")
			{
				if (false == movie->_movieName.Equals(inMovieName))
				{
					continue;
				}
			}

			if (inTrigger)
			{
				movie->_boxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
			else
			{
				movie->_boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

#if WITH_EDITOR
void AGsMovieTrigger::RefreshWidgetText()
{
	if (EditorWidgetComp)
	{
		if (UUserWidget* widget = EditorWidgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget* editorWidget = Cast<UGsEditorWidget>(widget))
			{
				FString log = "Movie : " + _movieName;
				editorWidget->DescriptionText->SetText(FText::FromString(log));
			}
		}
	}
}

void AGsMovieTrigger::SetVisible(bool inVisible)
{
	if (_boxCollision)
	{
		_boxCollision->SetVisibility(inVisible);
		EditorWidgetComp->SetVisibility(inVisible);
	}
}

#endif
