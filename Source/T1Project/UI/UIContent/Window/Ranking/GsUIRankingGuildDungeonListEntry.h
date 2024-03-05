// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingListEntryBase.h"
#include "GsUIRankingGuildDungeonListEntry.generated.h"

class UTextBlock;
class UImage;
class UWidgetSwitcher;

/**
 * 기사단 던전 랭킹 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIRankingGuildDungeonListEntry : public UGsUIRankingListEntryBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _iconEmblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockClearTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherGuild;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void SetData(FGsRankingMyData* InMyRanking) override;

protected:
	void SetGuildInfo(GuildEmblemId InEmblemId, const FString& InGuildName);
	void SetValueInfo(uint64 InStage, uint64 InClearTime);

	void HideAll();
};
