#pragma once

#include "GsSkillCheckBase.h"

/*
  타겟을 체크한다.
*/
class T1PROJECT_API FGsSkillCheckTarget : public IGsSkillCheckBase
{
	using Super = IGsSkillCheckBase;

	enum TargetCheckErrType
	{
		None,
		NotValid,
		NotPvP,
		Abnormality
	};

private:
	class UGsGameObjectLocalPlayer* _local	= nullptr;
	class FGsSkillDataBuilder*	_skillDataBuilder	= nullptr;

	TargetCheckErrType _currentErrType;

public:
	explicit FGsSkillCheckTarget(class UGsGameObjectLocalPlayer* inLocal, class FGsSkillDataBuilder* inSkillDataBuilder) :
		_local(inLocal),
		_skillDataBuilder(inSkillDataBuilder) {};
	virtual ~FGsSkillCheckTarget() = default;

public:
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) override;

public:
	virtual EGsSkillCheckResultType GetErrType() const override;
	virtual FText GetErrMessage() const override;
};
