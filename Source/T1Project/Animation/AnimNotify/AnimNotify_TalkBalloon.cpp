// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_TalkBalloon.h"

#include "ActorEx/GsCharacterNonPlayer.h"

#include "UI/UIContent/Billboard/GsUIBillboardTalkBalloon.h"


void UAnimNotify_TalkBalloon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AGsCharacterNonPlayer* actor = Cast<AGsCharacterNonPlayer>(MeshComp->GetOwner());
	if (nullptr == actor)
		return;

	if (_talkBalloonData.IsNull())
		return;

	if (UGsUIBillboardTalkBalloon* talkBalloon = actor->GetTalkBalloon())
	{
		talkBalloon->ShowNotifiedTalkBallon(_talkBalloonData.GetRow());
	}
}
