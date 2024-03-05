// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsUIPopupSkillCycleSetting.generated.h"

class UTextBlock;
class UGsButton;
class UGsSwitcherButton;
class UGsUIPartySettingTab;
class UGsUINumpadDefault;
class FGsSkill;

/**
 * 스킬 자동 사용 주기 설정
 */
UCLASS()
class T1PROJECT_API UGsUIPopupSkillCycleSetting : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbSkillName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbCycleDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleOnOff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartySettingTab* _toggleEnableInPVP;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUINumpadDefault* _numPadUI;

private:
	CreatureWeaponType _weaponType = CreatureWeaponType::NONE;
	SkillId _rootSkillId = INVALID_SKILL_ID;
	SkillId _skillId = INVALID_SKILL_ID;
	int32 _cycleSecond = 0;

	// 값이 다를 때만 보내는 것을 체크하기 위함
	bool _bEnableCyclePrev = false;
	bool _bEnableInPVPPrev = false;
	int32 _cycleSecondPrev = 0;

	int32 _maxSkillCycleSecond = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void OnInputCancel() override;

public:
	void SetData(SkillId InRootSkillId, const FGsSkill* InActiveSkill);
	void SetOnOff(bool bIsOn);

protected:
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();

	void OnClickOnOff();
	void OnClickEnableInPVP();

	void OnChangeNumpadNumber(int32 InNumber);
};
