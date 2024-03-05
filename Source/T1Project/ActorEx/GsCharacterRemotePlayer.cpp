// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCharacterRemotePlayer.h"
#include "ActorComponentEx/GsUROComponent.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"

AGsCharacterRemotePlayer::AGsCharacterRemotePlayer() : Super()
{
	AutoPossessPlayer = EAutoReceiveInput::Type::Disabled;
	AutoPossessAI = EAutoPossessAI::Spawned;

	_uroComponent = CreateDefaultSubobject<UGsUROComponent>(UROName);

}

void AGsCharacterRemotePlayer::BeginPlay()
{
	Super::BeginPlay();

	InitSkeletalMeshURO();
}

// 인터랙션 클릭
void AGsCharacterRemotePlayer::ClickInteraction()
{
	if (_callbackInteractionClick != nullptr)
	{
		_callbackInteractionClick(false);
	}
}

// 인터랙션 클릭 콜백 세팅
void AGsCharacterRemotePlayer::SetInteractionClickCallback(TFunction<void(bool)> In_callback)
{
	_callbackInteractionClick = In_callback;

}

