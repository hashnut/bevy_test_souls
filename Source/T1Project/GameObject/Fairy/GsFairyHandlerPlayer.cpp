#include "GsFairyHandlerPlayer.h"
#include "Engine/World.h"
#include "UObjectGlobals.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "ActorEx/GsCharacterPlayer.h"
#include "ActorEx/GsActorFieldFairy.h"

#include "Data/GsDataContainManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Curves/CurveFloat.h"

void FGsFairyHandlerPlayer::Initialize(UGsGameObjectBase* Owner)
{
	_ownPlayer = Owner->CastGameObject<UGsGameObjectPlayer>();
	
}

void FGsFairyHandlerPlayer::InitializeActor(class AActor* Owner)
{
	if (auto actor = Cast<AGsCharacterPlayer>(_ownPlayer->GetCharacter()))
	{
		const auto fairySpringArm = actor->GetPartsFairySpringArm();
		_springEndLength = fairySpringArm->TargetArmLength * fairySpringArm->TargetArmLength * 4.f;
		_springRelativeLocation = fairySpringArm->GetRelativeLocation();
	}
}

void FGsFairyHandlerPlayer::Finalize()
{
	UpdateEquipFairy(INVALID_FAIRY_ID, true);
}

void FGsFairyHandlerPlayer::Update(float Delta)
{
	if (_ownPlayer && _fairy.IsValid())
	{
		if (auto moveHandler = _ownPlayer->GetMovementHandler())
		{
			if (auto actor = Cast<AGsCharacterPlayer>(_ownPlayer->GetCharacter()))
			{
				bool IsMoveAction = moveHandler->IsMoveState();
				if (!IsMoveAction)
				{
					const auto fairySpringArm = actor->GetPartsFairySpringArm();
					auto relaticeTr = fairySpringArm->GetSocketTransform(USpringArmComponent::SocketName,
						ERelativeTransformSpace::RTS_Component);
					IsMoveAction = relaticeTr.GetLocation().SizeSquared() > _springEndLength;

				}
				// 이동 이면서 페어리 탈것버프 이동인지 확인
				else if (_ownPlayer->IsVehicleMode())
				{
					const auto fairySpringArm = actor->GetPartsFairySpringArm();
					if (_springArmLengthCurve)
					{
						float length = _springArmLengthCurve->GetFloatValue(_springArmCurrentTime);
						fairySpringArm->TargetArmLength = length;
						if (_springArmCurrentTime >= _springArmMaxTime)
						{
							_springArmLengthCurve = nullptr;
						}
						_springArmCurrentTime += Delta;
					}
					_fairy->SetIsVehicleMove(true);
				}
				else
				{
					_fairy->SetIsVehicleMove(false);
				}

				_fairy->SetIsMove(IsMoveAction);
			}
		}
	}
}

