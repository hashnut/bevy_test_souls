// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameObjectNonPlayer.h"

#include "Components/CapsuleComponent.h"
#include "ActorEx/GsCharacterNonPlayer.h"
#include "ActorComponentEx/GsWidgetComponentTalkBalloon.h"

#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/State/NonPlayer/GsStateNonPlayerSpawn.h"
#include "GameObject/State/NonPlayer/GsStateNonPlayerIdle.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerNonPlayer.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "GameObject/Define/GsGameObjectText.h"

#include "DataSchema/GsSchemaInteractionData.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/LockOn/GsLockOnHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Scan/GsScanHandler.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsContentsEventManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"

#include "Message/MessageParam/GsAIMessageParam.h"

#include "Map/Minimap/MapIcon/GsMinimapIconHandler.h"

#include "Quest/Management/GsQuestTargetManagement.h"

#include "ContentsEvent/GsContentsEventNpcPrayer.h"

#include "UI/UIContent/Billboard/GsUIBillboardNonPlayer.h"
#include "UI/UIContent/Billboard/GsUIBillboardTalkBalloon.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"

#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsText.h"
#include "DrawDebugHelpers.h"

#include "SharedPointer.h"

int UGsGameObjectNonPlayer::catchGhost = -1;

void UGsGameObjectNonPlayer::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectNonPlayer::GetObjectType());

	// interaction 
	_interactionMessageParam._objectType = GetObjectType();
	_interactionMessageParam._targetGameID = GetGameId();
	
	//bak1210 : final 에서호출
	CreateSubClass();
}

