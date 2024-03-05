// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsBossInfoSpawnListViewEntry.generated.h"

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
class T1PROJECT_API UGsBossInfoSpawnListViewEntryData : public UObject
{
	GENERATED_BODY()

public:		
	FString _crone;
};

UCLASS()
class T1PROJECT_API UGsBossInfoSpawnListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _weekTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _spawnTimeTextBlock;		

private:
	TWeakObjectPtr<UGsUIBossDetailInfoPanel> _parent;		

public:
	virtual void NativeOnInitialized() override;	
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;
};
