// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFlow/GameContents/ContentsGame/GsContentsGameBase.h"

#include "Message/GsMessageGameObject.h"


/**
 *  창고npc 관련
 */
class T1PROJECT_API FGsGameStateNpcDepot : public FGsContentsGameBase
{
	using Super = FGsContentsGameBase;

private:
	MsgGameObjHandle _interactionMsgHandler;

public:
	FGsGameStateNpcDepot();
	virtual ~FGsGameStateNpcDepot();

public:
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	virtual bool IsChangeSoundMixer() const override { return false; }

public:
	void OnInteractionContents(const struct IGsMessageParam* In_data);

};
