// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsQuestInstance.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 *	다단계 랭크 미션
 */
class T1PROJECT_API FGsQuestRankInstance final : public FGsQuestInstance
{
	using Super = FGsQuestInstance;

public:
	virtual void Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey) override;
	virtual void SetQuestState(QuestState inState) override;	

protected:
	// 자동 이동
	virtual bool StartAutoProgress(bool inIsAuto = false) override;

public:
	virtual bool IsShowHudUI() const;	// hudUI에 표시 안한다.
};
