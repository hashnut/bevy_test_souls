// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectPlayer.h"
#include "GsGameObjectRemotePlayer.generated.h"

/**
 * 타 유저 캐릭터 관리 클래스
 * 모든것은 NonPlayer와 마찬가지로 서버 기반으로 동작해야 한다.
 */
UCLASS()
class T1PROJECT_API UGsGameObjectRemotePlayer final : public UGsGameObjectPlayer
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = GameObject, Meta = (AllowPrivateAccess = true))
	class AGsCharacterPlayer* _actor;
protected:
	// 스텟변경 핸들러
	FDelegateHandle		_changeStatHandle;

public:
	UGsGameObjectRemotePlayer();
	virtual ~UGsGameObjectRemotePlayer();

public:
	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	virtual void Update(float Delta) override;
	// iff 변경됨
	virtual void OnIFFChanged()override;

	// widget hp bar show/hide cheat option
	virtual void UpdateWidgetShowHPBar() override;

public:
	virtual void ActorDespawn() override;
	virtual void CreateSubClass() override;
	void SetRemotePlayerHidden();

protected:
	virtual void InitializeActor(AActor* Owner) override;

	virtual void InitializeFSM() override;
	
	/*virtual void InitializeSubClass() override;*/

private:
	void CallbackOverlapedCharacterBase(int32 inOverlapCount);

	void BlockLocalPlayer(bool InBlock);

protected:
	virtual FGsGameObjectData* CreateData(const struct FGsSpawnParam* SpawnParam) override;

	virtual TSharedPtr<FGsMovementHandlerBase>	CreateMovement() override;
	virtual TSharedPtr<FGsPartsHandlerBase> CreateParts() override;
	virtual TSharedPtr<FGsSkillHandlerBase> CreateSkill() override;
	virtual class TSharedPtr<FGsStatBase> CreateCreatureStat() override;

public:
	virtual void NetChangeWeaponFinishedAck(PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ* Packet) override;

public:
	virtual void OnStiffen(const FVector& StiffenPos, float fDuration) override;
	virtual void OnWarp()override;
	virtual void OnSpawn(bool In_isAlive, bool In_isNormalReason = true);

public:
	virtual void NetDeathAck(PD::SC::PKT_SC_CREATURE_DEATH_READ* Packet) override;

public:
	virtual EGsGameObjectType GetObjectType() const override;

public:
	virtual AActor* GetActor() const override;
	virtual void ClearActor() override;
	virtual void OnDie() override;

	UFUNCTION(BlueprintCallable, Category = "GameObject")
	class AGsCharacterPlayer* GetRemoteCharacter() const;


public:
	// 몸통 클릭 타게팅
	void OnClickTargeting();

};
