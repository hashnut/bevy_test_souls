#include "GsSkillCheckArray.h"
#include "GsSkillCheckBase.h"
//#include "DataSchema/Skill/GsSchemaSkillSet.h"
//#include "Skill/GsSkill.h"
#include "Skill/GsSkill.h"
#include "T1Project.h"
#include "Util/GsText.h"
#include "Util/GsClassUtil.h"
#include "../../Define/GsGameObjectDefine.h"
#include "../GsSkillHandlerLocalPlayer.h"
#include "UnrealTemplate.h"

void FGsSkillCheckArray::Initialize()
{
	for (IGsSkillCheckBase* checker : _checks)
	{
		checker->Initialize();
	}
}

void FGsSkillCheckArray::Finalize()
{
	for (IGsSkillCheckBase* checker : _checks)
	{
		checker->Finalize();
	}
}

EGsSkillCheckResultType FGsSkillCheckArray::ProcessCheck(const FGsSkill* NextSkill)
{
	if (NextSkill)
	{
		for (IGsSkillCheckBase* checker : _checks)
		{
			if (false == checker->ProcessCheck(NextSkill))
			{
				GSLOG(Warning, TEXT("SkillID [%d] : IsSkillAvailable failed reason [%s]"), NextSkill->GetSkillId(), *GetEnumToString(
					EGsSkillCheckResultType, checker->GetErrType()));

				// 에러 메세지 저장
				SetErrorMessage(checker->GetErrMessage());

				return checker->GetErrType();
			}
		}
	}
	
	return EGsSkillCheckResultType::Pass;
}

EGsSkillCheckResultType FGsSkillCheckArray::ProcessCheck(const class FGsSkill* NextSkill, int Index)
{
	if (NextSkill)
	{
		IGsSkillCheckBase* checker = GetCheckClass(Index);
		if (checker)
		{
			if (false == checker->ProcessCheck(NextSkill))
			{
				GSLOG(Warning, TEXT("SkillID [%d], index[%d], IsSkillAvailable failed reason[%s]"), NextSkill->GetSkillId(), Index, *GetEnumToString(
					EGsSkillCheckResultType, checker->GetErrType()));
				// 에러 메세지 저장
				SetErrorMessage(checker->GetErrMessage());

				return checker->GetErrType();
			}
		}
	}

	return EGsSkillCheckResultType::Pass;
}

void FGsSkillCheckArray::Add(IGsSkillCheckBase* inChecker)
{
	_checks.Add(inChecker);
}

void FGsSkillCheckArray::Remove(IGsSkillCheckBase* inChecker)
{
	if (nullptr != inChecker)
	{
		_checks.Remove(inChecker);
		delete inChecker;
	}	
}

void FGsSkillCheckArray::RemoveAll()
{
	for (IGsSkillCheckBase* checker : _checks)
	{
		checker->Finalize();
		delete checker;
	}
	_checks.Empty();
}

IGsSkillCheckBase* FGsSkillCheckArray::GetCheckClass(int Index) const
{
	if (_checks.Num() <= Index)
	{
		return nullptr;
	}
	return _checks[Index];
}

void FGsSkillCheckArray::SetErrorMessage(const FText& ErrMessage)
{
	_lastErrMessage = ErrMessage;
}

const FText& FGsSkillCheckArray::GetErrMessage() const
{
	return _lastErrMessage;
}
