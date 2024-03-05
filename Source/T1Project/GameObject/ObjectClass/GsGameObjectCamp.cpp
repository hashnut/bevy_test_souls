// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/ObjectClass/GsGameObjectCamp.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/GsCampAnimInstance.h"
#include "ActorEx/GsCamp.h"
#include "ActorEx/GsCompleteCamp.h"
#include "ActorEx/GsCampBase.h"
#include "ActorComponentEx/GsHitEffectComponent.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorComponentEx/GsWidgetComponentButton.h"
#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerNonPlayer.h"
#include "GameObject/Npc/GsSchemaNpcBalance.h"
#include "GameObject/Npc/GsSchemaNpcData.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/LockOn/GsLockOnHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Scan/GsScanHandler.h"
#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"
#include "UI/UIContent/Billboard/GsUIBillboardNonPlayer.h"
#include "UI/UIContent/Billboard/GsUIBillboardCamp.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "DataSchema/GsSchemaInteractionData.h"
#include "DataSchema/GameObject/Camp/GsSchemaCampData.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Public/TimerManager.h"
#include "Sound/GsSoundPlayer.h"
#include "Data/GsDataContainManager.h"
#include "Guild/GsGuildPlayerData.h"
#include "Guild/GsGuildHelper.h"


#include "T1Project.h"

// Test
#include "DrawDebugHelpers.h"



void UGsGameObjectCamp::Initialize(const struct FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);
	SET_FLAG_TYPE(_objectType, UGsGameObjectCamp::GetObjectType());

	_interactionMessageParam._objectType = GetObjectType();
	_interactionMessageParam._targetGameID = GetGameId();

	//bak1210 : final 에서호출
	CreateSubClass();

	_campMgr = GSCamp();

	_spawnReasonType = SPAWN_REASON::NORMAL;
	_isCompleteBuildCamp = false;
	_isLoadCampActor = false;
	_buildingLevel = 0;
	_campID = 0;
	_guildWorldId = INVALID_WORLD_ID;

	BindMsg();
}

void UGsGameObjectCamp::InitializeActor(AActor* Owner)
{
	Super::InitializeActor(Owner);

	//bak1210 : CreateSubClass에서 옮김
	SetInteractionActive(true);
}

void UGsGameObjectCamp::RemoveSubClass()
{

	Super::RemoveSubClass();
}

void UGsGameObjectCamp::ActorDespawn()
{
	RemoveMsg();
	if (FGsCampManager* campMgr = GSCamp())
	{
		campMgr->DespawnCamp(GetGameId());
	}
	if (nullptr != _actor)
	{
		_actor->Destroy();
		_actor = nullptr;
	}

	Super::ActorDespawn();
}

void UGsGameObjectCamp::BindMsg()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr != msgMgr)
	{
		if (0 < _gameObjectMsgHandler.Num() || 0 < _guildMsgHandler.Num())
		{
			RemoveMsg();
		}

		_gameObjectMsgHandler.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_START, this,
			&UGsGameObjectCamp::StartInteraction));
		_gameObjectMsgHandler.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_CANCEL, this,
			&UGsGameObjectCamp::InteractionContentsByPropCancel));

		_guildMsgHandler.Emplace(msgMgr->GetGuild().AddUObject(MessageContentGuild::RECEIVE_JOIN_SUCCESS, this, &UGsGameObjectCamp::UpdateLocalPlayerGuildState));
		_guildMsgHandler.Emplace(msgMgr->GetGuild().AddUObject(MessageContentGuild::RECEIVE_LEAVE, this, &UGsGameObjectCamp::UpdateLocalPlayerGuildState));
		_guildMsgHandler.Emplace(msgMgr->GetGuild().AddUObject(MessageContentGuild::RECEIVE_CHANGE_MY_AUTHORITY, this, &UGsGameObjectCamp::UpdateLocalPlayerGuildState));
	}
}

void UGsGameObjectCamp::UpdateLocalPlayerGuildState(const IGsMessageParam* inMessage)
{
	UGsGameObjectLocalPlayer* localPlayer = GetCampMgr()->GetLocalPlayerObject();
	if (nullptr != localPlayer)
	{
		localPlayer->CheckInteractionCampState(this);
	}
	SetInteractionActive(true);
}

