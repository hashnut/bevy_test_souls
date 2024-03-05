// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUISampleTreeViewEntry.generated.h"

/**
 * TreeView Entry ¿¹Á¦
 */
UCLASS()
class T1PROJECT_API UGsUISampleTreeViewEntry :
	public UUserWidget,
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	FText _testText;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickEntry();
};
