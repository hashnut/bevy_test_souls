// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NoExportTypes.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Checker/GsSkillCheckArray.h"

DECLARE_DELEGATE(FGsUseSkillRequestInterval);
/**
* 최종 가공된 사용 스킬 구조체 정보
*/
struct T1PROJECT_API FGsUseSkillRequest
{
	const class FGsSkill* _skillData	= nullptr;
	// 검색 대상
	int64 _targetGameId = 0;

	float _deltaTime = 0.f;
	float _elapsedTime = 5.f;

	bool IsVaild() const { return nullptr != _skillData; }

	// 데이터 클리어
	void Clear();
};

/**
* 캐스팅 스킬의 캐스팅 정보
*/
struct T1PROJECT_API FGsCastingSkillRequest
{
	// 스킬 아이디
	int _castingSkillId = 0;
	// 캐스팅완료 확인
	bool _castingComplete = false;
	// 캐스팅 시간
	FDateTime _castingTimer;

	bool IsVaild() const { return _castingSkillId != 0; }
	float GetElapsed() { return (FDateTime::Now() - _castingTimer).GetTotalSeconds(); }

	// 데이터 클리어
	void Clear();
};

/**
* 예약 스킬의 정보
*/
struct T1PROJECT_API FGsSkillReserve
{
	const class FGsSkill* _skillData = nullptr;
	float _attackRange = 0.f;

	bool IsVaild() const { return nullptr != _skillData; }
	// 데이터 클리어
	void Clear();
};

/**
 * 실제 SkillRunner에서 실행 되기 이전 최종 Data를 가공하는 역활
 * 스킬 Vaild 체크 부터, 타겟선정, 대상 이동 거리 같은 최종정보를 이곳에서 가공한다.
 */
class T1PROJECT_API FGsSkillDataBuilder
{
private:
	// 예약 정보
	FGsSkillReserve _reserveData;
	// 최종 가공된 데이터 구조체
	FGsUseSkillRequest _requestData;
	// 캐스팅 스킬의 캐스팅 정보 구조체
	FGsCastingSkillRequest _requestCastingData;

	// 스킬 유효성 체크 클래스
	FGsSkillCheckArray _skillCheck;

	// 스킬 유효성 체크중 타겟 관련 처리를 위해 요청할 타겟 정보
	// @see : bool FGsSkillCheckTarget::ProcessCheck(const FGsSkill* NextSkill)
	class UGsGameObjectBase* _checkTarget = nullptr;

	class UGsGameObjectLocalPlayer* _owner = nullptr;

public:
	FGsSkillDataBuilder() = default;
	~FGsSkillDataBuilder() = default;

public:
	void Initialize(class UGsGameObjectLocalPlayer* Owner, class FGsSkillHandlerLocalPlayer* SkillHandler);
	void Finalize();

	void ClearReserveData();
	void ClearRequestData();
	// 캐스팅 정보는 서버응답을 받아서 Clear처리
	void ClearCastingData();

public:
	EGsSkillCheckResultType BuildValidateData(const FGsSkill* SkillData);
	class UGsGameObjectBase* BuildTargetData(const FGsSkill* SkillData);
	float BuildApproachRangeData(const FGsSkill* SkillData, bool IsMoveInput, bool IsSkillInput);

	bool UpdateRequestData(float Delta);

public:
	const FGsUseSkillRequest& GetRequestData() const { return _requestData; }
	const FGsSkillReserve& GetReserveData() const { return _reserveData; }
	const FGsCastingSkillRequest& GetCastingData() const { return _requestCastingData; }
	class UGsGameObjectBase* GetCheckTarget() const { return _checkTarget; }

	const FText& GetErrorMessage() const { return _skillCheck.GetErrMessage(); }

public:
	void SetRequestData(const FGsSkill* SkillData);
	void SetReserveData(const FGsSkill* SkillData, float AttackRange);
	void SetCastingData(int SkillId, bool Complete = false);

	void SetRequestDataRestoreTime(float ElapsedTime);
};
