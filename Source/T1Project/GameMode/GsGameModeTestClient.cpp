// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameModeTestClient.h"

#include "Kismet/GameplayStatics.h"

#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/GameObject/GsSchemaPlayerShape.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "Util/GsTableUtil.h"
#include "Util/GsLevelUtil.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Skill/Runner/GsSkillRunnerLocalPlayer.h"
#include "GameObject/GsSchemaShapeData.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/HUD/GsHUDTestClient.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Management/GsScopeHolder.h"
#include "ControllerEx/GsPlayerController.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "Engine/WorldComposition.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "Animation/GsAnimInstanceState.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "Animation/AnimMontage.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Management/GsMessageHolder.h"
#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"


AGsGameModeTestClient::AGsGameModeTestClient()
{
	UWorldComposition::_unloadLevelEvent.Unbind();
	UWorldComposition::_unloadLevelEvent.BindUObject(this, &AGsGameModeTestClient::OnCheckUnloadStreamingLevel);

	HUDClass = AGsHUDTestClient::StaticClass();
}

AGsGameModeTestClient::~AGsGameModeTestClient()
{
	UWorldComposition::_unloadLevelEvent.Unbind();
}

void AGsGameModeTestClient::OnTestClientAutoMove(const struct IGsMessageParam* InParam)
{
	UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	UGsGameObjectLocalPlayer* castLocal = localPlayer->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsMovementHandlerLocalPlayer* movement = castLocal->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}
	if(nullptr != InParam)
	{
		const FGsPrimitiveText* param = InParam->Cast<const FGsPrimitiveText>();
		AActor* TargetActor = nullptr;;
		for (int i = 0; i < _navDestList.Num(); ++i)
		{
			if (_navDestList[i]->GetName() == param->_data.ToString())
			{
				TargetActor = _navDestList[i];
				break;
			}
		}
		if (nullptr == TargetActor)
			return;

		
		FVector origin, Extend;
		TargetActor->GetActorBounds(true, origin, Extend);
		float GoalR = (Extend.X > Extend.Y) ? Extend.X : Extend.Y;
		movement->ChangeAutoMoveState().StartPos(castLocal,
			TargetActor->GetActorLocation(),
			GoalR + 50.0f,
			FGsAutoMoveCompletedDelegate::CreateUObject(this, &AGsGameModeTestClient::CallbackMoveToPosEnd),
			FGsAutoMoveCompletedOneDelegate::CreateUObject(this, &AGsGameModeTestClient::CallbackMoveToPosFailed));
	}
	else
	{
		movement->Stop();
	}
}

void AGsGameModeTestClient::CallbackMoveToPosEnd()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_MOVE_STOP, nullptr);
}

void AGsGameModeTestClient::CallbackMoveToPosFailed(const FPathFollowingResult& Result)
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_MOVE_STOP, nullptr);
}

void AGsGameModeTestClient::OnTestClientWarp(const struct IGsMessageParam* InParam)
{
	const FGsPrimitiveText* param = InParam->Cast<const FGsPrimitiveText>();
	if (param)
	{
		AActor* TargetActor = nullptr;;
		for (int i = 0; i < _navDestList.Num(); ++i)
		{
			if (_navDestList[i]->GetName() == param->_data.ToString())
			{
				TargetActor = _navDestList[i];
				break;
			}
		}
		if (nullptr == TargetActor)
			return;

		UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		UGsGameObjectLocalPlayer* castLocal = localPlayer->CastGameObject<UGsGameObjectLocalPlayer>();

		AGsCharacterLocalPlayer* actor = castLocal->GetLocalCharacter();
		check(actor);
		actor->TeleportTo(TargetActor->GetActorLocation(), actor->GetActorRotation());
		
		if (FGsGameObjectStateManager* fsm = castLocal->GetBaseFSM())
		{
			if (UGsAnimInstanceState* anim = castLocal->GetLocalCharacter()->GetAnim())
			{
				if (FGsSkillHandlerBase* skill = castLocal->GetSkillHandler())
				{
					FGsSkillRunnerBase* skillRunner = skill->GetSkillRunner();
					if (const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(castLocal, CommonActionType::SPAWN))
					{
						skillRunner->StartRunner(castLocal, commonData);
					}

					if (UAnimMontage* resAni = skillRunner->GetAni())
					{
						float len = anim->Montage_Play(resAni);
						_movementBlockReleaseTime = len * 0.4f;
						fsm->DelayProcessEvent(len * 0.8f, EGsStateBase::None, EGsStateBase::Idle);

						LocalPlayerMovementBlock(true);
												
					}
					else
					{
						fsm->ProcessEvent(EGsStateBase::Idle);
					}
				}
			}
		}
	}
}

