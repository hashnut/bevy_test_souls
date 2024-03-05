// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsUITrayItemNameToolTip.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnFinished);

class UTextBlock;
/**
 *  itemicon name tooltip
 */
UCLASS()
class T1PROJECT_API UGsUITrayItemNameToolTip : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _nameToolTipDurationTime = 3.f;

	TWeakObjectPtr<UUserWidget> _targetWidget;

public:
	FOnFinished _onFinishedDelegate;

private:
	float _curTime = 0.f;
	bool _isHovered = false;

	bool _bReserveShow = false;
	FVector2D _absTargetPos;

	FVector2D _padding;

public:
	void SetNameTooltipData(const FText& InName , ItemGrade InGrade);
	void SetHoverMode(bool InIsHover);
	void SetTarget(TWeakObjectPtr<UUserWidget> InTarget);

	// AddToViewport 이전에 세팅할 것
	void SetTargetPosition(const FVector2D& InAbsPos);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void UpdateTooltipPosition(const FGeometry& MyGeometry);
	
};
