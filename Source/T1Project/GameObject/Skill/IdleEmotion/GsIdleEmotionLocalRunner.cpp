#include "GsIdleEmotionLocalRunner.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"

#include "DataSchema/GameObject/Common/GsSchemaIdleEmotionActionData.h"

FGsIdleEmotionLocalRunner::FGsIdleEmotionLocalRunner(UGsGameObjectBase* owner)
	: FGsIdleEmotionBaseRunner(owner)
{
	
}

void FGsIdleEmotionLocalRunner::InitIdleEmotion(const FGsSchemaIdleEmotionActionData* ptrData)
{
	if (ptrData)
	{
		Super::InitIdleEmotion(ptrData);

		LoadIdleEmotionData(ptrData);
	}
}

void FGsIdleEmotionLocalRunner::FinalizeIdleEmotion()
{
	Super::FinalizeIdleEmotion();

	_useEmotionActList.Reset();
}

void FGsIdleEmotionLocalRunner::Update(float Delta) //motion check
{	
	Super::Update(Delta);

	if (IsOnTimeAutoIdleEmotion())
	{
		ResetAutoIdleEmotionTime();
		ChangeNextEmotion();
	}
}

void FGsIdleEmotionLocalRunner::SetEnableAutoIdleEmotionMotion(bool enableIdleMotion)
{
	if (_isLockEmotion) return;
	if (_isAutoIdleEmotionActive == enableIdleMotion) return;

	_isAutoIdleEmotionActive = enableIdleMotion;

	GMessage()-> GetUI().SendMessage(MessageUI::SOCIALMOTION_ENABLE_UPDATE, nullptr);

	ResetAutoIdleEmotionTime(enableIdleMotion);
}

bool FGsIdleEmotionLocalRunner::IsOnTimeAutoIdleEmotion()
{	
	if (_isAutoIdleEmotionActive && _autoEmotionStartTime > 0.f)
	{
		auto motionFinishedTime = _curMotion ? _curMotion->totalRuntime : 1.f;
		if ((FApp::GetCurrentTime() - _autoEmotionStartTime) >= (_autoEmotionDelayTime + motionFinishedTime))
		{
			if (_owner == nullptr || _owner->IsPeacefulIdle() == false)
			{	
				ResetAutoIdleEmotionTime();
				GMessage()->GetUI().SendMessage(MessageUI::SOCIALMOTION_ENABLE_UPDATE, nullptr);
				return false;
			}
			if (_instanceMotion._action == _curMotion && _instanceMotion._isLoop) 
				return false;
			return true;
		}
	}
	return false;
}

bool FGsIdleEmotionLocalRunner::IsEmotionMotionFinished()
{
	if (_isAutoIdleEmotionActive && _autoEmotionStartTime > 0.f)
	{
		if (_instanceMotion._action == _curMotion && _instanceMotion._isLoop) return false;

		return (FApp::GetCurrentTime() - _autoEmotionStartTime) >= (_curMotion ? _curMotion->totalRuntime : 1.f);
	}
	return true;
}

void FGsIdleEmotionLocalRunner::ResetAutoIdleEmotionTime(bool restart)
{
	static auto MinTimeSocial = GData()->GetGlobalData()->MinTimeAutoEmotionReact;
	static auto MaxTimeSocial = GData()->GetGlobalData()->MaxTimeAutoEmotionReact;
	static auto OffsetTimeStartSocial = GData()->GetGlobalData()->OffsetTimeStartAutoEmotionReact;

	_autoEmotionStartTime = 0;
	_autoEmotionDelayTime = 0;
	_curMotion = nullptr;

	if (_isAutoIdleEmotionActive)
	{
		_autoEmotionStartTime = FApp::GetCurrentTime();
		_autoEmotionDelayTime = FMath::RandRange(MinTimeSocial, MaxTimeSocial) + (restart ? OffsetTimeStartSocial : 0.f);
	}
}

void FGsIdleEmotionLocalRunner::PlayEmotionMotion(UserSocialEmotion emotionType, bool isLoop /*= false*/)
{
	if (auto iter = _emotionMapData.Find(emotionType))
	{
		int motionIndex = FMath::Rand() % iter->Num();

		_instanceMotion._randRate = 1.f;
		_instanceMotion._isLoop = isLoop;
		_instanceMotion._action = (*iter)[motionIndex];

		PlayInstantIdleEmotion(_instanceMotion);
	}
}

void FGsIdleEmotionLocalRunner::PlayInstantIdleEmotion(IN const FGsIdleMotionInfo& info) 
{
	if (auto skillHandler = _owner->GetSkillHandler())
	{
		ResetAutoIdleEmotionTime();

		_curMotion = info._action;
		skillHandler->StartEmotionMotion(_curMotion);
	}
}

const FGsSchemaSkillCommon* FGsIdleEmotionLocalRunner::GetNextAutoIdleEmotion() const
{
	if (_isAutoIdleEmotionActive && _useEmotionActList.Num() > 0)
	{
		int curIndex = 0;
		int nxtIndex = 0;
		auto list = _useEmotionActList;

		if (_curMotion)
		{
			for (TArray<FGsIdleMotionInfo>::RangedForIteratorType Iter = list.begin(); Iter != list.end(); ++Iter, ++curIndex)
			{
				if ((*Iter)._action == _curMotion)
				{
					break;
				}
			}
		}

		if (_isSeqenceRepeat)
		{
			if (list.IsValidIndex(curIndex + 1)) nxtIndex = curIndex + 1;
		}
		else
		{
			if (_curEmotionExclued && _curMotion != nullptr)
			{
				list.RemoveAt(curIndex);
			}

			if (_randomRateApply)
			{
				float curRandRate = FMath::FRand();
				list.RemoveAll([curRandRate](auto& e) { return e._randRate < curRandRate; });
			}

			nxtIndex = FMath::Rand() % list.Num();
		}

		return list[nxtIndex]._action;
	}
	return nullptr;
}

void FGsIdleEmotionLocalRunner::LoadIdleEmotionData(const FGsSchemaIdleEmotionActionData* ptrData)
{
	_isSeqenceRepeat = false;

	_useEmotionActList.Reset();
	if(ptrData)
	{ 
		_isSeqenceRepeat = ptrData->useSequence;
		for (const auto& e : ptrData->motionList)
		{	
			if (e.useIdleMotion)
			{
				const auto skillCommon = e.motion.GetRow();
				_useEmotionActList.Emplace(FGsIdleMotionInfo{ e.randRate, false, skillCommon });
			}
		}
	}
}

void FGsIdleEmotionLocalRunner::ChangeNextEmotion()
{
	if (_instanceMotion._action == _curMotion)
		_instanceMotion._action = nullptr;

	_curMotion = GetNextAutoIdleEmotion();
	if(_curMotion)
	{
		if (auto skillHandler = _owner->GetSkillHandler())
		{
			skillHandler->StartEmotionMotion(_curMotion);
		}
	}
}
