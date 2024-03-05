#pragma once

#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"

/*
* 스킬 핸들러: 플레이어블 이벤트 로컬용
*/

struct FGsSchemaSkillSet;
struct FGsSkillNetData;
namespace PD
{
	namespace SC
	{
		struct PKT_SC_START_SKILL_READ;
	}
}
class UGsGameObjectBase;

class FGsSkillHandlerPlayableEventLocalPlayer : public FGsSkillHandlerLocalPlayer
{
	using Super = FGsSkillHandlerLocalPlayer;

private:
	int _reserveSkillId = 0;
	bool _isReserveSkill = false;

	// 글로벌 쿨타임
	bool _isGlobalCoolTime = false;
	// 시작 시간
	int64 _startTimeTick;

	float _maxTimeTick;
public:
	FGsSkillHandlerPlayableEventLocalPlayer() = default;
	virtual ~FGsSkillHandlerPlayableEventLocalPlayer() = default;

	// 가상 함수
public:
	virtual void Initialize(UGsGameObjectBase* Owner) override;
	virtual bool OnSkill(int ID) override;
	virtual void Update(float Delta) override;

	virtual bool OnReserveSkill() override;
	bool IsReserveSkill() const;
	virtual void ClearReserveSkill(uint32 Id = 0) override;

	virtual const FGsSkillNetData* GetAttackStateNetData() override
	{
		return GetNetSkillData();
	}
	// 예약 데이터 있는가
	virtual bool IsReserveData() override;
protected:	
	virtual void NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData) override;
	virtual void NetBuffSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData) override;

	virtual void CreateSkillRunner() override;
	
public:
	// 글로벌 쿨타임 시작
	void StartGlobalCooltime();
};