// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupSkillCycleSetting.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Common/Numpad/GsUINumpadDefault.h"
#include "UI/UIContent/Popup/PartySetting/GsUIPartySettingTab.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Skill/GsSkill.h"
#include "UTIL/GsTableUtil.h"
#include "DataSchema/Config/GsSchemaBattleConfig.h"


void UGsUIPopupSkillCycleSetting::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupSkillCycleSetting::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupSkillCycleSetting::OnClickCancel);

	_numPadUI->OnChangeNumber().BindUObject(this, &UGsUIPopupSkillCycleSetting::OnChangeNumpadNumber);

	_toggleOnOff->OnToggleButtonClicked.BindUObject(this, &UGsUIPopupSkillCycleSetting::OnClickOnOff);
	_toggleEnableInPVP->OnToggleButtonClicked.BindUObject(this, &UGsUIPopupSkillCycleSetting::OnClickEnableInPVP);
}

void UGsUIPopupSkillCycleSetting::NativeConstruct()
{
	Super::NativeConstruct();

	_maxSkillCycleSecond = 1800;
	if (const UGsTable* battelTable = FGsSchemaBattleConfig::GetStaticTable())
	{
		TArray<const FGsSchemaBattleConfig*> battleRows;
		battelTable->GetAllRows(battleRows);

		if (battleRows.IsValidIndex(0))
		{
			_maxSkillCycleSecond = battleRows[0]->maxSkillIntervalSecond;
		}
	}	
}

void UGsUIPopupSkillCycleSetting::NativeDestruct()
{
	_weaponType = CreatureWeaponType::NONE;
	_rootSkillId = INVALID_SKILL_ID;
	_skillId = INVALID_SKILL_ID;
	_cycleSecond = 0;

	Super::NativeDestruct();
}

void UGsUIPopupSkillCycleSetting::OnInputCancel()
{
	Close();
}

void UGsUIPopupSkillCycleSetting::SetData(SkillId InRootSkillId, const FGsSkill* InActiveSkill)
{
	if (nullptr == InActiveSkill ||
		nullptr == InActiveSkill->_tableData ||
		nullptr == InActiveSkill->_userSkillSetData)
	{
		_rootSkillId = INVALID_SKILL_ID;
		_skillId = INVALID_SKILL_ID;
		_weaponType = CreatureWeaponType::NONE;

		return;
	}

	_rootSkillId = InRootSkillId;
	_skillId = InActiveSkill->GetSkillId();
	_weaponType = InActiveSkill->GetSkillRequireWeaponType();

	// 현재 상태 저장. 값이 달라진 경우에만 서버에 값을 보낸다
	_bEnableCyclePrev = InActiveSkill->IsActiveSkillCycle();
	_bEnableInPVPPrev = InActiveSkill->IsActiveSkillCycleInPVP();
	_cycleSecondPrev = InActiveSkill->GetCycleSecond();

	_tbSkillName->SetText(InActiveSkill->_userSkillSetData->nameText);
	_tbSkillName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InActiveSkill->_tableData->grade));

	SetOnOff(InActiveSkill->IsActiveSkillCycle());
	_toggleEnableInPVP->SetIsSelected(InActiveSkill->IsActiveSkillCycleInPVP());
	
	_numPadUI->SetMaxNumber(_maxSkillCycleSecond);
	_numPadUI->SetNumber(InActiveSkill->GetCycleSecond());
}

void UGsUIPopupSkillCycleSetting::SetOnOff(bool bIsOn)
{
	_toggleOnOff->SetIsSelected(bIsOn);

	FText textDesc;
	if (bIsOn)
	{
		// TEXT: 설정된 시간이 지난 후, 스킬을 자동 사용합니다... 최대시간 : {0}초
		FText textFormat;
		if (FText::FindText(TEXT("SkillUIText"), TEXT("SkillCycleSetting_Guide_ON"), textFormat))
		{
			textDesc = FText::Format(textFormat, _maxSkillCycleSecond);
		}
	}
	else
	{
		// TEXT: 즉시 스킬을 자동 사용합니다.
		FText::FindText(TEXT("SkillUIText"), TEXT("SkillCycleSetting_Guide_OFF"), textDesc);
	}

	_tbCycleDesc->SetText(textDesc);

	_toggleEnableInPVP->SetIsEnabled(bIsOn);
	_numPadUI->SetIsEnabled(bIsOn);
}

void UGsUIPopupSkillCycleSetting::OnClickOk()
{
	if (INVALID_SKILL_ID != _skillId &&
		CreatureWeaponType::NONE != _weaponType)
	{
		// 0초일 경우 bEnableCycle 옵션은 강제로 false
		bool bEnableCycle = _toggleOnOff->GetIsSelected();
		if (0 == _cycleSecond)
		{
			bEnableCycle = false;
		}

		bool bEnableInPVP = _toggleEnableInPVP->GetIsSelected();

		// 현재 값에서 달라졌을 경우에만 보내도록 처리
		if (_bEnableCyclePrev != bEnableCycle ||
			_bEnableInPVPPrev != bEnableInPVP ||
			_cycleSecondPrev != _cycleSecond)
		{
			FGsNetSendServiceWorld::SendRequestUseSkillCycle(_rootSkillId, _skillId,
				bEnableCycle, bEnableInPVP, _cycleSecond);
		}
	}

	Close();
}

void UGsUIPopupSkillCycleSetting::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupSkillCycleSetting::OnClickOnOff()
{
	// 토글을 위해 가져온 값을 반전하여 세팅
	bool bIsOn = !_toggleOnOff->GetIsSelected();
	_toggleOnOff->SetIsSelected(bIsOn);

	SetOnOff(bIsOn);
}

void UGsUIPopupSkillCycleSetting::OnClickEnableInPVP()
{
	// 토글을 위해 가져온 값을 반전하여 세팅
	bool bIsOn = !_toggleEnableInPVP->GetIsSelected();
	_toggleEnableInPVP->SetIsSelected(bIsOn);
}

void UGsUIPopupSkillCycleSetting::OnChangeNumpadNumber(int32 InNumber)
{
	// 저장
	_cycleSecond = FMath::Clamp(InNumber, 0, _maxSkillCycleSecond);
}
