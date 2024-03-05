// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAbnormalityHandlerSkillPreviewPlayer.h"

#include "Animation/AnimMontage.h"

#include "ActorEx/GsCharacterBase.h"

#include "DataSchema/Skill/GsSchemaSkillReaction.h"
#include "DataSchema/Skill/RootMotion/GsSchemaRootmotionSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityEffectUtilSet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyMoveLocation.h"

#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Data/GsResourceManager.h"

#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Skill/Notify/SkillPreview/GsSkillNotifyNodePreviewMoveLocation.h"

#include "UTIL/GsDir.h"


void FGsAbnormalityHandlerSkillPreviewPlayer::Add(const AbnormalityData& NetData, bool isRefresh)
{
	Super::Add(NetData, isRefresh);

	TWeakPtr<FGsAbnormalityData> abnormalityData = GetAbnormalityData(NetData.GetInstanceId());
	if (abnormalityData.IsValid())
	{
		TArray<AbnormalityEffectType> effectTypes;
		abnormalityData.Pin()->GetAllEffectType(effectTypes);
		for (AbnormalityEffectType effectType : effectTypes)
		{
			// 넉백 루트모션 처리
			if (effectType == AbnormalityEffectType::CC_KNOCK_BACK)
			{
				const FGsSchemaSkillReaction* reactionData = GSkill()->GetReactionData(_owner, EReactionType::KNOCKBACK);
				if (nullptr == reactionData)
				{
					return;
				}

				const FGsSchemaRootmotionSet* rootmotionData = reactionData->rootMotionId.GetRow();
				if (nullptr == rootmotionData)
				{
					return;
				}

				if (UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>())
				{
					if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(reactionData->aniPath))
					{
						FTransform tm = res->ExtractRootMotionFromTrackRange(0.f,
							res->GetPlayLength());

						AGsCharacterBase* characterActor = _owner->GetCharacter();
						if (nullptr == characterActor)
						{
							return;
						}

						USkeletalMeshComponent* meshComponent = characterActor->GetMesh();
						if (nullptr == meshComponent)
						{
							return;
						}

						FQuat meshRelativeRot = meshComponent->GetRelativeRotation().Quaternion();
						// 스켈레탈 메시 기준으로 제작되어 이동량은 변환후 반영, 회전 변화량은 바로 반영
						FTransform deltaTm(tm.GetRotation(), meshRelativeRot.RotateVector(tm.GetLocation()));

						const FRotator& rot = GSSkillPreview()->GetObjectRotation(_owner->GetGameId());
						const FVector& pos = _owner->GetLocation();
						FTransform localTm(rot, pos);
						FTransform::Multiply(&tm, &deltaTm, &localTm);

						castObject->StartRootmotion(FGsDir::RotatorToAngle(rot),
							_owner->GetLocation(), tm.GetLocation(), rootmotionData->id);
						castObject->SetRootmotionIgnoreNavData(true);
					}
				}
			}
			else if (effectType == AbnormalityEffectType::CC_MOVE_LOCATION)
			{
				const FGsSchemaAbnormalityIndexSet* indexSet = abnormalityData.Pin()->GetEnableAbnormalityEffectByEffectType(effectType);
				if (nullptr == indexSet)
				{
					return;
				}

				if (indexSet->levelList.Num() < 1)
				{
					return;
				}

				const FGsSchemaAbnormalityIndexLevelInfo& info = indexSet->levelList[1];
				const FGsSchemaAbnormalityCrowdControlSet* tableData = info.effectRow.GetRowByType<FGsSchemaAbnormalityCrowdControlSet>();
				if (nullptr == tableData)
				{
					return;
				}

				const FGsSchemaSkillNotifySet* skillNotifySet = UGsTableUtil::FindRowById<UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(
					tableData->param.skillNotifyId);
				if (nullptr == skillNotifySet)
				{
					return;
				}

				UGsGameObjectBase* casterObject = GSSkillPreview()->FindObject(NetData.GetCasterId());
				if (nullptr == casterObject)
				{
					return;
				}

				for (const FGsSchemaSkillNotifyElementRow& element : skillNotifySet->notifyElementList)
				{
					if (const FGsSchemaSkillNotifyElement* notifyElement = element.GetRow())
					{
						if (const FGsSchemaSkillNotifyMoveLocation* moveLocationNotifySet = notifyElement->typeInfo.GetRowByType<
							FGsSchemaSkillNotifyMoveLocation>())
						{
							FGsSkillNotifyNodePreviewMoveLocation::Action(moveLocationNotifySet, _owner, casterObject);
						}
					}
				}
			}
		}
	}
}

void FGsAbnormalityHandlerSkillPreviewPlayer::Update(float Delta)
{
	TArray<AbnormalityInstanceId> removeList;
	for (const TPair<AbnormalityInstanceId, TSharedPtr<FGsAbnormalityData>>& pair : _mapAbnormality)
	{
		pair.Value->UpdateRemainTime(Delta);
		if (pair.Value->GetRemainTime() <= 0.f)
		{
			removeList.Emplace(pair.Key);
		}
	}

	for (AbnormalityInstanceId el : removeList)
	{
		Remove(el);
	}
}

void FGsAbnormalityHandlerSkillPreviewPlayer::ExecuteNotify(const FGsSchemaAbnormalityIndexSet* EffectIndexSet)
{
	if (EffectIndexSet->effectType == AbnormalityEffectType::UTIL_ON_REMOVE_EXE_SKILL_NOTIFY)
	{
		if (EffectIndexSet->levelList.Num() < 1)
		{
			return;
		}

		const FGsSchemaAbnormalityIndexLevelInfo& info = EffectIndexSet->levelList[1];
		const FGsSchemaAbnormalityEffectUtilSet* tableData = info.effectRow.GetRowByType<FGsSchemaAbnormalityEffectUtilSet>();
		if (nullptr == tableData)
		{
			return;
		}

		const FGsSchemaSkillNotifySet* skillNotifySet = tableData->param.SkillNoityId.GetRow();
		if (nullptr == skillNotifySet)
		{
			return;
		}

		TArray<UGsGameObjectBase*> targetList;
		GSSkillPreview()->GetTargetList(targetList, _owner->GetGameId(), skillNotifySet->targetFilterInfo.GetRow());

		for (UGsGameObjectBase* el : targetList)
		{
			GSSkillPreview()->ExecuteNotify(0, _owner->GetGameId(), el->GetGameId(), skillNotifySet, 0);
		}
	}
}

void FGsAbnormalityHandlerSkillPreviewPlayer::Remove(AbnormalityInstanceId InstanceId)
{
	if (TSharedPtr<FGsAbnormalityData>* findData = _mapAbnormality.Find(InstanceId))
	{
		if (const FGsSchemaAbnormalitySet* abnormalityTable = (*findData)->GetTable())
		{
			for (int32 effectId : abnormalityTable->effectIdList)
			{
				if (const FGsSchemaAbnormalityIndexSet* effectIndexSet = UGsTableUtil::FindRowById<UGsTableAbnormalityIndexSet,
					FGsSchemaAbnormalityIndexSet>(effectId))
				{
					ExecuteNotify(effectIndexSet);
				}
			}
		}
	}

	Super::Remove(InstanceId);
}

void FGsAbnormalityHandlerSkillPreviewPlayer::Cancel()
{
	TArray<AbnormalityInstanceId> removeList;
	_mapAbnormality.GenerateKeyArray(removeList);
	for (AbnormalityInstanceId el : removeList)
	{
		Remove(el);
	}
}