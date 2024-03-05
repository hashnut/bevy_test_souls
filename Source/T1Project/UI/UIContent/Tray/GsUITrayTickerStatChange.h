// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UTIL/GsGameObjectUtil.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "StatChange/GsUIStatChangeData.h"
#include "GsUITrayTickerStatChange.generated.h"

/**
 * 레벨 업 티커
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerStatChange : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIStatChangePanel* _statChangePanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _closeButton;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;	

public:
	void Play(StatUpdateReason inReason, const TMap<StatType, TPair<int, int>>& inStatDiffMap);
	UFUNCTION()
		void OnClickClose();	
	UFUNCTION()
		void OnFinishAnimation();
};
