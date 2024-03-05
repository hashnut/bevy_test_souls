// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Chaos/Array.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageDelegateList.h"
#include "Classes/GsManager.h"
#include "GameObject/Reserve/GsReserveWork.h"
#include "GameObject/GsGameObjectHandler.h"

class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;
class UGsGameObjectDropObject;
struct FGsSchemaDropObject;
struct IGsMessageParam;

/**
 * Looting 관련 처리 핸들러 클래스
 * LocalPlayer만 가지고 있는 기능이기 때문에 현재는 따로 Base 클래스를 생성 하지 않음
 */
class T1PROJECT_API FGsLootingHandlerLocalPlayer : public IGsGameObjectHandler,
	public TGsMessageDelegateList<MGameObject, MessageGameObject>,
	public IGsReserveWork
{
public:
	// 예약 구조체
	struct ReserveLootingInfo
	{
		TWeakObjectPtr<UGsGameObjectDropObject> _targetObject;		
		bool _isMoveComplete = false;
		bool _isAutoLooting = true;
	};

private:
	TWeakObjectPtr<UGsGameObjectLocalPlayer> _local;	
	ReserveLootingInfo _reserveLootInfo;	

public:
	FGsLootingHandlerLocalPlayer() : IGsGameObjectHandler(GameObjectHandlerType::LOOT) {};
	virtual ~FGsLootingHandlerLocalPlayer() = default;

public:
	// 루팅 범위 구하기
	static float GetLootRange(bool In_isOrigin = false);

	// 가상 함수
public:
	// 예약 데이터 있는가
	virtual bool IsReserveData() override;
	// 재시도 해라
	virtual void DoRetry() override;
		 
public:
	// 초기화
	virtual void Initialize(UGsGameObjectBase* In_owner) override;
	virtual void Update(float In_delta) override;
	virtual void Finalize() override;

public:
	// 루팅 시도
	void TryLooting(UGsGameObjectBase* DropObject, bool isAutoLooing);
	// 이동 관련 처리 함수
	void OnMoveToLoot(UGsGameObjectBase* Target);
	// 예약된 데이터 클리어
	// GameId == 0 이면 무조건 Clear
	void ClearReserveLootInfo(uint64 GameId = 0);

private:
	// 이동 성공
	void CallbackMoveToLootEnd();
	// 이동 실패
	void CallbackMoveToLootFailed(const FPathFollowingResult& Result);

private:
	// DropObject(루팅) Pick
	void OnDropObjectPick(const IGsMessageParam* In_param);
	void OnSkillEnd(const IGsMessageParam* In_param);
	void OnClearReserveLootInfo(const IGsMessageParam* In_param);

public:
	// 예약된게 있나
	bool IsReserveLootng() const;
	UGsGameObjectDropObject* GetLootingTarget() const;	
	const ReserveLootingInfo& GetReservedLootingInfo();	
};
