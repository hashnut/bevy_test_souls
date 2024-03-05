// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIWindowFairy;
class UWidget;

/**
 * 페어리 튜토리얼에서 페어리 1번 인덱스 선택 기능(1번 인덱스에 타겟 고정)
 */
class T1PROJECT_API FGsTutorialActionFairySelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIWindowFairy> _window;

public:
	FGsTutorialActionFairySelect() : Super()
	{
	}
	virtual ~FGsTutorialActionFairySelect() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

private:
	void OnListEvent(UWidget* InTargetWidget);
};
