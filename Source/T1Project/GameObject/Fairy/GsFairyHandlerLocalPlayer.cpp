
#include "GsFairyHandlerLocalPlayer.h"
#include "ActorEx/GsActorFieldFairy.h"
#include "ActorComponentEx/GsWidgetComponentFairyTalk.h"


/*Fairy는 Data Object가 없는 Actor 이므로 Data는 handler가 가지고 처리하게 한다.*/
void FGsFairyHandlerLocalPlayer::UpdateEquipFairy(FairyId id, bool instant)
{
	Super::UpdateEquipFairy(id, instant);
}

void FGsFairyHandlerLocalPlayer::SetFairySkillEffect()
{
	Super::SetFairySkillEffect();

	if (_fairy.IsValid() && _fairyData.IsValid())
	{
		if (auto talkWidget = Cast<UGsWidgetComponentFairyTalk>(_fairy->GetFairyTalkWidget()))
		{
			talkWidget->SetSkillTalk();
		}
	}
}

void FGsFairyHandlerLocalPlayer::SetFairyVehicleEffect(bool IsActive, bool useTalk)
{
	Super::SetFairyVehicleEffect(IsActive, useTalk);

	if (_fairy.IsValid() && _fairyData.IsValid() && useTalk)
	{
		if (auto talkWidget = Cast<UGsWidgetComponentFairyTalk>(_fairy->GetFairyTalkWidget()))
		{
			talkWidget->SetData(_fairyData);
			talkWidget->SetVehicleTalk(IsActive ? EGsFairyTalkType::SpeedOn : EGsFairyTalkType::SpeedOff);
		}
	}
}

void FGsFairyHandlerLocalPlayer::SetFairyTalkInfo()
{
	if (_fairy.IsValid() && _fairyData.IsValid())
	{
		if (auto talkWidget = Cast<UGsWidgetComponentFairyTalk>(_fairy->GetFairyTalkWidget()))
		{
			talkWidget->SetData(_fairyData);
		}
	}
}
