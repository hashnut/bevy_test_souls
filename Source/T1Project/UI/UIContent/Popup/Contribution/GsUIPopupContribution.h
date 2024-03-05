// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Message/GsMessageContentDungeon.h"
#include "Classes/GsPoolUObject.h"
#include "GsUIPopupContribution.generated.h"

/**
 * 상단 미니맵 정보
 */

 class UListView;
 class UWidgetSwitcher;
 class UGsContributionListViewEntry;
 struct IGsMessageParam;
 struct FGsContributionData;

UCLASS()
class UGsUIPopupContribution : public UGsUIPopup
{
	GENERATED_BODY()

private:
	TArray<TWeakPtr<FGsContributionData>> _rankingList;
	FString _strMyName;

public:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _listView;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsContributionListViewEntry* _playerContribution;
	// 0: empty/ 1: list
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherList;

protected:
	UPROPERTY()
	UGsPoolUObject* _poolListItem;

private:
	MsgGuildDungeonHandleArray _listGuildDungeonDelegate;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BindMessages();
	void UnbindMessages();

private:
	//void OnInvalidRank(const IGsMessageParam* inParam);
	void OnUpdateGuildDungeonRanking(const IGsMessageParam*);
	void OnUpdateAgitRaidRanking(const IGsMessageParam*);

private:
	void InvalidateList();
};
