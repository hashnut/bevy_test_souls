// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UTIL/GsGameObjectUtil.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "GsUITrayTickerLevelUp.generated.h"

/**
 * 레벨 업 티커
 */
UCLASS()
class T1PROJECT_API UGsUILevelUpStatPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _statListView;

public:
	void SetData(const TMap<StatType, FGsStatDiffInfo>& inStatMap);
};

UCLASS()
class T1PROJECT_API UGsUITrayTickerLevelUp : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _textBlockLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIStatChangePanel* _statChangePanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _closeButton;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
		class UWidgetAnimation* _ani_start;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
		class UWidgetAnimation* _ani_end;
		
private:
	FWidgetAnimationDynamicEvent _animationFinishEvent;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	void CreateStatMap(OUT TMap<StatType, int32>& outStatArray, const struct FGsSchemaLevelStatTable* inStat);

public:
	void SetData(uint16 inPreLevel, uint16 inCurrentLevel);
	UFUNCTION()
		void OnClickClose();	
	UFUNCTION()
		void OnFinishAnimation();
};
