// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsMonsterLevel.h"
#include "GsUIRegionSceneItemDropEntry.generated.h"

/**
 * 기사단원 리스트의 entry
 */
UCLASS()
class T1PROJECT_API UGsUIRegionSceneItemDropEntry : public UUserWidget,
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:		
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _npcNameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _gradeTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _isAgressiveTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _levelTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _itemDropButton;

protected:
	FGsSchemaWorldMapSpoilsMonsterLevel _data;
	FText _levelFormat;
	bool _isSelect;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	void SetData(const struct FGsSchemaWorldMapSpoilsMonsterLevel& inData);
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

protected:
	UFUNCTION()
		void OnClickGoTo();
};