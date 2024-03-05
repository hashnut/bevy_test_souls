#pragma once

#include "../GsMovementState.h"
#include "../GsAutoMoveInfo.h"
#include "GsMovementAutoDefine.h"
#include "AI/Navigation/NavigationTypes.h"
#include "T1Project.h"

class FGsWaypointWork;
class AGsPlayerController;
class AGsCharacterLocalPlayer;
class UGsGameObjectLocalPlayer;
class UGsGameObjectCamp;
// 서버와 이동타입 정보를 맞추기 타입중 자동이동쪽을 처리하기 위한 클래스..
// 서버동기화를 위해 추출정보위치가 인풋이동과는 다르지만 서버쪽에 정보전달할때는 모션타입은 같다...
// 대당 클래스 타입체크를 통해 GsMovementHandlerLocalPlayer는 현재 자동이동중인지 확인까지도 가능해지는 효과를 볼수 있음

class T1PROJECT_API FGsMovementStateAuto : public FGsMovementState
{
protected:
	UPathFollowingComponent*	_pathComponent		= nullptr;
	AGsPlayerController*		_playerController	= nullptr;
	AGsCharacterLocalPlayer*	_characterLocal		= nullptr;
	UGsGameObjectLocalPlayer*	_owner				= nullptr;
	FGsAutoMoveInfo				_autoMoveInfo;		// 자동 이동 타겟 정보
	mutable FVector				_destination = FVector::ZeroVector;
	// 웨이 포인트 관련 처리
	FGsWaypointWork*			_waypointWork		= nullptr;
	// 자동진행시 이동 위치가 현재맵이 아니면
	// 워프위치로 이동후 워프타는데
	// 그때 워프 id
	int _warpId;
	// 이동할 map id
	int _moveToMapId;
	// 이동할 최종 위치
	FVector _moveToPos;
	// 다음맵 이동을 위해 워프로 갈때 위치(남은 거리 표시용)
	FVector _moveToWarpPos;
	// 컨텐츠 이동 타입
	EGsMovementAutoContentsType _movementAutoContentsType = EGsMovementAutoContentsType::None;
	// stop 후 재시도시 다시 데이터 복구용
	// https://jira.com2us.com/jira/browse/C2URWQ-6526
	EGsMovementAutoContentsType _backupMovementAutoContentsType = EGsMovementAutoContentsType::None;
	// 현재 내비 path 값
	FNavPathSharedPtr		_currentNavPathPtr;
	bool _isDrawPosSphere = false;
	// 도착지 못가도 진행 할지
	bool _isAllowPartialPath = true;
	// 도착 실패인지(마지막 노드에서 retry할지)
	bool _isNotReachTargetPos = false;
	// 종료 콜백 막는 처리(재이동 처리 시작때 호출)
	bool _isLockFinishCallback = false;
	// 다른맵으로 걸어서 가는가
	bool _isMoveToOtherMap = false;
public:
	explicit FGsMovementStateAuto();
	virtual ~FGsMovementStateAuto();

	//---------------------------------------------------------------------------------------------
	// FGsMovementState
	//---------------------------------------------------------------------------------------------
public:
	virtual const MotionType GetMotionType() const override;
	
public:
	virtual void SetDirection(const FVector& inDir) override;

	virtual int16 GetDirNetConverted() const override;
	virtual float GetFallingSpeed() const override;
	virtual const FVector& GetDirection() const override;
	virtual const FVector& GetLocation() const override;
	virtual const FVector& GetDestination() const override;	
	//---------------------------------------------------------------------------------------------
	// TGsState
	//---------------------------------------------------------------------------------------------
public:
	virtual void Enter() override;
	virtual void Exit()  override;
	virtual void Update(float inDeltaTime) override;

	virtual bool CanTransition(EGsMovementState inType) override;


	//---------------------------------------------------------------------------------------------
	// This
	//---------------------------------------------------------------------------------------------
private:
	void UpdateDirection();
	void CallbackMoving(UPathFollowingComponent* inComponent, FVector& inPos);
	void CallbackFinish(FAIRequestID inID, const FPathFollowingResult& inResult);
	// 워프로 이동 성공
	void CallbackMoveToWarpFinish();
public:	
	// 이전 실패 델리게이트 호출(새로 들어온것의 바인딩된 주체가 다르면)
	void ClearOldFailDelegate(const FGsAutoMoveCompletedOneDelegate& In_newFailDelegate);

