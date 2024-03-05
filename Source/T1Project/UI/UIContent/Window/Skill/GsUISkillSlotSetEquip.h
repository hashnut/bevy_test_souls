// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Skill/GsSkill.h"
#include "GsUISkillSlotSetEquip.generated.h"

class FGsSkill;
class UGsUISkillSlotEquip;
class UGsButton;
class UGsUITooltipDesc;

/**
 * 스킬 편집창의 프리셋
 */
UCLASS()
class T1PROJECT_API UGsUISkillSlotSetEquip : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, UGsUISkillSlotEquip*);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSlotSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRemoveAll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipInfo;

	// 이름으로 찾아둘까. 아니면 고정 갯수 바인딩 시켜놓을까. 
	UPROPERTY()
	TArray<UGsUISkillSlotEquip*> _slotList;

protected:
	bool _bEquipMode = false;
	bool _bIsCostume = false;
	CreatureWeaponType _weaponType = CreatureWeaponType::MAX;

public:
	FOnClickSlot OnClickSkillSlot;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void SetData(CreatureWeaponType InType, bool bInForced = false);
	void SetSkillCategory(int32 InIndex);
	void SetEnableEquip(bool bEnable, bool bIsEquipMode, bool bIsCostume, const FGsSkill* InSelectedSkill);

	void InvalidateSlotSet();
	void InvalidateSlotBySlotId(SkillSlotId InSlotId);

	const FGsSkill* RecursiveFindFirstChainSkill(const FGsSkill* InSkill) const;

protected:
	UFUNCTION()
	void OnClickSlot(UGsUISkillSlotEquip* InSlot);

	UFUNCTION()
	void OnClickRemoveAll();
	UFUNCTION()
	void OnPressInfo();

public:	
	UGsUISkillSlotEquip* FindSlotById(SkillSlotId InSlotId) const;
	const FGsSkill* GetSkill(SkillSlotId InSlotId) const;

protected:
	bool IsAllEmpty() const;
};
