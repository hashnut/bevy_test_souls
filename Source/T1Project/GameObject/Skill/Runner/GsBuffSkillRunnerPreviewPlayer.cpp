#include "GsBuffSkillRunnerPreviewPlayer.h"
#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "GameObject/ObjectClass/GsGameObjectSkillPreviewPlayer.h"
#include "ActorComponentEx/GsHitEffectComponent.h"
#//include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "ActorEx/GsCharacterBase.h"
#include "Misc/App.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
//#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"

FGsBuffSkillRunnerPreviewPlayer::FGsBuffSkillRunnerPreviewPlayer(UGsGameObjectSkillPreviewPlayer* previewObject, TFunction<void(int)> EndRun)
	: _previewObject(previewObject)
	, _callbackEndRun(EndRun)
{

}

void FGsBuffSkillRunnerPreviewPlayer::StartRunner(const FGsSchemaSkillSet* Data, SkillLevel level)
{
	ClearRunner();

	//fsm 설정은 없고, effect와 ani 재생만 하게 되는 건지... target...
	if (!Data) return;

	//FGsGameObjectStateManager::ProcessEvent(_previewObject, EGsStateBase::Attack);

	_currData = Data;
	_startTime = FApp::GetCurrentTime();
	if (const UGsGlobalConstant* globalConstant = GData()->GetGlobalData())
	{
		_totalRuntime = globalConstant->BuffSkillDelayTime;
	}

	if (Data->notifyInfoList.IsValidIndex(0))
	{
		auto notifyData = Data->notifyInfoList[0].skillNotifyId.GetRow();

		auto character = _previewObject->GetCharacter();
		//FGsGameObjectDataCreature* data = _previewObject->GetCastData<FGsGameObjectDataCreature>();
		//data->SetUserStatus(UserStatus::USER_STATE_COMBAT);
// 		if (UGsAnimInstanceState* anim = _previewObject->GetAnimInstance())
// 		{
// 			anim->SetBattleIdleType(false);
// 		}
		//if (auto resAnim = GResource()->Get<UAnimMontage>(EResourceType::AnimMontage, _currData->aniPath, true)) //동작이 없으므로, 프리뷰에서는 기본 동작 필요
			//character->PlayAnimMontage(resAnim, 1.f);
		//if (UGsSkillCollisionComponent* component = Cast<UGsSkillCollisionComponent>(
		//	character->GetComponentByClass(UGsSkillCollisionComponent::StaticClass())))
		//{

		TArray<UGsGameObjectBase*> targetList;
		if (Data->useSkillNotifyTargetFilter)
		{
			GSSkillPreview()->GetTargetList(targetList, _previewObject->GetGameId(), notifyData->targetFilterInfo.GetRow());
		}
		else
		{
			GSSkillPreview()->GetTargetList(targetList, _previewObject->GetGameId(), &Data->targetFilterInfo);
		}

		for (UGsGameObjectBase* el : targetList)
		{
			GSSkillPreview()->ExecuteNotify(Data->id, _previewObject->GetGameId(), el->GetGameId(), notifyData, 0);
		}
		//}
	}
}

void FGsBuffSkillRunnerPreviewPlayer::CancelRunner()
{
	ClearRunner();
}

void FGsBuffSkillRunnerPreviewPlayer::EndRunner()
{
	int skillId = 0;
	if (_currData)
	{
		skillId = _currData->id;
	}

	ClearRunner();
	//FGsGameObjectStateManager::ProcessEvent(_previewObject, EGsStateBase::AttackEnd);

	// Clear 이후 콜백 호출
	if (_callbackEndRun != nullptr)
	{
		_callbackEndRun(skillId);
	}
}

bool FGsBuffSkillRunnerPreviewPlayer::Update(float DeltaTime)
{
	if (IsValid())
	{
		return _totalRuntime > GetElapsed();
	}
	return true;
}

void FGsBuffSkillRunnerPreviewPlayer::ClearRunner()
{
	_currData = nullptr;
	_startTime = 0.f;
	_totalRuntime = 0.f;
}

bool FGsBuffSkillRunnerPreviewPlayer::IsValid() const
{
	return nullptr != _currData;
}

float FGsBuffSkillRunnerPreviewPlayer::GetElapsed() const
{
	return FApp::GetCurrentTime() - _startTime;
}
