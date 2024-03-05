// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "WidgetComponentEx.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInitWidgetEvent);

UCLASS(Blueprintable, ClassGroup = "UserInterface", hidecategories = (Object, Activation, "Components|Activation", Sockets, Base, Lighting, LOD, Mesh), editinlinenew, meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UWidgetComponentEx : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Child component ex")
		FInitWidgetEvent _onInitWidget;	
	UPROPERTY(BlueprintReadWrite)
		bool _enableFaceToCamera = false;
	UPROPERTY(BlueprintReadWrite)
		bool _isDistanceCheck = true;
	UPROPERTY(BlueprintReadWrite)
		float _viewDistance = 30000;
	UPROPERTY(BlueprintReadWrite)
		bool _isVisible = true;

public:
	UWidgetComponentEx();
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:
	void FaceToCameraTick(float in_deltaTime, enum ELevelTick in_tickType);
	virtual bool IsInViewDistance(UWorld* inWorld, enum ELevelTick inTickType);

public:
	void SetVisible(bool inVisible){_isVisible = inVisible;}
	void SetVisibleWidget();
	void SetInvisibleWidget();	
};
