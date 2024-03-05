// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupBattlePassExpDetail.generated.h"

class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupBattlePassExpDetail : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _nameText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _expText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FSlateBrush _iconImg;

protected:
	UFUNCTION()
	void OnClickClose();

	virtual void NativeOnInitialized() override;

public:

	void SetDetailInfo(FText name, FText exp, FSoftObjectPath iconPath);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
