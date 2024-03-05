// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/ObjectClass/GsGameObjectSanctumNexus.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "ActorEx/GsCamp.h"
#include "ActorEx/GsCompleteCamp.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsSanctumManager.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsSanctumMessageParam.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/LockOn/GsLockOnHandler.h"
#include "DataSchema/Sanctum/GsSchemaSanctumData.h"
#include "DataSchema/Sanctum/GsSchemaSanctumNexusData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"
#include "UTIL/GsTableUtil.h"
#include "Sound/GsSoundPlayer.h"
#include "Guild/GsGuildHelper.h"
#include "T1Project.h"


void UGsGameObjectSanctumNexus::InitializeActor(AActor* Owner)
{
	Super::InitializeActor(Owner);

	if (Owner->IsA<AGsCamp>())
	{
		_isCompleteCampState = false;
		InitActorConstructCamp(GetActor());
	}
	else if (Owner->IsA<AGsCompleteCamp>())
	{
		_isCompleteCampState = true;
		InitActorCompleteCamp(GetActor());
	}
}

void UGsGameObjectSanctumNexus::BindMessage()
{
	Super::BindMessage();

	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MGameObject& msgGameObj = msgMgr->GetGameObject();
	_msgHandleGameObjList.Emplace(msgGameObj.AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_START,
		this, &UGsGameObjectSanctumNexus::OnStartInteractionByProp));
	_msgHandleGameObjList.Emplace(msgGameObj.AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_CANCEL,
		this, &UGsGameObjectSanctumNexus::OnCancelInteractionByProp));
	_msgHandleGameObjList.Emplace(msgGameObj.AddUObject(MessageGameObject::DESPAW_REMOTE_PLAYER,
		this, &UGsGameObjectSanctumNexus::OnDespawnRemotePlayer));

	MGuild& msgGuild = msgMgr->GetGuild();
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_JOIN_SUCCESS,
		this, &UGsGameObjectSanctumNexus::OnGuildJoin));
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_LEAVE,
		this, &UGsGameObjectSanctumNexus::OnGuildLeave));
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_CHANGE_MY_AUTHORITY,
		this, &UGsGameObjectSanctumNexus::OnGuildChangeMyAuthority));
}

void UGsGameObjectSanctumNexus::UnBindMessage()
{
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		MGameObject& msgGameObj = msgMgr->GetGameObject();
		for (MsgGameObjHandle& handle : _msgHandleGameObjList)
		{
			msgGameObj.Remove(handle);
		}
		_msgHandleGameObjList.Empty();

		MGuild& msgGuild = msgMgr->GetGuild();
		for (MsgHandleGuild& handle : _msgHandleGuildList)
		{
			msgGuild.Remove(handle);
		}
		_msgHandleGuildList.Empty();
	}

	Super::UnBindMessage();
}

void UGsGameObjectSanctumNexus::SetInteractionActive(bool In_isActive, int In_index)
{
	AGsCampBase* campActor = GetCampActor();
	if (nullptr == campActor)
	{
		return;
	}
	
	if (false == In_isActive)
	{
		campActor->SetInteractionClickCallback(nullptr);
		campActor->SetUseInteractionButton(false);

		return;
	}

	// 우리 길드가 아닌경우
	if (false == IsMyGuild())
	{
		TWeakObjectPtr<UGsGameObjectSanctumNexus> weakPtr(this);
		campActor->SetInteractionClickCallback([weakPtr](bool In_isByWidget)
			{
				if (weakPtr.IsValid())
				{
					weakPtr->OnClickInteract(In_isByWidget);
				}
			});

		campActor->SetUseInteractionButton(false);
	}
	else
	{
		// 건설 완료된 경우
		if (_isCompleteCampState)
		{
			campActor->SetInteractionClickCallback(nullptr);
			campActor->SetUseInteractionButton(false);
		}
		else
		{
			TWeakObjectPtr<UGsGameObjectSanctumNexus> weakPtr(this);
			campActor->SetInteractionClickCallback([weakPtr](bool In_isByWidget)
				{
					if (weakPtr.IsValid())
					{
						weakPtr->OnClickInteract(In_isByWidget);
					}
				});

			campActor->SetUseInteractionButton(true);

			if (UGsUIBillboardInteractionButton* interactionButton = campActor->GetInteractionButton())
			{
				interactionButton->SetData(this, NpcFunctionType::SANCTUM_NEXUS, In_index);
			}
		}
	}
}