void UGsGameObjectCamp::RemoveMsg()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr != msgMgr)
	{
		for (TPair<MessageContentGuild, FDelegateHandle>& elem : _guildMsgHandler)
		{
			msgMgr->GetGuild().Remove(elem);
		}
		_guildMsgHandler.Empty();

		for (MsgGameObjHandle& elem : _gameObjectMsgHandler)
		{
			msgMgr->GetGameObject().Remove(elem);
		}
		_gameObjectMsgHandler.Empty();
	}
}

void UGsGameObjectCamp::OnChangedStat(const class FGsStatInfo& inStateInfo)
{
	Super::OnChangedStat(inStateInfo);

	if (false == _ActorVisible)
		return;

	bool showHPBar = IsShowHPBar();
	if (showHPBar == false)
	{
		if (StatType::HP == inStateInfo.GetStatType())
		{
			float hpRate = GetCurrentHPStatRate(&inStateInfo);
			if (nullptr != GetCompleteCampActor())
			{
				GetCompleteCampActor()->CheckDamageEffect(hpRate);
			}
		}
		return;
	}

	if (StatType::HP == inStateInfo.GetStatType())
	{
		float hpRate = GetCurrentHPStatRate(&inStateInfo);
		if (nullptr != GetCampActorBase())
		{
			GetCampActorBase()->SetHpBarWidgetProgress(hpRate);
		}
	}

	//if (StatType::HP == inStateInfo.GetStatType())
	//{
	//	float hp = (inStateInfo.GetStatValue() * 100 / (float)inStateInfo.GetMaxStatValue()) + 0.005f;
	//	if (nullptr != GetCampActorBase())
	//	{
	//		GetCampActorBase()->SetHpBarWidgetProgress(hp / 100.f);
	//	}
	//	//GSLOG(Error, TEXT("[UGsGameObjectCamp::OnChangedStat] - camp hp : %f"), hp / 100.f);
	//}

	//GSLOG(Error, TEXT("[UGsGameObjectCamp::OnChangedStat] - GetStatType : %d"), (uint8)inStateInfo.GetStatType());
	//GSLOG(Error, TEXT("[UGsGameObjectCamp::OnChangedStat] - GetStatValue : %d"), inStateInfo.GetStatValue());
}
float UGsGameObjectCamp::GetCurrentHPStatRate(const FGsStatInfo* In_hpStat)
{
	if (In_hpStat == nullptr)
	{
		return 0.0f;
	}

	if (StatType::HP != In_hpStat->GetStatType())
	{
		return 0.0f;
	}

	float hp = (In_hpStat->GetStatValue() * 100 / (float)In_hpStat->GetMaxStatValue()) + 0.005f;
	float hpRate = hp / 100.f;

	return hpRate;
}
AGsCampBase* UGsGameObjectCamp::GetCampActorBase()
{
	if (_isCompleteBuildCamp)
	{
		AActor* actor = GetActor();
		if (nullptr != actor)
		{
			return Cast<AGsCompleteCamp>(actor);
		}
	}
	else
	{
		AActor* actor = GetActor();
		if (nullptr != actor)
		{
			return Cast<AGsCamp>(actor);
		}
	}
	return nullptr;
}

void UGsGameObjectCamp::Update(float Delta)
{
	Super::Update(Delta);

	// For Check Log
	//DrawDebugInteractionRange();
}

// 인터랙션 디버그 범위 그리기
void UGsGameObjectCamp::DrawDebugInteractionRange()
{
	/*
	UWorld* world = GetWorld();
	if (world && _npcData)
	{
		// 시야 거리
		DrawDebugCircle(world, GetActor()->GetActorLocation(),
			_npcData->interactionRange, 100, FColor::Red, false, 1.f,
			0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
	}
	*/
}

// fsm 사용 안함
void UGsGameObjectCamp::InitializeFSM()
{
}

TSharedPtr<FGsMovementHandlerBase> UGsGameObjectCamp::CreateMovement()
{
	return nullptr;
}

TSharedPtr<FGsAbnormalityHandlerBase> UGsGameObjectCamp::CreateAbnormality()
{
	return MakeShareable(new FGsAbnormalityHandlerNonPlayer());
}

void UGsGameObjectCamp::OnIFFChanged()
{
	Super::OnIFFChanged();

	SetInteractionActive(true);
}

bool UGsGameObjectCamp::OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId,
	uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat,
	HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage)
{
	// Hit 처리
	SetHitAnim();

	return Super::OnHit(Striker, SubStriker, StriketSkillId, StrikerNotifyId, ResultType,
		DamageType, DamageStat, HealType, Damage, PassivityOwner, IsNoDamage);
}

void UGsGameObjectCamp::CompleteSpawn()
{
	_curStep = 0;
	_maxStep = 0;
}

