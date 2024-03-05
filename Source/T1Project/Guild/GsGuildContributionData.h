// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/* 
* FGsStageGameGuildDungeon 에 있던것을 같이 쓰기 위해 이동함
* 기사단 던전, 기사단 영지 던전 등의 기여도 표시에 활용되는 데이터
*/
struct FGsContributionData final
{
public:
	int32 _rank;
	float _contribution;
	FString _userName;

public:
	FGsContributionData() = default;
	explicit FGsContributionData(int32 InRank, float InContribution, const FString& InUsername)
		: _rank(InRank), _contribution(InContribution), _userName(InUsername)
	{
	}
	~FGsContributionData() = default;
};
