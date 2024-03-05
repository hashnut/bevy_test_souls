// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "GsGameViewPortClient.generated.h"

/**
 * 
 */
 class USignificanceManager;

UCLASS()
class T1PROJECT_API UGsGameViewPortClient : public UGameViewportClient
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TArray<FTransform> _viewPortArray;
	UPROPERTY(Transient)
	USignificanceManager* _significantManager;

protected:
	FDelegateHandle _handleResize;
	FDelegateHandle _handleTouch;

public:
	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
	virtual void BeginDestroy() override;
	virtual void Tick(float InDeltaTime) override;

	virtual void LostFocus(class FViewport* InViewport) override;
	virtual void ReceivedFocus(class FViewport* InViewport) override;

public:
	void InitSignificanceManager();	

public:	
	void ClearViewTransformArray()					{_viewPortArray.Empty();}
	void AddViewTransformArray(FTransform& inView)	{_viewPortArray.Add(inView);}

protected:
	void OnResizeViewport(class FViewport* InViewport, uint32 InNum);

public:
	void UpdateDragTriggerDistance();
};
