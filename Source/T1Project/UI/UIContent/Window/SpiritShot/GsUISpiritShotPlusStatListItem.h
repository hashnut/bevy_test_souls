// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


#include "GsUISpiritShotPlusStatListItem.generated.h"


class UTextBlock;


UCLASS()
class T1PROJECT_API UGsSpiritShotPlusStatListData : public UObject
{
	GENERATED_BODY()

private:
	FText _statName;
	FText _statValue;

	/************************************************************************/
	/* Setter                                                               */
	/************************************************************************/
public:
	void SetData(const FText& InStatName, const FText& InStatValue)
	{
		_statName = InStatName;
		_statValue = InStatValue;
	}

	void SetData(FText&& InStatName, FText&& InStatValue)
	{
		_statName = MoveTemp(InStatName);
		_statValue = MoveTemp(InStatValue);
	}

	void SetData(TPair<FText, FText>&& InStatNameValuePair)
	{
		SetData(MoveTemp(InStatNameValuePair.Key), MoveTemp(InStatNameValuePair.Value));
	}

	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
public:
	FText GetStatName() const { return _statName; }
	FText GetStatValue() const { return _statValue; }
};



/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISpiritShotPlusStatListItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()


	/************************************************************************/
	/* 위젯                                                                 */
	/************************************************************************/
protected:
	/** 스탯 이름 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtStatName;

	/** 스탯 수치 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtStatValue;


	/************************************************************************/
	/* Overrides                                                            */
	/************************************************************************/
protected:
	void NativeOnInitialized() final;

	/** 실제 엔트리의 데이터 세팅 */
	void NativeOnListItemObjectSet(UObject* InListItemData) final;
};
