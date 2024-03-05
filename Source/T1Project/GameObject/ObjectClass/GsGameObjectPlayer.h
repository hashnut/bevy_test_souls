// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectCreature.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageBoss.h"
#include "Message/GsMessageUserInfo.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Define/EGsPartsType.h"

#include "GsGameObjectPlayer.generated.h"

class FGsUseItemEffectHandler;
struct FGsSchemaShapeData;
/**
 * Player류에 해당 하는 base클래스
 */
UCLASS()
class T1PROJECT_API UGsGameObjectPlayer : public UGsGameObjectCreature
{
	GENERATED_BODY()

protected:
	//FGsGameObjectStateManager* UpperFsm;          //상체 Fsm
	// 유저 테이블 정보
	const struct FGsSchemaUserData* _pcData;	

protected:
	TArray<TPair<MessageUserInfo, FDelegateHandle>> _userInfoMsgList;
	MsgUIHandleArray _uiMsgList;
	MsgGameObjHandleArray _gameObjectMsgList;
	TArray<TPair<MessageBoss, FDelegateHandle>> _bossMsgList;

	// 디스폰 이유
	SPAWN_REASON		_despawnReason;

	// 워프 연출 노출 여부 (로비 -> 게임, 게임 -> 로비 등의 케이스 옵션 조절)
	bool _isWarpEffectOn = true;

	// Vehicle 이펙트 중복 처리 여부
	bool _isVehicleEffect = false;

public:
	UGsGameObjectPlayer();
	virtual ~UGsGameObjectPlayer();

public:
	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;	
	virtual void Update(float Delta) override;
	// visible 상태인지 갱신
	virtual void UpdateVisible() override;
	virtual void ChangeIdleType(EGsAnimStateIdleType inType) override;

public:
	virtual void CreateSubClass() override;
	virtual void ActorSpawned(AActor* Spawn) override;
	virtual void ActorDespawn() override;

protected:
	virtual void InitializeActor(class AActor* Owner) override;

	// 무기 변경에따라 AnimInstance 클래스가 동적으로 변경됨
	virtual void LoadAnimInstance(const FString& ClassPath);

public:
	virtual EGsGameObjectType GetObjectType() const override;

public:
	virtual TSharedPtr<class FGsInteractionHandlerBase> CreateInteractionHandler();
	virtual TSharedPtr<class FGsFairyHandlerPlayer> CreateFairyHandler();
	virtual TSharedPtr<class FGsAbnormalityHandlerBase> CreateAbnormality() override;

public:
	void OnUserBasicDataChange(const struct UserLookInfo& Data);
	void SetUserEquipData(const struct UserLookInfo& Data);

	bool IsTransPolymorph();
	bool IsVehicleMode();

public:
	virtual void NetChangeWeaponStartAck(CreatureWeaponType WeaponType);
	virtual void NetChangeWeaponFinishedAck(struct PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ* Packet);

public:
	virtual void OnDie() override;
	virtual void OnRevive() override;
	virtual void OnWarp();
	virtual void OnSpawn(bool In_isAlive, bool In_isNormalReason = true) {}
	virtual void OnSpawnFairyEffect();
	virtual void OnPlayerTeleportEffect(ETeleportEffectType teleportEffectType, FName socket = NAME_None, FVector scale = FVector::OneVector, FVector locOffset = FVector::ZeroVector, FRotator rotOffset = FRotator::ZeroRotator, USkeletalMeshComponent* mesh = nullptr);
	virtual void OnLearnSkill(SkillId InSkillId) {}

	virtual void OnVehicleEffect(bool useTalk = true) override;
	virtual void OnVehicleStartAnimation();
	virtual void OnRestoreVehicleEffect() override;

	virtual void UpdateTargetMark(bool inVisibleTargetMaker, bool inIsFirst = true) override;
	virtual void OnTargetClear(const struct IGsMessageParam* inParam) override;