void UGsGameObjectNonPlayer::InitializeActor(AActor* Owner)
{
	Super::InitializeActor(Owner);

	_newInteractionButtonSize = 0;
	_oldInteractionButtonSize = 0;

	AGsCharacterNonPlayer* npc = Cast<AGsCharacterNonPlayer>(_actor);
	if (nullptr == npc)
	{
		return;
	}

	// 캡슐 사이즈 조정(대사 연출 거리 확보용)
	bool isUseInteractionBlock = false;
	if (_npcData)
	{
		isUseInteractionBlock = (_npcData->npcFunctionType != NpcFunctionType::NONE) ? true : false;

		if (_npcData->iffFactionType == IffFactionType::MONSTER && npc)
		{
			// Face 스켈레탈 메시 컴퍼넌티 틱 비활성 처리
			npc->GetPartsSkeletalComponentFACE()->SetComponentTickEnabled(false);
		}				
		//bak1210 : 보스급들만 오버랩 처리를 해야함
		if (CreatureRankType::NAMED <= _npcData->grade)
		{
			_actor->OnOverlapedCharacterBase.BindUObject(this, &UGsGameObjectNonPlayer::CallbackOverlapedCharacterBase);
			_clientCollisionRule = false;
			BlockLocalPlayer(_clientCollisionRule);
		}
		
	}

	_interactionBlockCollider = npc->GetInteractionBlockCollider();
#ifdef USE_BLOCK_INTERACTION
	if (_interactionBlockCollider != nullptr)
	{
		if (isUseInteractionBlock == true)
		{
			float capsuleRadius = 0;
			float capsuleHeight = 0;
			if (_creatureCommonInfo)
			{
				capsuleHeight = _creatureCommonInfo->cylinderHeight;
			}
			if (_npcData)
			{
				capsuleRadius = _npcData->interactionRange * GData()->GetGlobalData()->LocalPlayerMoveToTargetGoalScale;
			}
			if (_interactionBlockCollider)
			{
				_interactionBlockCollider->SetCapsuleSize(capsuleRadius, capsuleHeight);
			}
			_interactionBlockCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			// 로컬이 충돌영역안에 있으면 콜리더 사이즈 lerp 시작한다
		}
		else
		{
			_interactionBlockCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
#else	
	if (_interactionBlockCollider != nullptr)
	{
		_interactionBlockCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
#endif

	//bak1210 - CreateSubClass에서 옮김 ----------------------------------------------------------------------------------
	// 인터렉션 이벤트 활성화 갱신
	UpdateInteractionEvent();

	// 퀘스트 show 이펙트 출력하는건가
	if (GSClientSpawn()->FindAndRemoveQuestShowListId(_npcData->id) == true)
	{
		OnSpawnDespawnEffect();
	}	

	// check interaction move target
	if (UGsGameObjectBase* findLocal =
		GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* local = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
		if (local == nullptr)
		{
			return;
		}
		if (FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
		{
			if (interactionHandler->IsReserveInteraction() == true &&
				EGsInteractionMoveType::PosMove == interactionHandler->GetReserveMoveType())
			{
				const FGsReserveInteractionInfo& reserveInteractionInfo = interactionHandler->GetReserveInfo();
				if (reserveInteractionInfo._target._tblId == _npcData->id)
				{
					SetVisibleInteractionMoveTargetMarker(true);
				}
			}
		}
	}

	//npc가 기능성 npc일때 affect navigation을 활성화 한다. 
	SetAffectNavigation();
	
	//대화용 npc 경우 NpcPawn으로 변경해야한다. 
	//투명 npc는 무시해야한다.
	SetNpcCollision();
	
	//----------------------------------------------------------------------------------	
}

void UGsGameObjectNonPlayer::UpdateInteractionEvent()
{
	int tblId = GetNpcTblId();
	FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
	if (questHandler == nullptr)
	{
		return;
	}
	FGsQuestTargetManagement* questTargetManagement = questHandler->GetQuestTargetManagement();
	if (questTargetManagement == nullptr)
	{
		return;
	}

	// 1. npc function 이  퀘스트 대화인가
	// 1. 1. 퀘스트 타겟인가
	// 1. 2. 침공지역이며 침공퀘스트를 가졌는가
	// 2. npc function 이 none 아닌가
	bool isQuestTarget = false;
	bool isInteractionActive = false;
	int outIndex = 0;

	if (false == UGsGameObjectUtil::IsEnemy(this))
	{
		if (_npcData->npcFunctionType == NpcFunctionType::QUEST_DIALOG)
		{
			if (questTargetManagement->IsQuestNpc(tblId) == true)
			{
				isInteractionActive = true;
				isQuestTarget = true;
			}

			if (questTargetManagement->GetQuestSubNpcStateIconIndex(tblId, outIndex) == true)
			{
				isInteractionActive = true;

				QuestContentsType contentsType = GSQuest()->GetSubQuestContenType(GetNpcTblId());
				_isInvadeQuestNpc = (contentsType == QuestContentsType::INVADE) ? true : false;
			}
		}
		else if (_npcData->npcFunctionType == NpcFunctionType::GATEKEEPER ||
			_npcData->npcFunctionType == NpcFunctionType::INVADE_GUARDIAN ||
			_npcData->npcFunctionType == NpcFunctionType::INVADE_WAGON ||
			_npcData->npcFunctionType == NpcFunctionType::INVADE_BOSS)
		{	// 2022/08/01 PKT - GateKeeper(문지기) 타입은 언제나 True
			isInteractionActive = true;
		}
		else if (_npcData->npcFunctionType != NpcFunctionType::NONE)
		{
			// 인터랙션 데이터 없으면 아이콘 표시 안함(필드 보스 추가 이슈) 
			const FGsSchemaInteractionData* findTbl =
				UGsTableUtil::FindRowById<UGsTableInteractionData, FGsSchemaInteractionData>(_npcData->npcFunctionType);

			if (findTbl)
			{
				if (_npcData->npcFunctionType == NpcFunctionType::EVENT_PRAYER)
				{
					// check current event state
					isInteractionActive = (nullptr == GSContentsEvent()->GetUniqueActivatedContentsEvent<const FGsContentsEventNpcPrayer>()) ? false : true;

				}
				else
				{
					isInteractionActive = true;
				}
			}
			else
			{
#if WITH_EDITOR
				FString functionType = FGsTextUtil::GetStringFromEnum(TEXT("NpcFunctionType"), _npcData->npcFunctionType);
				GSLOG(Warning, TEXT("Interaction data is missing - id : %d\ttype : %s"), _npcData->id, *functionType);
#endif
			}
		}
	}
	
	
	UGsDataContainManager* dataContainerManager = GData();
	if (nullptr == dataContainerManager)
	{
		return;
	}
	const UGsGlobalConstant* globalConstant = dataContainerManager->GetGlobalData();

	if (nullptr == globalConstant)
	{
		return;
}

	SetQuestTargetActive(isQuestTarget, outIndex);

	// 액티브 변수로 세팅하자	
	SetInteractionActive(isInteractionActive, outIndex);

	// 말풍선 사용하는 지 확인
	SetTalkBalloonActive();

	//침공전 방어시 마을 안 Npc는 말풍선과 인터렉션 버튼이 보이지 말아야한다.
	//퀘스트 진행이나 몸통 인터렉션은 가능하다. 
	if (CheckInvasionBattleMode())
	{
		ActiveTalkBallon(false);

#if HIDE_INTERACTION_IN_CHAOS_TOWN
		ActiveInteractionIcon(false);
#endif
	}
}

void UGsGameObjectNonPlayer::Update(float Delta)
{
	Super::Update(Delta);

	if (GSInteraction()->GetIsDrawInteractionRange() == true)
	{
		DrawDebugInteractionRange();
	}

	if (IsTreasureMonster())
	{
		if (_partyMonster && !IsMyPartyMonster())
		{
			_partyMonster = false;
			InvalidateMinimapTreasureMonster();
		}
	}
}

void UGsGameObjectNonPlayer::BindMessage()
{
	Super::BindMessage();

	FGsMessageHolder* msg = GMessage();
	if (msg)
	{
		_invasionHandleArray.Empty();
		_invasionHandleArray.Add(msg->GetInvasion().AddUObject(MessageInvasion::INVAION_START,
			this, &UGsGameObjectNonPlayer::OnStartInvasion));
		_invasionHandleArray.Add(msg->GetInvasion().AddUObject(MessageInvasion::INVAION_FINISH,
			this, &UGsGameObjectNonPlayer::OnFinishInvasion));
	}
}

void UGsGameObjectNonPlayer::UnBindMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (msg)
	{
		for (TPair<MessageInvasion, FDelegateHandle> messagePair : _invasionHandleArray)
		{
			msg->GetInvasion().Remove(messagePair);
		}
		_invasionHandleArray.Empty();
	}

	Super::UnBindMessage();
}

void UGsGameObjectNonPlayer::RemoveSubClass()
{
	Super::RemoveSubClass();


	// 퀘스트 hide 이펙트 출력하는건가
	if (FGsClientSpawnManager* clientSpawnManager = GSClientSpawn())
	{
		if (_npcData == nullptr)
		{
			return;
		}

		if (clientSpawnManager->FindAndRemoveQuestHideListId(_npcData->id) == true)
		{
			OnSpawnDespawnEffect();
		}
	}
}

void UGsGameObjectNonPlayer::InitializeFSM()
{
	if (_spawnReason == SPAWN_REASON::NORMAL || _spawnReason == SPAWN_REASON::SUMMON)
	{
		_fsm->Initialize(BPStateMachineName, this, FGsStateNonPlayerSpawn::GetInstance());
	}
	else
	{
		_fsm->Initialize(BPStateMachineName, this, FGsStateNonPlayerIdle::GetInstance());
	}
}

void UGsGameObjectNonPlayer::InitializeMovement()
{
	//sandbag은 movement handler가 없을 수 있다. 
	if (TSharedPtr<FGsMovementHandlerBase> movementHandler = CreateMovement())
	{
		AppendInitialize(movementHandler);

		// NpcData에서 참조
		if (const FGsSchemaCreatureCommonInfo* commonInfoTable = GetCommonInfoTable())
		{
			movementHandler->InitSpeed(commonInfoTable->moveSpeed, commonInfoTable->walkSpeed);
		}
	}
}

void UGsGameObjectNonPlayer::CallbackOverlapedCharacterBase(int32 inOverlapCount)
{
	_clientCollisionRule = inOverlapCount == 0;
	BlockLocalPlayer(_clientCollisionRule);
}

const FName DefaultMonster = TEXT("PawnBoss");
const FName BossMonster = TEXT("PawnBossBlock");
void UGsGameObjectNonPlayer::BlockLocalPlayer(bool InBlock)
{
	if (AGsCharacterBase* Actor = GetCharacter())
	{
		Actor->SetCollisionProfileName(_clientCollisionRule ? BossMonster : DefaultMonster);
	}
}

TSharedPtr<FGsAbnormalityHandlerBase> UGsGameObjectNonPlayer::CreateAbnormality()
{
	return MakeShareable(new FGsAbnormalityHandlerNonPlayer());
}


void UGsGameObjectNonPlayer::UpdateTargetMark(bool inVisibleTargetMaker, bool inIsFirst)
{
	if (_npcData)
	{
		if (CreatureRankType::BOSS != _npcData->grade &&
			CreatureRankType::RAID != _npcData->grade)
		{
			Super::UpdateTargetMark(inVisibleTargetMaker, inIsFirst);
		}
	}	
}

void UGsGameObjectNonPlayer::WaitUntilDie(FName in_anim)
{
	if (AActor* actor = GetActor())
	{
		if (AGsCharacterBase* character = Cast<AGsCharacterBase>(actor))
		{
			if (UGsAnimInstanceState* anim = character->GetAnim())
			{
				if (FGsGameObjectStateManager* fsm = GetBaseFSM())
				{
					float aniLen = anim->GetStateAnimationLength(fsm->GetBPStateName(), in_anim);

					// AI에서 이동 명령이 계속 들어오므로 임시로 타이머를 돌려 Despawn처리 진행
					if (UWorld* world = actor->GetWorld())
					{
						if (_deathHandle.IsValid())
						{
							world->GetTimerManager().ClearTimer(_deathHandle);
						}

						world->GetTimerManager().SetTimer(_deathHandle,
							FTimerDelegate::CreateLambda([this, aniLen]()
								{
									//Boss 는 Despawn 처리 되지 않고 별도 프로세스탐
									if (true == IsBossMonster())
									{
										BossMonsterDieProc();
									}
									else
									{
										GSAI()->RemoveMannerCheckTarget(GetGameId());
										GSGameObject()->DespawnObject(this);
									}
								}), aniLen, false);
					}
				}
			}
		}
	}
}

void UGsGameObjectNonPlayer::ChangeIdleType(EGsAnimStateIdleType Type)
{
	if (FGsAbnormalityHandlerBase* abnormality = GetAbnormalityHandler())
	{
		// 상태이상 아이들 변경 타입
		if (abnormality->IsAbnormalityEffectType(AbnormalityEffectType::CC_BURY) ||
			abnormality->IsAbnormalityEffectType(AbnormalityEffectType::UTIL_BURY_KEEP_TARGET))
		{
			if (UGsAnimInstanceState* anim = GetAnimInstance())
			{
				anim->SetIdleType(EGsAnimStateIdleType::Abnormality);
				return;
			}
		}
	}
	
	Super::ChangeIdleType(Type);
}

void UGsGameObjectNonPlayer::SetWidgetTreasureMonsterInfo(FString userName, int32 remainTimeSec, int32 fullTimeSec, uint64 ownGameId,
	GuildDBId guildId /*= INVALID_GUILD_DB_ID*/, UserDBId ownDBId /*= INVALID_USER_DB_ID*/)
{
	if (auto trmonster = GetNonCharacter())
	{
		FDateTime endTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC() + FTimespan(0, 0, remainTimeSec);
		_lifeTime = endTime.GetTicks(); //남의 몬스터일때만 보여줌

		_ownInfo = FGsOwnerInfo{ ownGameId, guildId, ownDBId, userName };

		if (auto widget = Cast<UGsUIBillboardNonPlayer>(trmonster->GetWidget()))
		{
			widget->SetOwnMonsterOption(userName, remainTimeSec, fullTimeSec, ownGameId != 0);
		}

		InvalidateMinimapTreasureMonster();
	}
}

void UGsGameObjectNonPlayer::ChangeWidgetTreasureMonsterGuildInfo(GuildDBId guildId)
{
	_ownInfo._guildId = guildId;
	if (auto trmonster = GetNonCharacter())
	{
		if (auto widget = Cast<UGsUIBillboardNonPlayer>(trmonster->GetWidget()))
		{
			widget->ChangeOwnMonsterOption();
		}
	}
}

void UGsGameObjectNonPlayer::UpdateWidgetAliveTime(int32 remainTimeSec)
{
	if (auto trmonster = GetNonCharacter())
	{
		if (auto widget = Cast<UGsUIBillboardNonPlayer>(trmonster->GetWidget()))
		{
			FDateTime endTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC() + FTimespan(0, 0, remainTimeSec);
			widget->SetRemainLifeTime(endTime.GetTicks());
		}
	}
}

// 보스 죽는 Anim 종료 및 Despawn 처리 후, Ending 처리
void UGsGameObjectNonPlayer::BossMonsterDieProc()
{
	DeathHandleReset();
}

void UGsGameObjectNonPlayer::ChangeCollisionPresetToPawn()
{
	//https://jira.com2us.com/jira/browse/CHR-8611
	const FName DefaultPreset = TEXT("Pawn");
	if (AGsCharacterBase* Actor = GetCharacter())
	{
		Actor->SetCollisionProfileName(DefaultPreset);
	}
}

bool UGsGameObjectNonPlayer::IsBossMonster() const
{
	if (_npcData)
	{
		return CreatureRankType::BOSS == _npcData->grade;
	}

	return false;
}

void UGsGameObjectNonPlayer::OnClickInteract(bool In_isByWidget)
{
	// 퀘스트 액티브 해제
	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_ACTIVE_OFF, nullptr);
	// 메세지 보낸다
	GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_BUTTON_CLICK, &_interactionMessageParam);

	AGsCharacterNonPlayer* npc = GetNonCharacter();
	if (nullptr == npc)
	{
		return;
	}

	if (UGsUIBillboardInteractionButton* interactionButton = npc->GetInteractionButton())
	{
		interactionButton->OnClickButton();
	}
}

// 몸통 클릭 타게팅
void UGsGameObjectNonPlayer::OnClickTargeting()
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

	if (IsPlayerTeamTreasureMonster()) //localPlayer의 소유
	{
		return;
	}

	//GSLOG(Log, TEXT("OnClickTargeting %s"), *GetActor()->GetName());
	lockOnHandler->SetLockOnTarget(this);
}

