// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsCharacterPlayer.h"
#include "GsCharacterSequencePlayer.generated.h"

class UGsCustomizeComponent;
class UGsGameObjectLocalPlayer;

//
// 시퀀스 플레이에 내케릭터를 조작하기 위한 인터페이스 함수
// 호출은 시퀀스내에 이벤트로 바인딩되어 있음
//

UCLASS()
class T1PROJECT_API AGsCharacterSequencePlayer : public AGsCharacterPlayer
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	UGsCustomizeComponent* _customizeComponent;

public:
	// 시네마틱용
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ApplyCurrentLocalPlayer(UGsGameObjectLocalPlayer* InGameObjectLocalPlayer);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	void ApplyCurrentLocalPlayerNoWeapon(UGsGameObjectLocalPlayer* InGameObjectLocalPlayer);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	UStaticMeshComponent* GetWeaponStaticMesh();

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	UStaticMeshComponent* GetWeaponSubStaticMesh();

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	bool IsMale(UGsGameObjectLocalPlayer* InGameObjectLocalPlayer);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "CharacterSequencePlayer")
	bool IsFemale(UGsGameObjectLocalPlayer* InGameObjectLocalPlayer);
	
public:
	// Sets default values for this character's properties
	AGsCharacterSequencePlayer();
};
