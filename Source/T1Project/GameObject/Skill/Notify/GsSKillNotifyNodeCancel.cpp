// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSKillNotifyNodeCancel.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "Management/GsMessageHolder.h"
#include "GameObject/State/GsGameObjectStateManager.h"

void FGsSKillNotifyNodeCancel::Initialize(UGsGameObjectBase* Owner, const IGsSkillRunnerData* SkillRunnerData,
	const FGsSchemaSkillNotify* NotifyInfo, const FGsSkillNetData* NetData, float OffsetRate)
{
	Super::Initialize(Owner, SkillRunnerData, NotifyInfo, NetData, OffsetRate);

	if (SkillRunnerData->GetType() == ActionCategory::SKILL)
	{
		if (const FGsSkillRunnerSKillData* skillData = static_cast<const FGsSkillRunnerSKillData*>(SkillRunnerData))
		{
			float rate = skillData->GetTableData()->cutRemainInfo.delayOffset / SkillRunnerData->GetTotalTime() * OffsetRate;
			_offset = SkillRunnerData->GetTotalTime() * rate;
		}
	}
}

void FGsSKillNotifyNodeCancel::Finalize()
{
	Super::Finalize();
	//IsActive = false;
}

void FGsSKillNotifyNodeCancel::Action(UGsGameObjectBase* Owner)
{
	Super::Action(Owner);

	//추후 LocalPlayer 전용 SkillNotify로 만들어야 할것 같다.
	if (EGsGameObjectType::LocalPlayer == Owner->GetObjectType())
	{		
		UGsGameObjectLocalPlayer* my = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
		FGsSkillHandlerLocalPlayer* skill = my->GetCastSkill<FGsSkillHandlerLocalPlayer>();
		if (FGsSkillRunnerLocalPlayer* runner = static_cast<FGsSkillRunnerLocalPlayer*>(skill->GetSkillRunner()))
		{
			// 스킬 캔슬가능 시점 Noti
			FGsGameObjectMessageParamSkill messageParam(Owner, UGsTableUtil::ConvertRow<int32>(runner->GetSkillData()) );
			GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_SKILL_CANCELABLE, &messageParam);
		}
	}

	
}

//콤보 SkillNode는 캔슬시 발동 되지 않음
//void FGsSKillNotifyNodeCancel::Cancel(UGsGameObjectBase* Owner)
//{
//	Super::Cancel(Owner);
//
//	// 켄슬 이벤트는 실행해야하고 콤보 액션은 중지
//	IsActive = false;
//}
