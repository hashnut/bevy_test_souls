// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/TextDecorator/GsRichTextBlockLinkDecorator.h"
#include "GsUIItemSkillOptionListEntry.generated.h"

class URichTextBlock;
struct FGsSchemaPassivitySet;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemSkillOptionListEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _skillOptionRichText;

private:
	TPair<UObject*, UGsRichTextBlockLinkDecorator::FGsOnClickRichTextBlockLinkDecoratorUObject> _onClickHandlePair;

protected:
	virtual void NativeDestruct() override;

public:
	void SetOptionText(FText InText);
	void SetOptionText(const FGsSchemaPassivitySet* passivitySet);

};
