// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObject/ObjectClass/GsGameObjectPropTouch.h"
#include "Message/GsMessageGameObject.h"
#include "GsGameObjectPropTouchInvasion.generated.h"

enum class InvasionTouchAction : uint8 { None = 0, Register = 1, EnrollingIn, Invasion };
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsGameObjectPropTouchInvasion : public UGsGameObjectPropTouch
{
	GENERATED_BODY()

protected:
	InvasionTouchAction _touchAction{ InvasionTouchAction::None };
	MsgGameObjHandle _propInstantStart;

protected:
	virtual void InitializeActor(AActor* Owner) override;

public:
	virtual EGsGameObjectType GetObjectType() const override;
	virtual void Initialize(const FGsSpawnParam* SpawnParam) override;
	virtual void SetInteractionActive(bool In_isActive, int In_Index = 0) override;
	virtual void SetInteractPropEnd(bool inSuccess) override;

public:
	void SetAciveCallbackInvasion(bool active);
	void SetInvasionInteractEnd(bool isSuccess);

	virtual void UpdatePropState() override;
	virtual void InvalidWidget() override;

	virtual bool IsContentsUnlock() override;

	void InstantStartPropInteraction(const struct IGsMessageParam* inMessage);
	void InstantcEndPropInteraction(const struct IGsMessageParam* inMessage);
	
	InvasionTouchAction GetCurrentTouchAction() { return _touchAction; }

	void OnStartInvasionInterAct();
	void OnCancelInvasionInterAct();
	int32 GetEnrolledPlayerCountInProp() const { return _interactionEndUserCount; }
};
