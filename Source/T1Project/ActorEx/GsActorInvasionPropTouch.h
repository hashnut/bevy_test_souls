// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorEx/GsActorPropTouch.h"
#include "GsActorInvasionPropTouch.generated.h"

class UGsUIBillboardInvasionWarp;
class USphereComponent;
class UWidgetComponent;
//class UWidgetInteractionComponent;

UENUM(BlueprintType)
enum class InvasionPropState : uint8 { Spawn, ActiveIdle, DeActiveIdle };
/**
 * 
 */
UCLASS()
class T1PROJECT_API AGsActorInvasionPropTouch : public AGsActorPropTouch
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _nameplateWidget;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	//USphereComponent* _sphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsActorPropTouch", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _interactionWidgetInvasion;

//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsActorPropTouch", meta = (AllowPrivateAccess = "true"))
//	UWidgetInteractionComponent* _interaction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _rootWidget;

	UPROPERTY()
	InvasionPropState _curIdleState{ InvasionPropState::DeActiveIdle };
	TFunction<void(bool)> _callbackInvasioinClick;

protected:
	UGsUIBillboardInvasionWarp* GetInvaderInfoPlate() const;

public:
	AGsActorInvasionPropTouch();

	virtual void BeginPlay() override;

public:
	void UpdateInvaderInfoPlate();
	virtual UGsUIBillboard* GetWidget() const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "GsInvasionPropState")
	void SetActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "GsInvasionPropState")
	void SetDeactive();

	UFUNCTION(BlueprintCallable)
	void OnChangedState(InvasionPropState curState);

	UFUNCTION(BlueprintCallable)
	InvasionPropState GetInvasionPropState() { return _curIdleState; }

	//USphereComponent* GetInvasionCollider();

	void SetCallbackInvasion(TFunction<void(bool)> In_callback);
};
