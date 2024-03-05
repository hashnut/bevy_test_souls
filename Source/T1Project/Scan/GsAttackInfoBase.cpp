#include "GsAttackInfoBase.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Core/Public/Misc/DateTime.h"

#include "Engine/Public/TimerManager.h"

// 공격 시작(시간 저장)
void FGsAttackInfoBase::StartAttack()
{
	ClearTimer();

	UWorld* world = GSGameObject()->GetWorld();
	if (world == nullptr)
	{
		return;
	}
	world->GetTimerManager().SetTimer(_timeHandleAttack, FTimerDelegate::CreateLambda([this]() {
		EndTime();
		}), GetWaitTime(), false);
	
	_isAttack = true;
}
void FGsAttackInfoBase::EndTime()
{
	_isAttack = false;

	ClearTimer();
}


void FGsAttackInfoBase::ClearTimer()
{
	if (_timeHandleAttack.IsValid() == false)
	{
		return;
	}

	UWorld* world = GSGameObject()->GetWorld();	
	if (world == nullptr)
	{
		return;
	}
	// 타이머 클리어
	world->GetTimerManager().ClearTimer(_timeHandleAttack);
	_timeHandleAttack.Invalidate();
}

void FGsAttackInfoBase::Finalize()
{
	ClearTimer();
}