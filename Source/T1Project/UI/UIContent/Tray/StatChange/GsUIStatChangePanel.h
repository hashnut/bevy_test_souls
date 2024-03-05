// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UTIL/GsGameObjectUtil.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GsUIStatChangeData.h"
#include "GsUIStatChangePanel.generated.h"

/**
 * 레벨 업 티커
 */


UCLASS()
class T1PROJECT_API UGsUIStatChangePanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _combatPowerTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _upDownWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _statListView;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
		class UWidgetAnimation* _openAnim;
		
private:
	FWidgetAnimationDynamicEvent _animationFinishEvent;
	TFunction<void(void)> _callbackFunc;

private:
	TMap<StatType, TPair<int, int>> _statMap;
	int32 _statSetIndex;
	int32 _maxStatSetCount;
	int32 _statSetShowCount = 3;
	float _statNotifyShowTime = 2;
	float _statNotifyProduceTime = 0.5;
	float _time = 0;
	TArray<StatType> _statKeyArray;
	bool _tickEnable;
	CreatureWeaponType _preWeaponType;
	CreatureWeaponType _curWeaponType;
	StatUpdateReason _reasonType;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	void Play(StatUpdateReason inReason , const float inOption, const TMap<StatType, TPair<int, int>>& inStatDiffMap , TFunction<void(void)> inCallbackFunc);

public:
	void InvalidCombatPower(const TMap<StatType, TPair<int, int>>& inStatDiffMap);
	void InitCombatPower();
	UFUNCTION()
		void OnFinishAnimation();
	void InvalidStatSet();
	bool CheckFinish();
	void RemoveShowStatType();
	void StartPlayAnimation();

};
