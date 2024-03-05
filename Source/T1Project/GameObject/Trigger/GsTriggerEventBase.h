// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameObject/Define/GsGameObjectDefine.h"
#include "TimerManager.h"

class UGsGameObjectBase;
class UWorld;

enum class EGsTriggerType
{
	Projectile,
};

/**
 * 각종 트리거 이벤트 제어 클래스
 */
class T1PROJECT_API FGsTriggerEventBase
{
protected:
	class UGsGameObjectBase* _owner = nullptr;
	EGsGameObjectType _targetType = EGsGameObjectType::Base;
	FTimerHandle _timerHandle;
	FTimerDelegate _timerDelegate;
	// 계속 체크할지 유무
	bool _isUpdate = false;

public:
	FGsTriggerEventBase() = default;
	virtual ~FGsTriggerEventBase();

public:
	// EGsGameObjectType TargetType : 이벤트를 전달할 대상 타입
	virtual void Active(class UGsGameObjectBase* Owner, EGsGameObjectType TargetType = EGsGameObjectType::Base);
	virtual void Deactive();

protected:
	// 결과값이 true인것은 현재 트리거의 상태가 변경됨을 의미
	// 실제 Trigger on/off값을 확인하려면 _isActive 갱신이 필요
	virtual bool CheckTrigger();

	class UWorld* GetWorld() const;

public:
	virtual EGsTriggerType GetType() const = 0;
};
