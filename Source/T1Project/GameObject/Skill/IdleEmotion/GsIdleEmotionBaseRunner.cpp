#include "GsIdleEmotionBaseRunner.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"

#include "DataSchema/GameObject/Common/GsSchemaIdleEmotionActionData.h"

FGsIdleEmotionBaseRunner::FGsIdleEmotionBaseRunner(UGsGameObjectBase* owner)
	: _owner(owner)
{
	
}

FGsIdleEmotionBaseRunner::~FGsIdleEmotionBaseRunner()
{
	
}

void FGsIdleEmotionBaseRunner::InitIdleEmotion(const FGsSchemaIdleEmotionActionData* ptrData)
{
	if (ptrData)
	{
		LoadEmotionActionData(ptrData);
	}
}

void FGsIdleEmotionBaseRunner::FinalizeIdleEmotion()
{
	_emotionMapData.Reset();
}

void FGsIdleEmotionBaseRunner::PlayEmotionMotion(UserSocialEmotion emotionType, bool isLoop /*= false*/)
{
	if (auto iter = _emotionMapData.Find(emotionType))
	{
		int motionIndex = FMath::Rand() % iter->Num();

		_instanceMotion._randRate = 1.f;
		_instanceMotion._isLoop = isLoop;
		_instanceMotion._action = (*iter)[motionIndex];

		if (auto skillHandler = _owner->GetSkillHandler())
		{
			_curMotion = _instanceMotion._action;
			_motionStartTime = FApp::GetCurrentTime();
			skillHandler->StartEmotionMotion(_curMotion);
		}
	}
}

bool FGsIdleEmotionBaseRunner::IsEmotionMotionFinished()
{
	if (_instanceMotion._action == _curMotion && _instanceMotion._isLoop) return false;

	auto motionFinishedTime = _curMotion ? _curMotion->totalRuntime : 1.f;
	return (FApp::GetCurrentTime() - _motionStartTime) >= motionFinishedTime;
}

void FGsIdleEmotionBaseRunner::LoadEmotionActionData(const FGsSchemaIdleEmotionActionData* ptrData)
{
	if (ptrData)
	{
		for (const auto& e : ptrData->motionList)
		{
			const auto skillCommon = e.motion.GetRow();
			if (!_emotionMapData.Find(e.emotion))
			{
				_emotionMapData.Add(e.emotion, TArray<const FGsSchemaSkillCommon*>());
			}
			_emotionMapData[e.emotion].Add(skillCommon);
		}
	}
}

