// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "GsUIGuildMemberGradeEntry.generated.h"

class UImage;
class UTextBlock;
/**
 * 길드 문장 변경창
 */
UCLASS()
class T1PROJECT_API UGsUIGuildMemberGradeEntry : 
	public UUserWidget, 
	public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;

protected:
	bool _bIsSelected = false;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override { return _bIsSelected; }

public:
	void SetData(GuildMemberGradeType InGrade, int32 InMaxCount, int32 InCurrCount);

protected:
	UFUNCTION()
	void OnClickSlot();
};