void UGsGameObjectCamp::OnDie()
{
	SetInteractionActive(false);
	if (nullptr != GetCampActorBase())
	{
		GetCampActorBase()->BeginDestroyAnim();
	}

	Super::OnDie();
}

void UGsGameObjectCamp::OnReceiveStepPacket()
{
	SetCampStep(_curStep);
}

void UGsGameObjectCamp::SetCampStep(uint32 InStep)
{
	//GSLOG(Error, TEXT("[UGsGameObjectCamp::SetCampStep] - InStep : %d"), InStep);
	if (_maxStep == InStep)
	{
		if (_isCompleteBuildCamp)
			return;

		LoadCompleteCamp();
		SetInteractionActive(true);
		return;
	}

	if (false == _isLoadCampActor)
	{
		LoadConstructCamp();
		SetInteractionActive(true);
	}

	uint32 resultStep = 0 < InStep ? InStep - 1 : 0;
	AGsCamp* campActor = GetConstructCampActor();
	UAnimInstance* anim = GetCharacter()->GetAnimationInstance();
	UGsCampAnimInstance* campAnim = Cast<UGsCampAnimInstance>(anim);
	if (campAnim && campActor)
	{
		if (0 < resultStep && campAnim->GetCurStepNum() == resultStep)
			return;

		campAnim->SetCampCurrentStep(InStep);
		if (SPAWN_REASON::NORMAL == _spawnReasonType)
		{
			campAnim->ChangeState(EGsStateCamp::Building_Camp);
		}
		else
		{
			campAnim->ChangeState(EGsStateCamp::Idle_Camp);
			_spawnReasonType = SPAWN_REASON::NORMAL;
		}
	}
}

const FString UGsGameObjectCamp::FindSoundRowName(SOUND_TYPE InType)
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

void UGsGameObjectCamp::SetHitAnim()
{
	if (GetCampActorBase())
	{
		GetCampActorBase()->BeginHitAnim();
	}
}

// 위젯 인터랙션 버튼 show/hide 옵션 갱신
void UGsGameObjectCamp::UpdateWidgetShowInteractionButtonOption()
{
}

void UGsGameObjectCamp::OnClickInteract(bool In_isByWidget)
{
	if (_maxStep == _curStep)
		return;

	GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_BUTTON_CLICK, &_interactionMessageParam);
}

void UGsGameObjectCamp::SetConstructAffectNavigation(const bool InState)
{
	if (false == _isCompleteBuildCamp)
	{
		AGsCamp* campActor = GetConstructCampActor();
		if (campActor)
		{
			campActor->SetAffectNavigation(InState);
		}
	}
}

// 인터랙션 액티브 처리
void UGsGameObjectCamp::SetInteractionActive(bool In_isActive, int In_Index)
{
	UWidgetComponent* widgetComponent = nullptr;
	AGsCampBase* campActor = GetCampActorBase();
	if (nullptr == campActor)
		return;

	widgetComponent = campActor->GetInteractionWidgetComponent();
	if (nullptr == widgetComponent)
	{
		campActor->SetInteractionClickCallback(nullptr);

		if (UGsUIBillboardInteractionButton* interactionButton = campActor->GetInteractionButton())
		{
			interactionButton->ReleaseResource();
		}
		return;
	}
	if (false == In_isActive)
	{
		campActor->SetInteractionClickCallback(nullptr);

		if (UGsUIBillboardInteractionButton* interactionButton = campActor->GetInteractionButton())
		{
			interactionButton->ReleaseResource();
		}
		return;
	}

	bool isEnemyCamp = false;
	UGsGameObjectLocalPlayer* localPlayer = GetCampMgr()->GetLocalPlayerObject();
	if (nullptr != localPlayer)
	{
		isEnemyCamp = localPlayer->IsEnemy(this);
	}
	if (true == isEnemyCamp)
	{
		campActor->SetInteractionClickCallback([this](bool In_isByWidget)
			{
				OnClickTargeting();
			});

		if (UGsUIBillboardInteractionButton* interactionButton = campActor->GetInteractionButton())
		{
			interactionButton->ReleaseResource();
		}

		widgetComponent->SetActive(false);
		widgetComponent->SetVisibility(false);
		campActor->SetUseInteractionButton(false);
	}
	else
	{
		if (_isCompleteBuildCamp)
		{
			campActor->SetInteractionClickCallback(nullptr);

			if (UGsUIBillboardInteractionButton* interactionButton = campActor->GetInteractionButton())
			{
				interactionButton->ReleaseResource();
			}
		}
		else
		{
			campActor->SetInteractionClickCallback([this](bool In_isByWidget)
				{
					OnClickInteract(In_isByWidget);
				});
			if (UGsUIBillboardInteractionButton* interactionButton = campActor->GetInteractionButton())
			{
				interactionButton->SetData(this, NpcFunctionType::CAMP, In_Index);
			}

			widgetComponent->SetActive(true);
			widgetComponent->SetVisibility(true);
			campActor->SetUseInteractionButton(true);
		}
	}
}

