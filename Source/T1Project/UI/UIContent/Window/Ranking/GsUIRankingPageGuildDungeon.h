// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingPageBase.h"
#include "GsUIRankingPageGuildDungeon.generated.h"


class UTextBlock;
/**
 * 기사단 던전 랭킹 UI
 */
UCLASS()
class T1PROJECT_API UGsUIRankingPageGuildDungeon : public UGsUIRankingPageBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRankingUnit2;

	/************************************************************************/
	/* Overrides                                                            */
	/************************************************************************/
protected:
	virtual void NativeConstruct() override;

public:
	virtual void Show() override;

protected:
	virtual void InvalidateList() override;
	virtual void InvalidateMyRanking() override;

};