void UGsGameObjectSanctumNexus::OnClickInteract(bool In_isByWidget)
{
	// 성소 공격 체크 위함
	if (false == IsMyGuild())
	{
		OnClickTargeting();
		return;
	}

	// 예외상황이나 다 지어진 경우 인터렉션 불가
	if (0 != _maxConstructionStep &&
		_maxConstructionStep == _currConstructionStep)
	{
		// 인터렉션 예약 취소
		GMessage()->GetSanctum().SendMessage(MessageContentSanctum::CANCEL_RESERVED_INTERACTION, nullptr);
		return;
	}

	if (GetIsActiveInteraction())
	{
		if (FGsMessageHolder* messageMgr = GMessage())
		{
			// 인터렉션 수행 후 인터렉션 예약 제거
			messageMgr->GetGameObject().SendMessage(MessageGameObject::INTERACTION_BUTTON_CLICK, &_interactionMessageParam);
			messageMgr->GetSanctum().SendMessage(MessageContentSanctum::CANCEL_RESERVED_INTERACTION, nullptr);
		}
	}
}

void UGsGameObjectSanctumNexus::UpdateWidgetShowHPBar()
{
	AGsCampBase* campActor = GetCampActor();
	if (nullptr == campActor)
	{
		return;
	}

	bool bIsShowHpBar = IsShowHpBar();
	float hpRate = 0.f;

	if (bIsShowHpBar)
	{
		if (const FGsStatInfo* hpStat = GetCreatureStatInfoByType(StatType::HP))
		{
			hpRate = GetCurrentHPStatRate(hpStat);
		}
	}

	campActor->SetNameplateHpBar(bIsShowHpBar, hpRate);
}

void UGsGameObjectSanctumNexus::OnChangedStat(const FGsStatInfo& inStateInfo)
{
	Super::OnChangedStat(inStateInfo);	

	// 무슨 기능인진 모르지만 가져온다
	if (false == _ActorVisible)
	{
		return;
	}

	// 건설 게이지도 HP이다
	if (StatType::HP != inStateInfo.GetStatType())
	{
		return;
	}

	AGsCampBase* actorCamp = GetCampActor();
	if (nullptr == actorCamp)
	{
		return;
	}

	float hpRate = GetCurrentHPStatRate(&inStateInfo);
	actorCamp->SetHpBarWidgetProgress(hpRate);

	actorCamp->SetDammageEffect(hpRate);
}

bool UGsGameObjectSanctumNexus::OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId,
	uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat,
	HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage)
{
	if (AGsCampBase* campActor = GetCampActor())
	{
		campActor->PlayAnimationHit();
	}

	return Super::OnHit(Striker, SubStriker, StriketSkillId, StrikerNotifyId, ResultType,
		DamageType, DamageStat, HealType, Damage, PassivityOwner, IsNoDamage);
}

void UGsGameObjectSanctumNexus::OnDie()
{
	SetInteractionActive(false);

	if (AGsCampBase* campActor = GetCampActor())
	{
		campActor->PlayAnimationDestroy();
	}

	Super::OnDie();
}

void UGsGameObjectSanctumNexus::OnSanctumSataus(SanctumAreaId InSanctumId, bool bInCanInteract)
{
	_sanctumId = InSanctumId;
	_isInteractionActive = bInCanInteract;
	
	UpdateSanctumName(_sanctumId);

	SetInteractionActive(_isInteractionActive);
}

void UGsGameObjectSanctumNexus::AddInteractionGameId(int64 InGameId) 
{
	if (0 >= InGameId)
	{
		return;
	}

	_interactionGameIdList.AddUnique(InGameId);
	
	CheckInteractionEffect();
}

