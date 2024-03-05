// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "GsUIHudQuickSlotList.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIHudQuickSlotList : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIReuseScroll* _reuseScroll;




protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	
};
