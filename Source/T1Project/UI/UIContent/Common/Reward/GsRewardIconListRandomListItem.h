// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsRewardIconListRandomListItem.generated.h"

struct FGsRewardIconDataWrapper;

/**
 * 보상 랜덤 ListView의 데이터 부분
 */
UCLASS()
class T1PROJECT_API UGsRewardIconListRandomListItem : public UObject
{
	GENERATED_BODY()

protected:
	FGsRewardIconDataWrapper* _iconData = nullptr;
	bool _bCenterPopup = false;
	bool _bEnableDetailPopup = false;

public:
	void Clear();
	void SetData(FGsRewardIconDataWrapper* InIconData, bool bInCenterPopup, bool bInEnableDetailPopup);
	FGsRewardIconDataWrapper* GetIconData() const { return _iconData; }
	bool IsCenterPopup() const { return _bCenterPopup; }
	bool IsEnableDetailPopup() const { return _bEnableDetailPopup; }
};
