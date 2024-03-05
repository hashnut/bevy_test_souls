// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameObject/Fairy/GsFairyHandlerLocalPlayer.h"
#include "GsWidgetComponentFairyTalk.generated.h"

struct FGsFairyData;
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UGsWidgetComponentFairyTalk : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UGsWidgetComponentFairyTalk();

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	void SetData(TWeakPtr<FGsFairyData> data);
	void SetSkillTalk();
	void SetVehicleTalk(EGsFairyTalkType TalkType);

protected:
	TWeakPtr<FGsFairyData> _data;

	friend class FGsFairyHandlerLocalPlayer;
	
};
