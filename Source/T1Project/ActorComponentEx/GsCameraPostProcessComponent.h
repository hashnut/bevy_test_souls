// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Classes/GsMessageHandlerOneParam.h"
#include "GsCameraPostProcessComponent.generated.h"

class UCurveFloat;
class UGsSimpleTimer;
class UGsCurveTimer;
class UCameraComponent;
class UMaterialInstance;

UENUM()
enum class EGsPlayerPostProcess : uint8
{
	None,
	DeathDimed,
	HpWarning,
	DeathDimedFadeOut,
	Blur,
};

UENUM()
enum class EGsPostProcessCurveEvent : uint8
{
	TickEvent = 0,
	FinishEvent,
};

USTRUCT()
struct FPostProcessMaterialData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "PostProcessMaterialData")
	UMaterialInstance* _materialInstance;
	UPROPERTY(EditDefaultsOnly, Category = "PostProcessMaterialData")
	float _initialBlendWeight = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "PostProcessMaterialData")
	TArray<EGsPlayerPostProcess> _usageType;
	UPROPERTY(EditDefaultsOnly, Category = "PostProcessMaterialData")
	bool _support_Platform_PC = true;
	UPROPERTY(EditDefaultsOnly, Category = "PostProcessMaterialData")
	bool _support_Platform_Android = true;
	UPROPERTY(EditDefaultsOnly, Category = "PostProcessMaterialData")
	bool _support_Platform_IOS = true;
};

USTRUCT()
struct FPostProcessCurveData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "PostProcessEffectData")
		UCurveFloat* _curve;
	UPROPERTY(EditDefaultsOnly, Category = "PostProcessEffectData")
		int32 _materialIndex;
};

//For save
USTRUCT()
struct FCameraMotionBlurData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "UGsCameraMotionBlur")
	UCurveFloat* _motionBlurTime;
	UPROPERTY(EditDefaultsOnly, Category = "UGsCameraMotionBlur")
	float _motionBlurAmount = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UGsCameraMotionBlur")
	bool _support_Platform_PC = true;
	UPROPERTY(EditDefaultsOnly, Category = "UGsCameraMotionBlur")
	bool _support_Platform_Android = true;
	UPROPERTY(EditDefaultsOnly, Category = "UGsCameraMotionBlur")
	bool _support_Platform_IOS = true;
};

//For runtime logic
UCLASS()
class UPostProcessCurve : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "PostProcessEffectData")
	UCurveFloat* _curve;
	UPROPERTY(EditDefaultsOnly, Category = "PostProcessEffectData")
	int32 _materialIndex;

public:
	using MPostProcessDelegate = TGsMessageHandlerOneParam<EGsPostProcessCurveEvent, const UPostProcessCurve*>;
	MPostProcessDelegate PostProcessCurveMessage;

private:
	int32 _realMaterialIndex;

	UPROPERTY()
	float _currentWeight;
	UPROPERTY()
	UGsSimpleTimer* _timer;

public:
	void Initialize();
	void Tick(float inTime);

public:
	UGsSimpleTimer* GetTimer() const { return _timer; }

	bool IsValid() const { return _curve != nullptr; }

	void SetRealMaterialIndex(int32 inIndex) { _realMaterialIndex = inIndex; }	
	int32 GetRealMaterialIndex() const { return _realMaterialIndex; }

	float GetCurrentWeight() const;

	void Set(const FPostProcessCurveData& inData)
	{ 
		_curve = inData._curve;
		_materialIndex = inData._materialIndex;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsCameraPostProcessComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<EGsPlayerPostProcess, UPostProcessCurve*> _postProcessMap;

private:
	UPROPERTY()
	UCameraComponent* _camera = nullptr;

	UPROPERTY()
	UGsCurveTimer* _motionBlurTimer;

	bool _notifyMotionBlur = false;
	static bool _activateMotionBlurMode;
	float _amoutMotionBlur = 0.0f;

public:	
	// Sets default values for this component's properties
	UGsCameraPostProcessComponent();	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type inType) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	void Initialize(UCameraComponent* inCamera);
	bool Start(EGsPlayerPostProcess inType);
	bool Reset(EGsPlayerPostProcess inType);
	bool Stop(EGsPlayerPostProcess inType);

public:
	UFUNCTION()
	void FinishMotionBlur(const UObject* timer);

public:
	void StartMotionBlurNotify(UCurveFloat* curveData, bool IgnorePreEvent = false);
	static void SetMotionBlurMode(bool inActive);

	//get set
public:
	UGsSimpleTimer* GetTimer(EGsPlayerPostProcess inType) const;
	UPostProcessCurve* GetCurveData(EGsPlayerPostProcess inType) const;
	bool Contains(EGsPlayerPostProcess inType) const;
};
