// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTargetRule.h"
#include "AutoTargetMode/GsManualTargetMode.h"
#include "AutoTargetMode/GsAutoTargetMode.h"
#include "AutoTargetMode/GsSemiAutoTargetMode.h"
#include "UTIL/GsClassUtil.h"
#include "Option/GsServerOption.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"


void FGsTargetRule::Initialize(UGsGameObjectLocalPlayer* Local)
{
	//_local = Local;
	// 타겟룰 타입별로 생성
	_targetRuleMode[(int)EGsTargetRuleType::ManualTarget] = new FGsManualTargetMode();
	_targetRuleMode[(int)EGsTargetRuleType::SemiAutoTarget] = new FGsSemiAutoTargetMode();
	_targetRuleMode[(int)EGsTargetRuleType::AutoTarget] = new FGsAutoTargetMode();

	for (int i = 0; i < (int)EGsTargetRuleType::TargetRuleType_MAX; ++i)
	{
		_targetRuleMode[i]->Initialize(Local);
	}
}

void FGsTargetRule::Finalize()
{
	for (int i = 0; i < (int)EGsTargetRuleType::TargetRuleType_MAX; ++i)
	{
		if (_targetRuleMode[i])
		{
			delete _targetRuleMode[i];
		}
	}
}

void FGsTargetRule::ApplyTargetList()
{
	if (FGsServerOption* serverOption = GOption()->GetServerOption())
	{
		for (int i = 0; i < (int)EGsTargetRuleType::TargetRuleType_MAX; ++i)
		{
			_targetRuleMode[i]->ApplyTargetList();
		}
	}
}

void FGsTargetRule::OnAutoTargetAction(OUT TArray<UGsGameObjectBase*>& OutTargetList, EGsTargetRuleType TargetRuleType)
{
	ChangeMode(_targetRuleMode[(int)TargetRuleType]);

	if (_currentMode)
	{
		_currentMode->SearchTarget(OutTargetList);

		if (_ignoreTargetGuid != 0)
		{
			uint64 captureGuide = _ignoreTargetGuid;
			OutTargetList.RemoveAll([captureGuide](UGsGameObjectBase* el)
				{
					return captureGuide == el->GetGameId();
				});
			// 무시 설정 되었던 타겟은 정상화 되었을수도 있으니 초기화
			_ignoreTargetGuid = 0;
		}
	}
}

UGsGameObjectBase* FGsTargetRule::OnAutoTargetAction(EGsTargetRuleType TargetRuleType)
{
	TArray<UGsGameObjectBase*> outTargetList;
	OnAutoTargetAction(outTargetList, TargetRuleType);
	if (outTargetList.Num() > 0)
	{
		return outTargetList[0];
	}
	return nullptr;
}

void FGsTargetRule::ChangeMode(FGsTargetModeBase* Mode)
{
	if (_currentMode != Mode)
	{
		if (_currentMode)
		{
			_currentMode->Exit();
		}

		_currentMode = Mode;
		_currentMode->Enter();
	}
}

void FGsTargetRule::SetIgnoreTargetId(uint64 In_IgnoreObjectGuid)
{
	_ignoreTargetGuid = In_IgnoreObjectGuid;
}

FGsAutoTargetMode* FGsTargetRule::GetAutoMode() const
{
	return FGsClassUtil::FCheckStaticCast<FGsAutoTargetMode>(_targetRuleMode[
		(int)EGsTargetRuleType::AutoTarget]);
}

bool FGsTargetRule::IsAutoMode() const
{
	return nullptr != _currentMode && _currentMode == _targetRuleMode[(int)EGsTargetRuleType::AutoTarget];
}