// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUITutorialSubDialog.generated.h"

class URichTextBlock;
class AActor;
struct FGsSchemaSubDialogCutData;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITutorialSubDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnRemainTimeOver);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	FSlateBrush _acterFaceImg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	URichTextBlock* _richTextScript;
	
	UPROPERTY()
	AActor* _actor = nullptr;

protected:
	virtual void NativeDestruct() override;

public:
	// 대화 연출 시작
	void StartAction(const FGsSchemaSubDialogCutData* inCutData);

	UFUNCTION(BlueprintImplementableEvent, Category = "GsUI")
	void StartAnimation();

//private:
	// 액터 삭제
	void DestroyActor();
};