void UGsGameObjectSanctumNexus::RemoveInteractionGameId(int64 InGameId)
{
	if (0 >= InGameId)
	{
		return;
	}

	_interactionGameIdList.Remove(InGameId);

	CheckInteractionEffect();
}

void UGsGameObjectSanctumNexus::UpdateSanctumName(SanctumAreaId InSanctumId)
{
	AGsCampBase* actorCamp = GetCampActor();
	if (nullptr == actorCamp)
	{
		_strFormattedName = TEXT("");
		return;
	}

	// 성소 이름 설정
	if (const FGsSchemaSanctumData* sanctumData = FGsSanctumManager::GetTableSanctumData(InSanctumId))
	{
		actorCamp->SetSanctumName(sanctumData->nameText);
	}
	else
	{
		actorCamp->SetSanctumName(FText::GetEmpty());
	}

	// 길드가 없을 경우
	if (INVALID_GUILD_DB_ID == _guildDBId)
	{
		_strFormattedName.Empty();
		actorCamp->SetShowNamePanel(false);

		return;
	}

	// 수호탑 이름 설정

	// 길드명 prefix 붙이기
	FText textGuildName;
	FGsGuildHelper::GetFormattedGuildName(_guildName, _guildWorldId, false, textGuildName);

	FText textNexusName;
	if (const FGsSchemaSanctumNexusData* nexusData = FGsSanctumManager::GetTableSanctumNexusDataBySanctumId(_sanctumId))
	{
		textNexusName = nexusData->nameText;
	}

	// "길드명(서버) 수호탑이름" 으로 조합
	_strFormattedName = FString::Format(TEXT("{0} {1}"), { *(textGuildName.ToString()), *(textNexusName.ToString()) });

	// 액터에 이름 저장
	actorCamp->SetShowNamePanel(true);
	actorCamp->SetCampName(FText::FromString(_strFormattedName));
}

void UGsGameObjectSanctumNexus::UpdateInteractionState()
{
	SetInteractionActive(_isInteractionActive);
}

void UGsGameObjectSanctumNexus::OnBattleEnd()
{
	_isInteractionActive = false;
	_interactionGameIdList.Empty();
}

void UGsGameObjectSanctumNexus::OnConstructionStep(int32 InStep)
{
	// 액터가 없으면 수행하지 않음
	AActor* actor = GetActor();
	if (nullptr == actor)
	{
		return;
	}

	// 완성 되었다. 값이 제대로 세팅이 안되어 maxStep이 0일 경우는 배제
	if (0 != _maxConstructionStep &&
		_maxConstructionStep == InStep)
	{
		// 완성 액터 상태가 아니면 로드 수행
		if (false == _isCompleteCampState)
		{
			LoadCompleteCamp();
			SetInteractionActive(_isInteractionActive);
		}

		return;
	}

	// 미완 상태인데 액터 상태가 다르면 로드 수행 
	if (_isCompleteCampState)
	{
		LoadConstructCamp();
		SetInteractionActive(_isInteractionActive);
	}

	// 단계에 맞는 애니메이션 설정
	if (AGsCampBase* campActor = GetCampActor())
	{
		if (SPAWN_REASON::NORMAL == _spawnReasonType)
		{
			campActor->PlayAnimationConstructionStep(InStep, true);
		}
		else
		{
			campActor->PlayAnimationConstructionStep(InStep, false);
			SetSpawnReasonType(SPAWN_REASON::NORMAL);
		}

		// 인터렉션 이펙트 출력 체크
		CheckInteractionEffect();

		// 엠블렘 갱신
		campActor->SetCampEmblemIcon(_guildEmblemId);
	}
}

