#pragma once
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/GsSchemaEnums.h"

struct FGsSchemaSkillCommon;
struct FGsSchemaIdleEmotionActionData;
class UGsGameObjectBase;

struct T1PROJECT_API FGsIdleMotionInfo
{
	float _randRate{ 1.f };
	bool _isLoop{ false };
	const FGsSchemaSkillCommon* _action;
};

class T1PROJECT_API FGsIdleEmotionBaseRunner : public TSharedFromThis<FGsIdleEmotionBaseRunner>
{
public:
	FGsIdleEmotionBaseRunner(UGsGameObjectBase* owner);
	virtual ~FGsIdleEmotionBaseRunner();

public:
	virtual void InitIdleEmotion(const FGsSchemaIdleEmotionActionData* ptrData);
	virtual void FinalizeIdleEmotion();

	virtual void Update(float Delta) {};
	virtual void PlayEmotionMotion(UserSocialEmotion emotionType, bool isLoop = false);

	virtual bool IsEmotionMotionFinished();		//동작이 끝났을 시간인가

	void SetEmotionLock(bool set) { _isLockEmotion = set; }

protected:
	void LoadEmotionActionData(const FGsSchemaIdleEmotionActionData* ptrData);

protected:
	bool _isLockEmotion{ false };
	FGsIdleMotionInfo _instanceMotion;

	const FGsSchemaSkillCommon* _curMotion{ nullptr };
	float _motionStartTime{ 0.f };

	TWeakObjectPtr<UGsGameObjectBase> _owner;
	TMap<UserSocialEmotion, TArray<const FGsSchemaSkillCommon*>> _emotionMapData; //나중에 emotion 종류 지정사용시
};

