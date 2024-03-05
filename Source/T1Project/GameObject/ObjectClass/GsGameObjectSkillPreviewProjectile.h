// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectProjectile.h"
#include "Engine/EngineTypes.h"
#include "Shared/Client/SharedEnums/SharedProjectileEnum.h"
#include "GsGameObjectSkillPreviewProjectile.generated.h"

/**
 * 스킬 미리보기에서 사용되어질 클라이언트 전용 발사체 클래스
 */
UCLASS()
class T1PROJECT_API UGsGameObjectSkillPreviewProjectile final : public UGsGameObjectProjectile
{
	GENERATED_BODY()

private:
	TArray<FTimerHandle> _listTimeHandler;

	// ProjectileMoveType::EXPLOSION 타입일 경우만 설정
	ProjectileTriggerType _triggerType = ProjectileTriggerType::NONE;

	// ActionInfo 갯수, 인덱스와 동일
	TArray<int> _listActionCallCount;

	// 프로젝타일을 소환한 스킬 정보도 포함한다.
	uint32 _skillId = 0;

protected:
	virtual void InitializeActor(AActor* Owner) override;

	void CheckCollision(int Index);
	void Action(const struct FGsSchemaProjectileActionInfo* ActionInfo, TArray<class UGsGameObjectBase*>& ListHitObject);

	void ClearTimerEvent();

public:
	virtual void ActorDespawn() override;

	void SetSpawnSKillId(uint32 SkillId);
	uint32 GetSkillId() const;

protected:
	TArray<class UGsGameObjectBase*> CheckCollision(const struct FGsSchemaProjectileCollisionInfo& Collision,
		TArray<class UGsGameObjectBase*> TargetList);
};
