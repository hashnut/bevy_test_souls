#include "GsSkillRunnerPlayableEventLocalPlayer.h"
#include "Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"

#include "GameObject/Skill/Notify/GsSkillNotifyNodeBase.h"
#include "GameObject/Skill/Notify/GsSKillNotifyNodeCancel.h"

#include "GameObject/Skill/Notify/PlayableEvent/GsSkillNotifyNodePlayableEventDamage.h"
#include "GameObject/Skill/Notify/PlayableEvent/GsSkillNotifyNodePlayableEventEndSection.h"

#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"

FGsSkillRunnerPlayableEventLocalPlayer::FGsSkillRunnerPlayableEventLocalPlayer(TFunction<void(int, bool)> EndRun) : Super(EndRun)
{

}

void FGsSkillRunnerPlayableEventLocalPlayer::StartRunner(UGsGameObjectBase* Owner)
{
	Super::StartRunner(Owner);

	// 캔슬 노티파이 등록(스킬 타입만)
	if (_skillData.IsValid())
	{
		if (_skillData.GetTableData()->cutRemainInfo.delayOffset > 0.f)
		{
			TSharedPtr<FGsSkillNotifyNodeBase> cancelInfo = MakeShareable(new FGsSKillNotifyNodeCancel());
			cancelInfo->Initialize(_owner, &_skillData, nullptr, _skillNetData, _scaledOffsetRate);
			_currentInfo.Get()->_listNotify.Emplace(cancelInfo);
		}
	}
}

TSharedPtr<FGsSkillNotifyNodeBase> FGsSkillRunnerPlayableEventLocalPlayer::CreateSkillNode(const FGsSchemaSkillNotifySet* Data)
{
	if (Data->type == SkillNotifyType::DAMAGE || Data->type == SkillNotifyType::VARY_HP_MP)
	{
		return MakeShareable(new FGsSkillNotifyNodePlayableEventDamage());
	}
	else if (Data->type == SkillNotifyType::END_SECTION)
	{
		if (const FGsSchemaSkillSet* skillData = GetSkillData())
		{
			return MakeShareable(new FGsSkillNotifyNodePlayableEventEndSection(skillData->targetFilterInfo.iffType));
		}
	}

	return nullptr;
}

float FGsSkillRunnerPlayableEventLocalPlayer::GetSpeedScale() const
{
	// 플레이어블 이벤트 스킬 애니메이션은 몽타주 리소스에서 직접 설정하므로 Notify Tick계산에서만 사용하고 외부 정보는 기본값으로 전달
	return 1.f;
}