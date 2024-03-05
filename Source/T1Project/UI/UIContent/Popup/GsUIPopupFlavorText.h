// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupFlavorText.generated.h"

class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupFlavorText : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsButton* _btnClose;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	FText _name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	FText _flavor;

public:
	struct PopupInitData
	{
		FText name;
		FText flavorText;
	};

protected:
	UFUNCTION(BlueprintCallable)
	void OnClickClose();

public:
	void InitializeData(PopupInitData* initParam = nullptr);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
