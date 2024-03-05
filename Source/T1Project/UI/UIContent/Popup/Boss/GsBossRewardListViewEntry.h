// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "GsBossRewardListViewEntry.generated.h"

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
class T1PROJECT_API UGsBossRewardListViewEntryData : public UObject
{
	GENERATED_BODY()

public:			
	int32 _maxRank;
	int32 _minRank;
	TArray<TSharedPtr<FGsBossRewardInfo>> _rewardArray;
	const FGsSchemaFieldBossRewardData* _rewardData = nullptr;

public:
	UGsBossRewardListViewEntryData() = default;
	void Initailize(FGsBossRankInfo* inInfo)
	{				
		_rewardArray.Empty();
		for (int32 i = 0; i < inInfo->_rewardList.Num(); ++i)
		{
			_rewardArray.Add(inInfo->_rewardList[i]);
		}
	}
	virtual ~UGsBossRewardListViewEntryData()
	{
		_rewardArray.Empty();
	}
};

UCLASS()
class T1PROJECT_API UGsBossRewardListViewEntry : public UUserWidget, public IUserObjectListEntry
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
		UListView* _rewardListView;

public:
	FText _rankText;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

private:
	void InvalidReward(UGsBossRewardListViewEntryData* inData);
	void SetRank(int32 inMinRank, int32 inMaxRank);
	void SetReward(TArray<TSharedPtr<FGsBossRewardInfo>>& inRewardData);
};
