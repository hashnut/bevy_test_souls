// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillRunnerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "../Notify/GsSKillNotifyNodeProjectile.h"
#include "../Notify/GsSkillNotifyNodeCollision.h"
#include "../Notify/GsSkillnotifyNodeCancel.h"
#include "../Notify/GsSkillNotifyNodeGuideEffect.h"
#include "Management/GsMessageHolder.h"

FGsSkillRunnerLocalPlayer::FGsSkillRunnerLocalPlayer(TFunction<void(int, bool)> EndRun) : Super(EndRun)
{

}

void FGsSkillRunnerLocalPlayer::StartRunner(UGsGameObjectBase* Owner)
{
	Super::StartRunner(Owner);
	if (IsValid())
	{
		FGsGameObjectMessageParamSkill messageParam(_owner, _currData->GetId());
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_SKILL_START, &messageParam);
	}
}

TSharedPtr<FGsSkillNotifyNodeBase> FGsSkillRunnerLocalPlayer::CreateSkillNode(const FGsSchemaSkillNotifySet* Data)
{
	if (TSharedPtr<FGsSkillNotifyNodeBase> superNode = Super::CreateSkillNode(Data))
	{
		return superNode;
	}	

	//switch (data->NotifyType)
	//{
	//case EGsSkillNotifyType::Projectile:
	//	return MakeShareable(new FGsSKillNotifyNodeProjectile());
	//case EGsSkillNotifyType::Collision:
	//case EGsSkillNotifyType::GroupCollision:
	//case EGsSkillNotifyType::RandomCollision:
	//	return MakeShareable(new FGsSkillNotifyNodeCollision());
	//case EGsSkillNotifyType::Cancel:
	//	return MakeShareable(new FGsSKillNotifyNodeCancel());
	//case EGsSkillNotifyType::SkillGuide:
	//	return MakeShareable(new FGsSkillNotifyNodeGuideEffect());
	//}

	return nullptr;
}

void FGsSkillRunnerLocalPlayer::CreateRuntimeSkillNode()
{
	// Cancel Notify
	if (_skillData.IsValid())
	{
#if WITH_EDITOR
		// 여러 디버깅 상황에 필요할수 있어 내플레이어만 에디터상에서 노티파이 설정
		if (_skillData.GetNotifyList())
		{
			SetNotifyNode(*_skillData.GetNotifyList());
		}
#endif
		if (const FGsSchemaSkillSet* skilltable = _skillData.GetTableData())
		{
			if (skilltable->cutRemainInfo.delayOffset > 0.f)
			{
				TSharedPtr<FGsSkillNotifyNodeBase> cancelInfo = MakeShareable(new FGsSKillNotifyNodeCancel());
				cancelInfo->Initialize(_owner, &_skillData, nullptr, _skillNetData, _scaledOffsetRate);
				_currentInfo.Get()->_listNotify.Emplace(cancelInfo);
			}
		}
	}
}

bool FGsSkillRunnerLocalPlayer::CanMove() const
{
	if (Super::CanMove())
	{
		return true;
	}

	if (UAnimMontage* animation = GetAni())
	{
		return animation->HasRootMotion();
	}

	return false;
}