#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GameObject/Boss/GsWorldBossHandler.h"
#include "GsUIWorldBossRankListViewEntry.generated.h"

UCLASS()
class T1PROJECT_API UGsUIWorldBossRankListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	FGsWorldBossRankData _worldBossData;	
};

UCLASS()
class T1PROJECT_API UGsUIWorldBossRankListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _rankWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _rankTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _userNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UImage* _imgGuildEmblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _guildNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _damageTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _rewardListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _borderAllReward;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(class UObject* inListItemObject) override;

	enum class EViewType {RANK,LAST_HIT,MY_INFO };
	void Set(const FGsWorldBossRankData& inRankData, EViewType InType = EViewType::RANK);

private:	
	void Clear();
};