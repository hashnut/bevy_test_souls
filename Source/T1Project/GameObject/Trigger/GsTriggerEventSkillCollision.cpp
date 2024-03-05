// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Trigger/GsTriggerEventSkillCollision.h"
#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "GameObject/ObjectClass/GsGameObjectProjectile.h"
#include "GameObject/Data/GsGameObjectData.h"

void FGsTriggerEventSkillCollision::Active(UGsGameObjectBase* Owner, EGsGameObjectType TargetType)
{
	FGsTriggerEventBase::Active(Owner, TargetType);

	UGsGameObjectProjectile* projectile = Owner->CastGameObject<UGsGameObjectProjectile>();
	const FGsSchemaProjectileSet* projecitleData = projectile->GetProjectileData();
	if (projecitleData->collisionInfoList.Num() > 0)
	{
		_collisionData.angle = projecitleData->collisionInfoList[0].angle;
		_collisionData.height = projecitleData->collisionInfoList[0].height;
		_collisionData.maxRadius = projecitleData->collisionInfoList[0].maxRadius;
		_collisionData.minRadius = projecitleData->collisionInfoList[0].minRadius;
		_collisionData.width = projecitleData->collisionInfoList[0].width;
		_collisionData.type = projecitleData->collisionInfoList[0].type;

		if (AActor* actor = Owner->GetActor())
		{
			_component = Cast<UGsSkillCollisionComponent>(
				actor->GetComponentByClass(UGsSkillCollisionComponent::StaticClass()));
			if (_component)
			{
				_isUpdate = true;
			}
		}

		_listInSideObject.Empty();
	}
}

void FGsTriggerEventSkillCollision::Deactive()
{
	for (int64 el : _listInSideObject)
	{
		if (UGsGameObjectBase* findObject = GSGameObject()->FindObject(EGsGameObjectType::Base, el))
		{
			findObject->OnTrigger(_owner->GetGameId(), GetType(), false);
		}
	}
	_listInSideObject.Empty();

	FGsTriggerEventBase::Deactive();
}

bool FGsTriggerEventSkillCollision::CheckTrigger()
{
	if (_component && _owner)
	{
		TArray<UGsGameObjectBase*> objList = GSGameObject()->FindObjectArray(_targetType);
		TArray<int64> dregsList = _listInSideObject;
		_component->Set(_collisionData);
		for (UGsGameObjectBase* el : objList)
		{
			bool inSideCheck = true;

			// 기존 inSide인 친구들은 outSide체크
			if (_listInSideObject.Contains(el->GetGameId()))
			{
				inSideCheck = false;

				// 쓰레기 데이터 검출 용 버퍼
				dregsList.Emplace(el->GetGameId());
			}

			FGsGameObjectData* gameObjectData = el->GetData();
			if (inSideCheck == _component->IsInsideCheck(el->GetLocation(), gameObjectData->GetScaledCapsuleRadius()))
			{
				el->OnTrigger(_owner->GetGameId(), GetType(), inSideCheck);

				if (inSideCheck)
				{
					// 새롭게 검출된 친구들
					_listInSideObject.Emplace(el->GetGameId());
				}
				else
				{
					_listInSideObject.Remove(el->GetGameId());					
				}
			}
		}
		// 쓰레기 데이터 제거
		if (dregsList.Num() > 0)
		{
			_listInSideObject.RemoveAll([&dregsList](int64 gameid)
				{
					return false == dregsList.Contains(gameid);
				});
		}
	}

	return false;
}