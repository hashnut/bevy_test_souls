#pragma once

#include "../Movement/GsAutoMoveInfo.h"
#include "../Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/GsGameObjectHandler.h"
#include "GameObject/GsGameObjectHandlerEnum.h"

class FGsAIAvoidCollisionHandler : public IGsGameObjectHandler
{
private:
	class UGsGameObjectLocalPlayer* _localPlayer = nullptr;

	// 2023/10/12 PKT - 회피 진행 중인가?
	bool _isAvoidingCollision = false;

	float _elapsedTime = 0.f;
	// 2023/10/12 PKT - 기존 AI가 지니고 있던 이동 정보(회피 정보가 아닌 본래 목적지)
	FGsAutoMoveInfo _orgMoveInfo;	
	// 2023/10/13 PKT - 목적지 Map이 현재 Map과 다른가?
	bool _isDifferentDestinationMap = false;

	EGsMovementAutoContentsType _OrgContentsType = EGsMovementAutoContentsType::None;

public:
	FGsAIAvoidCollisionHandler() : IGsGameObjectHandler(GameObjectHandlerType::AI_AVOIDCOLLISION) {}
	virtual ~FGsAIAvoidCollisionHandler() = default;

protected:
	void Reset();
	
	bool ReversgDestination(const FGsAutoMoveInfo& InOrgMoveInfo, EGsMovementAutoContentsType InMoveAutoContentsType, bool InIsOtherMap);

	// 2023/10/12 PKT - 회피 지점을 찾는다.
	bool findAvoidancePoint(const class UGsGameObjectLocalPlayer* InLocalPlayer, const class UGsGameObjectBase* InTargetObject, FVector& OutPoint);
	// 2023/10/19 PKT - 목적지에 도착 하였는가?
	bool IsDestinationReached(const FVector& InDest);

	// 2023/10/12 PKT - 본래 목적지 정보를 통해 가고자 했던 의도를 구함.
	bool IsMoveToInteractWithNPC();
	bool HasArrivedAtDestinationInteractWithNPC();
	bool HasArrivedAtDestinationQuestZone(const FVector& InGoalPos);

public:
	virtual void Initialize(class UGsGameObjectBase* In_local) override;
	virtual void Finalize() override;
	virtual void Update(float inDeltaTime) override;
	
	void BegineDetector();
	void EndDetector();

	// 2023/10/12 PKT - 전방에 장애물 감지
	bool ObstacleDetectedFront(const class UGsGameObjectBase* InBlockCreature, EGsMovementAutoContentsType InMoveAutoContentsType, const struct FGsAutoMoveInfo InAutoMoveInfo);
	// 2023/10/12 PKT - 회피 지점 도착 / 실패 Delegate
	void OnFinishAvoidanceMode();
	void OnFailedAvoidanceMode(const struct FPathFollowingResult& InResult);

public:
	bool IsAvoidingCollision() const			
	{
		return _isAvoidingCollision; 	
	}
};