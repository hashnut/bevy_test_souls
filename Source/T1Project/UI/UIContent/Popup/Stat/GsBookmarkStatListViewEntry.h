// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "NoExportTypes.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentHud.h"
#include "GsStatListViewEntry.h"
#include "GsBookmarkStatListViewEntry.generated.h"

/**
 *
 */

UCLASS()
class T1PROJECT_API UGsBookmarkStatListViewData : public UGsStatListViewEntryData
{
	GENERATED_BODY()

public:		
	bool _isLine;

public:
	void InitBookmark(const StatType inStatType, bool inIsLine)
	{
		Init(inStatType);		
		_isLine = inIsLine;
	}
};

UCLASS()
class T1PROJECT_API UGsBookmarkStatListViewEntry : public UGsStatListViewEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UGsToggleButton* _bookmarkToggleButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UWidgetSwitcher* _lineWidgetSwitcher;	

private:
	bool _isLine;	
	class FGsPlayerBookmarkStatController* _bookmarkStatControl;

private:
	MsgGameObjHandleArray _listEventDelegate;
	MsgHudHandleArray _hudMessageHandlerArray;
	FText _maxBookmarkStatOverflowText;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	void InvalidWidget();
	void InvalidFavorit();
	void InvalidStatName();
	void InvalidStatValue();
	void OnPlayerStatUpdate(const struct IGsMessageParam* inParam);
	void OnInvalidWidget();
	UFUNCTION()
	void OnClickBookmark();

private:
	void BindMessage();
	void UnbindMessage();
};