#pragma once

#include "CoreMinimal.h"
#include "../BattleArenaRank/GsBattleArenaRankData.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedRankEnum.h"
#include "GsUIBattleArenaSeasonRankItemEntry.generated.h"

UCLASS()
class T1PROJECT_API UGsBattleArenaSeasonRankItemEntryData : public UObject
{
	GENERATED_BODY()

protected:
	RankSearchType _rankSerarchType = RankSearchType::MAX;
	GuildWarTierDataId _tierDataId = INVALID_GUILD_WAR_TIER_DATA_ID;
	int32 _dataIndex = -1;

public:
	void SetData(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId, int32 InIndex);

	RankSearchType GetRankSearchType() const { return _rankSerarchType; }
	GuildWarTierDataId GetTierDataId() const { return _tierDataId; }
	int32 GetDataIndex() const { return _dataIndex; }
};


UCLASS()
class  T1PROJECT_API UGsUIBattleArenaSeasonRankItemEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelWidgetRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherRanking;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherRankRate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherRankRateArrow;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRankRate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRanking;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgTier;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _iconGuildEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textGuildName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textServerName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textPoint;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRecord;
	
protected:
	RankSearchType _rankSerarchType = RankSearchType::MAX;
	GuildWarTierDataId _tierDataId = INVALID_GUILD_WAR_TIER_DATA_ID;
	int32 _dataIndex = -1;

	ValidRankDataState _state = ValidRankDataState::Invalidate;

protected:
	virtual void NativeOnListItemObjectSet(UObject* InEntryData) override;

public:
	void SetData(RankSearchType InRankSearchType, const FGsBattleArenaRankListItem* InData);
	void Invalidate();
};
