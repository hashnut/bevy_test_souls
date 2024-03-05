#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIBattleArenaSeasonTierRewardItemEntry.generated.h"

UCLASS()
class T1PROJECT_API UGsUIBattleArenaSeasonTierRewardItemEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgTierSymbol;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textTierName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textScore;
	
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
	void SetData(const struct FGsSchemaBattleArenaTierSeasonReward* InData, GuildWarTierDataId InMyTierDataId);
	// 2024/2/1 PKT - 내 티어 영역인가?
	bool IsMyTier() const;
};
