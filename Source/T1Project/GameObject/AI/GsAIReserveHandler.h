#pragma once

#include "GameObject/GsGameObjectHandler.h"
#include "GameObject/GsGameObjectHandlerEnum.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "Message/GsMessageGameObject.h"

#include "Runtime/Engine/Classes/Engine/EngineTypes.h"

/*
* 
*/

class UGsGameObjectBase;
class FGsAIReserveJob;
class FGsAICondition;

struct IGsMessageParam;

class FGsAIReserveHandler : public IGsGameObjectHandler
{
private:
	TArray<FGsAIReserveJob*> _arrayReserveJob;
	TArray<FGsAIReserveJob*> _arrayPostAddReserveJob;
	int _reamainRetryCount;

	bool _isRetry = false;

	MsgGameObjHandleArray _gameobjectDelegates;

	float _pauseTime = 0.0f;
	int64 _pauseStartTime = 0;
	FVector _localPausePose;

	bool _isDirtyReserveRetry = false;
	// block target(local can't move)
	int64 _blockTargetGameId;

public:
	FGsAIReserveHandler() : IGsGameObjectHandler(GameObjectHandlerType::AI_RESERVE) {}
	virtual ~FGsAIReserveHandler() = default;

	// virtual func
public:
	// 초기화
	virtual void Initialize(UGsGameObjectBase* In_local) override;

	// 해제
	virtual void Finalize() override;

	// logic func
public:
	void ClearAllJob();
	void MakeJob(EGsAIActionType In_type, int In_reserveCount, 
		TArray<FGsAICondition*> In_actionCondition,
		TArray<FGsAICondition*> In_clearCondition,
		bool In_isHighPriority);
	void ClearJob(EGsAIActionType In_type, UGsGameObjectBase* In_target);
	void ClearReserveJob(FGsAIReserveJob* In_removeJob);
	void ClearAllJobByType(EGsAIActionType In_type);
	void AddJob(EGsAIActionType In_type, UGsGameObjectBase* In_target, int In_addIndex);

	void StartPauseTime(const FVector& In_currentLocalPos);
	void UpdatePauseTime(const FVector& In_currentLocalPos);

	void RetryByAutoPause();
	void ClearRetry();

	bool CheckFirstJob(FGsAIReserveJob*& Out_firstJob);

	void ClearRetrySingleJob(FGsAIReserveJob* In_job);
	void SetBlockTarget(FGsAIReserveJob* In_job);

	void PostMakeJobsByEvaluateConditions();

	// mesage func
public:
	void OnAIClearReserveJob(const IGsMessageParam* In_param);
	void OnAIClearReserveAllJobByType(const IGsMessageParam* In_param);
	void OnAIAddReserveJob(const IGsMessageParam* In_param);	
	void OnAIModeActive(const IGsMessageParam*);
	void OnLocalSpawnMeComplete(const IGsMessageParam*);
	void OnLocalAutoMoveStop(const IGsMessageParam*);

	// get, set
public:
	bool IsReserveJobEmpty();

	TArray<UGsGameObjectBase*> GetActionTarget(EGsAIActionType In_action);

	FGsAIReserveJob* GetFirstJob();
	// get first job
	// check clear job -> clear not possible job
	FGsAIReserveJob* GetClearConditionCheckedFirstJob();	

	bool GetIsRetry()
	{
		return _isRetry;
	}
	// check already exist
	bool IsExistTargetObj(UGsGameObjectBase* In_targetObj);
	// 액션 타입 이미 있는지 체크
	bool IsExistAction(EGsAIActionType In_actionType);

	bool GetIsDirtyReserveRetry()
	{
		return _isDirtyReserveRetry;
	}

	TArray<FGsAIReserveJob*> GetArrayReserveJob()
	{
		return _arrayReserveJob;
	}
	// 첫번재 잡이 해당 액션인가
	// 없으면 false
	bool IsFirstJobAction(EGsAIActionType In_action);
};