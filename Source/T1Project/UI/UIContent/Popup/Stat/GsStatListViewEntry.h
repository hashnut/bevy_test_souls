// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "NoExportTypes.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentHud.h"
#include "GsStatListViewEntry.generated.h"

/**
 *
 */

UCLASS()
class T1PROJECT_API UGsStatListViewEntryData : public UObject
{
	GENERATED_BODY()

public:	
	StatType _statType;	

public:
	void Init(const StatType inStatType)
	{
		_statType = inStatType;		
	}
};

UCLASS()
class T1PROJECT_API UGsStatListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* _statNameText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* _statValueText;
	UPROPERTY()
		class UGsUIVFX* _changedStatEffect;

protected:
	StatType _statType;
	int32 _statValue;

private:
	MsgGameObjHandleArray _listEventDelegate;
	MsgHudHandleArray _hudMessageHandlerArray;
	FText _maxBookmarkStatOverflowText;

public:	
	virtual void NativeConstruct() override;	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:	
	virtual void InvalidWidget();	

protected:
	void InvalidStatName();
	virtual void InvalidStatValue();
	void OnInvalidWidget(const struct IGsMessageParam* inParam);
	void OnInvalidWidget();

public:
	void SetStatName(StatType inStatType);
	void SetStatValue(const FText& inStatValueText, int32 inStatValue);
	void SetHighlight(bool inHighlight);

	int32 GetStatValue() { return _statValue; }

public:
	void ShowEffect();
};