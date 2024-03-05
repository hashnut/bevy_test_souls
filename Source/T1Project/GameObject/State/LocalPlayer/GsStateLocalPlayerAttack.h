// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
* Local 공격 상태
*/
class T1PROJECT_API FGsStateLocalPlayerAttack : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerAttack>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	//virtual bool Ready(UGsGameObjectBase* Owner) override;
	virtual void ReEnter(UGsGameObjectBase* Owner) override;
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;

public:
	virtual bool AllowMotionStop() const override { return false; }

private:
	// 캐스트 스킬 사용전 거리 판단
	bool IsInsideCastingRange(UGsGameObjectBase* Owner, const struct FGsSchemaSkillSet* SkillData) const;
	// 스킬 캔슬 가능 상태일때 이동 전환
	bool CanCancel(UGsGameObjectBase* Owner) const;
	// 타겟이 없어도 발동되는 스킬 타입 정의
	bool IsNoTargetingSkill(const struct FGsSchemaSkillSet* SkillData) const;
	// 타겟형 캐스팅 타입 처리
	bool TargetCastingProcess(class UGsGameObjectBase* Owner, class UGsGameObjectBase* Target, 
		class FGsSkillHandlerLocalPlayer* Skillhandler,	const struct FGsSchemaSkillSet* SkillData);
	bool SelfCastingProcess(class UGsGameObjectBase* Owner, class FGsSkillHandlerLocalPlayer* Skillhandler,
		const struct FGsSchemaSkillSet* SkillData);
	// 스킬 실행 처리
	void PlaySkill(UGsGameObjectBase* Owner);
};
