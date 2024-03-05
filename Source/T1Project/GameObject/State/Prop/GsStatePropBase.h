// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/State/GsStateBase.h"
#include "ActorEx/GsCharacterNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"

/**
* NPC 관련 상태들은 상하체 분리 구조가 아닌걸 가정한다.
*/
class T1PROJECT_API FGsStateBaseSingleProp : public IGsStateBase
{
public:
	// 애님 블루프린트에 가장 최우선으로 상태를 전송해줘야한다.
	virtual void Enter(UGsGameObjectBase* Owner) override
	{
		//UGsGameObjectPropInteraction* npc = Owner->CastGameObject<UGsGameObjectPropInteraction>();
		/*if (UGsAnimInstanceState* anim = npc->GetCharacter()->GetAnim())
		{
			anim->ChangeState(GetStateID(), GetAniRandomCount());
		}*/
	}
	virtual void ReEnter(UGsGameObjectBase* Owner) override {}
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override {}
	virtual void Exit(UGsGameObjectBase* Owner) override {}

	// NonPlayer State에 해당 되는 모든 State Getter
public:
	virtual IGsStateBase* GetState(uint8 StateID) const override;
};