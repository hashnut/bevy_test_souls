// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "../GsAbnormalityHandlerBase.h"
#include "Util/GsDequeue.h"
#include "DataSchema/GsSchemaEnums.h"

/* 
*	20220921
*	AbnormalityEffectSubType 을 사용하는 구조에서 EGsAbnormalityAnimProcessType을 이용하는 구조로 변경
*	차후 AbnormalityEffectSubType 삭제
*/

//struct FGsSchemaAbnormalityAnimationPriority;
//struct FGsSchemaAbnormalityIndexSet;
struct FGsSchemaAbnormalityCrowdControlSet;

static const FName LoopReactionAnimationEndName(TEXT("end"));

//FGsCrowdControlInfo와 달리 동시에 실행될 수 없다 
struct T1PROJECT_API FGsAbnormalityAnimReactionInfo
{
	EGsAbnormalityAnimProcessType _type;

	const struct FGsSchemaSkillReaction* _reactionData = nullptr;
	const struct FGsSchemaSkillReaction* _polyReactionData = nullptr;
	const struct FGsSchemaSkillReaction* _overrideReactionData = nullptr;

	bool _usePoseAnimation{ false };

	const FGsSchemaAbnormalityCrowdControlSet* _data = nullptr;

	TFunction<void()> _updateEvent;

	FGsAbnormalityAnimReactionInfo() {}
	FGsAbnormalityAnimReactionInfo(EGsAbnormalityAnimProcessType Type
		, const struct FGsSchemaSkillReaction* ReactionData
		, const struct FGsSchemaSkillReaction* polyReactionData, const TFunction<void()>& OnEvent)
		: _type(Type)
		, _reactionData(ReactionData)
		, _polyReactionData(polyReactionData)
		, _updateEvent(OnEvent)
	{
		_usePoseAnimation = (_type == EGsAbnormalityAnimProcessType::FRAMESTOP);
	}

	void SetCurrentData(const FGsSchemaAbnormalityCrowdControlSet* data)
	{
		_data = data;
	}

	bool IsValidData() const { return _data != nullptr; }
};

// 리액션 가변 시간 처리 구조
struct T1PROJECT_API FGsVariableLengthInfo
{
	FDateTime _timer;
	FName _endSectionName;
	float _jumpToEndTime = 0.f;
	float _animationPlayeRate = 1.f;

	float GetElapsed() const { return (FDateTime::Now() - _timer).GetTotalSeconds(); }
	bool IsJumpToSection() const
	{
		return _endSectionName != NAME_None && GetElapsed() >= _jumpToEndTime;
	}
};

struct T1PROJECT_API FGsAbnormalityReactInfo
{
	TWeakPtr<FGsAbnormalityAnimReactionInfo> _currentReaction;
	FGsVariableLengthInfo _variableLengthInfo;

	EGsAbnormalityAnimProcessType GetCurrentReactionProcessType() {
		if (TSharedPtr<FGsAbnormalityAnimReactionInfo> react = 
			_currentReaction.IsValid() ? _currentReaction.Pin() : nullptr)
		{
			return react->_type;
		}
		return EGsAbnormalityAnimProcessType::NONE;
	}

	float GetElapsed() const { return _variableLengthInfo.GetElapsed(); }
	bool IsJumpToSection() const
	{
		return _variableLengthInfo._endSectionName != NAME_None && 
			GetElapsed() >= _variableLengthInfo._jumpToEndTime;
	}

	void Clear()
	{	
		_currentReaction = nullptr;
	}

	void SetCurrentReaction(TWeakPtr<FGsAbnormalityAnimReactionInfo> act)
	{
		_currentReaction = act;
	}
};

/**
 * 리액션 처리 클래스
 * AbnormalityInstanceId 로 관리된다.
 */
class T1PROJECT_API FGsCrowdControlBase
{
protected:
	class UGsGameObjectBase* _owner = nullptr;

	TArray<TSharedPtr<FGsAbnormalityAnimReactionInfo>> _arrReactionInfo;
	