// 인터랙션 디버그 범위 그리기
void UGsGameObjectNonPlayer::DrawDebugInteractionRange()
{
	UWorld* world = GetWorld();
	if (world && _npcData)
	{
		// 시야 거리
		DrawDebugCircle(world, GetActor()->GetActorLocation(),
			_npcData->interactionRange, 100, FColor::Red, false, 1.f,
			0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
	}
}

AGsCharacterNonPlayer* UGsGameObjectNonPlayer::GetNonCharacter()
{
	return _actor ? Cast<AGsCharacterNonPlayer>(_actor) : nullptr;
}

// 인터랙션 액티브 처리
void UGsGameObjectNonPlayer::SetInteractionActive(bool In_isActive, int In_Index)
{
	int tblId = GetNpcTblId();
	FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler();
	if (questHandler == nullptr)
	{
		return;
	}
	FGsQuestTargetManagement* questTargetManagement = questHandler->GetQuestTargetManagement();
	if (questTargetManagement == nullptr)
	{
		return;
	}

	_isInteractionActive = In_isActive;
	
	int outIndex = 0;
	if (questTargetManagement->GetQuestSubNpcStateIconIndex(tblId, outIndex) == true)
	{
		_isInteractionActive = true;
	}
	// 상태 저장
	//_isInteractionActive = In_isActive;

	AGsCharacterNonPlayer* npc = GetNonCharacter();
	if (nullptr == npc)
	{
		return;
	}

	UWidgetComponent* widgetComponent = npc->GetInteractionWidgetComponent();
	if (nullptr == widgetComponent ||
		nullptr == _npcData)
	{
		return;
	}

	if (UGsGameObjectUtil::IsEnemy(this))
	{
		widgetComponent->SetActive(false);
		widgetComponent->SetVisibility(false);

		_isInteractionActive = false;

		npc->SetInteractionClickCallback([this](bool In_isByWidget)
			{
				OnClickTargeting();
			});

		if (UGsUIBillboardInteractionButton* interactionButton = npc->GetInteractionButton())
		{
			interactionButton->ReleaseResource();
		}
		npc->SetUseInteractionButton(false);
	}
	else if(_isInteractionActive == true /* && _npcData->npcFunctionType != NpcFunctionType::NONE*/)
	{
		if (UGsGameObjectUtil::CanInteraction(this) || IsClientObject())
		{
			auto enableNPCInteraction = _isInteractionActive = (SharedFunc::IsNpcNameRed(GGameData()->GetLoadWorldType(), _npcData->npcFunctionType,
				GGameData()->GetLoadWorldId(), GGameData()->GetUserData()->mHomeWorldId)) ? false : true;

			widgetComponent->SetActive(enableNPCInteraction);

			widgetComponent->SetVisibility(enableNPCInteraction);

			npc->SetUseInteractionButton(enableNPCInteraction);

			if (enableNPCInteraction)
			{
				if (UGsUIBillboardInteractionButton* interactionButton = npc->GetInteractionButton())
				{
					interactionButton->SetData(this, _npcData->npcFunctionType, outIndex);
				}

				if (_npcData->npcFunctionType == NpcFunctionType::QUEST_DIALOG)
				{
					QuestContentsType contentsType = GSQuest()->GetSubQuestContenType(GetNpcTblId());
					_isInvadeQuestNpc = (contentsType == QuestContentsType::INVADE) ? true : false;

					if (UGsUIBillboardInteractionButton* interactionButton = npc->GetInteractionButton())
					{
						interactionButton->SetQuestMarkData(this, _isInvadeQuestNpc, _npcData->npcFunctionType, outIndex);
					}		
				}
				npc->SetInteractionClickCallback([this](bool In_isByWidget)
					{
						OnClickInteract(In_isByWidget);
					});
			}
		}
	}
	else	
	{
		widgetComponent->SetActive(false);
		widgetComponent->SetVisibility(false);

		_isInteractionActive = false;

		npc->SetInteractionClickCallback([this](bool In_isByWidget)
			{
				OnClickTargeting();
			});

		if (UGsUIBillboardInteractionButton* interactionButton = npc->GetInteractionButton())
		{
			interactionButton->ReleaseResource();
		}
		npc->SetUseInteractionButton(false);
	}

	//침공전시 퀘스트 NPC라면 인터렉션 버튼 숨김 처리 
	if (CheckInvasionBattleMode())
	{
		ActiveTalkBallon(false);

#if HIDE_INTERACTION_IN_CHAOS_TOWN
		ActiveInteractionIcon(false);
#endif
	}
}

// 퀘스트 타겟 액티브인지
void UGsGameObjectNonPlayer::SetQuestTargetActive(bool In_isActive, int In_Index)
{
	Super::SetQuestTargetActive(In_isActive, In_Index);

	if (_npcData == nullptr)
	{
		return;
	}

	// 인터랙션 체크
	// 1. npc function 이  퀘스트 대화인가
	bool isInteractionActive = In_isActive;
	if (_npcData->npcFunctionType == NpcFunctionType::QUEST_DIALOG)
	{
		EQuestNpcState outNpcState;
		TSharedPtr<FGsQuestData> questData = GSQuest()->GetSubQuestData(GetNpcTblId(), outNpcState);
		if (questData.IsValid())
		{
			TSharedPtr<FGsQuestTableCache> questTableCache = questData->GetQuestTableCache();
			if (questTableCache.IsValid())
			{
				if (const FGsSchemaQuest* schemaQuest = questTableCache->GetSchemaQuest())
				{
					_isInvadeQuestNpc = (schemaQuest->questContentsType == QuestContentsType::INVADE) ? true : false;
				}
			}
		}
	}

	SetInteractionActive(In_isActive, In_Index);
}

void UGsGameObjectNonPlayer::ActorDespawn()
{
	Super::ActorDespawn();
	
	InvalidFieldBossBillboard();
}

void UGsGameObjectNonPlayer::OnDie()
{
	Super::OnDie();
	
	InvalidFieldBossBillboard();	
}

bool UGsGameObjectNonPlayer::IsTreasureMonster()
{
	return _npcData && _npcData->iffFactionType == IffFactionType::MONSTER_TREASURE && !_ownInfo._name.IsEmpty();
}

bool UGsGameObjectNonPlayer::IsPlayerTeamTreasureMonster()
{
	return IsMyTreasureMonster() || IsMyPartyMonster() || IsMyGuildMemberOwnMonster();
}

FText UGsGameObjectNonPlayer::GetNpcName()
{
	if (IsTreasureMonster() && _npcData)
	{
		FText outText;
		if (FText::FindText(TEXT("MonsterNameText"), TEXT("TreasureMonster_UI"), outText))
		{
			return FText::Format(outText, FText::FromString(_ownInfo._name), _npcData->nameText);
		}
	}

	return Super::GetNpcName();
}

NpcFunctionType UGsGameObjectNonPlayer::GetNpcFunctionType()
{
	return _npcData ? _npcData->npcFunctionType : NpcFunctionType::MAX;
}

bool UGsGameObjectNonPlayer::IsMyTreasureMonster()
{
	return _ownInfo._gameId != 0;
}

bool UGsGameObjectNonPlayer::IsMyPartyMonster()
{
	FGsPartyManager* partyManager = GSParty();
	if(partyManager== nullptr)
	{
		return false;
	}
	return  partyManager->GetIsInParty() && partyManager->IsInPartyName(_ownInfo._name);
}

bool UGsGameObjectNonPlayer::IsMyGuildMemberOwnMonster()
{
	return GGuild()->IsGuild() && GGuild()->GetGuildDBId() == _ownInfo._guildId;
}

bool UGsGameObjectNonPlayer::IsMonster()
{
	if (nullptr == _npcData)
		return false;

	if (IffFactionType::MONSTER == _npcData->iffFactionType ||
		IffFactionType::MONSTER_SPACECRACK == _npcData->iffFactionType ||
		IffFactionType::MONSTER_TREASURE == _npcData->iffFactionType ||
		IffFactionType::NPC_GUARD == _npcData->iffFactionType ||
		IffFactionType::NPC_INVASIONGUARDIAN == _npcData->iffFactionType ||
		IffFactionType::NPC_WAGON == _npcData->iffFactionType ||
		IffFactionType::NPC_WAGONTHIEF == _npcData->iffFactionType ||
		IffFactionType::NPC_WAGONPEDDLER == _npcData->iffFactionType ||
		IffFactionType::NPC_WAGONGUARD == _npcData->iffFactionType)
		return true;

	return false;
}

void UGsGameObjectNonPlayer::SetValidCheckPartyMonster()
{
	if (!_partyMonster)
	{
		InvalidateMinimapTreasureMonster();
	}
}

//기능성 Npc이고 마을에 존재한다면 true 아니면 false
bool UGsGameObjectNonPlayer::CheckInvasionBattleMode()
{
#if INVASION_TOWN_BATTLE_DEBUG
	bool isDefenseState = GGameData()->GetInvasionDefenseState();
	bool isFunctionNpc = IsFunctionalNpc(_npcData->npcFunctionType);
	bool isInTown = IsInTown();

	return isDefenseState && isFunctionNpc && isInTown;
#endif
	return GGameData()->GetInvasionDefenseState()
		&& IsFunctionalNpc(_npcData->npcFunctionType)
		&& IsInTown();
}

void UGsGameObjectNonPlayer::OnStartInvasion(const struct IGsMessageParam* inParam)
{
	if (CheckInvasionBattleMode())
	{
		ActiveTalkBallon(false);

#if HIDE_INTERACTION_IN_CHAOS_TOWN
		ActiveInteractionIcon(false);
#endif
	}
}

void UGsGameObjectNonPlayer::OnFinishInvasion(const struct IGsMessageParam* inParam)
{
	if (false == GGameData()->GetInvasionDefenseState()
		&& (IsFunctionalNpc(_npcData->npcFunctionType) && IsInTown()))
	{
		ActiveTalkBallon(true);

#if HIDE_INTERACTION_IN_CHAOS_TOWN
		ActiveInteractionIcon(true);
#endif
	}
}

void UGsGameObjectNonPlayer::ActiveTalkBallon(bool inActive)
{
	AGsCharacterNonPlayer* npc = GetNonCharacter();
	if (nullptr == npc)
	{
		return;
	}

	npc->SetUseTalkBalloon(inActive);
	npc->ShowTalkBalloonOnChangedState(inActive);
}

void UGsGameObjectNonPlayer::ActiveInteractionIcon(bool inActive)
{
	AGsCharacterNonPlayer* npc = GetNonCharacter();
	if (nullptr == npc)
	{
		return;
	}

	npc->SetUseInteractionButton(inActive);
	npc->ShowInteractionButton(inActive);
}

bool UGsGameObjectNonPlayer::IsInTown()
{
	AGsCharacterNonPlayer* npc = GetNonCharacter();
	if (nullptr == npc)
	{
		return false;
	}

	//현재 레벨을 얻어 온 후
	TArray<const FGsSchemaTownData*> townNpcList;
	if (false == GLevel()->TryGetAllSpawnNpcInTown(townNpcList))
	{
		return false;
	}

	FVector location = npc->GetActorLocation();
	const FGsSchemaTownData** find = townNpcList.FindByPredicate([&](const FGsSchemaTownData* town)->bool {
		return town->CreatureId.GetRow() == _npcData;
		});

	if (nullptr == find)
	{
		return false;
	}

	return true;
}

UGsWidgetComponentTalkBalloon* UGsGameObjectNonPlayer::SetTalkBalloonActive()
{
	UGsWidgetComponentTalkBalloon* talkBalloonComponent = Super::SetTalkBalloonActive();

	if (talkBalloonComponent && talkBalloonComponent->IsAvailable())
	{

		_isTalkBalloonActive = true;
		AGsCharacterNonPlayer* npc = GetNonCharacter();
		if (UGsUIBillboardTalkBalloon* talkBalloon = npc->GetTalkBalloon())
		{
			talkBalloon->OnStateChanged.AddDynamic(this, &UGsGameObjectNonPlayer::OnChangedTalkBalloonState);
		}
	}
	else
	{
		_isTalkBalloonActive = false;
	}

	return talkBalloonComponent;
}

void UGsGameObjectNonPlayer::OnChangedTalkBalloonState()
{
	UpdateWidgetShowInteractionButtonOption();
}

void UGsGameObjectNonPlayer::HideTalkBalloon()
{
	if (false == _isTalkBalloonActive)
	{
		return;
	}

	Super::HideTalkBalloon();
}

bool UGsGameObjectNonPlayer::IsFunctionalNpc(NpcFunctionType inNpcFucntion)
{
	switch (inNpcFucntion)
	{
	case NpcFunctionType::SHOP:
	case NpcFunctionType::REVIVAL:
	case NpcFunctionType::PURCHASE:
	case NpcFunctionType::QUEST_DIALOG:
	case NpcFunctionType::DEPOT:
	case NpcFunctionType::COMMUNITY:
	case NpcFunctionType::EVENT_SHOP:
	case NpcFunctionType::SHOP_EQUIP:
	case NpcFunctionType::SHOP_SKILL_BOOK:
	case NpcFunctionType::QUEST_BOARD:
	case NpcFunctionType::EVENT_PRAYER:	
	{
		return true;
		break;
	}	
	}
		
	return false;
}

//몬스터, 보스 몬스터의 콜리전은 블루프린터에서 셋팅한다. 
//추후에 로직으로 몬스터, 보스 몬스터 콜리전 셋팅을 생각해보자
//지금은 대화용 npc 연출만 신경쓰자
void UGsGameObjectNonPlayer::SetNpcCollision()
{
	//대화용 npc, 기능성 npc는 pawnNpc이다. 
	//투명 npc는 어떤 것과도 충돌하지 않는다. 
	//몬스터는 Pawn, 이미 블루 프린트에 셋팅되있기 때문에 변경하지 않는다.
	const FGsSchemaShapeData* shapeData = _npcData->shapeId.GetRow();
	if (nullptr == shapeData)
	{
		return;
	}

	bool isTransparency = _npcData->shapeId.GetRow()->isTransparency;
	AGsCharacterNonPlayer* npc = Cast<AGsCharacterNonPlayer>(GetActor());
	if (nullptr == npc)
	{
		return;
	}

	if (isTransparency)
	{
		npc->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);	
		npc->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Block);
	}
	else
	{
		if (IsFunctionalNpc(_npcData->npcFunctionType))
		{
			npc->GetCapsuleComponent()->SetCollisionProfileName(TEXT("PawnNpc"));			
		}
	}
}

