// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCharacterSequencePlayer.h"

#include "ActorComponentEx/GsCustomizeComponent.h"
#include "ActorEx/GsWeaponActor.h"

#include "GameMode/GsGameModeBase.h"

#include "Gameobject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"


// Sets default values
AGsCharacterSequencePlayer::AGsCharacterSequencePlayer() : Super()
{
	if (GetMesh())
		GetMesh()->bBoneScaleMode = false;

	_customizeComponent = CreateDefaultSubobject<UGsCustomizeComponent>(TEXT("CustomizeComponent"));
	_customizeComponent->SetupAttachment(RootComponent);
}

// BSAM - 엑터로 하지않고 ID로 하는것은 레벨 진입점에서 시퀸스 플레이시 아직 매쉬들이 로드 되지 않았을 수 있기 때문이다(실제로 테트스떄 그랬다)
// scale, morphTarget 을 적용 할려면 customHandler가 Actor나 Compont로 되어야 할거 같다. 논의 후 들어간다면 CustomHandler변경이 필요
void AGsCharacterSequencePlayer::ApplyCurrentLocalPlayer(UGsGameObjectLocalPlayer* InGameObjectLocalPlayer)
{
	if (InGameObjectLocalPlayer == nullptr) return;

	if (UWorld* world = GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::TestClient)
			{
				AGsCharacterPlayer* SourceActor = Cast< AGsCharacterPlayer>(InGameObjectLocalPlayer->GetCharacter());

				if (SourceActor)
				{					
					GetMesh()->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentBODY()->SkeletalMesh);

					// Art(AD, TA님)요청으로 Material 0 강제 셋팅.(CHR-9897)
					GetMesh()->SetMaterial(0, SourceActor->GetPartsSkeletalComponentBODY()->GetMaterial(0));

					_partsSkeletalComponentFACE->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentFACE()->SkeletalMesh);
					_partsSkeletalComponentHAIR->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentHAIR()->SkeletalMesh);

					// Art(AD, TA님)요청으로 Material 0 강제 셋팅.(CHR-9897)
					_partsSkeletalComponentFACE->SetMaterial(0, SourceActor->GetPartsSkeletalComponentFACE()->GetMaterial(0));
					_partsSkeletalComponentHAIR->SetMaterial(0, SourceActor->GetPartsSkeletalComponentHAIR()->GetMaterial(0));


					if (_customizeComponent)
					{
						_customizeComponent->SetCustomizeBoneScaleToSingleMode(InGameObjectLocalPlayer, this);
					}
						
				}
			}
			else
			{
				if (_customizeComponent)
					_customizeComponent->SetCustomize(InGameObjectLocalPlayer);
			}
		}
	}	
}
void AGsCharacterSequencePlayer::ApplyCurrentLocalPlayerNoWeapon(UGsGameObjectLocalPlayer* InGameObjectLocalPlayer)
{
	if (InGameObjectLocalPlayer == nullptr) return;

	if (UWorld* world = GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::TestClient)
			{
				AGsCharacterPlayer* SourceActor = Cast< AGsCharacterPlayer>(InGameObjectLocalPlayer->GetCharacter());

				if (SourceActor)
				{
					GetMesh()->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentBODY()->SkeletalMesh);
					_partsSkeletalComponentFACE->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentFACE()->SkeletalMesh);
					_partsSkeletalComponentHAIR->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentHAIR()->SkeletalMesh);

					if (_customizeComponent)
					{
						_customizeComponent->SetCustomizeBoneScaleToSingleMode(InGameObjectLocalPlayer, this);
					}
				}
			}
			else
			{
				if (_customizeComponent)
					_customizeComponent->SetCustomize(InGameObjectLocalPlayer, false);
			}
		}
	}
}

UStaticMeshComponent* AGsCharacterSequencePlayer::GetWeaponStaticMesh()
{
	AGsWeaponActor* weaponActor = GetWeapon();
	if (weaponActor)
	{
		return weaponActor->GetStaticMeshComponent();
	}

	return nullptr;
}

UStaticMeshComponent* AGsCharacterSequencePlayer::GetWeaponSubStaticMesh()
{
	AGsWeaponActor* weaponActor = GetWeaponSub();
	if (weaponActor)
	{
		return weaponActor->GetStaticMeshComponent();
	}

	return nullptr;
}

bool AGsCharacterSequencePlayer::IsMale(UGsGameObjectLocalPlayer* InGameObjectLocalPlayer)
{
	if (nullptr == InGameObjectLocalPlayer)
	{
		return false;
	}

	FGsGameObjectDataLocalPlayer* LocalData = (InGameObjectLocalPlayer->GetCastData<FGsGameObjectDataLocalPlayer>());
	if (LocalData)
	{
		if (LocalData->GetCreatureGenderType() == CreatureGenderType::MALE)
			return true;
	}

	return false;
}

bool AGsCharacterSequencePlayer::IsFemale(UGsGameObjectLocalPlayer* InGameObjectLocalPlayer)
{
	if (nullptr == InGameObjectLocalPlayer)
	{
		return false;
	}

	FGsGameObjectDataLocalPlayer* LocalData = (InGameObjectLocalPlayer->GetCastData<FGsGameObjectDataLocalPlayer>());
	if (LocalData)
	{
		if (LocalData->GetCreatureGenderType() == CreatureGenderType::FEMALE)
			return true;
	}

	return false;
}