	//같은 reaction Type 들이 있을 수 있다.
	FGsDequeue<TWeakPtr<FGsAbnormalityAnimReactionInfo>> _reactDataQueue;
	FGsAbnormalityReactInfo _reactionInfo; // _reactionVariableLength 를 포함하여 우선권을 가지고 실행되는 reaction

	struct AnimationProc
	{
		EGsAbnormalityAnimProcessType _prev{ EGsAbnormalityAnimProcessType::NONE };
		EGsAbnormalityAnimProcessType _next{ EGsAbnormalityAnimProcessType::NONE };
		EGsAbnormalityProcessType _processType{ EGsAbnormalityProcessType::None };
	};
	static TArray<AnimationProc> _animationPriority; //공용 테이블 한번만 로드하면 된다 static

public:
	FGsCrowdControlBase();
	virtual ~FGsCrowdControlBase() = default;

public:
	virtual void Initialize(class UGsGameObjectBase* Owner);
	virtual void Finalize();

public:	

	FORCEINLINE bool IsValidCurrentReaction()
	{
		return _reactionInfo._currentReaction.IsValid() && _reactionInfo._currentReaction.Pin()->IsValidData();
	};
	virtual void ChangeReactionState(EGsAbnormalityAnimProcessType animationType, bool isForced = false);
	virtual void UpdateReactionControl(const TMap<AbnormalityInstanceId, TSharedPtr<FGsAbnormalityData>>& TotalData);
	
	// 지금은 클라이언트에서 판단하므로 Update에서 계산한다.
	// 추후 네트워크처리가 되면 삭제
	//bool Update(float DeltaTime);
	
	// 최종 CC기 갱신
protected:
	//애니메이션 타입 기준 업뎃 처리
	virtual void UpdateStun();
	virtual void UpdateFear();
	virtual void UpdateKnockDown();
	virtual void UpdateKnockBack();
	virtual void UpdateFrameStop();
	virtual void UpdatePolymorp();
	virtual void UpdateTaunt();

	virtual void EndCurrentReactionState(EGsAbnormalityAnimProcessType endReaction);

public:
	// 가변 길이 리액션 정보 설정
	// 이동량이 정해져 있지않는 타입 (제자리용)
	// 서버에서 내려주는 Abnormality Duration 기준
	void SetVariableLengthReaction(float VariableLength, const FName& SectionName);
	// ReactionData에 있는 totalRuntime 기준
	void SetVariableLengthReaction(const struct FGsSchemaSkillReaction* ReactionData, const FName& SectionName);
	// 클리어 처리는 상태 클래스에서 진행
	void ClearVariableLengthReaction();

	//현재 Action이 유효한 Abnormality의 data
	const FGsSchemaAbnormalityCrowdControlSet* GetPriorValidAbnormality();

public:
	class UAnimMontage* GetPlayAni() const;
	const FGsAbnormalityReactInfo& GetReactionInfo() const;
	const FGsVariableLengthInfo& GetReactionVariableLength() const { return _reactionInfo._variableLengthInfo; }
	const FGsSchemaSkillReaction* GetCurrentReactionData() const;

	TWeakPtr<FGsAbnormalityAnimReactionInfo> GetCurrentReaction() { return IsValidCurrentReaction() ?  _reactionInfo._currentReaction : nullptr; }

	bool IsAnimationContollType(EGsAbnormalityAnimProcessType type);

	// 현재 Animation Control되는 Abnormality 중에 유표한 타입이 있는가
	bool IsValidAnimReaction(EGsAbnormalityAnimProcessType type);

	// 현재 유효한 Animation Control Abnormality가 있는가
	bool IsAnyOfValidAnimReaction(); 

	// 로직으로 가변처리해야 되는 타입
	bool IsVariableLengthReactionType(EReactionType Type);

	// 현재는 내플레이어 외는 정보를 알수 없음
	// 내플레이어 이외 오브젝트도 필요하다면,
	// 서버에서 내려 받거나, 따로 구현이 필요
	virtual bool IsCreatureAction() const { return false; }
	virtual bool IsCreatureActionType(CreatureActionType actionType) const { return false; }
	bool IsPolymorpShape() const;
	bool IsPoseAnimation() const;
};