void UGsGameObjectSanctumNexus::LoadConstructCamp()
{
	const FGsSchemaSanctumNexusData* sanctumNexusData = FGsSanctumManager::GetTableSanctumNexusDataBySanctumId(_sanctumId);
	if (nullptr == sanctumNexusData)
	{
		return;
	}

	int32 levelIndex = _guildBuildingLevel - 1;
	if (false == sanctumNexusData->npcDataId.IsValidIndex(levelIndex))
	{
		return;
	}

	const FGsSchemaNpcData* npcData = sanctumNexusData->npcDataId[levelIndex].GetRow();
	if (nullptr == npcData)
	{
		return;
	}

	const FGsSchemaShapeData* shapeData = npcData->shapeId.GetRow();
	if (nullptr == shapeData)
	{
		return;
	}

	// 지우기 위한 이전 액터 백업
	AActor* actorBackup = _actor;

	AActor* actorNew = LoadActorInter(shapeData->bPClass, _spawnPos, _spawnRot);
	if (nullptr == actorNew)
	{
		return;
	}

	AGsCharacterBase* actorChar = Cast<AGsCharacterBase>(actorNew);
	if (nullptr == actorChar)
	{
		GSLOG(Error, TEXT("[Sanctum] Fail cast to AGsCharacterBase!"));
		return;
	}

	_creatureCommonInfo = npcData->commonInfoId.GetRow();

	InitializeActor(actorNew);

	// 이전 액터 삭제
	if (actorBackup)
	{
		actorBackup->Destroy();
		actorBackup = nullptr;
	}
}

void UGsGameObjectSanctumNexus::LoadCompleteCamp()
{
	const FGsSchemaSanctumNexusData* sanctumNexusData = FGsSanctumManager::GetTableSanctumNexusDataBySanctumId(_sanctumId);
	if (nullptr == sanctumNexusData)
	{
		return;
	}

	int32 levelIndex = _guildBuildingLevel - 1;
	if (false == sanctumNexusData->completeCampShapeIdList.IsValidIndex(levelIndex))
	{
		return;
	}

	const FGsSchemaShapeData* shapeData = sanctumNexusData->completeCampShapeIdList[levelIndex].GetRow();
	if (nullptr == shapeData)
	{
		return;
	}

	// 지우기 위한 이전 액터 백업
	AActor* actorBackup = _actor;

	AActor* actorNew = LoadActorInter(shapeData->bPClass, _spawnPos, _spawnRot);
	if (nullptr == actorNew)
	{
		return;
	}

	if (_npcData)
	{
		_creatureCommonInfo = _npcData->commonInfoId.GetRow();
	}

	InitializeActor(actorNew);

	// 이전 액터 삭제
	if (actorBackup)
	{
		actorBackup->Destroy();
		actorBackup = nullptr;
	}
}

AActor* UGsGameObjectSanctumNexus::LoadActorInter(const FSoftObjectPath& InPath,
	const FVector& InSpawnPos, const FRotator& InSpawnRot)
{
	UWorld* world = GetWorld();
	if (nullptr == world)
	{
		return nullptr;
	}

	UClass* bpClass = UGsPathManager::GetClassStatic(InPath);
	if (nullptr == bpClass)
	{
		GSLOG(Error, TEXT("[Sanctum] Fail to load class: %s"), *(InPath.ToString()));
		return nullptr;
	}

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* spawnActor = world->SpawnActor(bpClass, &InSpawnPos, &InSpawnRot, spawnParam);
	if (nullptr == spawnActor)
	{
		GSLOG(Error, TEXT("[Sanctum] Fail to spawn campActor: %s"), *(InPath.ToString()));
		return nullptr;
	}

	return spawnActor;
}

