// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Runtime/Core/Public/Templates/Tuple.h"
#include "T1Project.h"

#include "GsUISpiritShotLevelStatListItem.generated.h"

//#define GET_VALUE(MyTuple, i)	TTupleElementGetterByIndex<i>::Get(MyTuple) 

class UTextBlock;
class UWidgetSwitcher;


UCLASS()
class T1PROJECT_API UGsUISpiritShotLevelStatListData : public UObject
{
	GENERATED_BODY()

private:
	Level _level;
	FText _statName;
	FText _statValue;
	bool _isLevelReached;

	/************************************************************************/
	/* Setter                                                               */
	/************************************************************************/
public:
	void SetData(Level InLevel, const FText& InStatName, const FText& InStatValue, bool InIsLevelReached)
	{
		_level = InLevel;
		_statName = InStatName;
		_statValue = InStatValue;
		_isLevelReached = InIsLevelReached;
	}

	void SetData(Level InLevel, FText&& InStatName, FText&& InStatValue, bool InIsLevelReached)
	{
		_level = InLevel;
		_statName = MoveTemp(InStatName);
		_statValue = MoveTemp(InStatValue);
		_isLevelReached = InIsLevelReached;
	}

	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
public:
	Level GetLevel() const { return _level; }
	FText GetStatName() const { return _statName; }
	FText GetStatValue() const { return _statValue; }
	bool IsLevelReached() const { return _isLevelReached; }
};

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISpiritShotLevelStatListItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	/************************************************************************/
	/* 위젯                                                                 */
	/************************************************************************/
protected:
		/** 레벨 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtLevelBefore;

	/** 스탯 이름 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtStatNameBefore;

	/** 스탯 수치 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtStatValueBefore;

	/** 레벨 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtLevelAfter;

	/** 스탯 이름 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtStatNameAfter;

	/** 스탯 수치 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtStatValueAfter;

	/** 스탯 도달 상태에 따라 컬러값 바꿔줄 위젯 스위처 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcherTextColor;


	/************************************************************************/
	/* Overrides                                                            */
	/************************************************************************/
protected:
	void NativeOnInitialized() final;

	/** 실제 엔트리의 데이터 세팅 */
	void NativeOnListItemObjectSet(UObject* InListItemData) final;
};
