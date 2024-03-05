// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UIContent/Window/Ranking/GsUIRankingPageBase.h"

#include "GsUIRankingPagePersonal.generated.h"

class UTextBlock;
class UGsUIRankingPersonalListEntry;

/**
 * 개인랭킹 UI
 */
UCLASS()
class T1PROJECT_API UGsUIRankingPagePersonal : public UGsUIRankingPageBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _spacerBadge;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _boxBadge;

protected:
	/************************************************************************/
	/* Overrides                                                            */
	/************************************************************************/
	virtual void InvalidateList() override;
	virtual void InvalidateMyRanking() override;

	void OnOpenUserContext(UGsRankingListItem* InListItem);

};