int AGsGameModeTestClient::GetCurrentLevelId()
{
	int LevelId = -1; // invaildId;
	TArray<FString> Tokens;
	GetGameInstance()->PIEMapName.ParseIntoArray(Tokens, TEXT("/"));
	//PIEMapName	L"/Game/Maps/AkunTemple/UEDPIE_0_AkunTemple"	FString
	int MapsIndex = 0;
	for (int i = 0; i < Tokens.Num(); ++i)
	{
		if (Tokens[i].Contains(TEXT("Maps")))
		{
			MapsIndex = i;
			break;
		}
	}
	// 잘못된 정보
	if (MapsIndex >= (Tokens.Num()-1)) // Maps 다음에 존재하기때문에 MapsIndex + 1 이 맵이름 (Maps가 가장 마지막이어도 유효하지 않음)
	{
		return LevelId;
	}

	FString levelName = Tokens[MapsIndex + 1];
	const UGsTableMapData* mapDataTable = Cast<UGsTableMapData>(FGsSchemaMapData::GetStaticTable());
	TArray<const FGsSchemaMapData*> dataList;
	if (false == mapDataTable->GetAllRows(dataList))
	{
		return LevelId;
	}
	
	for (int j = 0; j < dataList.Num(); ++j)
	{
		if (dataList[j]->nameText.Contains(levelName))
		{
			LevelId = dataList[j]->id;
			break;
		}
	}
	return LevelId;
}

void AGsGameModeTestClient::StartPlay()
{
	Super::StartPlay();

	GLevel()->SetCurrentLevelId(GetCurrentLevelId());

	// bak1210 
	// TEST 클라이언트 지원용 메시지 BIND 
	MGameObject& msgGameObject = GMessage()->GetGameObject();
	msgGameObject.AddUObject(MessageGameObject::TEST_CLIENT_AUTOMOVE,	this, &AGsGameModeTestClient::OnTestClientAutoMove);
	msgGameObject.AddUObject(MessageGameObject::TEST_CLIENT_WARP,		this, &AGsGameModeTestClient::OnTestClientWarp);

	
	if (nullptr != _scopeHolder)
	{
		_scopeHolder->StartPlayGameModeWorld();
	}


	NetUserData = new FGsNetUserData();
	NetUserData->mTid = (_genderType == CreatureGenderType::MALE) ? 1 : 2;

	/*if (const FGsSchemaUserData* pcTable = UGsTableUtil::FindRow<FGsSchemaUserData>(NetUserData->mTid))
	{
		if (const FGsSchemaPlayerShape* playerShape = pcTable->shapeId.GetRow())
		{
			if (const FGsSchemaShapeData* shapeData = playerShape->localplayerShapeId.GetRow())
			{
				FString path = shapeData->bPClass.ToString();
				path += TEXT("_C");

				PlayerClass = LoadObject<UClass>(nullptr, *path);
			}
		}
	}*/
	_playerSpawnTransform = UGsLevelUtil::GetPlayerStart(GetWorld());

	if(GetWorld()->WorldComposition)
	{
		if (UWorldComposition* worldcomposition = GetWorld()->WorldComposition)
		{
			worldcomposition->UpdateStreamingState(_playerSpawnTransform.GetLocation());
		}

		if (ULocalPlayer* pPlayer = GetWorld()->GetGameInstance()->FindLocalPlayerFromControllerId(0))
		{
			if (AGsPlayerController* pController = Cast<AGsPlayerController>(pPlayer->PlayerController))
			{
				pController->SetInitialSpawnLocation(_playerSpawnTransform.GetLocation());
			}
		}
		
		GetWorld()->FlushLevelStreaming(EFlushLevelStreamingType::Full);

		TerrianLoadFinish();

	}
	else
	{		
		NetUserData->mGender = _genderType;
		FGsSpawnParamLocalPlayer testLocalParam(EGsGameObjectType::LocalPlayer, _playerSpawnTransform.GetLocation(),
			_playerSpawnTransform.GetRotation().Rotator(), NetUserData);
		
		GSGameObject()->SpawnObject(&testLocalParam);
	}

	_summonHandler = new FGsSummonHandler();
	_summonHandler->Init();


	
	//bak1210 : 게임이 시작되면 네비게이션 목록을 수집한다.
	RefreshNavDest();
	
	//bak1210 : HUD를 1회 갱신합니다.
	if (ULocalPlayer* pPlayer = GetWorld()->GetGameInstance()->FindLocalPlayerFromControllerId(0))
	{
		if (AGsPlayerController* pController = Cast<AGsPlayerController>(pPlayer->PlayerController))
		{
			if (AGsHUDTestClient* hud = Cast<AGsHUDTestClient>(pController->GetHUD()))
			{
				hud->InvalidateHud();
			}
		}
	}
}

