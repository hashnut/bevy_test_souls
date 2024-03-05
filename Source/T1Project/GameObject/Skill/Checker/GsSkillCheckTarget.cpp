

#include "GsSkillCheckTarget.h"
#include "../GsSkillHandlerLocalPlayer.h"
#include "../Runner/GsSkillRunnerBase.h"


#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/Stat/GsStatBase.h"

#include "Skill/GsSkill.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


EGsSkillCheckResultType FGsSkillCheckTarget::GetErrType() const
{
	return EGsSkillCheckResultType::Target;
}

bool FGsSkillCheckTarget::ProcessCheck(const FGsSkill* NextSkill)
{
	_currentErrType = TargetCheckErrType::None;
	if (UGsGameObjectBase* target = _skillDataBuilder->GetCheckTarget())
	{
		// 나자신 대상 Self 스킬은 타겟에 대한 검사를 할 필요가 없다
		if (target == _local &&  FGsSkillHandlerLocalPlayer::IsMySelfSkill(NextSkill->_tableData))
		{
			return true;
		}

		if (false == target->IsAble())
		{
			_currentErrType = TargetCheckErrType::NotValid;
			return false;
		}

		if (target->IsObjectType(EGsGameObjectType::RemotePlayer))
		{
			FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
			if (nullptr == gameFlow)
			{
				return false;
			}

			FGsStageManagerGame* stage = gameFlow->GetStageManagerGame();
			if (nullptr == stage)
			{
				return false;
			}

			FGsStageMode::Game stageMode = stage->GetCurrentStageMode();

			// NonPk 지역 검사
			if (stageMode == FGsStageMode::NON_PK)
			{
				_currentErrType = TargetCheckErrType::NotPvP;
				return false;
			}
			// 아레나 지역 검사
			else if (stageMode == FGsStageMode::ARENA)
			{
				const FGsSchemaSkillSet* tableData = NextSkill->_tableData;
				if (tableData->targetFilterInfo.iffType != IffApplyType::ENEMY)
				{
					_currentErrType = TargetCheckErrType::NotPvP;
					return false;
				}
			}
		}

		// 타겟 상태 체크
		if (FGsAbnormalityHandlerBase* abnormality = target->GetAbnormalityHandler())
		{
			if (abnormality->IsAbnormalityEffectType(AbnormalityEffectType::UTIL_BURY_KEEP_TARGET))
			{
				_currentErrType = TargetCheckErrType::Abnormality;
				return false;
			}
		}
	}
	// 타겟이 없을경우 타겟 스킬인지 확인
	else if (NextSkill->_tableData->skillTargetMethod == SkillTargetMethod::TARGET)
	{
		_currentErrType = TargetCheckErrType::NotValid;
		return false;
	}

	return true;
}

FText FGsSkillCheckTarget::GetErrMessage() const
{
	FText findText;
	if (_currentErrType == TargetCheckErrType::Abnormality)
	{
		FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_CannotAttackState"), findText);
	}
	else
	{
		FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_Target"), findText);
	}
	return findText;
}

