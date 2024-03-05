// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillDetailInfoBase.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoEntry.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillHelper.h"
#include "DataSchema/Skill/Level/GsSchemaSkillCostConsumeInfo.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyProjectile.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoRichEntry.h"
#include "T1Project.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

void UGsUISkillDetailInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	// TEXT: 효과
	SetInfoUITitle(TEXT("SkillUI_Description_Effect"), _infoEffect);
	// TEXT: 대상
	SetInfoUITitle(TEXT("SkillUI_Description_Target"), _infoTarget);
	// TEXT: 사거리
	SetInfoUITitle(TEXT("SkillUI_Description_Range"), _infoRange);
	// TEXT: 소모
	SetInfoUITitle(TEXT("SkillUI_Description_Consume"), _infoCost);
	// TEXT: 재사용 대기 시간
	SetInfoUITitle(TEXT("SkillUI_Description_Cooltime"), _infoCoolTime);
	// TEXT: 천마석 사용 시 소모량
	SetInfoUITitle(TEXT("SkillUI_Description_SpiritShot"), _infoSpiritShot);
	
	// TEXT: 습득처
	SetInfoUITitle(TEXT("SkillUI_Description_Supply"), _infoAcquisition);
}

void UGsUISkillDetailInfoBase::NativeDestruct()
{
	_skill = nullptr;

	Super::NativeDestruct();
}

void UGsUISkillDetailInfoBase::SetData(const FGsSkill* InData, int32 InOverrideLevel)
{
	_skill = InData;

	if (nullptr == InData)
	{
		return;
	}

	int32 skillLevel = 1;
	if (0 < InOverrideLevel)
	{
		skillLevel = InOverrideLevel;
	}
	else
	{
		skillLevel = InData->GetTotalLevel();
	}

	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InData->_tableData->grade));
	_textBlockName->SetText(InData->_userSkillSetData->nameText);

	_textBlockDesc->SetText(InData->_userSkillSetData->descText);

	// 패시브 스킬의 경우, 레벨정보가 없다. 이 경우 함수 내부에서 Collapsed 처리한다
	SetEffectInfo(InData, skillLevel);
	SetTargetInfo(InData);
	SetRangeInfo(InData);
	SetCoolTimeInfo(InData, skillLevel);
	SetCostInfo(InData, skillLevel);
	SetSpiritShotInfo(InData);
	
	SetAcquisitionInfo(InData);
}

