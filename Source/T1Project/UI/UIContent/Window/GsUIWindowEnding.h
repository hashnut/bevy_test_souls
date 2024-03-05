// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "GsUIWindowEnding.generated.h"

class UButton;
/**
 *  엔딩구간엔서 띄울 윈도우
 */
UCLASS()
class T1PROJECT_API UGsUIWindowEnding : public UGsUIWindow
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	UButton*		BtnOk;


protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnReleased();


};
