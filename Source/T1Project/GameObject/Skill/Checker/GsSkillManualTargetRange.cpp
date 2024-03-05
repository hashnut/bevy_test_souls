
#include "GsSkillManualTargetRange.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "Skill/GsSkill.h"


EGsSkillCheckResultType FGsSkillManualTargetRange::GetErrType() const
{
	return EGsSkillCheckResultType::ManualTargetRange;
}

bool FGsSkillManualTargetRange::ProcessCheck(const FGsSkill* NextSkill)
{
	// 첫번째 타겟만
	if (UGsGameObjectBase* target = _target->GetTarget())
	{
		//  수동 이동 && 타겟 스킬 거리 체크
		const FGsSchemaSkillSet* tableData = NextSkill->_tableData;
		if (tableData->skillTargetMethod == SkillTargetMethod::TARGET)
		{
			if (FGsSkillHandlerLocalPlayer::IsPressType(FGsSkillHandlerLocalPlayer::Joystick))
			{
				float approachRange = FGsSkillHandlerBase::GetSkillApproachRange(_local, tableData);
				if (approachRange * approachRange < _local->GetTargetDistSquared(target))
				{
					return false;
				}
			}
		}
	}

	return true;
}

FText FGsSkillManualTargetRange::GetErrMessage() const
{
	FText findText;
	// FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_Target"), findText);
	return findText;
}

