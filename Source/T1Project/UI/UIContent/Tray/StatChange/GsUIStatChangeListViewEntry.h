// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UTIL/GsGameObjectUtil.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIContent/Popup/Stat/GsStatListViewEntry.h"
#include "GsUIStatChangeListViewEntry.generated.h"

/**
 * 레벨 업 티커
 */

UCLASS()
class T1PROJECT_API UGsUIStatChangeListViewEntryData : public UGsStatListViewEntryData
{
	GENERATED_BODY()

public:
	StatType _statType;
	int _oldVlaue;
	int _newValue;

public:
	UGsUIStatChangeListViewEntryData() = default;
	void Init(const StatType inStatType, int32 inOldValue, int32 inNewValue)
	{
		_statType = inStatType;
		_oldVlaue = inOldValue;
		_newValue = inNewValue;
	}
};


UCLASS()
class T1PROJECT_API UGsUIStatChangeListViewEntry : public UGsStatListViewEntry
{
	GENERATED_BODY()

protected:
	int32 _newValue;
	int32 _oldValue;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UWidgetSwitcher* _upDownWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
		class UWidgetAnimation* _openAnim;

public:	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void InvalidWidget() override;
	virtual void InvalidStatValue() override;	
	void InvalidUpDown();
};
