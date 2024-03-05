// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorEx/GsActorPropObject.h"
#include "ActorEx/Interface/GsMeshShaderInterface.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "Editor/GsSelectionPrevantInterface.h"
#include "Quest/GsActorQuestTargetInterface.h"
#include "ActorComponentEx/EGsMeshShaderEffect.h"
#include "GsActorPropTouch.generated.h"

/**
 *
 */
class UWidgetComponent;
class USphereComponent;
class UGsMeshShaderControlComponent;
class UGsUIBillboardInteractionButton;
class USkeletalMeshComponent;
class UBoxComponent;
class UGsUIBillboardPropTouch;
class UGsPropAnimInstance;
class UGsUIBillboard;
class UGsEffectComponent;

UCLASS()
class T1PROJECT_API AGsActorPropTouch : public AGsActorPropObject, public IGsActorInteractionInterface, public IGsSelectionPrevantInterface, public IGsActorQuestTargetInterface, public IGsMeshShaderInterface
{
	GENERATED_BODY()

protected:
	TFunction<void(bool)> _callbackInteractionClick;

	//prop shape
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* _propMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UGsMeshShaderControlComponent* _meshShaderComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* _boxComponent;

	//interaction
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UWidgetComponent* _interactionWidgetComponent;

	//quest
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* _questTargetMark;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
		UGsEffectComponent* _effectComponent = nullptr;

public:
	AGsActorPropTouch();

public:
	virtual void ClickInteraction() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type inReason) override;

private:
	void SetInteractionLocation();
	void SetCollision();

public:
	virtual UWidgetComponent* GetInteractionWidgetComponent() override { return _interactionWidgetComponent; }
	virtual UGsUIBillboardInteractionButton* GetInteractionButton() const override { return nullptr; }
	UGsUIBillboardPropTouch* GetPropTouchButton() const;
	virtual void SetUseInteractionButton(bool inUse) override;
	virtual void SetInteractionClickCallback(TFunction<void(bool)> In_callback) override;	
	virtual UStaticMeshComponent* GetQuestTargetMark() override { return _questTargetMark; }
	void InitMeshShaderComponent();
	UGsPropAnimInstance* GetAnim();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EventPropInteractionBegin();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EventPropInteractionActionFinish();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EventPropInteractionFinish();
	virtual UGsMeshShaderControlComponent* GetMeshShaderComponent() const override { return _meshShaderComponent; }
	virtual void StartMeshShader(EGsMeshShaderEffect inType, bool inIsRestor = false) override;

	virtual UGsUIBillboard* GetWidget() const { return nullptr; }
	UGsEffectComponent* GetEffectComponent() const { return _effectComponent; }
};