void UGsGameObjectCamp::OnClickTargeting()
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

// 위젯 이름 show/hide 옵션 갱신
void UGsGameObjectCamp::UpdateWidgetShowNameOption()
{
}

void UGsGameObjectCamp::NetDeathAck(PD::SC::PKT_SC_CREATURE_DEATH_READ* Packet)
{
	// 매너 타겟에서 삭제
	GSAI()->RemoveMannerCheckTarget(Packet->DeadGameId());

	Super::NetDeathAck(Packet);
}

AGsCamp* UGsGameObjectCamp::GetConstructCampActor() const
{
	return _actor ? Cast<AGsCamp>(_actor) : nullptr;
}

AGsCompleteCamp* UGsGameObjectCamp::GetCompleteCampActor() const
{
	return _actor ? Cast<AGsCompleteCamp>(_actor) : nullptr;
}

void UGsGameObjectCamp::StartInteraction(const IGsMessageParam* inMessage)
{
	if (nullptr == inMessage)
		return;

	const FGsPropInteractionMessageParamContents* param = static_cast<const FGsPropInteractionMessageParamContents*>(inMessage);
	if (param == nullptr)
	{
		return;
	}
	if (param->_gameId != GetGameId())
	{
		return;
	}
	SetInteractionActive(false);
}

void UGsGameObjectCamp::InteractionContentsByPropCancel(const IGsMessageParam* inMessage)
{
	if (nullptr == inMessage)
		return;

	const FGsPropInteractionMessageParamContents* param = static_cast<const FGsPropInteractionMessageParamContents*>(inMessage);

	if (param == nullptr)
	{
		return;
	}

	if (param->_gameId != GetGameId())
	{
		return;
	}

	SetInteractionActive(true);
}

void UGsGameObjectCamp::OnClickInteractionBtn(const IGsMessageParam* inMessage)
{
}

void UGsGameObjectCamp::UpdateCampName(AGsCampBase* InCampActor)
{
	// 길드명 prefix 붙이기
	FText textGuildName;
	FGsGuildHelper::GetFormattedGuildName(_campOwnerGuildName, _guildWorldId, false, textGuildName);	

	// 수호탑 이름
	FText findText;
	if (FGsCampManager* campMgr = GSCamp())
	{
		campMgr->GetCampName(GetGameId(), findText);
	}

	// "길드명(서버) 수호탑이름" 으로 조합
	_formattedCampName = FString::Format(TEXT("{0} {1}"), { *(textGuildName.ToString()), *(findText.ToString()) });	

	// 액터에 이름 저장
	if (nullptr != InCampActor)
	{
		InCampActor->SetSanctumName(FText::FromString(TEXT(""))); // 비워 넣으면 Collapsed 처리 된다
		InCampActor->SetCampName(FText::FromString(_formattedCampName));
	}
}

void UGsGameObjectCamp::UpdateEmblemIcon()
{
	AGsCampBase* campActor = GetCampActorBase();
	if (nullptr != campActor)
	{
		campActor->SetCampEmblemIcon(_guildEmblemID);
	}
}

void UGsGameObjectCamp::StartInteractionEffect()
{
	if (nullptr != GetConstructCampActor())
	{
		GetConstructCampActor()->OnInteractionEffect();
	}
}

void UGsGameObjectCamp::StopInteractionEffect()
{
	if (nullptr != GetConstructCampActor())
	{
		GetConstructCampActor()->OffInteractionEffect();
	}
}

// widget hp bar show/hide cheat option
void UGsGameObjectCamp::UpdateWidgetShowHPBar()
{
	AGsCampBase* campActor = GetCampActorBase();

	if (campActor == nullptr)
	{
		return;
	}

	UGsUIBillboardCamp* widget = campActor->GetCampNameplateWidget();
	if (widget == nullptr)
	{
		return;
	}
	bool showHPBar = IsShowHPBar();
	if (showHPBar == true)
	{
		UpdateStatWidget(widget);
	}
	widget->ShowHPBar(showHPBar);
}

