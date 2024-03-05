// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUISampleListViewEntry.generated.h"

/**
 * ListView Entry 예제
 */
UCLASS()
class T1PROJECT_API UGsUISampleListViewEntry : 
	public UUserWidget,
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	FText _testText;

protected:
	// 데이터 세팅. 목록에 표시될 때.
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
