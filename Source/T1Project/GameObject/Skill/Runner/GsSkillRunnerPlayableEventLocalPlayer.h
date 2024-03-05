#pragma once
#include "GameObject/Skill/Runner/GsSkillRunnerSkillPreviewPlayer.h"

class FGsSkillNotifyNodeBase;
struct FGsSchemaSkillNotifySet;

class FGsSkillRunnerPlayableEventLocalPlayer : public FGsSkillRunnerSkillPreviewPlayer
{
	using Super = FGsSkillRunnerSkillPreviewPlayer;

public:
	FGsSkillRunnerPlayableEventLocalPlayer(TFunction<void(int, bool)> EndRun);
	virtual ~FGsSkillRunnerPlayableEventLocalPlayer() = default;

protected:
	virtual void StartRunner(class UGsGameObjectBase* Owner) override;
	virtual TSharedPtr<FGsSkillNotifyNodeBase> CreateSkillNode(const FGsSchemaSkillNotifySet* Data) override;

public:
	virtual float GetSpeedScale() const override;
};