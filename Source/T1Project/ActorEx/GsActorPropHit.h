// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorEx/GsActorPropObject.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "GsActorPropHit.generated.h"

/**
 *
 */
class UWidgetComponent;
class USphereComponent;
class UGsMeshShaderControlComponent;
class UGsUIBillboardInteractionButton;
class USkeletalMeshComponent;

UCLASS()
class T1PROJECT_API AGsActorPropHit : public AGsActorPropObject, public IGsActorInteractionInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UWidgetComponent* _questWidgetComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UWidgetComponent* _interactionWidgetComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UGsMeshShaderControlComponent* _meshShaderComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* _propMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* _questTargetMark;

public:
	AGsActorPropHit();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type inReason) override;
	

public:	
	virtual void ClickInteraction() override;

public:
	virtual UWidgetComponent* GetInteractionWidgetComponent() override { return nullptr; }
	virtual UGsUIBillboardInteractionButton* GetInteractionButton() const override;
	virtual void SetUseInteractionButton(bool inUse) override;
	virtual void SetInteractionClickCallback(TFunction<void(bool)> In_callback) override {}
	UGsMeshShaderControlComponent* GetMeshShaderComponent() const { return _meshShaderComponent; }
	virtual UStaticMeshComponent* GetQuestTargetMark() { return _questTargetMark; }
};
