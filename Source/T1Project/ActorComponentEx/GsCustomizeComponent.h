// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GsCustomizeComponent.generated.h"

class FGsGameObjectDataLocalPlayer;
class AGsCharacterSequencePlayer;

// 시네마틱 플레이시 내케릭터를 복제하기 위한 용처로 활용되는 컴포턴트
// 활용처 AGsCharacterSequencePlayer 
//
UCLASS()
class T1PROJECT_API UGsCustomizeComponent : public USceneComponent
{
	GENERATED_BODY()
	
private:
	FGsCustomizeHandler _customizeHandler;
	bool	_bLateApply = false;
	CustomizeBodyData _bodydata;

public:	
	// Sets default values for this component's properties
	UGsCustomizeComponent();

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:	
	void SetCustomize(UGsGameObjectLocalPlayer* Local, bool In_useWeapon = true);
	void SetCustomizeBoneScaleToSingleMode(UGsGameObjectLocalPlayer* Local, AGsCharacterSequencePlayer* actor);
};
