// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Engine/DataTable.h"
#include "GsLobbyConstantData.generated.h"

USTRUCT(BlueprintType)
struct FGsLobbyUIInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyUIData")
	FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyUIData")
	FSoftObjectPath Path;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsLobbyConstantData : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "LobbyData")
	FSoftObjectPath _pathLobbyConstant;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "LobbyData")
	FSoftObjectPath _pathLocalizationConstant;

public:	
	// Sets default values for this component's properties
	UGsLobbyConstantData();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
