// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/GsGameObjectHandler.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Animation/GsAnimInstanceState.h"
#include "DataSchema/GsSchemaEnums.h"


namespace PD
{
	namespace SC
	{
		struct PKT_SC_START_SKILL_READ;
		struct PKT_SC_SKILL_FINISHED_READ;
		struct PKT_SC_SKILL_EFFECT_READ;
	}
}

// 서버 동기화용 데이터 구조체
// PD::SC::PKT_SC_START_SKILL_READ 에서 필요한 부분만 처리
struct T1PROJECT_API FGsSkillNetData
{
	uint32 _skillId;
	uint8 _skillLevel = 1;
	FVector _curPos;
	int16 _curDir;
	int64 _targetGameId;
	FVector _aimStartPos;
	FVector _aimTargetPos;
	SkillStartType _skillStartType;
};

class FGsIdleEmotionBaseRunner;
class FGsIdleEmotionLocalRunner;

/**
 * 
 */
class T1PROJECT_API FGsSkillHandlerBase : public IGsGameObjectHandler
{
protected:
	class UGsGameObjectBase* _owner			= nullptr;
	class FGsSkillRunnerBase* _skillRunner	= nullptr;
	FGsSkillNetData _skillNetData;

	// 소셜 데이터 캐싱
	TArray<const struct FGsSchemaSkillCommon*> _listSocial;
	// 현재 진핸중인 소셜Data
	const struct FGsSchemaSkillCommon* _currentSocial = nullptr;
	
	float _attackSpeedRate = 1.f; // 평타 공속
	float _castSpeedRate = 1.f; // 스킬 공속

	TSharedPtr<FGsIdleEmotionBaseRunner> _idleEmotionRunner;

public:
	FGsSkillHandlerBase() : IGsGameObjectHandler(GameObjectHandlerType::SKILL) {};
	virtual ~FGsSkillHandlerBase()	= default;

public:
	static float GetSkillAttackRange(class UGsGameObjectCreature* Owner, const struct FGsSchemaSkillSet* SkillData);
	static float GetSkillApproachRange(class UGsGameObjectCreature* Owner, const struct FGsSchemaSkillSet* SkillData);
	static EGsAnimSlotType GetRandomCombSkillSlot(bool IsCritical, int ComboIndex = 1);

public:
	virtual void Initialize(class UGsGameObjectBase* Owner) override;
	virtual void Update(float Delta) override;
	virtual void Finalize() override;

public:
	virtual void LoadSkill();
	virtual void LoadSocial();

protected:
	virtual void CreateSkillRunner();
	virtual void CreateIdleEmotionRunner();
	// 사용 애니메이션 리소스 데이터 정보 설정
	virtual void LoadAnimation();

protected:
	// 각 하위 클래스에서 상태 변환 처리
	virtual void StopCrowdControl() {};

public:
	virtual bool OnSkill(int Id);
	virtual void StopSkill(int Id = 0);
	virtual void CompulsionCancelSkill(int Id);

public:
	virtual void StartCommonAction(CommonActionType Type);
	virtual void EndCommonAction();

public:
	virtual void OnSocial(int Id);
	virtual void StopSocial(bool IsSocialIdleEnd = false);

protected:
	virtual void StartEmotionMotion(const FGsSchemaSkillCommon* motion);

public:
	// 서버 응답에 따른 처리
	const struct FGsSchemaSkillSet* NetSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet);	
	const struct FGsSchemaSkillSet* NetSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet);
	virtual void NetSkillFailure(uint32 Id, PD::Result ErrResult);
	virtual const struct FGsSchemaSkillNotifySet* NetSkillNotifyAction(PD::SC::PKT_SC_SKILL_EFFECT_READ* Packet);

protected:
	virtual void NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData);
	virtual void NetBuffSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData);
	virtual void NetActiveSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet, const FGsSchemaSkillSet* skillData);
	virtual void NetBuffSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet, const FGsSchemaSkillSet* skillData);

	virtual void CallbackEndRunner(int inSkillID, bool inIsEnd);

public:
	void SetAttackSpeedRate(float SpeedRate)		{ _attackSpeedRate = SpeedRate; }
	void SetCastSpeedRate(float SpeedRate)			{ _castSpeedRate = SpeedRate; }

public:
	void SetEmotionLock(bool set);
	virtual void SetAutoIdleEmotionEnable(bool enbable) {};
	virtual bool IsEndTimeIdleEmotionMotion();

public:
	virtual bool IsTargetedAvailableState() { return true; }

public:
	class FGsSkillRunnerBase* GetSkillRunner() const				{ return _skillRunner; }
	const FGsSkillNetData* GetNetSkillData()						{ return &_skillNetData; }
	const struct FGsSchemaSkillCommon* GetCurrentSocialData() const { return _currentSocial; }

	float GetAttackSpeedRate() const								{ return _attackSpeedRate; }
	float GetCastSpeedRate() const									{ return _castSpeedRate; }

	TWeakPtr<FGsIdleEmotionBaseRunner> GetIdleEmotionRunner()   { return _idleEmotionRunner; }

	void PlaySocialEmotion(UserSocialEmotion emotion);

	friend class FGsIdleEmotionBaseRunner;
	friend class FGsIdleEmotionLocalRunner;
};
