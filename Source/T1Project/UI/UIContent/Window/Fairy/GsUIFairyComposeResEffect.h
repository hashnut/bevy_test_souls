// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "GsUIFairyComposeResEffect.generated.h"

class UPanelWidget;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIFairyComposeResEffect : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIFx", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* MAGIC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIFx", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* RARE;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIFx", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* EPIC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIFx", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* UNIQUE;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIFx", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* LEGEND;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _FxGrade;

	UPROPERTY()
	TMap<CostumeGrade, UWidgetAnimation*> _costumeGradeEffects;

	UPROPERTY()
	TMap<FairyGrade, UWidgetAnimation*> _fairyGradeEffects;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetFairyResult(FairyGrade postGrade, bool isEffect = false);
	void SetCostumeResult(CostumeGrade postGrade, bool isEffect = false);
};
