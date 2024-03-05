// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "UTIL/GsGameObjectUtil.h"
#include "GsLevelUpStatListViewEntry.generated.h"

UCLASS()
class UGsLevelUpStatListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	StatType _statType1;
	FGsStatDiffInfo _statDiff1;	
	bool _showStat1;
	StatType _statType2;
	FGsStatDiffInfo _statDiff2;
	bool _showStat2;
};

/**
 * 레벨 업 티커
 */
UCLASS()
class T1PROJECT_API UGsLevelUpStatListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _statNameTextBlock1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _statValueTextBlock1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _statNameTextBlock2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _statValueTextBlock2;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;	
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

private:
	FText GetStatValue(StatType inStatType, int32 inValue);
};
