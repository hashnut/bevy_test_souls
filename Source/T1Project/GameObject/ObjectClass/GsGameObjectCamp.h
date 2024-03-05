// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/GsMessageGameObject.h"
#include "GameObject/Interaction/GsGameObjectInteractionInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Animation/GsCompleteCampAnimInstance.h"
#include "Message/GsMessageContents.h"
#include "GsGameObjectCamp.generated.h"

class AGsCamp;
class AGsCompleteCamp;
class AGsCampBase;
class FGsStatInfo;
class FGsCampManager;
class FGsMovementHandlerBase;
class FGsAbnormalityHandlerBase;
class UGsUIBillboardCamp;

struct IGsMessageParam;
/**
 * Camp ObjectClass
 */
UCLASS()
class T1PROJECT_API UGsGameObjectCamp final : public UGsGameObjectNonPlayerBase, public IGsGameObjectInteractionInterface
{
	GENERATED_BODY()

private:
	enum SOUND_TYPE : uint8
	{
		None = 0,
		ConstructComplete,
		ConstructStart,
		ConstructHit,
		Destroy,
	};

protected:
	TArray<TPair<MessageContentGuild, FDelegateHandle>> _guildMsgHandler;
	MsgGameObjHandleArray _gameObjectMsgHandler;

private:
	FGsInteractionMessageParam _interactionMessageParam;
	// 인터랙션 모드 flag
	bool _isInteractionActive = false;

	// 소유자에 대한 정보
	int64  _ownGameId{ 0 };
	GuildDBId _ownGuildId{ 0 };
	WorldId _guildWorldId = INVALID_WORLD_ID;
	FString	  _ownerName;
	FString _campOwnerGuildName;
	FString _formattedCampName;	

private:
	uint8 _guildEmblemID = 0;
	SPAWN_REASON _spawnReasonType = SPAWN_REASON::NORMAL;
	bool _isCompleteBuildCamp = false;
	bool _isLoadCampActor = false;
	int32 _curStep = 0;
	int32 _maxStep = 0;
	uint32 _campID = 0;
	uint16 _buildingLevel = 0;

private:
	FGsCampManager* _campMgr = nullptr;

public:
	virtual ~UGsGameObjectCamp() = default;

public:
	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	virtual void InitializeActor(AActor * Owner) override;
	virtual void Update(float Delta) override;
	// 위젯 이름 show/hide 옵션 갱신
	virtual void UpdateWidgetShowNameOption() override;
	virtual void OnIFFChanged() override;
	// widget hp bar show/hide cheat option
	virtual void UpdateWidgetShowHPBar() override;
	virtual void SetGameObjectActorVisible(bool In_isVisible) override;

protected:
	virtual void RemoveSubClass() override;
	virtual void InitializeFSM() override;
	virtual TSharedPtr<FGsMovementHandlerBase> CreateMovement() override;


protected:
	virtual TSharedPtr<FGsAbnormalityHandlerBase> CreateAbnormality() override;

public:
	virtual bool OnHit(UGsGameObjectBase * Striker, UGsGameObjectBase * SubStriker, uint32 StriketSkillId,
		uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat, 
		HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage) override;

public:
	virtual void ActorDespawn() override;
	virtual void OnDie() override;
	virtual void OnChangedStat(const class FGsStatInfo& inStateInfo) override;

public:
	void UpdateWidgetShowInteractionButtonOption();

public:
	void CompleteSpawn();
	void SetConstructAffectNavigation(const bool InState);
	void SetCampStep(uint32 InStep);
	void SetHitAnim();
	void OnReceiveStepPacket();
	void UpdateCampName(AGsCampBase* InCampActor);
	void UpdateEmblemIcon();
	void StartInteractionEffect();
	void StopInteractionEffect();
	void SetCreatureCommonInfoData(const struct FGsSchemaCreatureCommonInfo* InCreatureCommonInfo);
	void RevertCreatureCommonInfoData();
	AGsCampBase* GetCampActorBase();

private:
	void BindMsg();
	void RemoveMsg();
	void OnClickInteractionBtn(const IGsMessageParam * inMessage);
	void StartInteraction(const IGsMessageParam * inMessage);
	void InteractionContentsByPropCancel(const IGsMessageParam * inMessage);
	void UpdateLocalPlayerGuildState(const IGsMessageParam * inMessage);
	void UpdateStatWidget(UGsUIBillboardCamp * In_widget);

private:
	const FString FindSoundRowName(SOUND_TYPE InType);
	FGsCampManager* GetCampMgr();

private:
	void LoadConstructCamp();
	void LoadCompleteCamp();

public:
	AGsCamp* GetConstructCampActor() const;
	AGsCompleteCamp* GetCompleteCampActor() const;

public:
	// 타게팅
	void OnClickTargeting();

public:
	// 인터랙션 액티브 처리
	virtual void SetInteractionActive(bool In_isActive, int In_Index = 0) override;
	// 인터랙션 액티브인지
	virtual bool GetIsActiveInteraction() override { return _isInteractionActive; }
	// 인터랙션 버튼 클릭
	virtual void OnClickInteract(bool In_isByWidget) override;
	// 인터랙션 디버그 범위 그리기
	virtual void DrawDebugInteractionRange() override;

public:
	virtual void NetDeathAck(PD::SC::PKT_SC_CREATURE_DEATH_READ * Packet) override;

public:
	void SetCurStep(int32 InStep) { _curStep = InStep; }
	void SetMaxStep(int32 InMaxStep) { _maxStep = InMaxStep; }
	void SetCampID(uint32 InCampID) { _campID = InCampID; }
	void SetBuildingLevel(uint16 InLevel) { _buildingLevel = InLevel; }
	void SetCampOwnerGuildName(FString InName) { _campOwnerGuildName = InName; }
	void SetOwnerGuildDbid(const uint64 InDbid) { _ownGuildId = InDbid; }
	void SetGuildEmblemID(const uint8 InID) { _guildEmblemID = InID; }
	void SetSpawnReasonType(const SPAWN_REASON InType) { _spawnReasonType = InType; }
	void SetGuildWorldId(const WorldId InWorldId) { _guildWorldId = InWorldId; }

public:
	virtual EGsGameObjectType GetObjectType() const override { return EGsGameObjectType::Camp; }

public:
	const uint8 GetGuildEmblemID() { return _guildEmblemID; }
	const uint64 GetOwnerGuildDbid() { return _ownGuildId; }
	const uint32 GetCampID() { return _campID; }
	const uint16 GetBuildingLevel() { return _buildingLevel; }
	const bool IsCompleteBuildingCamp() { return _isCompleteBuildCamp; }
	FString GetFormattedCampName() const { return _formattedCampName; }
	bool IsShowHPBar();
	float GetCurrentHPStatRate(const FGsStatInfo * In_hpStat);
	void OnCampNameNotifyFinish();
	const float GetCampRadius();
};
