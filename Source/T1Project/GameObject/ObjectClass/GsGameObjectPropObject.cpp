// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectPropObject.h"
#include "ActorEx/GsActorPropObject.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Runtime/DataCenter/Public/DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPropEnum.h"
#include "Runtime/DataCenter/Public/Map/GsMapIconType.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"


void UGsGameObjectPropObject::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectPropObject::GetObjectType());

	//  테이블 데이터 얻기
	_isCanceling = false;
}

void UGsGameObjectPropObject::InitializeActor(class AActor* inOwner)
{
	Super::InitializeActor(inOwner);

	_actor = Cast<AGsActorPropObject>(inOwner);
}

void UGsGameObjectPropObject::ActorDespawn()
{
	Super::ActorDespawn();
}
void UGsGameObjectPropObject::CreateSubClass()
{
	Super::CreateSubClass();
}