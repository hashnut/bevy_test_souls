// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAbnormalityHandlerNonPlayer.h"
#include "Animation/GsAnimInstanceState.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "../ObjectClass/GsGameObjectCreature.h"

void FGsAbnormalityHandlerNonPlayer::OnActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData,
	bool IsNewDataAdded, bool isSpawnEffect)
{
	Super::OnActive(InstanceId, inData, IsNewDataAdded, isSpawnEffect);

	if (IsNewDataAdded)
	{
		if (IsAbnormalityEffectType(AbnormalityEffectType::CC_BURY))
		{
			if (UGsGameObjectCreature* nonPlayer = _owner->CastGameObject<UGsGameObjectCreature>())
			{
				nonPlayer->ChangeIdleType(EGsAnimStateIdleType::Abnormality);

				// 로컬 플레이어 강제 타겟 해제 요청
				FGsGameObjectMessageParam Param(_owner);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_TARGET_CLEAR_FORCE, &Param);
			}
		}

		FGsGameObjectMessageParamAbnormality param(_owner, InstanceId, false);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::ABNORMALITY_UPDATE_NOT_LOCAL, &param);
	}
}

void FGsAbnormalityHandlerNonPlayer::OnDeActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData)
{
	Super::OnDeActive(InstanceId, inData);

	if (false == IsAbnormalityEffectType(AbnormalityEffectType::CC_BURY))
	{
		FGsTargetHandlerBase* targetHandler = _owner->GetTargetHandler();
		if (UGsGameObjectCreature* nonPlayer = _owner->CastGameObject<UGsGameObjectCreature>())
		{
			// 기존 FGsTargetHandlerNonPlayer 로직에서 정의된 룰을 따름
			nonPlayer->ChangeIdleType(targetHandler->GetTarget() == nullptr ? 
				EGsAnimStateIdleType::Normal : EGsAnimStateIdleType::Battle);

			FGsGameObjectMessageParamAbnormality param(_owner, InstanceId, true);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::ABNORMALITY_UPDATE_NOT_LOCAL, &param);
		}
	}
}
