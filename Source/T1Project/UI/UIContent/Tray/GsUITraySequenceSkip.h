// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITraySequenceSkip.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITraySequenceSkip : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSkip;

protected:
	virtual void NativeOnInitialized() override;
	
	// 시퀀스 중에 다른 tray 보다 높게 나와야 되어서
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_TUTORIAL - 1; }

protected:
	UFUNCTION()
	void OnClickSkip();

};
