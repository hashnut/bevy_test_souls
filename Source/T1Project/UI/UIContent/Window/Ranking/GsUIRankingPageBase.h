// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataSchema/Ranking/GsSchemaRankingTypeObjective.h"
#include "DataSchema/Ranking/GsSchemaRankingGroupCategory.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIRankingPageBase.generated.h"

class UListView;
class UTextBlock;
class UGsRankingListItem;
class UGsUIRankingListEntryBase;
class UWidgetSwitcher;

/**
 * 랭킹 페이지 기본 클래스
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIRankingPageBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	/************************************************************************/
	/* Sub Tab, Group Tab                                                   */
	/************************************************************************/
	const FGsSchemaRankingTypeObjective* _subTabData = nullptr;
	int32 _minRank = 1;
	int32 _maxRank = 1;
	WorldId _worldId = 0;

protected:
	/************************************************************************/
	/* UI                                                                   */
	/************************************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockServerGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRankingUnit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsRankingListItem))
	UListView* _listView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRankingListEntryBase* _myRanking;

public:
	/************************************************************************/
	/* Functions                                                     */
	/************************************************************************/
	virtual void Clear();
	virtual void Show();

	UListView* GetListView() { return _listView; }

protected:
	virtual void NativeOnInitialized() override;

protected:
	virtual void InvalidateList() {}
	virtual void InvalidateMyRanking() {}

	virtual void ScrollListViewDefault();
	
};
