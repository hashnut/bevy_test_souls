// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Trigger/GsTriggerEventBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Management/GsMessageHolder.h"

FGsTriggerEventBase::~FGsTriggerEventBase()
{
	UWorld* world = GetWorld();
	world->GetTimerManager().ClearTimer(_timerHandle);
}

void FGsTriggerEventBase::Active(UGsGameObjectBase* Owner, EGsGameObjectType TargetType)
{
	_owner = Owner;
	_targetType = TargetType;

	UWorld* world = GetWorld();
	// 업데이트 시작
	_timerDelegate.BindLambda([this]() {
		if (CheckTrigger())
		{
			// 글로벌 메세지를 전송한다.
			FGsTriggerMessageParam param(_owner->GetGameId(), GetType(), true);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::COMMON_TRIGGER_EVENT_UPDATE, &param);
		}

		if (_isUpdate)
		{
			if (UWorld* world = GetWorld())
			{
				_timerHandle = world->GetTimerManager().SetTimerForNextTick(_timerDelegate);
			}
		}});
	_timerHandle = world->GetTimerManager().SetTimerForNextTick(_timerDelegate);

	// 각 파생 클래스에서 제어
	// _isUpdate = true;
}

void FGsTriggerEventBase::Deactive()
{
	_isUpdate = false;
}

bool FGsTriggerEventBase::CheckTrigger()
{
	return false;
}

UWorld* FGsTriggerEventBase::GetWorld() const
{
	if (_owner)
	{
		if (AActor* actor = _owner->GetActor())
		{
			return actor->GetWorld();
		}
	}
	return nullptr;
}
