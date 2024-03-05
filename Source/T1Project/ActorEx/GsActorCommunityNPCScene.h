// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsActorCommunityNPCScene.generated.h"

class USceneCaptureComponent2D;
class USkeletalMeshComponent;
class UCameraComponent;
class AGsCharacterNonPlayer;

UCLASS()
class T1PROJECT_API AGsActorCommunityNPCScene : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _characterRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _camera;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//USceneCaptureComponent2D* _captureCamera;

	UPROPERTY(Category = "GsActor", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* _mesh;

	UPROPERTY(Category = "GsActor", VisibleAnywhere, BlueprintReadWrite, EditFixedSize, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* _meshFace;

	// 클릭 시 보일 이펙트 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	FSoftObjectPath _touchVFXPath;

	// 이펙트 ZOrder
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	int32 _touchVFXZOrder = 1000;

private:
	bool _isTouchStart{ false };
	
public:	
	// Sets default values for this actor's properties
	AGsActorCommunityNPCScene();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActionSelectTab();
	void ActionTouch(const FVector2D& pos);

	void LoadAnimInstance(const FString& ClassPath);
	void InitAction();
	void EndAction();
};
