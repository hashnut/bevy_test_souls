// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "GsSkillCollisioHandlerComponent.generated.h"

struct FTimerHandle;
struct FGsSchemaSkillNotifySet;
struct FGsSchemaSkillCollision;
class UGsGameObjectBase;
class UGsGameObjectCreature;
//class UGsSkillNotifyDataBase;
//class UGsSkillNotifyDataCollision;
//class UGsSkillNotifyDataGroupCollision;
//class UGsSkillNotifyDataRandomCollision;
class UGsSkillCollisionComponent;
class UGsSkillGuideEffectComponent;

/**
 * 복수 충돌/랜덤 충돌 등의 기능이 들어오면서 SkillNotify에서 처리할수 없는 구조
 * 스킬 시간 설정등이 실제 충돌체 처리보다 짧을수 있어 따로 충돌 기능 컴퍼넌트 생성
 * UGsSkillCollisioComponent의 충돌 로직과
 * UGsSkillGuideEffectComponent의 예시선 정보 출력을 타입에 따라 활용
 */
UCLASS()
class T1PROJECT_API UGsSkillCollisioHandlerComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UGsSkillCollisionComponent*		_skillCollisionComponent = nullptr;
	UPROPERTY()
	UGsSkillGuideEffectComponent*	_skillGuideEffectComponent = nullptr;

private:
	UGsGameObjectBase*		_owner = nullptr;
	UGsGameObjectBase*		_target = nullptr;
	UGsGameObjectCreature*	_ownerCreature = nullptr;

	//TArray<FTimerHandle>	ListTimerHandle;

	// 지정된 예시선 위치에 충돌 판정 (GroupCollision)
	FVector _fixedWorldPos;

	// 충돌 정보 캐싱
	//TArray<UGsSkillNotifyDataCollision*> _listCollisionInfo;
	
	// 랜덤 콜리전 정보
	//UGsSkillNotifyDataGroupCollision* _pickCollision = nullptr;

public:
	virtual void EndPlay(EEndPlayReason::Type inType) override;

public:
	void Set(UGsGameObjectBase* owner, const FGsSchemaSkillCollision& Data, int TargetCount = 1, UGsGameObjectBase* target = nullptr);

public:
	FVector GetCurrentPos() const;

private:
	//Single 충돌
	//void SetSingle(const UGsSkillNotifyDataCollision* Data, FVector Offset = FVector::ZeroVector);	
	// Group 충돌
	//void SetGroup(const UGsSkillNotifyDataGroupCollision* Data);
	// Random 충돌
	//void SetRandom(const UGsSkillNotifyDataRandomCollision* Data);

private:
	void ResetComponent(UGsGameObjectBase* owner);

private:
	// 랜덤 시작
	//void TimerCallbackRandomStart();

	// 그룹 충돌 활성
	//void TimerCallbackGroupCollision();

	// 개별 충돌 활성
	//void TimerCallbackSingleCollision(
	//	const UGsSkillNotifyDataCollision* Data, FVector Offset , float SkillCoefficientValue
	//);
};