//몬스터, 보스 몬스터의 콜리전은 블루프린터에서 셋팅한다. 
//추후에 로직으로 몬스터, 보스 몬스터 콜리전 셋팅을 생각해보자
//지금은 대화용 npc 연출만 신경쓰자
void UGsGameObjectNonPlayer::SetAffectNavigation()
{
	if (nullptr == _npcData)
	{
		return;
	}

	const FGsSchemaShapeData* shapeData = _npcData->shapeId.GetRow();
	if (nullptr == shapeData)
	{
		return;
	}

	bool isTransparency = shapeData->isTransparency;
	AGsCharacterNonPlayer* npc = Cast<AGsCharacterNonPlayer>(GetActor());
	if (nullptr == npc)
	{
		return;
	}

	auto capsuleComp = npc->GetCapsuleComponent();
	if (nullptr == capsuleComp)
	{
		return;
	}

	if (false == isTransparency
		&& IsFunctionalNpc(_npcData->npcFunctionType))
	{
		capsuleComp->SetCanEverAffectNavigation(true);
	}
	else if(NpcFunctionType::SPACE_CRACK_CONTAMINANT == _npcData->npcFunctionType)
	{
		capsuleComp->SetCanEverAffectNavigation(true);
	}
}

// visible 상태인지 갱신
void UGsGameObjectNonPlayer::UpdateVisible()
{
	// 대화 스폰이면 킨다
	const TArray<int>& arrayDialogSpawn = GSClientSpawn()->GetDialogAllSpawnID();
	bool isDialogSpawn = arrayDialogSpawn.Contains(GetNpcTblId());
	// 연출중이면 visible 끈다
	bool isHiddenInGame = (IsHidden() && false == isDialogSpawn) ? true : false;

	GetActor()->SetActorHiddenInGame(isHiddenInGame);

	// save flag( using in UGsGameObjectBase::ActorSpawned)
	_ActorVisible = !isHiddenInGame;
}

