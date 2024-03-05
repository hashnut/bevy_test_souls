// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillNotifyNodePreviewMoveLocation.h"
#include "Management/ScopeGame/GsSkillPreviewManager.h"

#include "GameObject/ObjectClass/GsGameObjectSkillPreviewPlayer.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"

#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyMoveLocation.h"

FGsSkillNotifyNodePreviewMoveLocation::FGsSkillNotifyNodePreviewMoveLocation(IffApplyType SkillIffType)
{
	_skillIffType = SkillIffType;
}

// Static function
void FGsSkillNotifyNodePreviewMoveLocation::Action(const FGsSchemaSkillNotifyMoveLocation* MoveLocationData,
	UGsGameObjectBase* Owner, UGsGameObjectBase* Target)
{
	if (nullptr == Owner || nullptr == Target)
	{
		return;
	}

	// https://jira.gamevilcom2us.com/wiki/pages/viewpage.action?pageId=237038726
	UGsGameObjectBase* target = Owner;
	FVector destPos = Owner->GetLocation();
	if (MoveLocationData->centerType == SkillAreaCenter::TARGET)
	{
		target = Target;
	}

	float totalRadius = target->GetData()->GetScaledCapsuleRadius() + Owner->GetData()->GetScaledCapsuleRadius();

	if (MoveLocationData->moveType == SkillNotifyMoveType::NEAR_TARGET)
	{
		FVector dir = Target->GetLocation() - Owner->GetLocation();
		float distance = dir.Size() - totalRadius;
		dir.Normalize();
		destPos += dir * distance;
	}
	else if (MoveLocationData->moveType == SkillNotifyMoveType::TO_TARGET)
	{
		FVector dir = (Target->GetLocation() - Owner->GetLocation()).GetSafeNormal();
		destPos = Owner->GetLocation() + dir * MoveLocationData->distance;
	}
	else if (MoveLocationData->moveType == SkillNotifyMoveType::FROM_TARGET)
	{
		FVector dir = (Owner->GetLocation() - Target->GetLocation()).GetSafeNormal();
		destPos = target->GetLocation() + dir * MoveLocationData->distance;
	}
	else if (MoveLocationData->moveType == SkillNotifyMoveType::FRONT)
	{
		FVector dir = target->GetRotation().Vector();
		destPos = target->GetLocation() + dir * (MoveLocationData->distance + totalRadius);
	}
	else if (MoveLocationData->moveType == SkillNotifyMoveType::BACK)
	{
		FVector dir = target->GetRotation().Vector() * -1.f;
		destPos = target->GetLocation() + dir * (MoveLocationData->distance + totalRadius);
	}

	// 현재 SkillRunner의 _netSkillData Pos, dir을 변경한다.
	GSSkillPreview()->SetOwnerPostion(destPos);
	GSSkillPreview()->SetOwnerDirection((target->GetLocation() - destPos).GetSafeNormal());

	// MoveLocation 보간 연출 실행
	UGsGameObjectCreature* castObject = Owner->CastGameObject<UGsGameObjectCreature>();
	castObject->StartMoveLocation(castObject->GetLocation(), destPos, MoveLocationData->duration);
}

void FGsSkillNotifyNodePreviewMoveLocation::Action(class UGsGameObjectBase* Owner)
{
	Super::Action(Owner);
	
	// 리스트 형태이지만 리스트로 활용하지 않음. 0번만 사용한다
	const FGsSchemaSkillNotifyElement* element = _currNotifyData->notifyElementList[0].GetRow();
	if (const FGsSchemaSkillNotifyMoveLocation* moveLocationNotifySet = element->typeInfo.GetRowByType<
		FGsSchemaSkillNotifyMoveLocation>())
	{
		// 타겟 정보 얻기
		TArray<UGsGameObjectBase*> targetList;
		if (const FGsSchemaSkillTargetFilter* targetFilterSet = _currNotifyData->targetFilterInfo.GetRow())
		{
			GSSkillPreview()->GetTargetList(targetList, Owner->GetGameId(), targetFilterSet);
		}
		else
		{
			GSSkillPreview()->GetTargetList(targetList, Owner->GetGameId(), _skillIffType);
		}

		if (targetList.Num() == 0)
		{
			return;
		}

		Action(moveLocationNotifySet, Owner, targetList[0]);
	}
}