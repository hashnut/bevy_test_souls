// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Skill/Notify/GsSkillNotifyNodeBase.h"
#include "GameObject/Skill/Data/GsSkillDataBase.h"
#include "Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "UTIL/GsClassUtil.h"

// 한스킬에 해당하는 SkillNotify 클래스 캐시 정보
struct T1PROJECT_API FGsSkillNotifyCache
{
public:
	TArray<TSharedPtr<FGsSkillNotifyNodeBase>> _listNotify;

public:
	void Clear()							 
	{ 
		for (TSharedPtr<FGsSkillNotifyNodeBase>& el : _listNotify)
		{
			el.Get()->Finalize();
		}

		_listNotify.Reset();
	}

	bool IsValid() { return _listNotify.Num() > 0; }

	// !!!!호출과 동시에 제거
	TSharedPtr<FGsSkillNotifyNodeBase> Pop()
	{ 
		if (_listNotify.Num() > 0)
		{
			return _listNotify.Pop();
		}
		return nullptr;
	}

	// 경과 시간에 따른 Notify 정보 꺼내오기
	void Pop(TArray<TSharedPtr<FGsSkillNotifyNodeBase>>& OutList, float ElapsedTime)
	{
		for (int i = 0; i < _listNotify.Num(); ++i)
		{
			if (_listNotify[i].Get()->GetOffset() <= ElapsedTime)
			{
				OutList.Emplace(_listNotify[i]);
				_listNotify[i]->SetPendingSkill(true);

			}
			else
			{
				break;
			}
		}

		_listNotify.RemoveAll([](TSharedPtr<FGsSkillNotifyNodeBase> el)
			{
				return el->IsPendingKill();
			});
	}
};

/**
 * 실행중인 스킬 실행기 클래스
 * 내 플레이어를 제외하고는 클라이언트 자체적으로 실행해야 될 상황이 없을것으로 판단됨
 * 기본적인 실행기(FGsSkillRunnerBase)는 Server에서 받은 Result 정보를 가지고 실행 처리를 진행 한다.
 * @see : virtual void UseSkillNode(int SkillNotifyId)
 * 
 * 실제 데이터가 설정되는 타이밍과  Update가 시작되는 타이밍이 다르므로
 * IsValid() 체크가 필요한 경우가 많이 있을수 있다.
 * @see : IsValid()
 */
class T1PROJECT_API FGsSkillRunnerBase
{
protected:
	FGsSkillRunnerSKillData		_skillData;
	FGsSkillRunnerCommonData	_commonData;
	// 여기의 접근은 인터페이스 함수호출로만 접근을 유도한다. (캐스팅 처리는 타입검사를 통해서)
	const IGsSkillRunnerData* _currData = nullptr;

protected:
	class UGsGameObjectBase*		_owner	= nullptr;	
	const struct FGsSkillNetData*	_skillNetData = nullptr;

	// 실제 Attack Speed 배율(애니메이션 재생 속도)
	float						_speedScale = 1.f;
	// _scale을 기준으로 notify Time 에 적용되는 비율값
	float						_scaledOffsetRate = 1.f;
	float						_totalRuntime = 0.f;
	float						_cancelTime = 0.f;
	float						_elapsedTime = 0.f;
	float						_offsetTime = 0.f;
	FDateTime					_timer;
	TFunction<void(int, bool)>	_callbackEndRun;

protected:
	// 현재 활성화 중인 SKillNotify정보
	TSharedPtr<FGsSkillNotifyCache> _currentInfo;

public:
	FGsSkillRunnerBase(TFunction<void(int, bool)> EndRun);
	virtual ~FGsSkillRunnerBase();

public:
	// 서버에서 판단된 결과 정보를 이함수를 통해 호출한다.
	virtual void UseSkillNode(int SkillNotifyId);

public:
	// Common Skill
	void StartRunner(class UGsGameObjectBase* Owner, const struct FGsSchemaSkillCommon* Data);

	// Reation
	void StartRunner(class UGsGameObjectBase* Owner, const struct FGsSchemaSkillReaction* Data);

	// Skill
	void StartRunner(class UGsGameObjectBase* Owner, const struct FGsSchemaSkillSet* Data, float SpeedScale, 
		const struct FGsSkillNetData* NetData = nullptr);

	// 타임 라인 Jump구간 설정
	void JumpToRunner(class UGsGameObjectBase* Owner, float JumpOffset);

	virtual void CancelRunner();
	virtual void EndRunner();
	virtual bool Update(float DeltaTime);

protected:
	virtual void StartRunner(class UGsGameObjectBase* Owner);
	virtual void ClearRunner();

public:
	virtual TSharedPtr<FGsSkillNotifyNodeBase> CreateSkillNode(const struct FGsSchemaSkillNotifySet* Data);

protected:
	// SkillNode 데이터 생성이후 작업
	virtual void CreateRuntimeSkillNode() {}

protected:
	virtual void SetNotifyNode(const TArray<FGsSchemaSkillNotify>& listNotify);

public:
	virtual bool CanMove() const;
	bool IsValid() const;
	bool IsEnd();
	bool CanCancel();

public:
	const struct FGsSchemaSkillSet* GetSkillData() const;
	const struct FGsSkillNetData* GetNetSkillData() const;
	virtual float GetSpeedScale() const;
	float GetElapsedTime() const;
	float GetTotalRunTime() const;
	float GetCancelTime() const;
	class UAnimMontage* GetAni() const;
};
