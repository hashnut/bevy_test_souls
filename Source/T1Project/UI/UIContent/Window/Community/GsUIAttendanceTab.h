// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIAttendanceTab.generated.h"

class UTextBlock;
class UImage;
class UGsButton;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIAttendanceTab :
	public UUserWidget,
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickTab, const CommunityEventId, const bool);

protected:
	// 탭 이름
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTextBlock* _textName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textName;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UWidgetSwitcher* _btnSwitcher;

	bool _bIsSelected = false;
	CommunityEventId _eventId = INVALID_COMMUNITY_EVENT_ID;

	UPROPERTY()
	bool _isRedDot = false;

public:
	FOnClickTab _delegateClickTab;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	// 데이터 셋팅
	void SetData(CommunityEventId id, const FText& inTabName);
	// 데이터 셋팅
	void SetData(CommunityEventId id, const FText& inTabName, bool inIsRedDot);
	// 데이터 셋팅
	void SetData(const FText& inTabName, bool inIsRedDot);
	// 레드닷 셋팅
	void SetRedDot(bool inIsRedDot);

protected:
	UFUNCTION()
	void OnClickNormal();

	UFUNCTION()
	void OnClickSelected();

public:
	CommunityEventId GetEventId() { return _eventId; }
};
