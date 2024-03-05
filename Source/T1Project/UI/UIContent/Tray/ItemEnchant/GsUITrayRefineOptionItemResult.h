// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayRefineOptionItemResult.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayRefineOptionItemResult : public UGsUITray
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
};