/*Fairy는 Data Object가 없는 Actor 이므로 Data는 handler가 가지고 처리하게 한다.*/
void FGsFairyHandlerPlayer::UpdateEquipFairy(FairyId id, bool instant)
{	
	FairyFeatureId featureID = INVALID_FAIRY_FEATURE_ID;
	if (_fairyData.IsValid()) featureID = _fairyData.Pin()->GetFairyFeatureID();

	_fairyData = GSFairy()->GetFairyData(id);

	if (!_fairyData.IsValid() || _fairyData.Pin()->GetFairyFeatureID() != featureID)
	{
		if (_fairy.IsValid())
		{
			_fairy->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			_fairy->SetDespawn(instant);				
		}		

		_fairy.Reset();
	}

	if (!_fairyData.IsValid() || _fairy.IsValid()) return;
	
	if (auto actor = Cast<AGsCharacterPlayer>(_ownPlayer->GetCharacter()))
	{
		if (UWorld* world = actor->GetWorld())
		{
			const auto fairySpringArm = actor->GetPartsFairySpringArm();			

			if (auto fairyData = _fairyData.Pin())
			{
				FString strPath = fairyData->GetFairySpawnObjectPath().GetAssetPathString() + TEXT("_C");
				const auto& baseEffectData = fairyData->GetFairyBaseEffect();
				const auto skillEffect = fairyData->GetFairySkillEffect();
				const auto vehicleEffect = fairyData->GetFaityVehicleEffect();
				const auto& fairyTeleportStartMontagePath = fairyData->GetPathTeleportStartFairy();
				const auto& fairyTeleportArriveMontagePath = fairyData->GetPathTeleportArriveFairy();
				const auto& fairyTeleportParticleEffectList = fairyData->GetFairyTeleportParticleEffectList();

				if (UClass* loadObjectC = LoadObject<UClass>(nullptr, *strPath))
				{
					FActorSpawnParameters spawnParam = FActorSpawnParameters();
					spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					_fairy = Cast<AGsActorFieldFairy>(world->SpawnActor(loadObjectC, &FTransform::Identity, spawnParam));
					if (_fairy.IsValid())
					{
						_fairy->AttachToComponent(fairySpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
						_fairy->SetSpawn(true);
						_fairy->SetBaseEffect(baseEffectData, skillEffect, vehicleEffect);
						_fairy->SetTeleportEffect(fairyTeleportStartMontagePath, fairyTeleportArriveMontagePath, fairyTeleportParticleEffectList);

						// 블루프린트 노드에 통신
						_fairy->UpdateFairyOwner(_ownPlayer->IsObjectType(EGsGameObjectType::LocalPlayer));

						_fairy->SetActorHiddenInGame(actor->IsHidden());

						SetFairyTalkInfo();
					}
				}
			}
		}
	}
}

void FGsFairyHandlerPlayer::ShowFairyWarpEffect()
{
	if (_fairy.IsValid())
	{
		_fairy->OnFairyWarpEffect();
	}
}

void FGsFairyHandlerPlayer::ShowFairySpawnEffect()
{
	if (_fairy.IsValid())
	{
		_fairy->OnFairySpawnEffect();
	}
}

void FGsFairyHandlerPlayer::SetFairyHiddenInGame()
{
	if (_fairy.IsValid())
	{
		auto ownCharacter = Cast<AGsCharacterPlayer>(_ownPlayer->GetCharacter());
		check(ownCharacter);

		_fairy->SetActorHiddenInGame(ownCharacter->IsHidden());
	}
}

void FGsFairyHandlerPlayer::RestoreEquipFairy()
{
	if (auto fairyData = _fairyData.Pin())
	{
		auto fairyID = fairyData->_id;
		_fairyData.Reset();

		UpdateEquipFairy(fairyID);
		OnRestoreVehicleEffect();
	}
}

void FGsFairyHandlerPlayer::SetFairySkillEffect()
{
	if (_fairy.IsValid())
	{
		_fairy->SetSkill(true);
	}
}

void FGsFairyHandlerPlayer::SetFairyRelativeLocation(const FVector& RelativeLocation)
{
	if (AGsCharacterPlayer* actor = Cast<AGsCharacterPlayer>(_ownPlayer->GetCharacter()))
	{
		USpringArmComponent* fairySpringArm = actor->GetPartsFairySpringArm();
		fairySpringArm->SetRelativeLocation(RelativeLocation);
	}
}

void FGsFairyHandlerPlayer::SetFairyVehicleEffect(bool IsActive, bool useTalk)
{
}

void FGsFairyHandlerPlayer::OnVehicleEffect(UCurveFloat* curveData, const FVector& reletivePosition, bool useTalk)
{
	if (curveData)
	{
		_springArmLengthCurve = curveData;
		// 첫번째키에 무조건 디폴트값을 설정한다는 가정
		_springArmDefaultLength = _springArmLengthCurve->GetFloatValue(0.f);

		_springArmLengthCurve->GetTimeRange(_springArmCurrentTime, _springArmMaxTime);
	}
	if (_fairy.IsValid())
	{
		_fairy->OnActiveVehicleEffect();
	}
	SetFairyVehicleEffect(true, useTalk);
	if (false == FVector::ZeroVector.Equals(reletivePosition))
	{
		SetFairyRelativeLocation(reletivePosition);
	}
}

void FGsFairyHandlerPlayer::OnRestoreVehicleEffect()
{
	if (false == FMath::IsNearlyZero(_springArmDefaultLength))
	{
		if (auto actor = Cast<AGsCharacterPlayer>(_ownPlayer->GetCharacter()))
		{
			const auto fairySpringArm = actor->GetPartsFairySpringArm();
			fairySpringArm->TargetArmLength = _springArmDefaultLength;
		}
		_springArmLengthCurve = nullptr;
		_springArmCurrentTime = 0.f;

		_fairy->SetIsVehicleMove(false);
	}
	if (_fairy.IsValid())
	{
		_fairy->OnDeactiveVehicleEffect();
	}

	if (AGsCharacterPlayer* actor = Cast<AGsCharacterPlayer>(_ownPlayer->GetCharacter()))
	{
		USpringArmComponent* fairySpringArm = actor->GetPartsFairySpringArm();
		fairySpringArm->SetRelativeLocation(_springRelativeLocation);
	}

	SetFairyVehicleEffect(false);
}

bool FGsFairyHandlerPlayer::CheckFairyChange(FairyId id)
{
	if (auto data = _fairyData.Pin())
	{
		return data->_id != id;
	}
	return true;
};

void FGsFairyHandlerPlayer::InternalSpawnActor(class UClass* ObjectClass, const TArray<FGsSchemaFairyEffect>& BaseEffect, const FGsSchemaFairyEffect* SkillEffect)
{
	auto ownCharacter = Cast<AGsCharacterPlayer>(_ownPlayer->GetCharacter());
	check(ownCharacter);

	if (UWorld* world = ownCharacter->GetWorld())
	{
		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		_fairy = Cast<AGsActorFieldFairy>(world->SpawnActor(ObjectClass, &FTransform::Identity, spawnParam));
		if (_fairy.IsValid())
		{
			const auto fairySpringArm = ownCharacter->GetPartsFairySpringArm();
			_fairy->AttachToComponent(fairySpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);

			_fairy->SetSpawn(false);
			_fairy->SetBaseEffect(BaseEffect, SkillEffect, nullptr);
			//_fairy->SetTeleportEffect(fairyTeleportStartMontagePath, fairyTeleportArriveMontagePath, fairyTeleportParticleEffectList);
			_fairy->SetActorHiddenInGame(ownCharacter->IsHidden());

			SetFairyTalkInfo();
		}
	}
}
