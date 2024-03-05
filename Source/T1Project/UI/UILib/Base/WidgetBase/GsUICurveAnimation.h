// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "GsUICurveAnimation.generated.h"


UENUM(BlueprintType)
enum class EGsUIAnimationTriggerType : uint8
{
	OPEN = 0,
	CLOSE,
	TRIGGER,
	TRIGGER1,
	TRIGGER2,
	TRIGGER3,
};


UENUM(BlueprintType)
enum class EGsUICurveAnimationType : uint8
{
	SCALE = 0,
	RENDER_OPACITY,
	TRANSLATION,
	//SCALE_VECTOR,
};
//----------------------------------------------------------------------------------------------------------------------
/*
 * FGsUICurveAnimationData
 * : UI 공통 커브 애니메이션의 커브 정보 저장.
 */
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsUICurveAnimationData
{
	GENERATED_USTRUCT_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "GsUI")
	EGsUICurveAnimationType _animationType;

	UPROPERTY(EditAnywhere, Category = "GsUI")
	class UCurveBase* _curve = nullptr;

	TWeakObjectPtr<class UWidget> _target;
	float _currTime = 0.0f;
	float _maxTime = 0.0f;

public:
	void Reset();
	void SetTarget(class UWidget* InTarget);
	bool IsFinished();
	float GetMaxTime();	
	void InvalidateValue(float InTime);
};
//----------------------------------------------------------------------------------------------------------------------
/*
 * FGsUICurveAnimation
 * : UI 공통 커브 애니메이션 및 타이머 관리를 위한 구조체. GsUIWidgetBase::PlayCurveAnimation 참고.
 */
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsUICurveAnimation
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "GsUI")
	EGsUIAnimationTriggerType _triggerType;

	UPROPERTY(EditAnywhere, Category = "GsUI")
	TArray<FGsUICurveAnimationData> _animationList;

protected:
	// 주의: target은 비우면 안된다.
	UPROPERTY()
	class UWidget* _target = nullptr;

protected:
	float _currTime = 0.0f;	
	bool _bIsPlaying = false;

public:
	DECLARE_DELEGATE_OneParam(FGsOnCurveAnimationFinished, EGsUIAnimationTriggerType);
	FGsOnCurveAnimationFinished OnCurveAnimationFinished;

public:
	void Reset();
	void Clear();
	void SetTarget(class UWidget* InTarget);
	void Play(class UWidget* InOwner);
	void Update(float InDeltaTime);
	bool IsPlaying() const { return _bIsPlaying; }
};
//----------------------------------------------------------------------------------------------------------------------
/*
 * FGsUICurveAnimationList
 * : FGsUICurveAnimation 리스트를 관리
 */
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsUICurveAnimationList
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "GsUI")
	TArray<FGsUICurveAnimation> _animationList;

public:
	void Clear();
	void SetTarget(EGsUIAnimationTriggerType InTriggerType, class UWidget* InTarget);
	bool Play(EGsUIAnimationTriggerType InTriggerType, class UWidget* InOwner);
	void Update(float InDeltaTime);
	bool IsPlaying(EGsUIAnimationTriggerType InTriggerType);
};
