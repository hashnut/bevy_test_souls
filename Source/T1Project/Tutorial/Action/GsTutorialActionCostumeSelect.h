// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIWindowCostume;
class UWidget;

/**
 * 코스튬 튜토리얼에서 코스튬 1번 인덱스 선택 기능(1번 인덱스에 타겟 고정)
 */
class T1PROJECT_API FGsTutorialActionCostumeSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIWindowCostume> _window;

public:
	FGsTutorialActionCostumeSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionCostumeSelect() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

private:
	void OnListEvent(UWidget* InTargetWidget);
};
