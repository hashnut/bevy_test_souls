#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsUIBattleArenaSeasonRankTierCategoryEntry.generated.h"

UCLASS()
class T1PROJECT_API UGsUIBattleArenaSeasonRankTierCategoryEntry : public UUserWidget, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _toggleButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherToggle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _tierSymbolDisable;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _tierNameDisable;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _tierSymbolEnable;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _tierNameEnable;

	// 2024/2/8 PKT - Tier Data Id
	GuildWarTierDataId _tierId = INVALID_GUILD_WAR_TIER_DATA_ID;

protected:
	virtual void NativeConstruct() override;

public:
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

	IGsToggleGroupEntry* GetToggleGroupEntry();

public:
	void SetData(const struct FGsSchemaBattleArenaTierData* InData);

	GuildWarTierDataId GetDataId() const { return _tierId; }
};
