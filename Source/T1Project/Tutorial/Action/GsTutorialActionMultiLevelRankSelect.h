// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIWindowMultiLevelRank;
class UWidget;

/**
 * 승급창에서 첫번째 슬롯 선택 액션
 */
class T1PROJECT_API FGsTutorialActionMultiLevelRankSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIWindowMultiLevelRank> _window;

public:
	FGsTutorialActionMultiLevelRankSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionMultiLevelRankSelect() {}

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

private:
	void OnListEvent(UWidget* InTargetWidget);
};
