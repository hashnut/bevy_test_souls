// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_PlayerTeleportEffect.h"

//#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
//#include "T1Project/ActorEx/GsCharacterLocalPlayer.h"
//#include "GameObject/Define/GsGameObjectDefine.h"

void UAnimNotify_PlayerTeleportEffect::Notify(USkeletalMeshComponent* In_meshComp, UAnimSequenceBase* Animation)
{
	// 노티파이로는 파티클 이펙트가 현재 플레이어에게만 보이게 되어 워프 시작/끝 타이밍에 코드로 파티클 재생
	//if (AGsCharacterLocalPlayer* characterLocal = Cast<AGsCharacterLocalPlayer>(In_meshComp->GetOwner()))
	//{
	//	if (true == characterLocal->IsHidden())
	//		return;

	//	UGsGameObjectLocalPlayer* local = characterLocal->GetGameObjectPlayer();
	//	if (nullptr == local)
	//		return;

	//	if (EPlayerTeleportEffect::None == _playerTeleportEffect)
	//	{
	//		return;
	//	}
	//	else
	//	{
	//		local->OnLocalTeleportEffect(
	//			(uint8)_playerTeleportEffect,
	//			SocketName,
	//			Scale,
	//			LocationOffset,
	//			RotationOffset,
	//			In_meshComp
	//		);
	//	}
	//}
}