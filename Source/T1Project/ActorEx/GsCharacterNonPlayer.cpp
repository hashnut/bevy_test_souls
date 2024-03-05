// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCharacterNonPlayer.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/Classes/Materials/MaterialInterface.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"

#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"

#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "ActorComponentEx/GsSkillCollisioHandlerComponent.h"
#include "ActorComponentEx/GsSkillGuideEffectComponent.h"
#include "ActorComponentEx/GsWidgetComponentButton.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnElement.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"

#include "T1Project/Sequence/GsSequencePlayer.h"

#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "Map/Minimap/GsMinimapFunc.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"
#include "UI/UIContent/Billboard/GsUIBillboardTalkBalloon.h"
#include "UI/UIContent/Billboard/GsUIBillboardNpcDebug.h"
#include "UI/UIContent/Billboard/GsUIBillboardDebugText.h"

#include "ActorComponentEx/GsUROComponent.h"
#include "ActorComponentEx/GsWidgetComponentTalkBalloon.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

#include "Cheat/GsCheatManager.h"
#include "UTIL/GsLevelUtil.h"


//const FName NPC_DEBUG_WIDGET = TEXT("NpcDebugWidget");
const FName DebugTextInfoComponentName = TEXT("DebugTextInfoWidget");

// Sets default values
AGsCharacterNonPlayer::AGsCharacterNonPlayer() : Super()
{
	USkeletalMeshComponent* skelMeshComp = GetMesh();
	if (skelMeshComp)
	{
		// 인터랙션 버튼
		_interactionWidget = CreateDefaultSubobject<UGsWidgetComponentButton>(InteractionButtonComponentName);
		_interactionWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));		
		_interactionWidget->SetWidgetSpace(EWidgetSpace::Screen);
		_interactionWidget->SetDrawSize(FVector2D(100, 100.0f));
		_interactionWidget->SetCastShadow(false);
		_interactionWidget->SetupAttachment(skelMeshComp);
		_interactionWidget->SetGenerateOverlapEvents(false);

		// 블락 처리(대화 연출 최소 간격 확보용)
#if 0
		_interactionBlockCollision = CreateDefaultSubobject<UCapsuleComponent>(NPCInteractionBlockCapsuleComponentName);
		_interactionBlockCollision->InitCapsuleSize(100.0f, 100.0f);
		_interactionBlockCollision->SetCollisionProfileName(NPCInteractionBlockPresetName);
		_interactionBlockCollision->SetGenerateOverlapEvents(false);

		_interactionBlockCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		_interactionBlockCollision->SetShouldUpdatePhysicsVolume(true);
		_interactionBlockCollision->SetCanEverAffectNavigation(false);
		_interactionBlockCollision->bDynamicObstacle = true;
		
		_interactionBlockCollision->SetRelativeLocation(FVector::ZeroVector);

		if (UCapsuleComponent* capsule = GetCapsuleComponent())
		{
			_interactionBlockCollision->SetupAttachment(capsule);			
		}
#endif

		//debug widget		
		/*_npcDebugWidget = CreateDefaultSubobject<UWidgetComponent>(NPC_DEBUG_WIDGET);
		_npcDebugWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 400));
		_npcDebugWidget->SetWidgetSpace(EWidgetSpace::Screen);
		_npcDebugWidget->SetDrawSize(FVector2D(400, 100.0f));
		_npcDebugWidget->SetCastShadow(false);
		_npcDebugWidget->SetupAttachment(skelMeshComp);
		ShowNpcDebugWidget(false);*/
	}
}

// Called when the game starts or when spawned
void AGsCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();		

	SetQuestMarkLocation();

	InitSkeletalMeshURO();

}
// 인터랙션 버튼 쓸지
void AGsCharacterNonPlayer::SetUseInteractionButton(bool In_isUse)
{
	// 인터랙션 버튼
	if (_interactionWidget)
	{
		if (In_isUse == true)
		{
			if (nullptr == _interactionWidget->GetUserWidgetObject())
			{
				_interactionWidget->InitWidget();				
			}
		}
	}	
}

void AGsCharacterNonPlayer::ShowInteractionButton(bool inShow)
{
	if (_interactionWidget)
	{
		_interactionWidget->SetVisibility(inShow);
	}
}