void UGsUISkillDetailInfoBase::SetEffectInfo(const FGsSkill* InData, int32 InSkillLevel)
{
	FString strEffect;
	if (GSEffectText()->GetEffectTextSkill(InData->GetSkillId(), InSkillLevel, strEffect))
	{
		_infoEffect->SetDesc(FText::FromString(strEffect));
		_infoEffect->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		return;
	}
	
	_infoEffect->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISkillDetailInfoBase::SetTargetInfo(const FGsSkill* InData)
{
	if (InData->_tableData)
	{
		const FGsSchemaSkillTargetFilter* targetFilter = nullptr;

		// 발사체가 하나라도 있으면 발사체가 기준이 된다
		if (const FGsSchemaProjectileSet* projectileSet = GetProjectileSet(InData->_tableData))
		{
			targetFilter = &projectileSet->targetFilterInfo;
		}
		else
		{
			targetFilter = &(InData->_tableData->targetFilterInfo);
		}

		// 1명 초과일 때만 찍는다
		if (targetFilter &&
			1 < targetFilter->targetCount)
		{
			// TEXT: 최대 {0} 명		
			FText textFormat;
			if (FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Description_Target_N"), textFormat))
			{
				FText textTarget;
				GetIffApplyTypeText(targetFilter->iffType, textTarget);

				FText textDesc = textFormat.Format(textFormat, targetFilter->targetCount);

				if (false == textTarget.IsEmpty())
				{
					FString strDesc;
					strDesc.Append(textTarget.ToString());
					strDesc.Append(TEXT(" "));
					strDesc.Append(textDesc.ToString());

					textDesc = FText::FromString(strDesc);
				}

				_infoTarget->SetDesc(textDesc);
				_infoTarget->SetVisibility(ESlateVisibility::HitTestInvisible);

				return;
			}
		}
	}

	_infoTarget->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISkillDetailInfoBase::SetRangeInfo(const FGsSkill* InData)
{
	// 액티브 스킬만 사거리 표시
	if (InData->_tableData &&
		SkillCategorySet::ACTIVE == InData->GetSkillCateogrySet())
	{
		if (0 < InData->_tableData->maxApproachRange)
		{
			// TEXT: 최대
			FText textMax;
			FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Description_Max"), textMax);

			FText textRange;
			FGsUIStringHelper::GetFormatedFractionalDigitsText(InData->_tableData->maxApproachRange * 0.01f, 
				0, 2, textRange);

			// maxApproachRange 값은 cm값으로 m로 변환을 위해 0.01을 곱해서 표현한다
			FString strDesc = FString::Format(TEXT("{0} {1}m"), 
				{ *(textMax.ToString()), *(textRange.ToString()) });

			FText textDesc = FText::FromString(strDesc);
			_infoRange->SetDesc(textDesc);
			_infoRange->SetVisibility(ESlateVisibility::HitTestInvisible);

			return;
		}
	}

	_infoRange->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISkillDetailInfoBase::SetCoolTimeInfo(const FGsSkill* InData, int32 InSkillLevel)
{
	const FGsSchemaSkillLevelInfo* levelSet = InData->GetLevelInfo(InSkillLevel);
	if (nullptr == levelSet)
	{
		_infoCoolTime->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_infoCoolTime->SetVisibility(ESlateVisibility::HitTestInvisible);

	// cooldownTime 이 float이라 소수점 값이 있을 경우 잘못표기 될수 있어 초로만 표기
	FString strTime;	
	//FGsTimeStringHelper::GetTimeString(FTimespan::FromSeconds(levelSet->cooldownTime), strTime);
	strTime = FGsTimeStringHelper::GetSecondString(levelSet->cooldownTime);

	_infoCoolTime->SetDesc(FText::FromString(strTime));
}

void UGsUISkillDetailInfoBase::SetCostInfo(const FGsSkill* InData, int32 InSkillLevel)
{
	const FGsSchemaSkillLevelInfo* levelSet = InData->GetLevelInfo(InSkillLevel);
	if (nullptr == levelSet)
	{
		_infoCost->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	const FGsSchemaSkillCostConsumeInfo& costInfo = levelSet->costConsumeInfo;

	FString strCost;

	// 소모 아이템
	for (const FGsSchemaSkillCostItemInfo& costItemInfo : costInfo.requireItemList)
	{
		if (0 != costItemInfo.itemId &&
			0 < costItemInfo.itemAmount)
		{
			if (const FGsSchemaItemCommon* itemCommon = FGsItem::GetTableDataByID(costItemInfo.itemId))
			{
				if (false == strCost.IsEmpty())
				{
					strCost.Append(TEXT(", "));
				}

				FText textFormat;
				FText::FindText(TEXT("SystemText"), TEXT("NUnits"), textFormat);

				strCost.Append(itemCommon->name.ToString());
				strCost.Append(TEXT(" "));
				strCost.Append(FString::Format(*textFormat.ToString(), { costItemInfo.itemAmount }));
			}
		}
	}

	// 소모 스탯
	for (const FGsSchemaSkillStatInfo& statInfo : costInfo.statInfoList)
	{
		if (false == strCost.IsEmpty())
		{
			strCost.Append(TEXT(", "));
		}

		FGsStatHelper::GetDesplayStatNameAndValue(statInfo.type, statInfo.value, strCost, false);
	}

	// 소모 정보 텍스트 없으면 가림
	if (strCost.IsEmpty())
	{
		_infoCost->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_infoCost->SetVisibility(ESlateVisibility::HitTestInvisible);
		_infoCost->SetDesc(FText::FromString(*strCost));
	}
}

void UGsUISkillDetailInfoBase::SetSpiritShotInfo(const FGsSkill* InData)
{
	const FGsSchemaSkillLevelSet* levelSet = InData->GetCurrLevelSet();

	if (nullptr == levelSet ||
		0 >= levelSet->spiritShotConsumption)
	{
		_infoSpiritShot->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_infoSpiritShot->SetVisibility(ESlateVisibility::HitTestInvisible);

	// TEXT: {0}개
	FText textFormat;
	FText::FindText(TEXT("SystemText"), TEXT("NUnits"), textFormat);

	FText textCount = FText::Format(textFormat, levelSet->spiritShotConsumption);
	_infoSpiritShot->SetDesc(textCount);
}

void UGsUISkillDetailInfoBase::SetAcquisitionInfo(const FGsSkill* InData)
{
	_infoAcquisition->SetVisibility(ESlateVisibility::Collapsed);
	_infoAcquisition->SetDesc(FText::FromString(TEXT("")));

	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	SkillId UserSkillSetId = InData->GetUserSkillSetId();
	if (INVALID_SKILL_ID == UserSkillSetId)
	{
		GSLOG(Error, TEXT("INVALID_SKILL_ID == UserSkillSetId"));
		return;
	}

	UGsSkillManager* SkillManager = GSkill();
	if (nullptr != SkillManager)
	{
		// 업그레이드 스킬인 경우, 실제 획득할 스킬북은 패시브 스킬의 스킬북이므로, 패시브 스킬의 스킬 ID 로 대체함.
		const SkillId CurrentPassiveSkillId = SkillManager->FindPassiveSkillIdByUpgradeSkill(UserSkillSetId);
		if (INVALID_SKILL_ID != CurrentPassiveSkillId)
		{
			UserSkillSetId = CurrentPassiveSkillId;
		}
	}

	const UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager)
	{
		return;
	}

	const FText RichTextSkillId = SupplyManager->MakeRichTextSkillId(UserSkillSetId);
	if (!RichTextSkillId.IsEmpty())
	{
		// 획득처 정보 입력
		_infoAcquisition->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_infoAcquisition->SetDesc(RichTextSkillId);
	}
}

void UGsUISkillDetailInfoBase::SetInfoUITitle(const FTextKey& InTextKey, UGsUISkillDetailInfoEntry* TargetInfoUI)
{
	FText textTitle;
	FText::FindText(TEXT("SkillUIText"), InTextKey, textTitle);
	TargetInfoUI->SetTitle(textTitle);
}

void UGsUISkillDetailInfoBase::SetInfoUITitle(const FTextKey& InTextKey, UGsUISkillDetailInfoRichEntry* TargetInfoUI)
{
	FText textTitle;
	FText::FindText(TEXT("SkillUIText"), InTextKey, textTitle);
	TargetInfoUI->SetTitle(textTitle);
}

void UGsUISkillDetailInfoBase::GetIffApplyTypeText(IffApplyType InType, OUT FText& OutText)
{
	OutText = FText::GetEmpty();

	switch (InType)
	{
	case IffApplyType::ENEMY:
		// TEXT: 적
		FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Description_Target_Enemy"), OutText);
		break;
	case IffApplyType::PARTY:
		// TEXT: 파티원
		FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Description_Target_Party"), OutText);
		break;
	case IffApplyType::GUILD:
		// TEXT: 기사단원
		FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Description_Target_Guild"), OutText);
		break;
	}
}

const FGsSchemaProjectileSet* UGsUISkillDetailInfoBase::GetProjectileSet(const FGsSchemaSkillSet* InSkillSet) const
{
	if (nullptr == InSkillSet)
	{
		return nullptr;
	}

	for (const FGsSchemaSkillNotify& notify : InSkillSet->notifyInfoList)
	{
		if (const FGsSchemaSkillNotifySet* notifySet = notify.skillNotifyId.GetRow())
		{
			if (SkillNotifyType::FIRE_PROJECTILE == notifySet->type)
			{
				for (int i = 0; i < notifySet->notifyElementList.Num(); ++i)
				{
					if (const FGsSchemaSkillNotifyElement* element = notifySet->notifyElementList[i].GetRow())
					{
						if (const FGsSchemaSkillNotifyProjectile* projectileNotify = 
							element->typeInfo.GetRowByType<FGsSchemaSkillNotifyProjectile>())
						{
							return UGsTableUtil::FindRowById<UGsTableProjectileSet, FGsSchemaProjectileSet>(
								projectileNotify->projecttileId);
						}
					}
				}
			}
		}
	}

	return nullptr;
}
