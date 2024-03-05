// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/GsMessageGameObject.h"
#include "GsAreaNameHUD.generated.h"

UCLASS(BlueprintType)
class T1PROJECT_API AGsAreaNameHUD : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AreaName Actor")
		FText AreaAtTheTopName;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AreaName Actor")
		FText AreaAtTheBottomName;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AreaName Actor")
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AreaName Actor")
		FVector2D WidgetScreenSize = FVector2D(400, 180);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AreaName Actor")
		class UWidgetComponentEx* EditorWidgetComp;

private:
	MsgGameObjHandleArray _msgDelegates;

private:
	void PlaySound();

public:	
	// Sets default values for this actor's properties
	AGsAreaNameHUD();

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostRegisterAllComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	bool	IsInCollider = false;

public:
	UFUNCTION()
		void ShowAreaNameHUD(UPrimitiveComponent* in_overlappedComponent,
			AActor* in_otherActor,
			UPrimitiveComponent* in_otherComp,
			int32 in_otherBodyIndex,
			bool in_bFromSweep,
			const FHitResult& in_SweepResult);

	UFUNCTION()
		void EndAreaNameHUD(UPrimitiveComponent* in_overlappedComponent,
			AActor* in_otherActor,
			UPrimitiveComponent* in_otherComp,
			int32 in_otherBodyIndex);

	// 로컬 스폰 끝
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam*);

public:
	bool GetIsInCollider()
	{
		return IsInCollider;
	}
	const FString GetAreaTopName()
	{
		return AreaAtTheTopName.ToString();
	}
	const FString GetAreaBottomName()
	{
		return AreaAtTheBottomName.ToString();
	}
	bool IsIn(const FVector& inLocation);

public:
	// 로딩 무비가 출력중인지
	bool GetIsShowLoadingMovie();
	// 로딩시 뜨는 페이드 이펙트 떠있는지
	bool GetIsShowFadeEffect();

#if WITH_EDITOR
private:
	void ShowAreaNameText();
	void RefreshWidget();
	void RefreshWidgetLocation();
#endif


};
