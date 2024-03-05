// Fill out your copyright notice in the Description page of Project Settings.


#include "GsComboSkillGroup.h"
#include "../Management/ScopeGame/GsGameObjectManager.h"
#include "GsSkill.h"
#include "../Data/GsDataContainManager.h"
#include "../GameObject/ObjectClass/GsGameObjectBase.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Core/Public/Algo/Sort.h"

FGsComboSkillGroup::~FGsComboSkillGroup()
{
	if (_resetTimerHandle.IsValid() && GSGameObject())
	{
		if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			local->GetWorld()->GetTimerManager().ClearTimer(_resetTimerHandle);
		}
	}
	_callbackChainSkillEnd = nullptr;
}

void FGsComboSkillGroup::Init(TFunction<void(FGsComboSkillGroup*)> CallbackSkillEnd)
{
	_currentIndex = 0;
	_callbackChainSkillEnd = CallbackSkillEnd;
}

void FGsComboSkillGroup::Reset()
{
	// 처음으로 되돌아감
	_currentIndex = 0;
	if (_resetTimerHandle.IsValid())
	{
		if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			local->GetWorld()->GetTimerManager().ClearTimer(_resetTimerHandle);
		}
	}

	if (_callbackChainSkillEnd)
	{
		_callbackChainSkillEnd(this);
	}
}

void FGsComboSkillGroup::Add(const FGsSkill* SkillData)
{
	if (false == _listSkillData.Contains(SkillData))
	{
		_listSkillData.Emplace(SkillData);
		Algo::Sort(_listSkillData, [](const FGsSkill* el1, const FGsSkill* el2)
			{
				return el1->_tableData->sequenceInfo.comboInfo.currentStage < 
					el2->_tableData->sequenceInfo.comboInfo.currentStage;
			});
	}
}

// 다음 스텝으로 치환
const FGsSkill* FGsComboSkillGroup::Next(bool IgnoreRestoreTime)
{
	// 다음 스킬이 있는지 확인한다.
	if (_currentIndex < _listSkillData.Num())
	{
		const FGsSchemaSkillChain& chainInfo = _listSkillData[_currentIndex]->_tableData->sequenceInfo;		
		if (chainInfo.comboInfo.currentStage != chainInfo.maxStage)
		{
			++_currentIndex;
			if (_currentIndex < _listSkillData.Num() && _listSkillData[_currentIndex]->_isLearn)
			{
				const FGsSkill* nextSkill = _listSkillData[_currentIndex];
				if (false == IgnoreRestoreTime)
				{
					StartTimer(chainInfo.comboInfo.resetTime);
				}
				return nextSkill;
			}
			else
			{
				_currentIndex = 0;
			}
		}
	}

	return nullptr;
}

void FGsComboSkillGroup::StartTimer(float ResetTime)
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		local->GetWorld()->GetTimerManager().SetTimer(_resetTimerHandle, FTimerDelegate::CreateLambda([this]() {
			Reset();
			}), ResetTime, false);
	}
}

const FGsSkill* FGsComboSkillGroup::GetCurrentData() const
{
	if (_currentIndex < _listSkillData.Num())
	{
		return _listSkillData[_currentIndex];
	}
	return nullptr;
}

const FGsSkill* FGsComboSkillGroup::GetFirstSkill() const
{
	if (_listSkillData.Num() > 0)
	{
		return _listSkillData[0];
	}
	return nullptr;
}

TArray<const FGsSkill*> FGsComboSkillGroup::GetAllData() const
{
	return _listSkillData;
}

bool FGsComboSkillGroup::Contain(const FGsSkill* SkillData) const
{
	return Contain(SkillData->_tableData->id);
}

bool FGsComboSkillGroup::Contain(int32 SkillId) const
{
	for (int i = 0; i < _listSkillData.Num(); ++i)
	{
		if (_listSkillData[i]->_tableData->id == SkillId)
		{
			return true;
		}
	}
	return false;
}

bool FGsComboSkillGroup::IsFirst() const
{
	return _currentIndex == 0;
}

bool FGsComboSkillGroup::IsRemainSkill() const
{
	if (const FGsSkill* currentSkill = GetCurrentData())
	{
		return currentSkill->_tableData->skillChainCondition == SkillChainCondition::PREV_SKILL_REMAIN;
	}
	return false;
}