bool UGsGameObjectCamp::IsShowHPBar()
{
	// 1. cheat option on check
	bool isShowHPBarOption = GOption()->GetIsShowHPbar();
	// 2. check our camp(our camp always show)
	uint64 campGuildDBId = GetOwnerGuildDbid();
	uint64 localGuildDBId = GGuild()->GetGuildDBId();
	bool isMyGuildCamp = (campGuildDBId == localGuildDBId) ? true : false;

	bool isShowHPBar = (isMyGuildCamp == true) ? true : isShowHPBarOption;
	return isShowHPBar;
}

void UGsGameObjectCamp::UpdateStatWidget(UGsUIBillboardCamp* In_widget)
{
	if (In_widget == nullptr)
	{
		return;
	}

	const FGsStatInfo* hpStat = GetCreatureStatInfoByType(StatType::HP);
	if (hpStat == nullptr)
	{
		return;
	}

	float hpRate = GetCurrentHPStatRate(hpStat);
	In_widget->SetProgressBarValue(hpRate);
}
// after set db id , update show hp bar 
void UGsGameObjectCamp::OnCampNameNotifyFinish()
{
	// update hpbar show
	UpdateWidgetShowHPBar();
	UpdateEmblemIcon();
}

const float UGsGameObjectCamp::GetCampRadius()
{
	AGsCampBase* baseCamp = GetCampActorBase();
	if (nullptr != baseCamp)
	{
		FVector meshBoundsExtends = baseCamp->GetActorMeshBoundsExtends();
		return (meshBoundsExtends.X + meshBoundsExtends.Y) * 0.5f;
	}
	return 0.f;
}

void UGsGameObjectCamp::LoadConstructCamp()
{
	AGsCharacterBase* loadCampActor = GetCampMgr()->LoadConstructCampActor(GetGameId());
	if (nullptr == loadCampActor)
	{
		GetCampMgr()->ShowCampErrorPopupUI("loadCampActor is nullptr - Create Camp Object Error");
		return;
	}
	AGsCamp* campActor = Cast<AGsCamp>(loadCampActor);
	if (nullptr != campActor)
	{
		UpdateCampName(campActor);
		campActor->SetCampEmblemIcon(_guildEmblemID);

		if (_creatureCommonInfo)
		{
			float radius = DEFAULT_CAPSULE_RADIUS;
			float height = DEFAULT_CAPSULE_HEIGHT;

			radius = _creatureCommonInfo->cylinderRadius;
			height = _creatureCommonInfo->cylinderHeight;
			if (UCapsuleComponent* capsule = campActor->GetCapsuleComponent())
			{
				capsule->Deactivate();
				capsule->SetCapsuleSize(radius, height);
				// GameObjectData도 갱신
				if (FGsGameObjectDataCreature* data = GetCastData<FGsGameObjectDataCreature>())
				{
					data->SetScaledCapsuleRadius(capsule->GetScaledCapsuleRadius());
					data->SetScaledCapsuleHalfHeight(capsule->GetScaledCapsuleHalfHeight());
					data->SetApproachDistanceSquared(capsule->GetScaledCapsuleRadius() * capsule->GetScaledCapsuleRadius());
				}
			}
		}

		campActor->SetActorLocation(GetCampMgr()->GetTargetCampPos());
		
		FVector originVec;
		FVector extendVec;
		campActor->GetActorBounds(true, originVec, extendVec);
		campActor->SetNavigationAddOffsetValue(FVector(0.f, 0.f, extendVec.Z));
	}
	//_interactionRange = FGsInteractionHandlerLocalPlayer::GetInteractionRange(_npcData, this, true);
	_isLoadCampActor = true;

	// update hpbar show
	UpdateWidgetShowHPBar();
}