void AGsCharacterNonPlayer::InitWidgetComponent()
{
	Super::InitWidgetComponent();

	FTransform trans = GetMesh()->GetSocketTransform(TEXT("OverHeadSocket"), RTS_Component);

	// 인터랙션 버튼
	if (_interactionWidget)
	{
		if (nullptr == _interactionWidget->GetWidgetClass().Get())
		{
			if (UGsPathManager* pathMgr = GPath())
			{
				UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("InteractionButton"));
				if (widgetClass)
				{
					_interactionWidget->SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
				}
			}
		}
		_interactionWidget->SetRelativeLocation(trans.GetTranslation());		
	}

	/*if (_npcDebugWidget)
	{
		UClass* widgetClass = GPath()->GetClass(EGsPathType::UI, TEXT("BillboardNpcDebug"));
		if (widgetClass)
		{
			_npcDebugWidget->SetWidgetClass(widgetClass);
		}
	}*/
}

void AGsCharacterNonPlayer::OnHit(HitResultType ResultType, EGsStateBase CurrentState, 
	const FGsSchemaCreatureCommonInfo* CommonData)
{
	Super::OnHit(ResultType, CurrentState, CommonData);

	if (UGsUIBillboardTalkBalloon* talkBalloon = Cast<UGsUIBillboardTalkBalloon>(_talkBalloonWidget->GetUserWidgetObject()))
	{
		talkBalloon->OnHit();
	}
}

UGsUIBillboardInteractionButton* AGsCharacterNonPlayer::GetInteractionButton() const
{
	if (_interactionWidget)
	{
		return Cast<UGsUIBillboardInteractionButton>(_interactionWidget->GetUserWidgetObject());
	}
	return nullptr;
}

//void AGsCharacterNonPlayer::InitNpcDebugWidget(FGsGameObjectData* inGameData, float inSightRadius)
//{
//	UGsUIBillboardNpcDebug* widget = Cast<UGsUIBillboardNpcDebug>(_npcDebugWidget->GetWidget());
//	if(nullptr == widget)
//	{
//		return;
//	}
//
//	int32 levelId = GLevel()->GetCurrentLevelId();
//	int64 uniqueId = (uint32)inGameData->GetSpawnId();
//	FGsSchemaSpawnData spawnData;
//	FGsSchemaSpawnElement spawnElement;
//	FString npcInfo;
//	if (false == UGsLevelUtil::TryFindSpawnInfo(levelId, uniqueId, spawnData, spawnElement))
//	{
//		npcInfo = TEXT("Can't find npc");
//	}
//	else
//	{
//		int64 gameId = inGameData->GetGameId();
//		float sightRadius = inSightRadius;
//		int32 gorupId = spawnData.SpawnGroupId;
//		int32 tableId = levelId;
//		int32 respawnInterval = spawnElement.RespawnInterval;
//		//find group id, find respawn interval		
//
//		npcInfo = TEXT("GoupdID : ") + FString::FromInt(gorupId) + TEXT("\n")
//			+ TEXT("TableID : ") + FString::FromInt(tableId) + TEXT("\n")
//			+ TEXT("UniqueID : ") + FString::FromInt(uniqueId) + TEXT("\n")
//			+ TEXT("SightRange : ") + FString::FromInt(sightRadius) + TEXT("\n")
//			+ TEXT("RespawnInterval : ") + FString::FromInt(respawnInterval);
//	}	
//
//	widget->SetNpcInfoText(npcInfo);
//}
//
//void AGsCharacterNonPlayer::ShowNpcDebugWidget(bool inShow)
//{
//	//set npc info
//	if (inShow)
//	{
//		if (false == _npcDebugWidget->IsVisible())
//		{
//			_npcDebugWidget->SetVisibility(true);
//		}
//	}
//	else
//	{
//		if (_npcDebugWidget->IsVisible())
//		{
//			_npcDebugWidget->SetVisibility(false);
//		}
//	}	
//}

void AGsCharacterNonPlayer::OnChangeRenderState(bool bIsRendered)
{
	Super::OnChangeRenderState(bIsRendered);

	if (UGsUIBillboardInteractionButton* interaction = GetInteractionButton())
	{
		interaction->OnChangeRenderState(bIsRendered);
	}

	if (UGsUIBillboardTalkBalloon* talkBalloon = GetTalkBalloon())
	{
		talkBalloon->OnChangeRenderState(bIsRendered);
	}
}

