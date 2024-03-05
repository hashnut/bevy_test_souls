// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control//GsToggleGroupEntry.h"
#include "GsUIBattlePassTab.generated.h"

class UGsSwitcherButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBattlePassTab : public UUserWidget, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switchButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _tabName;

protected:
	bool _isSelected{ false };

public:
	virtual void NativeOnInitialized() override;

	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

	void SetToggleInfo(int groupId);
	void SetTitle(FText titleText) { _tabName = titleText; };

	IGsToggleGroupEntry* GetToggleGroupEntry();
};
