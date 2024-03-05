#pragma once

#include "GsSkillCheckBase.h"

/*
  FGsSkillCheckTarget에서 체크해도 될것 같지만
  이타입은 에러 메세지 출력을 요구 할 것 같으므로 따로 정의해둔다.
*/
class T1PROJECT_API FGsSkillManualTargetRange : public IGsSkillCheckBase
{
	using Super = IGsSkillCheckBase;

private:
	class UGsGameObjectLocalPlayer* _local	= nullptr;
	class FGsTargetHandlerBase*	_target		= nullptr;

public:
	explicit FGsSkillManualTargetRange(class UGsGameObjectLocalPlayer* inLocal, class FGsTargetHandlerBase* inTarget) :
		_local(inLocal),
		_target(inTarget) {};
	virtual ~FGsSkillManualTargetRange() = default;

public:
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) override;

public:
	virtual EGsSkillCheckResultType GetErrType() const override;
	virtual FText GetErrMessage() const override;
};
