// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsPlayerStatDistributionController.h"
#include "GsPlayerBookmarkStatController.h"

class T1PROJECT_API FGsPlayerStatHandler
{
public:
	TUniquePtr<class FGsPlayerStatDistributionController> _distributionStatHandler;
	TUniquePtr<class FGsPlayerBookmarkStatController> _bookmarkStatHandler;

public:
	void Init();
	void Close();
	void Reset();

	//get main stat
public:	
	bool TryGetMainStat(OUT TMap<StatType, const class FGsStatInfo*>& outStatMap);

public:
	class FGsPlayerStatDistributionController* GetStatDistributionController();
	class FGsPlayerBookmarkStatController* GetBookmarkStatController();

public:
	static const class FGsStatInfo* GetPlayerStat(StatType inStatType);
	static bool TryGetPlayerStatText(StatType inStatType, OUT FText& outPlayerText);

public:
	static FGsPlayerStatHandler* GetInstance();
};
