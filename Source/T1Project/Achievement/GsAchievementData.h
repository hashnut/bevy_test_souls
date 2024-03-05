#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Achievement/GsSchemaAchievementDetail.h"
#include "DataSchema/Achievement/GsSchemaAchievementCommon.h"
#include "DataSchema/Achievement/GsSchemaAchievementCondition.h"
#include "Reward/GsRewardHelper.h"

struct T1PROJECT_API FGsAchievementData
{
	AchievementId _achievementId{ INVALID_ACHIEVEMENT_ID };
	//AchievementCategoryId _category{ 0 };
	AchievementGroupId _groupId{ 0 };

	uint64 _achievePoint{ 0 }; //ÇöÀç ·¹º§¿¡¼­ÀÇ È¹µæ °ª
	bool _isComplete{ false };
	bool _isRewarded{ false };
	
	enum class AchieveProcess { Complete = 0, GoOn, NoGoOn };
	AchieveProcess _goOn{ AchieveProcess::NoGoOn };

	uint64 _preAchievePoint{ 0 }; //ÀÌÀü ·¹º§¿¡¼­ÀÇ È¹µæ °ª
	bool _IsProgressUp{ false };
	FText _description;

	const FGsSchemaAchievementCommon* _data{ nullptr };
	const FGsSchemaAchievementDetail* _detail{ nullptr };
	const FGsSchemaRewardData* _reward{ nullptr };

	FGsAchievementData() = default;
	FGsAchievementData(const FGsSchemaAchievementCommon* data, uint32 achievePoint = 0, bool rewarded = false)
		: _achievementId(data->id)
		//, _category(data->category)
		, _groupId(data->groupId)
		, _achievePoint(achievePoint)
		, _isComplete(achievePoint > data->completePoint)
		, _isRewarded(rewarded)
		, _preAchievePoint(achievePoint)
		, _IsProgressUp(false)
		, _data(data)
		, _detail(data->detail.GetRow())
		, _reward(data->rewardId.GetRow())
	{
		_description = FText::Format(_detail->textDesc, data->completePoint);
		if (_isRewarded) SetProcess(AchieveProcess::Complete);
	}

	void SetProcess(AchieveProcess procType) { _goOn = procType; };
	void SetInfo(uint64 achievePoint, bool rewarded, uint64 preAchievePoint = 0, bool isProgressUp = true)
	{
		_preAchievePoint = preAchievePoint > 0 ? preAchievePoint : _achievePoint;
		_achievePoint = achievePoint;
		_isComplete	  = rewarded || _achievePoint >= CompletePoint();
		_isRewarded	  = rewarded;

		_IsProgressUp = isProgressUp;

		_description = FText::Format(_detail->textDesc, CompletePoint());
		if (_isRewarded) SetProcess(AchieveProcess::Complete);
	}

	uint8 GetLevel() { return _data ? _data->level : 0; }
	uint64 CurrentPoint() { return _achievePoint; }
	uint64 CompletePoint() { return _data ? _data->completePoint : 0;}
	uint64 PreAchievePoint() { return _preAchievePoint; }
	bool IsComplete() { return _isComplete; }
	bool IsRewarded() { return _isRewarded; }
	bool IsEnableRecvReward() { return _isComplete ? !_isRewarded : false; };
	float GetProgressRate() { return _isComplete || CompletePoint() <= 0 ? 1.f : _achievePoint / (float)(CompletePoint());  }
	float GetPreProgressRate() { return CompletePoint() <= 0 ? 1.f : _preAchievePoint / (float)(CompletePoint()); }
	bool IsMainAchievement() { return _data ? _data->isTheme : false; }

	int32 TabIndex() { return _detail != nullptr ? _detail->tabIndex : 0; }
	FText Title() { return _detail != nullptr ? _detail->textTitle : FText::GetEmpty(); }
	FText Description() { return _description; }
	FSoftObjectPath IconPath() { return _detail != nullptr ? _detail->icon : FSoftObjectPath(); }
	AchieveProcess AchievementProcessState() { return IsRewarded() ? AchieveProcess::Complete : _goOn; }

	const TArray<const FGsSchemaRewardItemBagData*> GetRewards()
	{
		TArray<const FGsSchemaRewardItemBagData*> arr;
		if (_reward && _reward->rewardBoxList.IsValidIndex(0))
		{
			FGsRewardHelper::UnboxingRewardBox(_reward->rewardBoxList[0].rewardBoxId, arr);
		}
		return arr;
	}

	const FGsSchemaAchievementCondition* GetConditionDBData() { return _data ? _data->achievementCondition.GetRow() : nullptr; }	
};