// 스폰 디스폰 이펙트 출력
void UGsGameObjectNonPlayer::OnSpawnDespawnEffect()
{
	AGsCharacterBase* actorCharacter = GetCharacter();
	if (nullptr == actorCharacter)
	{
		return;
	}

	UGsEffectComponent* effComp = actorCharacter->GetEffectComponent();
	if (nullptr == effComp)
	{
		return;
	}
	
	FSoftObjectPath softPath = GData()->GetGlobalData()->_npcShowHideEffect;
	if (false == softPath.IsValid())
	{
		return;
	}

	UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(softPath);
	if (nullptr == load)
	{
		return;
	}

	UParticleSystem* particle = Cast<UParticleSystem>(load);
	if (nullptr == particle)
	{
		return;
	}

	FVector effectScale = UGsGameObjectUtil::GetParticleEffectScale(this);
	effComp->OnEffect(particle, true, EPSCPoolMethod::AutoRelease, EAttachLocation::KeepRelativeOffset,	FVector(ForceInit), effectScale);
}

bool UGsGameObjectNonPlayer::OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker,
	uint32 StriketSkillId, uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, 
	DamageStatType DamageStat, HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage)
{
	if (_fsm != nullptr && _fsm->IsState(EGsStateBase::SpaceCrackIdle))
	{
		return false;
	}

	return Super::OnHit(Striker, SubStriker, StriketSkillId, StrikerNotifyId, ResultType,
		DamageType, DamageStat, HealType, Damage, PassivityOwner, IsNoDamage);
}

