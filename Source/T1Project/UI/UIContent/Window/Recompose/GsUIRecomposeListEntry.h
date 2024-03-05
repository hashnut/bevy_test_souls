// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "GsUIRecomposeListEntry.generated.h"


class UImage;
class UTextBlock;
class UGsButton;
/**
 * 합성강화 복구 시스템
 * 코스튬/페어리 재합성 등급 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIRecomposeListEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
	
	DECLARE_DELEGATE_OneParam(FOnClickGradeSlot, UUserWidget*);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _widgetFocus;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;

public:
	FOnClickGradeSlot OnClickGradeSlot;

protected:
	CostumeGrade _costumeGrade;
	FairyGrade _fairyGrade;
	bool _canRecompose = false;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClickedGradeSlot();

public:
	void SetCostumeGrade(CostumeGrade InGrade, bool InCanRecompose);
	void SetFairyGrade(FairyGrade InGrade, bool InCanRecompose);

	void SetIsSelected(bool InIsSelected);

	CostumeGrade GetCostumeGrade() { return _costumeGrade; }
	FairyGrade GetFairyGrade() { return _fairyGrade; }
};
