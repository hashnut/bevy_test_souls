// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GsCharacterLobbyPlayer.generated.h"

/**
 * 
 */
class FGsCameraHandlerLobby;

class UStaticMeshComponent;
class UMaterial;

UCLASS()
class T1PROJECT_API AGsCharacterLobbyPlayer : public AGsCharacterLocalPlayer
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTouchPress, FVector /*InLocation*/);
	
	// 멤버변수
private:
	bool _bTouchPressed = false;
	FVector _prevTouchLocation = FVector::ZeroVector;
	// 터치 줌인, 아웃용
	// 0번 터치 이전 위치
	FVector _prevTouchLoc1;
	// 1번 터치 이전 위치
	FVector _prevTouchLoc2;

	// 터치줌 조절값(mobile)
	float _touchZoomFactorMobile = 0.5;
	// touch zoom power pc
	float _touchZoomFactorPC = 0.5;
	// pitch 회전에 곱할 비율값
	float _rotPitchAspectRate = 0.0f;
	// pitch 회전 추가 곱
	float _pitchRotMul = 1.0f;
	float _touchMoveFactorMobile = 0.25f; // 터치 이동거리 조절값(값이 작아질수록 움직임 작아짐)(mobile)
	float _touchMoveFactorPC = 0.25f; // 터치 이동거리 조절값(값이 작아질수록 움직임 작아짐)(pc)
	float _touchMoveLimit = 1.5f; // 터치 이동거리 제한값(min, max가 됨)
	bool _isZoomMode = false;
	// 이전 거리
	float _prevDist;
	// 로비 줌 핸들러
	FGsCameraHandlerLobby* _cameraHandler = nullptr;	
public:
	FOnTouchPress _onTouchPressDelegate;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;

	virtual void Tick(float In_delta) override;


protected:
	// 위젯 컴포넌트 동작을 막기위해 오버라이드
	virtual void InitWidgetComponent() override;

public:
	UFUNCTION(BlueprintCallable, Category = "GameObject")
	class UStaticMeshComponent* GetWeaponMesh();

	UFUNCTION(BlueprintCallable, Category = "GameObject")
	class UStaticMeshComponent* GetWeaponSubMesh();

	UFUNCTION(BlueprintCallable, Category = "GameObject")
	class UMaterial* GetWeaponMaterial();

	UFUNCTION(BlueprintCallable, Category = "GameObject")
	class UMaterial* GetWeaponSubMaterial();

	// 로직함수
	// 이벤트
public:
	// Touch Interaction
	void OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location); // Location Z == 1
	void OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location); // Location Z == 0
	void OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location); // Location Z == 1
	void OnMoveRotateYaw(float Value);
	void OnMoveRotatePitch(float Value);
	void OnZoomIn();
	void OnZoomOut();

public:
	// 줌모드 갱신
	void UpdateZoomMode();
	void ZoomIn(float In_val);
	void ZoomOut(float In_val);
	// 스프링 암 다른데 붙인다(키 조절 카메라 움직임 이슈)
	void AttachSpringArmTarget(AActor* In_target);
	

	void InitState(float In_zoomRate);
	

	// get, set
public:
	float GetTouchMoveLimitMax() const { return _touchMoveLimit; }
	float GetTouchMoveLimitMin() const { return -_touchMoveLimit; }
	FOnTouchPress& GetTouchPressDelegate() { return _onTouchPressDelegate; }
	FGsCameraHandlerLobby* GetCameraHandlerLobby() { return _cameraHandler; }
	// 줌 강도 가져오기
	float GetZoomPower();

	void SetTouchZoomFactorMobile(float In_value)
	{
		_touchZoomFactorMobile = In_value; 
	}

	float GetTouchMoveFactor();
};