void UGsGameObjectNonPlayer::OnRunAway()
{
	Super::OnRunAway();

	_fsm->ProcessEvent(EGsStateBase::RunAway);

	if (auto gameFlow = GMode()->GetGameFlow())
	{
		if (auto contents = gameFlow->GetContentsManagerGame())
		{
			if (auto hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud))
			{
				if (FGsScanHandler* scanHandler = hud->GetScanHandler())
					scanHandler->RemoveScanMemberInfo(GetGameId());
			}
		}
	}

	FGsAIReserveParam aiReserveClearParam(EGsAIActionType::ATTACK_ANYONE, this);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam);
}

void UGsGameObjectNonPlayer::OnIFFChanged()
{
	UpdateInteractionEvent();
}

void UGsGameObjectNonPlayer::NetDeathAck(PD::SC::PKT_SC_CREATURE_DEATH_READ* Packet)
{
	// 몬스터면 매너 타겟에서 삭제
	GSAI()->RemoveMannerCheckTarget(Packet->DeadGameId());

	Super::NetDeathAck(Packet);
}

// 위젯 이름 show/hide 옵션 갱신
void UGsGameObjectNonPlayer::UpdateWidgetShowNameOption()
{
	// npc만 처리한다
	if (_npcData == nullptr ||
		_npcData->iffFactionType == IffFactionType::MONSTER)
	{
		return;
	}

	// FIX:
	UGsUIBillboardCreature* widget = _actor->GetWidget();
	if (widget == nullptr)
	{
		return;
	}
	widget->UpdateByType(EGsBillboardUpdateType::All);
}

