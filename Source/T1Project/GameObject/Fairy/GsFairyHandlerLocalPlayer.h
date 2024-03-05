#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Fairy/GsFairyHandlerPlayer.h"
#include "ActorEx/GsActorFieldFairy.h"
#include "Fairy/GsFairyData.h"

class UGsGameObjectPlayer;
class AGsActorFieldFairy;

class T1PROJECT_API FGsFairyHandlerLocalPlayer : public FGsFairyHandlerPlayer
{
	using Super = FGsFairyHandlerPlayer;
public:
	virtual void UpdateEquipFairy(FairyId id, bool instant = true) override;
public:
	virtual void SetFairySkillEffect() override;
	virtual void SetFairyVehicleEffect(bool IsActive, bool useTalk = false) override;
	virtual void SetFairyTalkInfo() override;
};