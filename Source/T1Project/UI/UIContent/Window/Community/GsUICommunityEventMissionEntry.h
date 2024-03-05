// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "CommunityEventMission/GsCommunityEventMissionData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUICommunityEventMissionEntry.generated.h"

class UGsButton;
class UTextBlock;
class UPanelWidget;
class UGsSwitcherButton;
class UImage;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItemReward;
class UGsUIIconItem;
class UGsUIProgressBar;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICommunityEventMissionEntry :
	public UUserWidget,
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, CommunityEventMissionCommonId);

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _missionProgress;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _completeCanvasPanel;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textName;	

	bool _bIsSelected = false;

	UPROPERTY()
	bool _isRedDot = false;

	int _slotIndex = 0;

	CommunityEventMissionCommonId _commonId = INVALID_COMMUNITY_EVENT_MISSION_COMMON_ID;

public:
	FOnClickSlot _delegateClickSlot;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSlot();

protected:
	UGsUIIconItemReward* GetIcon();
	void OnClickIconSlot(class UGsUIIconItem& InIcon);
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

public:
	// 데이터 셋팅
	void SetData(const FText& inCommonName, TSharedPtr<FGsCommunityEventMissionCommon> inMissionCommon, bool inIsRedDot);
	// 데이터 셋팅
	void SetData(TSharedPtr<FGsCommunityEventMissionCommon> inMissionCommon);
};
