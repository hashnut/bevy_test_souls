// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIBattlePassTab.h"
#include "UI/UIControlLib/Control//GsToggleGroupEntry.h"
#include "GsUIBattlePassMissionTab.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBattlePassMissionTab : public UGsUIBattlePassTab
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIControl")
	int32 _slotIndexLock = 2;

protected:
	bool _isLock{ false };
	
public:
	virtual void SetIsSelected(bool bInIsSelected) override;
	void SetTabLock(bool isLock);
};