// 인터랙션 클릭 콜백 세팅
void AGsCharacterNonPlayer::SetInteractionClickCallback(TFunction<void(bool)> In_callback)
{
	Super::SetInteractionClickCallback(In_callback);

	if (_interactionWidget != nullptr)
	{
		if (UGsWidgetComponentButton* wcButton = Cast<UGsWidgetComponentButton>(_interactionWidget))
		{
			wcButton->SetClickCallback(In_callback);
		}
	}
}

void AGsCharacterNonPlayer::SetQuestMarkLocation()
{
	// 퀘스트 타겟 마크
	if (_questTargetMark)
	{
		FTransform trans = GetMesh()->GetSocketTransform(TEXT("OverHeadSocket"), RTS_Component);

		_questTargetMark->SetRelativeLocation(trans.GetTranslation() +
			GData()->GetGlobalData()->_interactionButtonPosOffset);
	}
}

//void AGsCharacterNonPlayer::DrawSightRadius(float inSight)
//{	
//	if (UWorld* world = GetWorld())
//	{
//		// 시야 거리
//		DrawDebugCircle(world, GetActorLocation(),
//			inSight, 64, FColor::Red, false, 0.02f,
//			0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
//		DrawDebugString(world, GetActorLocation()
//			, FString::Printf(TEXT("sight radius : %f"), inSight)
//			, nullptr
//			, FColor::White
//			, 0.02f
//			, false);
//	}
//}

#if	!UE_BUILD_SHIPPING
void AGsCharacterNonPlayer::ShowDebugTextInfo(bool isOn)
{
	// 디버그 텍스트 위젯 컴포넌트를 동적으로 생성한다 (Finalize 시 해당 위젯 컴포넌트 삭제 처리)
	if (isOn)
	{
		// 이미 동적으로 생성된 위젯이 있으면 Visibility 설정만 바꾸어 준다
		if (_billboardDebugText)
		{
			_billboardDebugText->SetVisibility(ESlateVisibility::HitTestInvisible);
			_billboardDebugText->SetInitializedFlag();
			return;
		}

		//UWidgetComponent* debugTextInfoWidget = Cast<UWidgetComponent>(AddComponent(
		//	DebugTextInfoComponentName,
		//	true,
		//	FTransform()/*FTransform(FQuat(), FVector(0.0f, 0.0f, 400), FVector::OneVector)*/,
		//	nullptr,
		//	false));
		UWidgetComponent* debugTextInfoWidget = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(), DebugTextInfoComponentName);
		
		if (nullptr == debugTextInfoWidget)
			return;

		debugTextInfoWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
		debugTextInfoWidget->SetWidgetSpace(EWidgetSpace::Screen);
		debugTextInfoWidget->SetDrawSize(FVector2D(525.0f, 315.0f));
		debugTextInfoWidget->SetCastShadow(false);

		debugTextInfoWidget->SetupAttachment(GetMesh());
		debugTextInfoWidget->CreationMethod = EComponentCreationMethod::Native;
		debugTextInfoWidget->RegisterComponent();

		UGsPathManager* pathMgr = GPath();
		if (nullptr == pathMgr)
			return;

		UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("DebugTextInfo"));
		if (nullptr == widgetClass)
			return;

		_billboardDebugText = Cast<UGsUIBillboardDebugText>(CreateWidget(GetWorld(), widgetClass));

		debugTextInfoWidget->SetWidget(_billboardDebugText);
	}
	// 기존에 생성된 디버그 텍스트 위젯 컴포넌트를 안 보이게 처리한다
	else
	{
		// 이미 기존에 생성된 위젯이 있으면, 기존 위젯을 hide 처리만 해 준다.
		if (_billboardDebugText)
		{
			_billboardDebugText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}	
}

//void AGsCharacterNonPlayer::OnDebugTextInfo(const FGsDebugTextInfo& prevDebugTextInfo, const FGsDebugTextInfo& curDebugTextInfo)
//{
//	// 위젯 갱신 요청
//	_billboardDebugText->SetDebugTextInfo(prevDebugTextInfo, curDebugTextInfo);
//}

void AGsCharacterNonPlayer::OnDebugTextInfo(TArray<FString>&& InWholeDebugTextArray)
{
	if (nullptr == _billboardDebugText)
	{
		ShowDebugTextInfo(true);
	}

	// 위젯 갱신 요청
	_billboardDebugText->SetDebugTextInfo(MoveTemp(InWholeDebugTextArray));
}
#endif

