// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/UIContent/Common/Entry/GsUISimpleTextViewEntry.h"
#include "GsBossInfoWeekListViewEntry.generated.h"

/**
 *
 */

class UGsButton;
class UImage;
class UGsUIPopupBossInfo;
class UTextBlock;
struct FGsSchemaFieldBossData;
struct FGsSchemaNpcData;
class UGsUIBossDetailInfoPanel;

UCLASS()
class T1PROJECT_API UGsBossInfoWeekListViewEntryData : public UObject
{
	GENERATED_BODY()

public:	
	int32 _weekIndex;
	const FGsSchemaFieldBossData* _bossData;	
	TWeakObjectPtr<UGsUIBossDetailInfoPanel> _parent;
};

UCLASS()
class T1PROJECT_API UGsBossInfoWeekListViewEntry : public UGsUISimpleTextViewEntry
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UGsUIBossDetailInfoPanel> _parent;
	const FGsSchemaFieldBossData* _bossData;
	int32 _weekIndex;

public:
	virtual void NativeOnInitialized() override;	
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

public:
	UFUNCTION()
	void OnClickWeek();
};
