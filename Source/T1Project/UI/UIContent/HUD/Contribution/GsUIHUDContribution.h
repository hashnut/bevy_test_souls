// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageContentDungeon.h"

#include "GsUIHUDContribution.generated.h"

/**
 * 상단 미니맵 정보
 */

 class UListView;
 class UGsButton;
 class UWidgetSwitcher;
 class UGsContributionListViewEntry;
 struct IGsMessageParam;


UCLASS()
class T1PROJECT_API UGsUIHUDContribution : public UGsUIHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UWidgetSwitcher* _buttonSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UListView* _listView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _openButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _closeButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsContributionListViewEntry* _playerContribution;

private:
	MsgGuildDungeonHandleArray _listGuildDungeonDelegate;

private:
	EGsUIHUDMode _hudMode;
	bool _isOpen;
	bool _isVisible = false;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void InvalidateAllInternal() override;

public:
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;
	UFUNCTION()
	void OnShowContribution();
	UFUNCTION()
	void OnHideContribution();

public:	
	void OnShowContribution(const IGsMessageParam* inParam);
	void OnHideContribution(const IGsMessageParam* inParam);
};
