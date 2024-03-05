// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsTargetHandlerBase.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageInput.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/GsMessageDelegateList.h"

#include "GameObject/Target/AutoTarget/GsTargetRule.h"

/**
 * Object 타겟 Local클래스
 * 타겟팅 Visualize기능을 포함한다.
 */
class T1PROJECT_API FGsTargetHandlerLocalPlayer : 
	public FGsTargetHandlerBase,
	public TGsMessageDelegateListTwoParam<MGameObject, MessageGameObject, 
										MInput, MessageInput>
{
	using Super = FGsTargetHandlerBase;

private:
	// 2차 타겟 정보
	class UGsGameObjectBase* _secondTarget = nullptr;

	// 이전 타겟 정보
	class UGsGameObjectBase* _prevTarget = nullptr;
	class UGsGameObjectBase* _prevSecondTarget = nullptr;
	
	// 최종 타겟 캐시 정보
	class UGsGameObjectBase* _cacheLastTarget = nullptr;
	// 타겟 UI 보여질 타겟 정보
	class UGsGameObjectBase* _widgetTarget = nullptr;


	struct TargetHUD*	_targetHudInfo = nullptr;
	float				_autoUserTargetClearTime = 5.f;
	float				_startTime = -1.f;

	// 나를 공격한 대상 정보
	// Pc와 Npc를 구분한다.
	// Ai 클래스에도 비슷한 정보가 있는것으로 보이므로 확인 필요
	int64 _attackerPc = 0;
	int64 _attackerNpc = 0;

	// 강제 스킬 사용 타겟 정보
	int64 _skillSelectTarget = 0;
	// 수동 1차 타겟 설정시 죽을때까지 공격할 대상 정보
	int64 _manualModeAutoTarget = 0;

	// 타겟 선정 처리 클래스
	class FGsTargetRule* _targetRule = nullptr;

	// 메세지 Param 캐싱
	FGsGameObjectMessageParamTarget _messageParam;

	// 타겟 변경 인덱스
	int32 _targetChangeIndex = INDEX_NONE;

public:
#if WITH_EDITOR
	// 치트용 오토 타겟 금지 플레그
	bool _checkAutoTargetFlag = false;
#endif

public:
	FGsTargetHandlerLocalPlayer()			= default;
	virtual ~FGsTargetHandlerLocalPlayer()	= default;

public:
	virtual void Initialize(class UGsGameObjectBase* owner);
	virtual void Finalize() override;

public:
	virtual void Update(float Delta) override;

public:
	// 타입에 따른 타겟 설정
	class UGsGameObjectBase* OnTarget(FGsTargetRule::EGsTargetRuleType TargetRuleType, bool isRotate = false,
		SelectTargetFilter selectFilter = SelectTargetFilter::None, IffApplyType IffType = IffApplyType::ENEMY );

	virtual bool SetTarget(class UGsGameObjectBase* target, bool isRotate = false,
		SelectTargetFilter selectFilter = SelectTargetFilter::None) override;

	virtual void ClearTarget(bool isAllClear = false) override;
	virtual void ClearTarget(UGsGameObjectBase* target) override;
	virtual void ClearSkillSelectTarget(int64 InGameId = 0) override;
	void ClearAttackerTarget(int64 InGameId = 0);	

	void SetAttacker(class UGsGameObjectBase* Target);
	void SetSkillSelectTarget(class UGsGameObjectBase* InTarget);

	int64 GetNpcAttacker() const { return _attackerNpc; }
	int64 GetPcAttacker() const { return _attackerPc; }	
	class FGsTargetRule* GetTargetRule() const { return _targetRule; }
	class UGsGameObjectBase* GetManualModeAutoTarget() const;
	class UGsGameObjectBase* GetSkillSelectTarget() const;

	// 관전 대상 정보 넣기
	void SetObserveTarget(class UGsGameObjectBase* InTarget);
	// observe target hud clear
	void ClearObserveTarget();

private:
	void OnCounterAttack(class UGsGameObjectBase* Target);

private:
	void ClearManualModeAutoTarget();

	void UpdateTargetRange();
	void CallbackLoacalHit(const struct IGsMessageParam* inParam);
	void CallbackCheckDespawnTarget(const struct IGsMessageParam* inParam);
	void CallbackTargetClear(const struct IGsMessageParam* inParam);
	void CallbackAIModeActive(const IGsMessageParam* Param);
	void CallbackTargetChangeInput(const IGsMessageParam* Param);
	void CallbackUpdateScanInfo(const IGsMessageParam* Param);
	
	void UpdateTargetUI(class UGsGameObjectBase* InTarget, SelectTargetFilter InTargetFilterType);
	void UpdateTargetEffect();
	void ShowTargetEffect(class UGsGameObjectBase* InTarget, bool InVisible, bool InIsFirst);
	void SetEnemyInfoHudUI(class UGsGameObjectBase* InTarget);

public:
	virtual UGsGameObjectBase* GetSecondTarget() const override;
	// 현재 타겟 설정 이펙트 포커스인 타겟 얻기
	class UGsGameObjectBase* GetLastTarget() const;
	class UGsGameObjectBase* GetWidgetTarget() const;

	bool SetPartySkillSelectTarget(int In_partySlotId);
	void SetSelfSkillSelectTarget();
	bool SetLastTargetSelectTarget();

private:
	float GetElapsed() const;

	int64 FindScanMemberGameId(int32 InOrderNumber);
};

// Target Visualize (HUD) 에 사용될 구조체
struct TargetHUD
{
public:
	TWeakObjectPtr<class UGsGameObjectCreature> _target;
	CreatureRankType _rankType = CreatureRankType::MAX;
	int32 _level= 0;
	FString _name= "";
	time_t _lifeTime = 0;

	TargetHUD() = default;

	void ResetData()
	{
		if (_target.IsValid())
		{
			_target.Reset();
		}
		_rankType = CreatureRankType::MAX;
		_level = 0;
		_name = "";
		_lifeTime = 0;
	}
};
