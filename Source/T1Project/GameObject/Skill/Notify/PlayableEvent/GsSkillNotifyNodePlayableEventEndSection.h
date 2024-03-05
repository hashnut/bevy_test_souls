#pragma once
#include "GameObject/Skill/Notify/SkillPreview/GsSkillNotifyNodePreviewEndSection.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"

class UGsGameObjectBase;

class FGsSkillNotifyNodePlayableEventEndSection : public FGsSkillNotifyNodePreviewEndSection
{
	using Super = FGsSkillNotifyNodePreviewEndSection;

public:	
	FGsSkillNotifyNodePlayableEventEndSection(IffApplyType SkillIffType);
	virtual ~FGsSkillNotifyNodePlayableEventEndSection() = default;


public:
	virtual void Action(UGsGameObjectBase* Owner) override;
};