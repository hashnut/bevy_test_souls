// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/ObjectClass/GsGameObjectSanctumBase.h"
#include "Message/GsMessageContents.h"
#include "GsGameObjectSanctumNexus.generated.h"

class AActor;
class AGsCampBase;
class FGsStatInfo;
struct FGsSchemaCreatureCommonInfo;
struct IGsMessageParam;

/**
 * 성소 건설 ~ 건설 완료 (캠프에서 건설중/완료 나눠쓰던 로직 그대로 가져온다)
 * 
 * 일단 임시로 캠프 재활용
 * AGsCamp
 * AGsCompleteCamp
 */
UCLASS()
class T1PROJECT_API UGsGameObjectSanctumNexus final : public UGsGameObjectSanctumBase
{
	GENERATED_BODY()

public:
	using MsgHandleGuild = TPair<MessageContentGuild, FDelegateHandle>;

private:
	enum SOUND_TYPE : uint8
	{
		None = 0,
		ConstructComplete,
		ConstructStart,
		ConstructHit,
		Destroy,
	};

private:
	const FGsSchemaCreatureCommonInfo* _creatureCommonInfo = nullptr;

	// 소유 길드 정보
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;
	GuildEmblemId _guildEmblemId = INVALID_GUILD_EMBLEM_ID;
	FString _guildName;
	WorldId _guildWorldId = INVALID_WORLD_ID;
	int32 _guildBuildingLevel = 1;

	// 건설단계
	int32 _currConstructionStep = 0;
	int32 _maxConstructionStep = 0; // 완성단계

	// False: AGsCamp, True: AGsCompleteCamp
	bool _isCompleteCampState = false;

	TArray<MsgHandleGuild> _msgHandleGuildList;
	MsgGameObjHandleArray _msgHandleGameObjList;

	TArray<int64> _interactionGameIdList;

public:
	virtual ~UGsGameObjectSanctumNexus() = default;

public:
	virtual void InitializeActor(AActor* Owner) override;

protected:
	virtual void BindMessage() override;
	virtual void UnBindMessage() override;

	// IGsGameObjectInteractionInterface override
public:
	virtual void SetInteractionActive(bool In_isActive, int In_index = 0) override;	
	virtual void OnClickInteract(bool In_isByWidget) override;

public:
	virtual void UpdateWidgetShowHPBar() override;

protected:
	virtual void OnChangedStat(const class FGsStatInfo& inStateInfo) override;

public:
	virtual bool OnHit(UGsGameObjectBase * Striker, UGsGameObjectBase * SubStriker, uint32 StriketSkillId,
		uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat,
		HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage) override;
	virtual void OnDie() override;

	// UGsGameObjectSanctumBase override
public:
	virtual void OnSanctumSataus(SanctumAreaId InSanctumId, bool bInCanInteract) override;
	virtual void AddInteractionGameId(int64 InGameId) override;
	virtual void RemoveInteractionGameId(int64 InGameId) override;
	virtual void UpdateSanctumName(SanctumAreaId InSanctumId) override;
	virtual void UpdateInteractionState() override;
	virtual void OnBattleEnd() override;
	virtual bool IsNexus() const override { return true; }
	virtual GuildDBId GetGuildDBId() const override { return _guildDBId; }
	virtual GuildEmblemId GetGuildEmblemId() const override { return _guildEmblemId; }

public:
	void OnConstructionStep(int32 InStep);

protected:
	void LoadConstructCamp();
	void LoadCompleteCamp();

	AActor* LoadActorInter(const FSoftObjectPath& InPath, const FVector& InSpawnPos, const FRotator& InSpawnRot);

	void InitActorConstructCamp(AActor* InActor);
	void InitActorCompleteCamp(AActor* InActor);

public:
	// 타길드 수호탑 인터렉션 시 타게팅
	void OnClickTargeting();

protected:
	void OnStartInteractionByProp(const IGsMessageParam* InParam);
	void OnCancelInteractionByProp(const IGsMessageParam* InParam);
	void OnDespawnRemotePlayer(const IGsMessageParam* InParam);

	void OnGuildJoin(const IGsMessageParam*);
	void OnGuildLeave(const IGsMessageParam*);
	void OnGuildChangeMyAuthority(const IGsMessageParam*);

	void CancelPlayerInteraction();

public:
	void SetCurrConstructionStep(int32 InStep) { _currConstructionStep = InStep; }
	void SetMaxConstructionStep(int32 InStep) { _maxConstructionStep = InStep; }
	void SetGuildBuildingLevel(int32 InLevel) { _guildBuildingLevel = InLevel; } // 필요한가?

	void SetGuildInfo(GuildDBId InGuildDBId, GuildEmblemId InGuildEmblemId, const FString& InGuildName,
		WorldId InWorldId, int32 InGuildBuildingLevel);
	void SetGuildEmblem(GuildEmblemId InGuildEmblemId);

protected:
	void CheckInteractionEffect();

public:
	AGsCampBase* GetCampActor();
	bool IsMyGuild();

protected:
	bool IsShowHpBar();
	float GetCurrentHPStatRate(const FGsStatInfo* In_hpStat);
	const FString FindSoundRowName(SOUND_TYPE InType);
};
