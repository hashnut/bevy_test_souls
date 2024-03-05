// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Skill/GsUITargetDetailInfoBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "GsUISkillDetailInfoBase.generated.h"

class FGsSkill;
class UScrollBox;
class UTextBlock;
class UGsUISkillDetailInfoEntry;
class FGsSkillUIData;
struct FGsSchemaProjectileSet;
struct FGsSchemaSkillSet;
class UGsUISkillDetailInfoRichEntry;

/**
 * 스킬 상세정보 Base클래스
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUISkillDetailInfoBase : public UGsUITargetDetailInfoBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	// 설명글
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;
	// 효과
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoEffect;
	// 대상
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoTarget;
	// 사거리
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoRange;
	// 소모
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoCost;
	// 쿨타임
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoCoolTime;
	// 천마석 사용 시 소모량
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoEntry* _infoSpiritShot;
	// 획득처
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISkillDetailInfoRichEntry* _infoAcquisition;

protected:
	const FGsSkill* _skill = nullptr;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void SetData(const FGsSkill* InData, int32 InOverrideLevel = 0) override;

protected:
	// 즉발형 스킬: 효과, 대상, 사거리, 소모, 쿨타임, 천마석, 습득처
	// 지속형 스킬: 효과, 대상, 사거리, 소모, 쿨타임, 천마석, 습득처
	// 패시브 스킬: 별도 UI(UGsUISkillDetailInfoPassivity)

	void SetEffectInfo(const FGsSkill* InData, int32 InSkillLevel);
	void SetTargetInfo(const FGsSkill* InData);
	void SetRangeInfo(const FGsSkill* InData);
	void SetCoolTimeInfo(const FGsSkill* InData, int32 InSkillLevel);
	void SetCostInfo(const FGsSkill* InData, int32 InSkillLevel);
	void SetSpiritShotInfo(const FGsSkill* InData);
	void SetAcquisitionInfo(const FGsSkill* InData);
	
	void SetInfoUITitle(const FTextKey& InTextKey, UGsUISkillDetailInfoEntry* TargetInfoUI);
	void SetInfoUITitle(const FTextKey& InTextKey, UGsUISkillDetailInfoRichEntry* TargetInfoUI);
	void GetIffApplyTypeText(IffApplyType InType, OUT FText& OutText);

	const FGsSchemaProjectileSet* GetProjectileSet(const FGsSchemaSkillSet* InSkillSet) const;
};
