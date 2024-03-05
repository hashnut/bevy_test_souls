// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Classes/GsManager.h"
#include "../../EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "../GsScopeHolder.h"
#include "GsSkillPreviewManager.generated.h"

// 각 파트별 GameObject의 시작 GameId를 지정
enum class EGsSkillPreivewGameObjectStartID
{
	OwnerPlayer = 0,
	PartyPlayer = 1,
	EnemyPlayer = 3,
	Projectile = 10,
};

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsSkillPreviewManager : public UObject, public IGsManager, public IGsEventProgressInterface
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class AGsActorSkillPreviewSceneObject* _sceneObject = nullptr;

	UPROPERTY()
	class UGsGameObjectBase* _owner = nullptr;

	UPROPERTY()
	TArray<class UGsGameObjectBase*> _listPartyPlayer;

	UPROPERTY()
	TArray<class UGsGameObjectBase*> _listEnemyPlayer;

	UPROPERTY()
	TArray<class UGsGameObjectBase*> _listProjectile;

	const struct FGsSchemaSkillPreviewSet* _skillPreviewData = nullptr;
	FTimerHandle _delayTimer;
	// 내 캐릭터 성별 정보
	CreatureGenderType _currentGenderType = CreatureGenderType::FEMALE;
	// 내 캐릭터 무기 정보
	CreatureWeaponType _currentWeaponType = CreatureWeaponType::MAX;

	class USceneComponent* _partyDummyScene[2] = { nullptr, };
	class USceneComponent* _enemyDummyScene[3] = { nullptr, };

private:
	// 네트워크 처리 대응용 동기화 변수 설정
	// 캐릭터 최종 위치, 방향 정보
	FVector _ownerPosition;
	FVector _ownerDir;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	virtual void Update(float inTick) override;

	// 이벤트 처리
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

	// 씬 오브젝트 스폰
	class AGsActorSkillPreviewSceneObject* SpawnSceneObject();
	// 씬 제거
	void DespawnSceneObject();

public:
	// 스킬 실행
	void PlaySkillPreview(int In_SkillId, uint8 In_SkillLevel = 1, const struct FGsSchemaCostumeCommon* In_CostumeData = nullptr);
	void PlaySkillPreview(const struct FGsSchemaSkillSet* In_SkillData, uint8 In_SkillLevel = 1,
		const struct FGsSchemaCostumeCommon* In_CostumeData = nullptr);

	// 네트워크 처리 대응용 동기화 함수 설정
public:
	// 스킬 시작전 동기화 대응 변수들을 초기화
	void ClearSyncVariable();

	// 내캐릭터 위치 정보
	void SetOwnerPostion(const FVector& In_Pos);
	void SetOwnerDirection(const FVector& In_Dir);
	const FVector& GetOwnerPostion() const;
	const FVector& GetOwnerDirection() const;
	// SkillCategory에 따른 SkillStartType 정의
	SkillStartType GetSkillStartType(const struct FGsSchemaSkillSet* In_SkillData) const;

	// 프로젝타일 스폰
	class UGsGameObjectBase* SpawnProjectile(const struct FGsSchemaProjectileSet* In_ProjectileData,
		const FVector& In_Pos, const FVector& In_Dir, uint32 In_SkillId);
	// 스킬 사용 요청
	void ExcuteStartSkill(class UGsGameObjectBase* In_TargetObject, struct PD::SC::PKT_SC_START_SKILL_READ* In_Packet);	
	// 상태이상 발동
	void ExecuteNotify(uint32 In_SkillId, int64 In_CastObjectID, int64 In_TargetObjectID,
		const struct FGsSchemaSkillNotifySet* In_NotifyData, int64 In_SubGameId);

private:
	// 모든 배치 오브젝트 설정
	bool SetupGameObject(const FGsSchemaSkillSet* In_SkillData, const struct FGsSchemaCostumeCommon* In_CostumeData);

	class UGsGameObjectBase* SpawnGameObject(const FSoftObjectPath& In_Path, 
		class USceneComponent* In_Attach, const struct FGsSpawnParam* In_SpawnParam);
	void DespawnGameObject(class UGsGameObjectBase* In_GameObject);

	// 위치 설정
	void InitializeLocationAndRotation(class USceneComponent* In_DummyScene, UGsGameObjectBase* In_GameObject,
		const FTransform& In_Transform);

	// 데미지 처리
	void ExcuteDamage(class UGsGameObjectBase* In_TargetObject, struct PD::SC::PKT_SC_SKILL_DAMAGED_READ* In_Packet);
	void ExcuteHeal(class UGsGameObjectBase* In_TargetObject, struct PD::SC::PKT_SC_SKILL_HEALED_READ* In_Packet);
	void Internal_ExecuteAbnormality(class UGsGameObjectBase* In_TargetObject,
		AbnormalityApplyType In_ApplyType, int In_Id);

	// 애니메이션 정지 및 노티파이 정리
	void CancelAnimation(class UGsGameObjectBase* In_GameObject);

	// 상태이상 정리
	void CancelAbnormality(class UGsGameObjectBase* In_GameObject);

	UFUNCTION()
	void OnActorDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void OnCostumeEffectLoadComplate(AActor* In_Actor);

public:
	// 모든 설정된 배치 오브젝트 제거
	void DespawnAllGameObject();
	void ClearTimer();

	class UGsGameObjectBase* FindObject(int64 In_GameId);
	void GetTargetList(OUT TArray<class UGsGameObjectBase*>& Out_TargetList, int64 In_CasterId, IffApplyType In_ApplyType);
	void GetTargetList(OUT TArray<class UGsGameObjectBase*>& Out_TargetList, int64 In_CasterId, const struct FGsSchemaSkillTargetFilter* In_TargetFilter);
	FRotator GetObjectRotation(int64 In_GameId);

	// 씬에 부착된 카메라 정보 얻기
	class UCameraComponent* GetSceneCamera() const;

	// 액터로 GameObject 찾기
	class UGsGameObjectBase* FindObjectByActor(class AActor* In_FindActor);

private:
	class UGsGameObjectBase* FindObject(TArray<class UGsGameObjectBase*>& In_FindList, int64 In_GameId);
};

#define GSSkillPreview() UGsScopeHolder::GetGameManagerFType<UGsSkillPreviewManager>(UGsScopeGame::EManagerType::SkillPreview)
