// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillRunnerCreature.h"
#include "../Notify/GsSkillNotifyNodeCollision.h"
#include "../Notify/GsSkillNotifyNodeGuideEffect.h"
#include "../Notify/GsSkillNotifyNodeBeamParticle.h"
#include "Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifySkillGuide.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyBeamParticle.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "../../ObjectClass/GsGameObjectBase.h"

FGsSkillRunnerCreature::FGsSkillRunnerCreature(TFunction<void(int, bool)> EndRun) : Super(EndRun)
{

}

bool FGsSkillRunnerCreature::Update(float DeltaTime)
{
	// 데이터 셋팅도 안 되었네
	if (false == IsValid())
	{
		return true;
	}

	// SkillNotifyList 체크
	if (_currentInfo.Get()->IsValid())
	{
		TArray<TSharedPtr<FGsSkillNotifyNodeBase>> actionNodes;
		_currentInfo.Get()->Pop(actionNodes, GetElapsedTime());

		for (TSharedPtr<FGsSkillNotifyNodeBase> el : actionNodes)
		{
			el.Get()->Action(_owner);
		}
	}
	
	// RunnerBase::Update는 스킬전체 시간이 만료되었는지를 체크한다 (완전종료)
	return Super::Update(DeltaTime);
}

TSharedPtr<FGsSkillNotifyNodeBase> FGsSkillRunnerCreature::CreateSkillNode(const FGsSchemaSkillNotifySet* Data)
{
	if (TSharedPtr<FGsSkillNotifyNodeBase> superNode = Super::CreateSkillNode(Data))
	{
		return superNode;
	}

	// 클라이언트 Notify는 하나이다.
// 	const FGsSchemaSkillNotifyElement* element = Data->notifyElementList[0].GetRow();
// 	if (Data->type == SkillNotifyType::SKILLGUIDE_EFFECT)
// 	{
// 		const FGsSchemaSkillNotifySkillGuide* skillGuideTable = element->typeInfo.GetRow<FGsSchemaSkillNotifySkillGuide>();
// 		if (skillGuideTable)
// 		{
// 			// 가이드 이펙트 노드가 있는 스킬은 동기화 이슈로 회전 연출 보간 처리를 하지않는다.
// 			_isSmoothRotation = false;
// 
// 			return MakeShareable(new FGsSkillNotifyNodeGuideEffect(skillGuideTable,
// 				&_skillData.GetTableData()->collisionInfoList.Last()));
// 		}
// 	}
// 	else if (Data->type == SkillNotifyType::BEAM_PARTICLE_EFFECT)
// 	{
// 		const FGsSchemaSkillNotifyBeamParticle* beamParticleEffect = element->typeInfo.GetRow<FGsSchemaSkillNotifyBeamParticle>();
// 		if (beamParticleEffect)
// 		{
// 			return MakeShareable(new FGsSkillNotifyNodeBeamParticle(beamParticleEffect));
// 		}
// 	}

	return nullptr;
}