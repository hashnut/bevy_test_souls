#pragma once

#include "GameObject/Skill/GsSkillHandlerNonPlayer.h"

/*
* 스킬 핸들러: 플레이어블 이벤트 몬스터 용
*/
class FGsSkillHandlerPlayableEventNonPlayer : public FGsSkillHandlerNonPlayer
{
	using Super = FGsSkillHandlerNonPlayer;

private:
	// 스킬 end 콜백
	TFunction<void(bool)> _callbackSkillEnd;

public:
	FGsSkillHandlerPlayableEventNonPlayer() = default;
	virtual ~FGsSkillHandlerPlayableEventNonPlayer() = default;

public:
	virtual void LoadSocial() override {}
protected:
	virtual void CallbackEndRunner(int inSkillID, bool inIsEnd) override;
	virtual void CreateSkillRunner() override;
	virtual void CreateIdleEmotionRunner() override {}
public:
	void SetAniIdle();
	// net skill 데이터 채워 넣기
	void SetNetSkillData(int In_skillId);
	// set
public:
	void SetCallbackSkillEnd(TFunction<void(bool)> In_callback)
	{
		_callbackSkillEnd = In_callback;
	}
};