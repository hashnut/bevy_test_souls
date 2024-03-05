// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Tray/GsUITrayTickerAnimation.h"
#include "GsUITrayTickerInvadeOneText.generated.h"

/**
 * 
 */
UENUM()
enum class EGsInvadeOneType : uint8
{
	TEMPLE_KNIGHT_DEATH,
	CARRIAGE_SPAWN,
	DEPAWN_MONSTER_SPAWN
};

UCLASS()
class T1PROJECT_API UGsUITrayTickerInvadeOneText : public UGsUITrayTickerAnimation
{
	GENERATED_BODY()	

public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UTextBlock* _contextTextBlock;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UWidgetSwitcher* _iconSwitcher;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* _iconImage;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* _inAnim;
public:
	virtual void NativeOnInitialized() override;
	void SetInvadeOneTextData(const FText& inContext, const FSoftObjectPath& inIconPath);

public:
	virtual TrayCondition GetTrayCondition() const override { return TrayCondition::TC_Priority; }

};
