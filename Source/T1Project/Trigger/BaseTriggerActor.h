// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTriggerActor.generated.h"

class UGsEditorWidget;
class UBoxComponent;
class UWidgetComponentEx;

const FName TRIGGER_COLLISION_PROFILE = TEXT("Trigger");

UCLASS(BlueprintType)
class T1PROJECT_API ABaseTriggerActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BaseTriggerActor")
	bool CheckOnlyPlayer = true;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BaseTriggerActor")
	int32 MaxCount = 0;

protected:
	int32 CurCount = 0;

#if WITH_EDITORONLY_DATA
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	FVector2D WidgetScreenSize = FVector2D(400.f, 100.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Change level actor")
	UWidgetComponentEx* EditorWidgetComp;
#endif

public:
	// Sets default values for this actor's properties
	ABaseTriggerActor();

public:
	virtual void OnConstruction(const FTransform& InTransform) override;
	virtual void PostRegisterAllComponents() override;
	virtual void BeginPlay() override;

protected:
	bool IsPlayer(AActor* in_actor) const;

#if WITH_EDITOR
protected:
	virtual void RefreshWidget();
	virtual void RefreshWidgetLocation();
	virtual void RefreshWidgetText();

public:
	virtual void SetVisible(bool inVisible);

public:
	virtual FVector GetWidgetLocation() { return GetActorLocation(); }
#endif
};
