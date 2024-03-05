// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingListEntryBase.h"
#include "GsUIRankingGuildListEntry.generated.h"

class UTextBlock;
class UImage;
class UWidgetSwitcher;

/**
 * 기사단 랭킹 정보 슬롯 UI
 */
UCLASS()
class T1PROJECT_API UGsUIRankingGuildListEntry : public UGsUIRankingListEntryBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _iconEmblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockValue;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPercent;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherValue;

protected:
	// 아이템 UI 설정 시 호출
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void SetData(FGsRankingMyData* InMyRanking) override;

protected:
	void ShowGuildInfo(GuildEmblemId InEmblemId, const FString& InGuildName);
	void HideGuildInfo();
	void SetValueText(uint64 InValue);
};
