// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DataSchema/GsSchemaUIColorData.h"
#include "Ranking/GsRankingData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIRankingListEntryBase.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UImage;
struct FGsSchemaUIColorDataRow;

/**
 * 랭킹 정보 슬롯 UI 기본 클래스
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIRankingListEntryBase : 
	public UUserWidget,
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRanking; // ** or 상위 **% 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockVariance; // 증가 감소 유지

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherRanking;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherVariance;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherVarianceArrow;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBackground;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GsUI")
	FGsSchemaUIColorDataRow _colorUP;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GsUI")
	FGsSchemaUIColorDataRow _colorDown;

protected:
	EGsRankingGrade _rankingGrade = EGsRankingGrade::Other;

protected:
	virtual void NativeConstruct() override;

	// UI에 보여질 때 호출된다.
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	virtual void SetData(const FGsRankingData* InRankingData);
	virtual void SetData(FGsRankingMyData* InMyRanking);

protected:
	void SetEmblem(UImage* InIconEmblem, GuildEmblemId InEmblemId);
	void SetRanking(uint16 InRanking);
	void SetRankingPercent(float InRanking);
	void SetRankingVariance(int64 InVariance);
};