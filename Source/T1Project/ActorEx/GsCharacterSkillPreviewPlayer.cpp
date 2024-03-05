// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorEx/GsCharacterSkillPreviewPlayer.h"
#include "Components/WidgetComponent.h"
#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "ActorComponentEx/GsSkillCollisioHandlerComponent.h"
#include "ActorComponentEx/GsSkillGuideEffectComponent.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"
#include "GameObject/Define/GsGameObjectText.h"
#include "GsCharacterBase.h"

AGsCharacterSkillPreviewPlayer::AGsCharacterSkillPreviewPlayer() : Super()
{
	_skillCollision = CreateDefaultSubobject<UGsSkillCollisionComponent>(SkillCollisionComponentName);
	_skillCollision->SetupAttachment(RootComponent);

	_skillCollisionHandle = CreateDefaultSubobject<UGsSkillCollisioHandlerComponent>(SkillCollisioHandlerComponentName);
	_skillCollisionHandle->SetupAttachment(RootComponent);

	_skillGuideEffect = CreateDefaultSubobject<UGsSkillGuideEffectComponent>(SkillGuideEffectComponentName);
	_skillGuideEffect->SetupAttachment(RootComponent);

	_nameplateWidget->SetVisibility(false);
	_nameplateWidget->SetHiddenInGame(true);
}