	void StartActor(UGsGameObjectLocalPlayer* inOwner, const AActor* inTargetActor, float inAcceptanceRadius = 5.f,
		const FGsAutoMoveCompletedDelegate& inSuccessDelegate = nullptr,
		const FGsAutoMoveCompletedOneDelegate& inFailDelegate = nullptr,
		bool In_isMoveToOtherMap = false);
	void StartPos(UGsGameObjectLocalPlayer* inOwner, const FVector& inTargetLocation, float inAcceptanceRadius = 5.f,
		const FGsAutoMoveCompletedDelegate& inSuccessDelegate = nullptr,
		const FGsAutoMoveCompletedOneDelegate& inFailDelegate = nullptr, 
		bool In_isMoveToOtherMap = false);
	void StartMapIdPos(UGsGameObjectLocalPlayer* inOwner, int inMapId, const FVector& inTargetLocation,
		float inAcceptanceRadius = 5.f,
		const FGsAutoMoveCompletedDelegate& inSuccessDelegate = nullptr,
		const FGsAutoMoveCompletedOneDelegate& inFailDelegate = nullptr);	
	void StartMapIdPositions(UGsGameObjectLocalPlayer* inOwner, int inMapId, const TArray<FVector>& inArrayTargetLocation,
		const FGsAutoMoveCompletedDelegate& inSuccessDelegate = nullptr,
		const FGsAutoMoveCompletedOneDelegate& inFailDelegate = nullptr,
		bool inIsCenterNode = false,
		float In_useAcceptanceRadius = 0.0f);
	void Start(UGsGameObjectLocalPlayer* inOwner, const FGsAutoMoveInfo& inAutoMoveData);
	
	void ClearData();

	// 초기화
	void Initialize(UGsGameObjectLocalPlayer* In_local);
	// 클리어
	void Finalize();
public:
	bool HasReached(const FVector& inTargetLocation, float inAcceptanceRadius) const;

public:
	void SetCharacterLocal(AGsCharacterLocalPlayer* inCharacter) { _characterLocal = inCharacter; }

public:
	void SetPathFollowingComponent(UPathFollowingComponent* inComponent);
	void SetPlayerController(AGsPlayerController* inController);
	void SetData(const FGsAutoMoveInfo& inAutoMoveData) { _autoMoveInfo = inAutoMoveData; }
	void SetAcceptanceRadius(float inAcceptanceRadius);
	void SetMovementAutoContentsType(EGsMovementAutoContentsType In_type);
	void SetIsDrawWaypointDebugPos(bool In_isDraw);
	void SetCurrentNavPath(FNavPathSharedPtr In_path);	
	void SetIsDrawPosSphere(bool In_isDraw)
	{
		_isDrawPosSphere = In_isDraw;
	}
	void SetIsAllowPartialPath(bool In_isAllow)
	{
		_isAllowPartialPath = In_isAllow;
	}
	void SetIsLockFinishCallback(bool In_isLock)
	{
		_isLockFinishCallback = In_isLock;
	}	
	void SetUpdateDesination(bool inUpdate);
public:	
	const FGsAutoMoveInfo&	GetData() const				{ return _autoMoveInfo; }
	const FAIMoveRequest&	GetDataRequest() const		{ return _autoMoveInfo._moveRequest; }
	AGsPlayerController*	GetPlayerController() const	{ return _playerController; }
	const EGsMovementAutoContentsType GetMovementAutoContentsType()const  { return _movementAutoContentsType; }

	void AddPathPoint(FVector In_pos);
	
	FGsWaypointWork* GetWaypointWork() const { return _waypointWork; }
	// 현재 이동 목표 mapId
	int GetMoveToMapId()const;

	// 다음맵 이동할 위치
	const FVector& GetNextMapPos() const
	{
		return _moveToPos;
	}
	// 다음맵으로 이동하는가
	bool IsMoveNextMap() const
	{
		if (_warpId == 0)
		{
			return false;
		}
		return true;
	}

	bool GetIsNotReachTargetPos() const
	{
		return _isNotReachTargetPos;
	}
	// 웨이포인트 남은 거리
	float GetWayPointMoveTargetDist() const;


	/**
	 * Auto Move 상태일때.. 장애물을 회피 하기 위해 장애물 수집.. 해제..
	 */
	void ObstacleDetectedFront(const class UGsGameObjectBase* InBlockObject);

	// 목적지가 대상 캠프 안에 있는가
	// 충돌체 안으로 들어온 이벤트(수호탑 내에 퀘스트 목표가 있을 때 진행 불가 이슈)
	// https://jira.com2us.com/jira/browse/C2URWQ-6363
	bool IsGolPosInTargetCamp(const AActor* In_targetCamp, float& Out_dist);

	// stop 후 재시도시 다시 데이터 복구용
	// https://jira.com2us.com/jira/browse/C2URWQ-6526
	void RestoreBackupMovementAutoContentsType();

	bool IsMoveToOtherMap()
	{
		return _isMoveToOtherMap;
	}
};
