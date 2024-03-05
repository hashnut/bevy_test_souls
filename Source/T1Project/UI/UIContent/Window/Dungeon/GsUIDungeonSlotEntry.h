// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsSchemaEnums.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "../UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsUIDungeonSlotEntry.generated.h"

class UImage;
class UGsButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIDungeonSlotEntry : public UUserWidget, public IGsToggleGroupEntry
{
	GENERATED_BODY()
	
protected:
	// ·¹º§orÃþ
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClicked;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgDimmed;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSelectedEdge;

private:
	const class FGsDungeonData* _dungeonData = nullptr;	
	
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION()
	void OnClickSelected();

public:
	void SetData(const class FGsDungeonData* InDungeonData);
	void Invalidate();

	// 2023/2/24 PKT - IGsToggleGroupEntry..
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

	const class FGsDungeonData* GetData() const;
};
