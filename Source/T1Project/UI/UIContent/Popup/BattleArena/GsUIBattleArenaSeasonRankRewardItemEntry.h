#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "GsUIBattleArenaSeasonRankRewardItemEntry.generated.h"

UCLASS()
class T1PROJECT_API UGsUIBattleArenaSeasonRankRewardItemEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRankRange;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _listViewRewardSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSelected;

protected:
	UPROPERTY()
	class UGsPoolUObject* _rewardEntryPool;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetData(const struct FGsSchemaBattleArenaRankingSeasonReward* InData, int32 InMyRank);
	// 2024/2/1 PKT - 내 랭크 영역인가?
	bool IsMyRank() const;
};
