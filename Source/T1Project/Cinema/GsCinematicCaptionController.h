// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsCinematicCaptionController.generated.h"


//class UGsUITrayCimenaticCaption;

UCLASS()
class T1PROJECT_API AGsCinematicCaptionController : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caption")
	FString _strTextNamespace;

protected:
	TWeakObjectPtr<class UGsUITrayCinematicCaption> _trayCaption;

public:	
	// Sets default values for this actor's properties
	AGsCinematicCaptionController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Caption")
	void OnKeyFrameEvent(FString InTextKey, EGsCinematicCaptionType InType = EGsCinematicCaptionType::DEFAULT);

	UFUNCTION(BlueprintCallable, Category = "Caption")
	void StartSound();

protected:
	void OpenCaptionUI();
	void CloseCaptionUI();
};
