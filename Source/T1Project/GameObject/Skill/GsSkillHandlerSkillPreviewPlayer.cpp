// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Skill/GsSkillHandlerSkillPreviewPlayer.h"

#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Animation/AnimMontage.h"
#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsHommingProjectileComponent.h"

#include "GameObject/ObjectClass/GsGameObjectSkillPreviewPlayer.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/State/RemotePlayer/GsStateRemotePlayerAttack.h"

#include "Runner/GsSkillRunnerSkillPreviewPlayer.h"
#include "Runner/GsBuffSkillRunnerPreviewPlayer.h"

#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "UTIL/GsDir.h"
#include "Data/GsResourceManager.h"

void FGsSkillHandlerSkillPreviewPlayer::Initialize(UGsGameObjectBase* Owner)
{
	Super::Initialize(Owner);

	_buffSkillRunner = new FGsBuffSkillRunnerPreviewPlayer(Cast<UGsGameObjectSkillPreviewPlayer>(Owner), [this](int skillid) {});
}

void FGsSkillHandlerSkillPreviewPlayer::Finalize()
{
	if (_buffSkillRunner)
	{
		delete _buffSkillRunner;
		_buffSkillRunner = nullptr;
	}

	Super::Finalize();
}

void FGsSkillHandlerSkillPreviewPlayer::Update(float Delta)
{
	if (_buffSkillRunner->IsValid() && !_buffSkillRunner->Update(Delta))
	{
		_buffSkillRunner->EndRunner();
	}

	Super::Update(Delta);
}

void FGsSkillHandlerSkillPreviewPlayer::CreateSkillRunner()
{
	_skillRunner = new FGsSkillRunnerSkillPreviewPlayer([this](int skillid, bool isEnd) { CallbackEndRunner(skillid, isEnd); });
}

void FGsSkillHandlerSkillPreviewPlayer::NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData)
{
	// 선판정등의 서버 영향을 받는 로직들을 분리하기 위해 상위 상속 호출을 생략
	//auto skillData = Super::NetSkillStart(Packet);
	AGsCharacterBase* characterActor = _owner->GetCharacter();
	if (nullptr == characterActor)
	{
		return;
	}

	if (skillData)
	{
		_skillNetData._skillId = Packet->SkillId();
		_skillNetData._skillLevel = Packet->SkillLevel();
		_skillNetData._curPos = Packet->CurPos();
		_skillNetData._curDir = Packet->CurrDir();
		_skillNetData._aimStartPos = Packet->AimStartPos();
		_skillNetData._aimTargetPos = Packet->AimTargetPos();
		_skillNetData._targetGameId = Packet->TargetId();
		_skillNetData._skillStartType = Packet->SkillStartType();

		// 유도체 관련 처리
		if (UGsHommingProjectileComponent* hommingComponent = characterActor->GetHommingProjectile())
		{
			// 유도체 정보 초기화
			hommingComponent->ClearHommingProjectileInfo();

			TArray<UGsGameObjectBase*> targetList;
			GSSkillPreview()->GetTargetList(targetList, _owner->GetGameId(), &skillData->targetFilterInfo);
			// 스킬 미리보기는 무조건 타격
			for (UGsGameObjectBase* el : targetList)
			{
				AGsCharacterBase* targetActor = el->GetCharacter();
				if (nullptr == targetActor)
				{
					continue;
				}

				hommingComponent->SetTargetActor(targetActor);

				if (skillData->noTimeline)
				{
					// 공격자 설정
					hommingComponent->SetHommingProjectileInfo(HitResultType::NORMAL, targetActor);
					// 타겟의 유도체 목표지점 설정
					// 소켓정보를 외부에서 얻어와야할수도 있다.
					// targetActor->SetHomingTargetPoint(characterActor->GetActorLocation(), TEXT("HitEffectSocket"));
				}
			}
		}

		// 루트 모션 스킬인가 확인
		if (Packet->ReactionId() != 0)
		{
			if (UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>())
			{
				if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(skillData->aniPath))
				{
					FTransform tm = res->ExtractRootMotionFromTrackRange(0.f,
						res->GetPlayLength());
					FTransform localTm(FGsDir::AngleToRotator(Packet->CurrDir()), Packet->CurPos());
					FTransform::Multiply(&tm, &tm, &localTm);

					castObject->StartRootmotion(Packet->CurrDir(), Packet->CurPos(), tm.GetLocation(), Packet->ReactionId());
					castObject->SetRootmotionIgnoreNavData(true);
				}
			}
		}		

		// 캐스팅 스킬
		if (skillData->category == SkillCategory::CAST)
		{
			if (Packet->SkillStartType() == SkillStartType::CAST)
			{
				FGsGameObjectStateManager::ProcessEvent(_owner, EGsStateBase::Casting);
				return;
			}
			else
			{
				// 누락된 AnimNotify 선처리
				// 각 State클래스에서 처리 할것
				// FGsStateRemotePlayerAttack::PlayMontageMissingAnimNotify(_owner, this, skillData, skillData->castInfo.sectionName);

				// 해당 위치로 강제 이동
				// 스킬 전체를 적용시킬것인지 추후 고려
				FVector pos = Packet->CurPos();
				FVector dir = FGsDir::angleToDir(Packet->CurrDir());

				// 지형 높이 고려 안함
				characterActor->SetActorLocationAndRotation(pos, dir.Rotation());
			}
		}

		// Attack 상태 전환
		FGsGameObjectStateManager::ProcessEvent(_owner, EGsStateBase::Attack);

		// SkillRunner 설정
		float fSpeedRate = skillData->isNormal ? GetAttackSpeedRate() : GetCastSpeedRate();
		_skillRunner->StartRunner(_owner, skillData, fSpeedRate, GetNetSkillData());
	}
}

void FGsSkillHandlerSkillPreviewPlayer::NetBuffSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData)
{
	// Super::NetBuffSkillStart(Packet, skillData);

	if (_buffSkillRunner)
	{
		_buffSkillRunner->StartRunner(skillData, Packet->SkillLevel());
	}
}