// 위젯 인터랙션 버튼 show/hide 옵션 갱신
void UGsGameObjectNonPlayer::UpdateWidgetShowInteractionButtonOption()
{
	// npc만 처리한다
	if (_npcData == nullptr ||
		_npcData->iffFactionType == IffFactionType::MONSTER)
	{
		return;
	}

	AGsCharacterNonPlayer* npc = GetNonCharacter();
	if (nullptr == npc)
	{
		return;
	}

	if (UGsUIBillboardInteractionButton* interactionButton = npc->GetInteractionButton())
	{
		if (_isTalkBalloonActive)
		{
			if (UGsUIBillboardTalkBalloon* talkBalloon = npc->GetTalkBalloon())
			{
				if (false == talkBalloon->IsClosed())
				{ // 말풍선이 열려있으면 상호작용 버튼 닫기
					interactionButton->SetVisible(false);
					return;
				}
			}
		}

		interactionButton->UpdateShowInteractionButtonOption();
	}
}

void UGsGameObjectNonPlayer::InvalidFieldBossBillboard()
{
	if (nullptr == _npcData)
	{
		return;
	}

	if (NpcFunctionType::FIELD_BOSS == _npcData->npcFunctionType)
	{
		FGsBossHandler* bossHandler = FGsBossHandler::GetInstance();
		if (bossHandler)
		{
			FGsFieldBossHandler* handler = bossHandler->GetFieldBossHandler();
			if (handler)
			{
				handler->RemoveFieldBossInfo(GetGameId());
				handler->ClearContribution();
			}
		}

		GMessage()->GetBoss().SendMessage(MessageBoss::HIDE_BOSS_BILLBOARD, nullptr);
	}
}

