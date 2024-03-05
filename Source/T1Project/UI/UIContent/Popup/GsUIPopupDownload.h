// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupDownload.generated.h"

class FText;
class UTextBlock;
class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupDownload : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _downloadSizeText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnCancel;

	// T: Yes, F: No
	TFunction<void(bool)> _buttonCallback;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool OnBack() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOk();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();

public:
	void SetData(float inDownloadSize, TFunction<void(bool)> InCallback = nullptr);
};
