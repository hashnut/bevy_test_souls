// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingPageBase.h"
#include "GsUIRankingPageGuild.generated.h"


/**
 * 기사단 랭킹 UI
 */
UCLASS()
class T1PROJECT_API UGsUIRankingPageGuild : public UGsUIRankingPageBase
{
	GENERATED_BODY()

protected:
	/************************************************************************/
	/* Overrides                                                            */
	/************************************************************************/
	virtual void InvalidateList() override;
	virtual void InvalidateMyRanking() override;

};