void UGsGameObjectCamp::LoadCompleteCamp()
{
	AActor* backupActor = _actor;
	// complete Camp Spawn
	AGsCharacterBase* charBase = GetCampMgr()->LoadCompleteCampActor(GetGameId(), SPAWN_REASON::NORMAL == _spawnReasonType);
	if (nullptr == charBase)
	{
		GSLOG(Error, TEXT("[UGsGameObjectCamp::SetCampStep] AGsCharacterBase cast is nullptr !!!"));
		return;
	}
	AGsCompleteCamp* completeCamp = Cast<AGsCompleteCamp>(charBase);
	if (nullptr == completeCamp)
	{
		GSLOG(Error, TEXT("[UGsGameObjectCamp::SetCampStep] AGsCompleteCamp cast is nullptr !!!"));
		return;
	}
	
	UpdateCampName(completeCamp);
	completeCamp->SetCampEmblemIcon(_guildEmblemID);

	bool showHPBar = IsShowHPBar();
	if (nullptr != GetConstructCampActor() && showHPBar == true)
	{
		float value = GetConstructCampActor()->GetHpBarValue();
		completeCamp->SetHpBarWidgetProgress(value);
	}

	if (backupActor)
	{
		backupActor->Destroy();
		backupActor = nullptr;
	}
	completeCamp->SetSpawnIndex(SPAWN_REASON::NORMAL == _spawnReasonType ? 0 : 1);
	completeCamp->ChangeAnimState(EGsStateCampComplete::PreSpawn_CompleteCamp);
	completeCamp->SetHpBarAndNameWidgetVisibility(true);
	completeCamp->OnIdleEffect();

	if (SPAWN_REASON::NORMAL == _spawnReasonType)
	{
		UGsSoundPlayer* soundPlayer = GSoundPlayer();
		if (nullptr != soundPlayer)
		{
			soundPlayer->PlaySound2D(FindSoundRowName(SOUND_TYPE::ConstructComplete));
		}
	}

	if (_creatureCommonInfo)
	{
		float radius = DEFAULT_CAPSULE_RADIUS;
		float height = DEFAULT_CAPSULE_HEIGHT;

		radius = _creatureCommonInfo->cylinderRadius;
		height = _creatureCommonInfo->cylinderHeight;

		if (UCapsuleComponent* capsule = completeCamp->GetCapsuleComponent())
		{
			capsule->SetCapsuleSize(radius, height);
			// GameObjectData도 갱신
			if (FGsGameObjectDataCreature* data = GetCastData<FGsGameObjectDataCreature>())
			{
				data->SetScaledCapsuleRadius(capsule->GetScaledCapsuleRadius());
				data->SetScaledCapsuleHalfHeight(capsule->GetScaledCapsuleHalfHeight());
				data->SetApproachDistanceSquared(capsule->GetScaledCapsuleRadius() * capsule->GetScaledCapsuleRadius());
			}
		}	
	}
	completeCamp->SetActorLocation(GetCampMgr()->GetTargetCampPos());

	FVector originVec;
	FVector extendVec;
	completeCamp->GetActorBounds(true, originVec, extendVec);
	completeCamp->SetNavigationAddOffsetValue(FVector(0.f, 0.f, extendVec.Z));

	//_interactionRange = FGsInteractionHandlerLocalPlayer::GetInteractionRange(_npcData, this, true);
	_isCompleteBuildCamp = true;

	// update hpbar show
	UpdateWidgetShowHPBar();
}

void UGsGameObjectCamp::SetCreatureCommonInfoData(const struct FGsSchemaCreatureCommonInfo* InCreatureCommonInfo)
{
	_creatureCommonInfo = InCreatureCommonInfo;
}

// complete Camp 일때, creature commomInfo 및 interaction range 갱신처리 (원복처리)
void UGsGameObjectCamp::RevertCreatureCommonInfoData()
{
	if (nullptr == _npcData)
	{
		_npcData = UGsTableUtil::FindRow<FGsSchemaNpcData>(_gameObjectData->GetTableId());
	}
	if (_npcData)
	{
		_creatureCommonInfo = _npcData->commonInfoId.GetRow();
	}
}

FGsCampManager* UGsGameObjectCamp::GetCampMgr()
{
	if (nullptr == _campMgr)
	{
		_campMgr = GSCamp();
	}
	return _campMgr;
}

void UGsGameObjectCamp::SetGameObjectActorVisible(bool In_isVisible)
{
	Super::SetGameObjectActorVisible(In_isVisible);

	if (_isCompleteBuildCamp)
	{
		AGsCompleteCamp* completeCamp = GetCompleteCampActor();
		if (completeCamp)
		{
			// false 면 off 처리
			if (false == In_isVisible)
			{
				completeCamp->OffIdleEffect(true);
				completeCamp->AllOffDamageEffect();
			}
			else
			{
				completeCamp->OnIdleEffect();
				if (FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>())
				{
					const FGsStatInfo* statInfo =  creatureData->GetCreatureStatInfo(StatType::HP);
					if (statInfo)
					{
						OnChangedStat(*statInfo);
					}
				}
			}
		}
	}
	else
	{

	}
}
