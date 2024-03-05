// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIServerMigrateCheckConditionEntry.generated.h"

class UGsButton;
class UTextBlock;
class UWidgetSwitcher;
struct MigrateConditionItem;
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIServerMigrateCheckConditionEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	// 2023/10/30 PKT - off. on
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherConditionImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textConditionName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFeature;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDesc;

	// 2023/10/30 PKT - 
	FSlateColor _validColor;
	// 2023/10/30 PKT - 가능 문구
	FText _msgPossible;
	// 2023/10/30 PKT - 불가능 문구
	FText _msgImpossible;

	ServerMigrateType _serverMigrateType = ServerMigrateType::NONE;

	TWeakPtr<MigrateConditionItem> _data{ nullptr };
	//const struct FGsSchemaServerMigrateConditionData* _schemaConditionData = nullptr;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClickedFeature();

protected:
	void SetUIByFeatureType();

	void CheckedInvalidate();
public:
	void SetData(ServerMigrateType type, TWeakPtr<MigrateConditionItem> data) { _serverMigrateType = type; _data = data; }
	void RefreshUI();
	//void SetData(const struct FGsSchemaServerMigrateGroupData* InGroupData, int32 InConditionIndex);
};
