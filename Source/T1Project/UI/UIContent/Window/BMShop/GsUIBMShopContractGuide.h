// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIBMShopContractGuide.generated.h"

class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBMShopContractGuide : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDetail;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnJapanWarn;


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;


protected:
	UFUNCTION()
	void OnClickDetail();
	UFUNCTION()
	void OnClickJapanWarn();


};