void UGsGameObjectSanctumNexus::InitActorConstructCamp(AActor* InActor)
{
	AGsCharacterBase* actorChar = Cast<AGsCharacterBase>(InActor);
	if (nullptr == actorChar)
	{
		GSLOG(Error, TEXT("[Sanctum] Fail cast to AGsCharacterBase!"));
		return;
	}

	if (_npcData)
	{
		_creatureCommonInfo = _npcData->commonInfoId.GetRow();
	}

	actorChar->ActiveGravity(false);
	if (UCapsuleComponent* capsule = actorChar->GetCapsuleComponent())
	{
		capsule->SetEnableGravity(false);
	}

	AGsCamp* actorCamp = Cast<AGsCamp>(InActor);
	if (nullptr == actorCamp)
	{
		GSLOG(Error, TEXT("[Sanctum] Fail cast to AGsCamp!"));
		return;
	}

	actorCamp->SetIsSanctum(true);

	SetCapsuleAndTracePos(actorCamp, _spawnPos, false, _creatureCommonInfo);

	UpdateSanctumName(_sanctumId);
	actorCamp->SetCampEmblemIcon(_guildEmblemId);

	// update hpbar show
	UpdateWidgetShowHPBar();

	SetInteractionActive(_isInteractionActive);

	// 액터 로딩 전 단계가 올 경우에 대한 처리
	OnConstructionStep(_currConstructionStep);

	// 인터렉션 예약 체크 타이밍에 액터 없는 경우를 대비하여 호출함. 예약없으면 어짜피 수행 안됨.
	FGsSanctumMsgParamSanctumId param(INVALID_SANCTUM_AREA_ID, GetGameId());
	GMessage()->GetSanctum().SendMessage(MessageContentSanctum::TRY_START_RESERVED_INTERACTION, &param);
}

void UGsGameObjectSanctumNexus::InitActorCompleteCamp(AActor* InActor)
{
	AGsCompleteCamp* actorCamp = Cast<AGsCompleteCamp>(InActor);
	if (nullptr == actorCamp)
	{
		GSLOG(Error, TEXT("[Sanctum] Fail cast to AGsCompleteCamp!"));
		return;
	}	

	actorCamp->SetIsSanctum(true);

	// 액터 세팅
	UpdateSanctumName(_sanctumId);
	actorCamp->SetCampEmblemIcon(_guildEmblemId);
	actorCamp->SetSpawnIndex(SPAWN_REASON::NORMAL == _spawnReasonType ? 0 : 1);
	actorCamp->ChangeAnimState(EGsStateCampComplete::PreSpawn_CompleteCamp);
	actorCamp->SetHpBarAndNameWidgetVisibility(true);
	actorCamp->OnIdleEffect();

	if (SPAWN_REASON::NORMAL == _spawnReasonType)
	{
		UGsSoundPlayer* soundPlayer = GSoundPlayer();
		if (nullptr != soundPlayer)
		{
			soundPlayer->PlaySound2D(FindSoundRowName(SOUND_TYPE::ConstructComplete));
		}
	}

	SetCapsuleAndTracePos(actorCamp, _spawnPos, true, _creatureCommonInfo);

	UpdateWidgetShowHPBar();

	SetInteractionActive(_isInteractionActive);
}

void UGsGameObjectSanctumNexus::OnClickTargeting()
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
	lockOnHandler->SetLockOnTarget(this);
}

void UGsGameObjectSanctumNexus::OnStartInteractionByProp(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	// 인터렉션 시작 시 끄기
	if (const FGsPropInteractionMessageParamContents* param = 
		InParam->Cast<const FGsPropInteractionMessageParamContents>())
	{
		if (GetGameId() == param->_gameId)
		{
			SetInteractionActive(false);
		}
	}
}

void UGsGameObjectSanctumNexus::OnCancelInteractionByProp(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	// 인터렉션 캔슬 시 다시 켜기
	if (const FGsPropInteractionMessageParamContents* param =
		InParam->Cast<const FGsPropInteractionMessageParamContents>())
	{
		if (GetGameId() == param->_gameId)
		{
			SetInteractionActive(_isInteractionActive);
		}
	}
}

void UGsGameObjectSanctumNexus::OnDespawnRemotePlayer(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	if (const FGsPrimitiveUInt64* param = InParam->Cast<const FGsPrimitiveUInt64>())
	{
		RemoveInteractionGameId(param->_data);
	}
}

void UGsGameObjectSanctumNexus::OnGuildJoin(const IGsMessageParam*)
{	
	CancelPlayerInteraction();
}