void UGsGameObjectNonPlayer::InvalidateMinimapTreasureMonster()
{
	if (IsTreasureMonster())
	{
		if (UGsMinimapIconHandler* minimapIconHandler = GSMinimap()->GetIconHandler())
		{
			_partyMonster = IsMyPartyMonster();

			FGsGameObjectMessageParamMapIcon paramIcon(this,
				IsMyTreasureMonster() ? EGsMapIconType::MyTreasureMonster : 
				IsMyPartyMonster() ? EGsMapIconType::TreasureMonster : EGsMapIconType::Monster);

			//GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_DESPAWN_ICON, &paramIcon);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPAWN_ICON, &paramIcon);
		}
	}
}

void UGsGameObjectNonPlayer::OnChangedStat(const FGsStatInfo& inStateInfo)
{
	Super::OnChangedStat(inStateInfo);

	if (StatType::HP == inStateInfo.GetStatType())
	{
		if (AGsCharacterBase* characterActor = GetCharacter())
		{
			characterActor->ReceiveChangeHP(inStateInfo.GetStatValue(), inStateInfo.GetMaxStatValue());
		}
	}
}

bool UGsGameObjectNonPlayer::IsAble()
{
	int64 gameId = GetGameId();
	// client spawn target cant attack 
	if (GSClientSpawn()->IsClientSpawnGameId(gameId) == true)
	{
		return false;
	}

	if (false == Super::IsAble())
	{
		return false;
	}

	return true;
}

void UGsGameObjectNonPlayer::SetVisibleInteractionMoveTargetMarker(bool In_visible)
{
	AGsCharacterNonPlayer* npc = GetNonCharacter();
	if (nullptr == npc)
	{
		return;
	}

	UGsGlobalConstant* globalConstant = GData()->GetGlobalData();
	npc->SetVisibleTargetMaker(In_visible, globalConstant->_interactionMoveTargetMarkerDecal);
}

void UGsGameObjectNonPlayer::LookAt(const FRotator& LookRotator, bool IsSmoothRotation)
{
	if (IsSmoothRotation)
	{
		// 서버와 연동하는 회전 애니메이션 보간 처리가 있는 Npc의 경우는 처리하지 않음
		if (const FGsSchemaNpcData* npcTable = GetNpcData())
		{
			if (npcTable->rotateSpeed > 0)
			{
				// Super::LookAt(LookRotator, true);
				return;
			}
		}
		Super::LookAt(LookRotator, true);
	}
	Super::LookAt(LookRotator, false);
}
// widget hp bar show/hide cheat option
void UGsGameObjectNonPlayer::UpdateWidgetShowHPBar()
{
	// only use monster
	if (_npcData == nullptr ||
		_npcData->iffFactionType != IffFactionType::MONSTER)
	{
		return;
	}


	UGsUIBillboardCreature* widget = _actor->GetWidget();
	if (widget == nullptr)
	{
		return;
	}
	widget->UpdateByType(EGsBillboardUpdateType::All);
}

const FGsSchemaShapeData* UGsGameObjectNonPlayer::GetPolymorphShapeData()
{
	if (_npcData && _npcData->iffFactionType == IffFactionType::MONSTER)
	{
		return _npcData->polymorpShapeId.GetRow();
	}

	return nullptr;
}

#if	!UE_BUILD_SHIPPING
void UGsGameObjectNonPlayer::ShowDebugTextInfo(bool isOn)
{
	// 공격 가능한 NPC 인 경우에만 테스트 (몬스터인 경우에만)
	if (false == IsMonster())
		return;

	if (AGsCharacterNonPlayer* npc = Cast<AGsCharacterNonPlayer>(_actor))
	{
		npc->ShowDebugTextInfo(isOn);
	}
}

void UGsGameObjectNonPlayer::OnDebugTextInfo(PD::SC::PKT_SC_DEBUG_TEXT_INFO_READ* packet)
{
	// 공격 가능한 NPC 인 경우에만 업데이트 (몬스터인 경우에만)
	if (false == IsMonster())
		return;

	// 본래라면 이전과 이후 정보를 비교하여 하나씩 세팅해 주려 했으나, 서버에서 한번에 전송하고 있음..
	TArray<FString> wholeTextArray;

	using debugInfoIter = PD::SC::PKT_SC_DEBUG_TEXT_INFO_READ::DebugTextInfoListIterator;
	for (debugInfoIter iter = packet->GetFirstDebugTextInfoListIterator();
		iter != packet->GetLastDebugTextInfoListIterator(); ++iter)
	{
		wholeTextArray.Emplace(iter->DebugTextInfo());
	}

	// Call
	if (AGsCharacterNonPlayer* npc = Cast<AGsCharacterNonPlayer>(_actor))
	{
		npc->OnDebugTextInfo(MoveTemp(wholeTextArray));
	}
}
#endif