	virtual void SetGameObjectActorVisible(bool In_isVisible) override;

public:
	// 아이템 사용
	void OnUseItem(ItemCategorySub In_category);

public:
	// return Value
	// case 0 : Skip Xray
	// case 1 : Do Xray
	virtual int UpdateCombatStatus(bool InImmediately = false) override;
	virtual void UpdateWeaponChange(CreatureWeaponType CurrWeaponType);

	virtual void UpdatePartyWeaponChange(CreatureWeaponType CurrWeaponType);
	// 스탯 갱신
	virtual void OnChangedStat(const class FGsStatInfo& inStatInfo) override;
	// GameObjectData 갱신
	virtual void OnGameObjectDataUpdate(EGsGameObjectDataMember MemberType) override;
	// 위젯 이름 show/hide 옵션 갱신
	virtual void UpdateWidgetShowNameOption() override;
protected:
	// 전투/평화 상태 변경시 소켓 Attach 변경 타이밍 콜백
	virtual void CallbackStatusAttachSocketStart();

public:
	void UpdateBillboard(EGsBillboardUpdateType InType);

public:
	void UpdateFairyEquip(FairyId id = 0);
	void RestoreEquipFairy();
	virtual void ChangeFairyEquip(FairyId id);

	// 페어리 액터 연출에 따른 visible 상태 갱신
	void UpdateFairyActorVisible();

public:
	void UpdateCostumeEquip(CostumeId lookatId, CostumeId costumeId, bool isHairOn = true);
	virtual void ChangeCostumeEquip(CostumeId lookatId, uint32 costumeId, bool isHairOn = true);
	void SetCustomizeDataBonScale(const CustomizeFaceData& InFaceData, const CustomizeBodyData& InBodyData);

protected:
	virtual void BindMessage() override;
	virtual void FinalizeMessageDelegateList() override;
	void OnLookInfoChanged(uint64 InGameId);

public:
	void OnChangeContribution(const struct IGsMessageParam* inParam = nullptr);
	void OnChangeAggroTarget(const struct IGsMessageParam* inParam = nullptr);

protected:
	void OnHideContribution(const struct IGsMessageParam* inParam = nullptr);
	void OnHideAggroTarget(const struct IGsMessageParam* inParam = nullptr);
	void OnHideFieldBossBillboard(const struct IGsMessageParam* inParam = nullptr);

protected:
	void OnTitleUpdateBillboard(const struct IGsMessageParam* inParam = nullptr);

public:
	UFUNCTION(BlueprintCallable, Category = "GameObject")
		class AGsCharacterPlayer* GetPlayerCharacter() const;

public:
	const struct FGsSchemaUserData* GetPcTable() const { return _pcData; }
	virtual const struct FGsSchemaCreatureCommonInfo* GetCommonInfoTable() const override;

	virtual class FGsInteractionHandlerBase* GetInteractionHandler();
	virtual class FGsFairyHandlerPlayer* GetFairyHandler();

	template<typename T = class FGsInteractionHandlerBase>
	T* GetCastInteraction()
	{
		return FGsClassUtil::FCheckStaticCast<T>(GetInteractionHandler());
	}
public:
	//virtual FGsGameObjectStateManager* GetUpperFSM() const;

public:
	void PopChatBubble(FText chat);
	void PopEmojiBubble(int32 emojiID);

	void SetDespawnReason(SPAWN_REASON In_reason)
	{
		_despawnReason = In_reason;
	}

	SPAWN_REASON GetDespawnReason()
	{
		return _despawnReason;
	}

	UserStatus  GetUserStatus();

public:
	virtual const FGsSchemaShapeData* GetPolymorphShapeData() override;

	bool IsWarpEffectOn() { return _isWarpEffectOn; }
	void SetIsWarpEffectOn(bool In_isWarpEffectOn) { _isWarpEffectOn = In_isWarpEffectOn; }
};
