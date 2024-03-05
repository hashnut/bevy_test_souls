// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/GsManager.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "Blueprint/UserWidget.h"

#include "GsTooltipManager.generated.h"


class UGsUITrayItemNameToolTip;
/**
 * 각종 툴팁 관리
 */
UCLASS()
class T1PROJECT_API UGsTooltipManager :
	public UObject, 
	public IGsManager
{
	GENERATED_BODY()
	
protected:
	TWeakObjectPtr<UUserWidget> _tooltipTargetWidget;

public:
	void SetNameTooltipTarget(TWeakObjectPtr<UUserWidget> InTarget) { _tooltipTargetWidget = InTarget; }
	TWeakObjectPtr<UUserWidget> GetNameTooltipTarget() { return _tooltipTargetWidget; }

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	UGsUITrayItemNameToolTip* OpenNameTooltip(UUserWidget* InTarget, const FText& InName, ItemGrade InGrade = ItemGrade::NORMAL);
	void CloseNameTooltip();

};