void UGsGameObjectSanctumNexus::OnGuildLeave(const IGsMessageParam*)
{
	CancelPlayerInteraction();
}

void UGsGameObjectSanctumNexus::OnGuildChangeMyAuthority(const IGsMessageParam*)
{
	CancelPlayerInteraction();
}

void UGsGameObjectSanctumNexus::CancelPlayerInteraction()
{
	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		if (UGsGameObjectLocalPlayer* localPlayer =
			Cast<UGsGameObjectLocalPlayer>(gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer)))
		{
			localPlayer->CheckInteractionSanctumState(this);
		}
	}

	SetInteractionActive(_isInteractionActive);
}

void UGsGameObjectSanctumNexus::SetGuildInfo(GuildDBId InGuildDBId, GuildEmblemId InGuildEmblemId, const FString& InGuildName,
	WorldId InWorldId, int32 InGuildBuildingLevel)
{
	_guildDBId = InGuildDBId;
	_guildEmblemId = InGuildEmblemId;
	_guildName = InGuildName;
	_guildWorldId = InWorldId;
	_guildBuildingLevel = InGuildBuildingLevel;
}

void UGsGameObjectSanctumNexus::SetGuildEmblem(GuildEmblemId InGuildEmblemId)
{
	_guildEmblemId = InGuildEmblemId;

	if (AGsCampBase* campActor = GetCampActor())
	{
		campActor->SetCampEmblemIcon(_guildEmblemId);
	}
}

void UGsGameObjectSanctumNexus::CheckInteractionEffect()
{
	if (_isCompleteCampState)
	{
		return;
	}

	if (AGsCampBase* actor = GetCampActor())
	{
		if (AGsCamp* actorCamp = Cast<AGsCamp>(actor))
		{
			if (0 < _interactionGameIdList.Num())
			{
				actorCamp->OnInteractionEffect();
			}
			else
			{
				actorCamp->OffInteractionEffect();
			}
		}
	}
}

AGsCampBase* UGsGameObjectSanctumNexus::GetCampActor()
{
	if (AActor* actor = GetActor())
	{
		if (actor->IsValidLowLevel())
		{
			return Cast<AGsCampBase>(actor);
		}
	}

	return nullptr;
}

bool UGsGameObjectSanctumNexus::IsMyGuild()
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (INVALID_GUILD_DB_ID != _guildDBId)
		{
			if (guildMgr->GetGuildDBId() == _guildDBId)
			{
				return true;
			}
		}
	}

	return false;
}

bool UGsGameObjectSanctumNexus::IsShowHpBar()
{
	// 우리 기사단 수호탑이면 무조건 표시
	if (IsMyGuild())
	{
		return true;
	}

	// 옵션 설정에 따름
	if (UGsOptionManager* optionMgr = GOption())
	{
		return optionMgr->GetIsShowHPbar();
	}

	return false;
}

float UGsGameObjectSanctumNexus::GetCurrentHPStatRate(const FGsStatInfo* In_hpStat)
{
	if (In_hpStat &&
		StatType::HP == In_hpStat->GetStatType())
	{
		int32 statValue = In_hpStat->GetStatValue();
		if (0 < statValue)
		{
			int32 maxStatValue = In_hpStat->GetMaxStatValue();

			if (0 < maxStatValue)
			{
				float hp = (statValue * 100 / static_cast<float>(maxStatValue)) + 0.005f;
				hp *= 0.01f;

				return hp;
			}
		}
	}

	return 0.f;
}

const FString UGsGameObjectSanctumNexus::FindSoundRowName(SOUND_TYPE InType)
{
	FString result;
	switch (InType)
	{
	case SOUND_TYPE::ConstructComplete:
		result = "BuildCompleteCamp";
		break;
	case SOUND_TYPE::ConstructHit:
		result = "BuildHitCamp";
		break;
	case SOUND_TYPE::ConstructStart:
		result = "BuildStartCamp";
		break;
	case SOUND_TYPE::Destroy:
		result = "DestroyCamp";
		break;
	}

	return result;
}