void AGsGameModeTestClient::RefreshNavDest()
{
	_navDestList.Empty();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("NavDest")), _navDestList);
}

void AGsGameModeTestClient::BeginDestroy()
{
	if (nullptr != NetUserData)
	{
		delete NetUserData;
		NetUserData = nullptr;
	}

	_navDestList.Empty();
	
	Super::BeginDestroy();
}


void AGsGameModeTestClient::TerrianLoadFinish()
{
	NetUserData->mGender = _genderType;
	FGsSpawnParamLocalPlayer testLocalParam(EGsGameObjectType::LocalPlayer, _playerSpawnTransform.GetLocation(),
		_playerSpawnTransform.GetRotation().Rotator(), NetUserData);

	UGsGameObjectBase* local = GSGameObject()->SpawnObject(&testLocalParam);

	if (local)
	{
		if (AGsCharacterLocalPlayer* character = Cast<AGsCharacterLocalPlayer>(local->GetCharacter()))
		{
			// 이름 빌보드 가림
			if (UGsUIBillboardCreature* widget = character->GetWidget())
			{
				widget->SetRenderOpacity(0.f);
			}
		}
	}

	UnloadDesignStreamingLevel();
	ClearDesignSteamingLevel();
}

void AGsGameModeTestClient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//로컬플레이어 묶인발 풀어주기(텔레포트시)
	if (_movementBlockReleaseTime > 0.0f)
	{
		_movementBlockReleaseTime -= DeltaSeconds;

		if (_movementBlockReleaseTime <= 0.0f)
		{
			LocalPlayerMovementBlock(false);
			_movementBlockReleaseTime = 0.0f;
		}
	}
}

void AGsGameModeTestClient::LocalPlayerMovementBlock(bool inFlag)
{
	UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	UGsGameObjectLocalPlayer* castLocal = localPlayer->CastGameObject<UGsGameObjectLocalPlayer>();
	if (AGsPlayerController* playerController = castLocal->GetLocalCharacter()->GetPlayerController())
	{
		if (inFlag)
		{
			playerController->SetIgnoreMoveInput(true);
			playerController->SetIgnoreLookInput(true);
		}
		else
		{
			playerController->ResetIgnoreLookInput();
			playerController->ResetIgnoreMoveInput();
		}
		
	}
}

void AGsGameModeTestClient::EndPlay(const EEndPlayReason::Type inEndPlayReason)
{
	if (nullptr != _summonHandler)
	{
		_summonHandler->Close();
		delete _summonHandler;
		_summonHandler = nullptr;
	}

	Super::EndPlay(inEndPlayReason);
}

bool AGsGameModeTestClient::OnCheckUnloadStreamingLevel(const FString& inLevel)
{
	return UGsLevelManager::IsServerLevel(inLevel);
}