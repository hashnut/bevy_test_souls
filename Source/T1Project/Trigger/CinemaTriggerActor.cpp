// Fill out your copyright notice in the Description page of Project Settings.


#include "CinemaTriggerActor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"

#include "T1Project/ActorEx/GsCharacterLocalPlayer.h"
#include "T1Project/ActorComponentEx/WidgetComponentEx.h"
#include "T1Project/GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "T1Project/GSGameInstance.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "T1Project/Level/GsLevelDefine.h"
#include "T1Project/Editor/Widget/GsEditorWidget.h"
#include "T1Project/Sequence/GsSequenceManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"

#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "T1Project.h"

#include "DataSchema/GsSchemaEnums.h"


bool ACinemaTriggerActor::IsShowDebugLog = false;

// Sets default values
ACinemaTriggerActor::ACinemaTriggerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACinemaTriggerActor::OnPlayCinema);

		if (true == IsShowDebugLog)
		{
			GSLOG(Log, TEXT("Add Function OnPlayCinema"));
		}

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

void ACinemaTriggerActor::OnPlayCinema(UPrimitiveComponent* in_overlappedComponent,
	AActor* in_otherActor,
	UPrimitiveComponent* in_otherComp,
	int32 in_otherBodyIndex,
	bool in_bFromSweep,
	const FHitResult& in_SweepResult)
{
	if (true == IsShowDebugLog)
	{
		GSLOG(Log, TEXT("OnPlayCinema"));
	}

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

	if (false == LevelSequenceID.IsEmpty())
	{
		if (true == IsShowDebugLog)
		{
			GSLOG(Log, TEXT("PlaySequence"));
		}

		// 시네마틱 플레이를 GMessage()로 변경/ 트리거에서 계속 시네마틱을 이용 할 경우 변경
		// 예전 방식 주석
		
		//// 강제로 카메라 보스뷰로 바꾸기
		//GSequence()->GetSequencePlayer()->PlaySequence(LevelSequenceID,
		//	// 임시코드
		//	// 연출 끝에서 보스 fsm idle로 바꾸기
		//	[this] {
		//		UGsGameObjectBase* findMonster =
		//			GSGameObject()->FindObjectNonPlayerByID(16000);

		//		if (nullptr != findMonster)
		//		{
		//			UGsGameObjectNonPlayer* castNpc = Cast<UGsGameObjectNonPlayer>(findMonster);
		//			check(castNpc);

		//			castNpc->GetBaseFSM()->ProcessEvent(EGsStateBase::Idle);
		//		}

		//	});

		if (BoxCollision)
		{
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

#if WITH_EDITOR
void ACinemaTriggerActor::RefreshWidgetText()
{
	if (EditorWidgetComp)
	{
		if (UUserWidget * widget = EditorWidgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget * editorWidget = Cast<UGsEditorWidget>(widget))
			{
				if (UGsGameInstance * instance = Cast<UGsGameInstance>(GetWorld()->GetGameInstance()))
				{
					if (false == LevelSequenceID.IsEmpty())
					{
						FString display = TEXT("Sequence : ") + LevelSequenceID;
						editorWidget->DescriptionText->SetText(FText::FromString(display));
					}
				}
			}
		}
	}
}
#endif