// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
 * Local 플레이어 리액션 상태
 */
class T1PROJECT_API FGsStateLocalPlayerReaction : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerReaction>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

protected:
	bool _isJumped{ false };

public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;

	virtual void IgnoreState(UGsGameObjectBase* Owner, IGsStateBase* ChangeState) override;

private:
	bool IsLock(class UGsGameObjectBase* Owner) const;
};


/**
 * Local 플레이어 리액션 종료 상태
 * 안전장치용
 * Reaction 상태는 Die / ReactionEnd 상태로만 상태전이를 허용
 */
class T1PROJECT_API FGsStateLocalPlayerReactionEnd : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerReactionEnd>

{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
};
