// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "GsBossContributionListViewEntry.generated.h"

/**
 *
 */

class UGsButton;
class UImage;
class UTextBlock;
class UListView;
class UWidget;
class UWidgetSwitcher;
struct FGsSchemaFieldBossRewardData;

UCLASS()
class T1PROJECT_API UGsBossContributionListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	int32 _fieldBossId;
	int32 _rank;
	FString _name;
	int32 _level;
	WorldId _worldId;
	FString _guildName;
	FString _worldPrefix;
	SocialMediaInfo _socialMediaInfo;
	float _contributionPercent;
	bool _isParty;
	TArray<TSharedPtr<FGsBossRewardInfo>> _rewardArray;
	const FGsSchemaFieldBossRewardData* _rewardData = nullptr;

public:
	UGsBossContributionListViewEntryData() = default;
	void Initailize(int32 inRank, FGsBossRankInfo* inInfo)
	{
		_rank = inRank;
		_name = inInfo->_usereName;
		_level = inInfo->_level;
		_guildName = inInfo->_guildName;
		_contributionPercent = inInfo->_contributeRate;
		_isParty = inInfo->_isParty;
		_worldId = inInfo->_worldId;
		_worldPrefix = inInfo->_worldPrefix;
		_socialMediaInfo.mNormal = inInfo->_socialMediaInfo.mNormal;
		_socialMediaInfo.mFriend = inInfo->_socialMediaInfo.mFriend;
		_socialMediaInfo.mGuild = inInfo->_socialMediaInfo.mGuild;
		_socialMediaInfo.mParty = inInfo->_socialMediaInfo.mParty;

		_rewardArray.Empty();
		for (int32 i = 0; i < inInfo->_rewardList.Num(); ++i)
		{
			_rewardArray.Add(inInfo->_rewardList[i]);
		}
	}
	virtual ~UGsBossContributionListViewEntryData()
	{
		_rewardArray.Empty();
	}
};

UCLASS()
class T1PROJECT_API UGsBossContributionListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UWidgetSwitcher* _rankWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UImage* _rank1_image;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UImage* _rank2_image;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UImage* _rank3_image;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _rankTextBlock;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _nameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _guildNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _contributionPercentTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UWidget* _guildPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UListView* _rewardListView;

public:
	FText _rankTextFormat;
	FText _levelTextFormat;
	FText _partyTextFormat;
	FText _invadePartyTextFormat;
	FText _resultLevelNameFormat;
	FText _resultLevelWorldNameFormat;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

private:
	void InvalidBossContribution(UGsBossContributionListViewEntryData* inData);
	void SetRank(int32 inRank);
	void SetReward(TArray<TSharedPtr<FGsBossRewardInfo>>& inRewardData);
};
