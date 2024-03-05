// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameObjectSandbag.h"
#include "ActorEx/GsCharacterSandbag.h"
#include "GsGameObjectCreature.h"

#include "Data/GsResourceManager.h"
#include "Data/GsDataContainManager.h"

#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/LockOn/GsLockOnHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GsGameFlowGame.h"

#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/State/Sandbag/GsStateSandbagSpawn.h"
#include "GameObject/State/Sandbag/GsStateSandbagIdle.h"

#include "Quest/GsActorQuestTargetInterface.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Engine/StaticMesh.h"


void UGsGameObjectSandbag::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectSandbag::GetObjectType());		

	_isCanHit = false;

	//bak1210 : final 에서호출
	CreateSubClass();
}

void UGsGameObjectSandbag::InitializeActor(AActor* inOwner)
{
	Super::InitializeActor(inOwner);

	_isCanHit = false;

	if (AGsCharacterSandbag* sandBagActor = GetSandbag())
	{
		sandBagActor->SetInteractionClickCallback([this](bool In_isByWidget)
			{
				OnClickTargeting();
			});
	}

	// 말풍선 사용하는 지 확인
	SetTalkBalloonActive();
}

//Sandbag은 이동 할 수 없다. 
TSharedPtr<FGsMovementHandlerBase> UGsGameObjectSandbag::CreateMovement()
{	
	return nullptr;
}

void UGsGameObjectSandbag::InitializeFSM()
{
	if (_spawnReason == SPAWN_REASON::NORMAL || _spawnReason == SPAWN_REASON::SUMMON)
	{
		_fsm->Initialize(BPStateMachineName, this, FGsStateSandbagSpawn::GetInstance());
	}
	else
	{
		_fsm->Initialize(BPStateMachineName, this, FGsStateSandbagIdle::GetInstance());
	}
}

AGsCharacterSandbag* UGsGameObjectSandbag::GetSandbag()
{
	return _actor ? Cast<AGsCharacterSandbag>(_actor) : nullptr;
}

void UGsGameObjectSandbag::SetQuestMarkActive(bool In_isActive, int In_typeIndex)
{
	_isQuestTargetActive = In_isActive;

	if (_actor == nullptr ||
		_actor->IsPendingKill() == true )
	{
		return;
	}

	IGsActorQuestTargetInterface* target = Cast<IGsActorQuestTargetInterface>(_actor);
	if (nullptr == target)
	{
		return;
	}

	UStaticMeshComponent* meshComponent = target->GetQuestTargetMark();
	if (meshComponent == nullptr)
	{
		return;
	}

	if (_isQuestTargetActive &&		
		_fsm && _fsm->IsState(EGsStateBase::Idle))
	{
		meshComponent->SetVisibility(true);
		FSoftObjectPath path = GData()->GetGlobalData()->_questTargetMark;
		if (static_cast<int>(QuestType::REPEAT) == In_typeIndex)
		{
			path = GData()->GetGlobalData()->_repeatQuestTargetMark;
		}
		UStaticMesh* mesh = GResource()->LoadSync<UStaticMesh>(path);
		meshComponent->SetStaticMesh(mesh);
	}
	else
	{
		meshComponent->SetVisibility(false);
	}
}

void UGsGameObjectSandbag::StartFadeIn()
{	
	if (AGsCharacterSandbag* actor = Cast<AGsCharacterSandbag>(_actor))
	{
		if (_fsm)
		{
			_fsm->ProcessEvent(EGsStateBase::Spawn);

			UCurveFloat* curve = GData()->GetGlobalData()->_propDissolveCurve;
			if (curve)
			{
				actor->GetMeshShaderComponent()->Add(EGsMeshShaderEffect::Dissolve
					, actor->GetMesh()
					, curve
					, [this]() {
					if (IsQuestTarget())
					{
						_fsm->ProcessEvent(EGsStateBase::Idle);
						SetQuestTargetActive(true);
					}
				});
				actor->GetMeshShaderComponent()->Start(true);
			}
		}		
	}
}

void UGsGameObjectSandbag::StartFadeOut()
{
	SetQuestTargetActive(false);
	
	if (AGsCharacterSandbag* actor = Cast<AGsCharacterSandbag>(_actor))
	{
		if (_fsm)
		{
			_fsm->ProcessEvent(EGsStateBase::Dead);

			UCurveFloat* curve = GData()->GetGlobalData()->_propDissolveCurve;
			if (curve)
			{
				actor->GetMeshShaderComponent()->Add(EGsMeshShaderEffect::Dissolve
					, actor->GetMesh()
					, curve
					, nullptr
					, false);
				actor->GetMeshShaderComponent()->Start();
			}
		}		
	}
}

bool UGsGameObjectSandbag::IsAble()
{
	if (false == Super::IsAble())
	{
		return false;
	}

	// 퀘스트 진행 중인 녀석인가 확인
	return _isQuestTargetActive;
}

// 몸통 클릭 타게팅
void UGsGameObjectSandbag::OnClickTargeting()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsLockOnHandler* lockOnHandler = hud->GetLockOnHandler();
	if (lockOnHandler == nullptr)
	{
		return;
	}

	//GSLOG(Log, TEXT("OnClickTargeting %s"), *GetActor()->GetName());
	lockOnHandler->SetLockOnTarget(this);
}

void UGsGameObjectSandbag::SetVisibleInfo(const FGsSchemaSpawnElement* inVisible)
{
	_isInitVisible = inVisible->InitVisible;
	_questCondtion.Empty();

	QuestId startQuestId;
	QuestId endQuestId;
	int32 num = inVisible->QuestCondition.Num();
	FGsSpawnObjectShowHide temp;

	for (int32 i = 0; i < num; ++i)
	{
		startQuestId = (inVisible->QuestCondition[i].StartConditionQuestId.GetRow() ? inVisible->QuestCondition[i].StartConditionQuestId.GetRow()->id : 0);
		endQuestId = (inVisible->QuestCondition[i].EndConditionQuestId.GetRow() ? inVisible->QuestCondition[i].EndConditionQuestId.GetRow()->id : 0);
		
		temp.StartConditionQuestId = startQuestId;
		temp.EndConditionQuestId = endQuestId;
		temp.QuestCheckType = inVisible->QuestCondition[i].QuestCheckType;

		_questCondtion.Add(temp);
	}

	_visibleType = inVisible->VisibleType;

	UpdateVisible();
}

void UGsGameObjectSandbag::UpdateVisible()
{
	bool isVisible = GSQuest()->IsVisibleShowInfoArray(_questCondtion,
		_isInitVisible, _visibleType);
	if (GetActor())
	{
		GetActor()->SetActorHiddenInGame(!isVisible);
	}
}

// visible 바로 세팅(대사 연출 관련 처리)
void UGsGameObjectSandbag::SetGameObjectActorVisible(bool In_isVisible)
{
	bool isVisible = false;
	// 키는거면 퀘스트 조건 체크후 켬
	if (In_isVisible == true)
	{
		isVisible =
			GSQuest()->IsVisibleShowInfoArray(_questCondtion,
				_isInitVisible, _visibleType);
	}
	Super::SetGameObjectActorVisible(isVisible);
}