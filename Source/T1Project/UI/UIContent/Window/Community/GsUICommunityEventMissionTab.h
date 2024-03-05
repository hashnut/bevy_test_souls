// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUICommunityEventMissionTab.generated.h"

class UGsSwitcherButton;
class UImage;
class UCanvasPanel;
class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICommunityEventMissionTab :
	public UUserWidget,
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _lock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _allRewarded;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDot;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textName;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _offMissionTabIdText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _onMissionTabIdText;

	bool _bIsSelected = false;

	UPROPERTY()
	bool _isRedDot = false;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickButton();

public:
	// 데이터 셋팅
	void SetData(const FText& inTabName);
	// 데이터 셋팅
	void SetData(const FText& inTabName, bool inIsLock, bool inIsRedDot, bool inIsAllRewarded);
	// 레드닷 셋팅
	void SetRedDot(bool inIsRedDot);	

	void SetTabId(CommunityEventMissionTabId inTabId);
};
