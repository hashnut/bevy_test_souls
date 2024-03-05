#pragma once
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsIdleEmotionBaseRunner.h"

/*
 * https://jira.com2us.com/wiki/pages/viewpage.action?pageId=277868991
 * 
 * Speed 0의 Idle Time 마다 정해진 Social Motion을 보여준다 - Client에서만
 * Timer는 유지하고, Motion은 FSM State를 Enter, Exit 하도록 한다.
 */
class T1PROJECT_API FGsIdleEmotionLocalRunner : public FGsIdleEmotionBaseRunner
{
private:
	using Super = FGsIdleEmotionBaseRunner;

public:
	// 현재는 skillHandler의 Runner로 구성한다. 나중에 다른 용도로 전환시에는 skillHandler를 받아 사용할 수 있도록 한다.
	FGsIdleEmotionLocalRunner(UGsGameObjectBase* owner);
	virtual ~FGsIdleEmotionLocalRunner() {}

public:
	virtual void InitIdleEmotion(const FGsSchemaIdleEmotionActionData* ptrData) override;
	virtual void FinalizeIdleEmotion() override;

	virtual void Update(float Delta) override;
	virtual void PlayEmotionMotion(UserSocialEmotion emotionType, bool isLoop = false) override;
	virtual bool IsEmotionMotionFinished() override;		//동작이 끝났을 시간인가

public:
	//motion 중지, 시작 - 중지 후 시작은 항상 time reset
	void SetEnableAutoIdleEmotionMotion(bool enableIdlemotion); 
	bool IsEnableEmotionMotion() { return _isAutoIdleEmotionActive; }
	
	bool IsOnTimeAutoIdleEmotion(); //idle Motion 시작할 시간
	void ResetAutoIdleEmotionTime(bool restart = false);

	void SetExcludeCurrentEmotion(bool set) { _curEmotionExclued = set; }
	void SetRandomRateApply(bool set) { _randomRateApply = set; }

	void PlayInstantIdleEmotion(IN const FGsIdleMotionInfo& info);
	const FGsSchemaSkillCommon* GetNextAutoIdleEmotion() const;

protected:
	void LoadIdleEmotionData(const FGsSchemaIdleEmotionActionData* ptrData);
	void ChangeNextEmotion();

protected:
	bool _isAutoIdleEmotionActive{ false };
	bool _isSeqenceRepeat{ false };
	bool _curEmotionExclued{ false };
	bool _randomRateApply{ false };

private:
	float _autoEmotionStartTime{ 0.f };
	float _autoEmotionDelayTime{ 0.f };

	TArray<FGsIdleMotionInfo> _useEmotionActList;
};

