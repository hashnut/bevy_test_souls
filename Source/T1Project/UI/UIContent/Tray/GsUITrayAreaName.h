// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "TextBlock.h"
#include "Overlay.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "SlateCore/Public/Layout/Geometry.h"

#include "GsUITrayAreaName.generated.h"

/**
 *  현재 위치한 지역명 출력 티커
 */

class UWidgetAnimation;

UCLASS()
class T1PROJECT_API UGsUITrayAreaName : public UGsUITray
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UTextBlock* KorName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UOverlay* _areaOverlay;

	// fade animation
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _aniFadeOut;
	// start-> close time
	UPROPERTY(Category = "GsUI", EditAnywhere)
	float _waitSecond = 5.0f;
private:
	// wait start time
	int64 _startTime;
	// wait flag
	bool _isWaitEnd = false;
	// wait tick time
	float _timeTickWaitTime = 0.0f;
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;
public:
	void SetAreaName(const FText& In_name);	

	void StartAnimation();

public:
	virtual TrayCondition GetTrayCondition() const override { return TrayCondition::TC_Priority; }
};
