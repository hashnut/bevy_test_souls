// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillDetailInfo.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Window/Skill/GsUISkillSlotEquipStep.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillUIData.h"
#include "Skill/GsSkillHelper.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UTIL/GsText.h"


void UGsUISkillDetailInfo::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnCreateEntry.RemoveDynamic(this, &UGsUISkillDetailInfo::OnCreateEntry);
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUISkillDetailInfo::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUISkillDetailInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnLearn->OnClicked.AddDynamic(this, &UGsUISkillDetailInfo::OnClickLearn);
	_btnEnchant->OnClicked.AddDynamic(this, &UGsUISkillDetailInfo::OnClickEnchant);
	_btnStartEquipMode->OnClicked.AddDynamic(this, &UGsUISkillDetailInfo::OnClickStartEquipMode);
	_btnCancelEquipMode->OnClicked.AddDynamic(this, &UGsUISkillDetailInfo::OnClickCancelEquipMode);
	_btnRemoveEquip->OnClicked.AddDynamic(this, &UGsUISkillDetailInfo::OnClickRemoveFromSlot);
	_btnCycleSetting->OnClicked.AddDynamic(this, &UGsUISkillDetailInfo::OnClickCycleSetting);

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_slotWidgetClass, _panelRootUpgradeStep);
	_slotHelper->OnCreateEntry.AddDynamic(this, &UGsUISkillDetailInfo::OnCreateEntry);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUISkillDetailInfo::OnRefreshEntry);

	_checkerButton.SetIntervalSecond(0.5f);
}

void UGsUISkillDetailInfo::NativeConstruct()
{
	Super::NativeConstruct();

	// TEXT: {0}단계
	FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_NStep"), _textFormatNStep);
}

void UGsUISkillDetailInfo::SetDataAll(FGsSkillUIData* InData)
{
	SetUpgradeStep(InData);

	if (InData)
	{	
		SetData(InData->GetSkill(), 0);

		if (InData->IsUpgradeSkill())
		{
			// 선택된 슬롯이 스크롤에 들어와 있도록 처리
			int32 index = InData->GetUpgradeStep();
			if (UWidget* slotWidget = _panelRootUpgradeStep->GetChildAt(index))
			{
				_scrollSlot->ScrollWidgetIntoView(slotWidget);
			}
			else
			{
				_scrollSlot->ScrollToStart();
			}
		}
	}
	else
	{
		SetData(nullptr, 0);

		_scrollSlot->ScrollToStart();
	}
}

void UGsUISkillDetailInfo::SetData(const FGsSkill* InData, int32 InOverrideLevel)
{
	Super::SetData(InData, InOverrideLevel);

	if (nullptr == InData)
	{	
		_panelRoot->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	_panelRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	SkillCategorySet categorySet = InData->GetSkillCateogrySet();
#if USE_SKILL_ENCHANT
	if (SkillCategorySet::PASSIVE == categorySet)
	{
		_panelRootLevelAndType->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_panelRootLevelAndType->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		int32 skillLevel = (0 < InOverrideLevel) ? InOverrideLevel : InData->GetTotalLevel();
		EGsUIColorType levelColorType = EGsUIColorType::SKILL_LEVEL_DEFAULT;

		if (0 < InData->GetBonusLevel())
		{
			levelColorType = EGsUIColorType::SKILL_LEVEL_ADD_BONUS;

			// 옵션레벨 추가 표기
			FString strBonusLevel = FString::Format(TEXT("(+{0})"), { InData->GetBonusLevel() });
			_textBlockBonusLevel->SetText(FText::FromString(strBonusLevel));
		}
		else
		{
			_textBlockBonusLevel->SetText(FText::GetEmpty());
		}

		FString strLevel = FString::Format(TEXT("Lv.{0}"), { skillLevel });
		_textBlockLevel->SetText(FText::FromString(strLevel));
		_textBlockLevel->SetColorAndOpacity(FGsUIColorHelper::GetColor(levelColorType));
	}
#else
	_panelRootLevelAndType->SetVisibility(ESlateVisibility::Collapsed);
#endif

	SelectUpgradeStep(InData->GetUpgradeStep()); // 0은 Root스킬이므로, 인덱스여도 -1을 하지 않음
	SetRequiredPassivity(InData);

	_scroll->ScrollToStart();

	bool bShowCycleSetting = false;
	
	if (_skill->_isLearn)
	{
		// 이미 배운 스킬
		if (SkillCategorySet::PASSIVE == categorySet)
		{
			_switcherEnchant->SetVisibility(ESlateVisibility::Collapsed);
			_switcherEquip->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			// B1 스킬 강화 버튼 막기(CHR-18926)
#if USE_SKILL_ENCHANT
			_switcherEnchant->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_switcherEnchant->SetActiveWidgetIndex(EGsSkillEnchantBtnState::ENCHANT);

			_btnEnchant->SetIsEnabled(InData->IsEnableEnchant());
#else
			_switcherEnchant->SetVisibility(ESlateVisibility::Collapsed);
#endif
			_switcherEquip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		// 액티브 스킬일 때만 스킬 주기 설정(버프 스킬은 적용하지 않음)
		if (SkillCategorySet::ACTIVE == categorySet)
		{
			// 업그레이드 스킬은 현재 단계 스킬만 설정 가능
			if (INVALID_SKILL_ID < _currUpgradeSkillId)
			{
				bShowCycleSetting = (_currUpgradeSkillId == _skill->GetSkillId()) ? true : false;
			}
			else
			{
				// 체인 스킬은 1단계만 설정 가능
				if (_skill->IsChainSkill())
				{
					if (1 >= _skill->GetChainStep())
					{
						bShowCycleSetting = true;
					}
				}
				else
				{
					bShowCycleSetting = true;
				}
			}
		}
	}
	else
	{
		_switcherEnchant->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_switcherEnchant->SetActiveWidgetIndex(EGsSkillEnchantBtnState::LEARN);	

		bool bCanLearn = FGsSkillHelper::CanLearn(_skill);
		_btnLearn->SetIsEnabled(bCanLearn);

		_switcherEquip->SetVisibility(ESlateVisibility::Collapsed);
	}

	_btnCycleSetting->SetVisibility(bShowCycleSetting ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUISkillDetailInfo::SetEquipMode(bool bIsEnableEquip, bool bIsEquipMode)
{
	if (nullptr == _skill ||
		false == bIsEnableEquip)
	{
		_switcherEquip->SetActiveWidgetIndex(EGsSkillEquipBtnState::EQUIP);
		_btnStartEquipMode->SetIsEnabled(false);

		return;
	}

	if (0 < _skill->GetSlotId())
	{
		// 장착중. 해제(2)
		_switcherEquip->SetActiveWidgetIndex(EGsSkillEquipBtnState::REMOVE);
	}
	else
	{
		if (bIsEquipMode)
		{
			// 장착모드 상태. 취소(1) 버튼 활성화
			_switcherEquip->SetActiveWidgetIndex(EGsSkillEquipBtnState::CANCEL);
		}
		else
		{
			// 장착모드가 아닌 상태. 장착(0) 버튼 활성화
			_switcherEquip->SetActiveWidgetIndex(EGsSkillEquipBtnState::EQUIP);
			_btnStartEquipMode->SetIsEnabled(true);
		}
	}
}

void UGsUISkillDetailInfo::OnCreateEntry(UWidget* InEntry)
{
	if (UGsUISkillSlotEquipStep* slot = Cast<UGsUISkillSlotEquipStep>(InEntry))
	{
		slot->OnClickSkillSlot.BindUObject(this, &UGsUISkillDetailInfo::OnClickSlotStep);
	}
}

void UGsUISkillDetailInfo::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _tempUpgradeSkillList.IsValidIndex(InIndex))
	{
		return;
	}

	if (false == _tempUpgradeSkillList[InIndex].IsValid())
	{
		return;
	}

	UGsUISkillSlotEquipStep* slot = Cast<UGsUISkillSlotEquipStep>(InEntry);
	if (nullptr == slot)
	{
		return;
	}

	slot->SetData(_tempUpgradeSkillList[InIndex].Pin().Get());

	// 마지막일 경우 화살표 감춤
	if (InIndex == (_tempUpgradeSkillList.Num() - 1))
	{
		slot->SetIsLastSlot(true);
	}
	else
	{
		slot->SetIsLastSlot(false);
	}
}

void UGsUISkillDetailInfo::OnClickLearn()
{
	// 광클방지
	if (_checkerButton.IsTimeOver())
	{
		OnClickEnchantOrLearn.ExecuteIfBound(false);

		_checkerButton.UpdateTime();
	}
}

void UGsUISkillDetailInfo::OnClickEnchant()
{
	// 광클방지
	if (_checkerButton.IsTimeOver())
	{
		OnClickEnchantOrLearn.ExecuteIfBound(true);

		_checkerButton.UpdateTime();
	}
}

void UGsUISkillDetailInfo::OnClickStartEquipMode()
{
	OnClickEquipOrCancel.ExecuteIfBound(true);
}

void UGsUISkillDetailInfo::OnClickCancelEquipMode()
{
	OnClickEquipOrCancel.ExecuteIfBound(false);
}

void UGsUISkillDetailInfo::OnClickRemoveFromSlot()
{
	OnClickRemoveEquip.ExecuteIfBound();
}

void UGsUISkillDetailInfo::OnClickCycleSetting()
{
	OnClickSkillCycleSetting.ExecuteIfBound();
}

void UGsUISkillDetailInfo::OnClickSlotStep(UGsUISkillSlotEquipStep* InSlot)
{
	if (const FGsSkill* skill = InSlot->GetSkill())
	{
		OnClickUpgradeStep.ExecuteIfBound(skill->GetUpgradeStep());
	}
	else
	{
		OnClickUpgradeStep.ExecuteIfBound(0);
	}
}

void UGsUISkillDetailInfo::SetUpgradeStep(const FGsSkillUIData* InData)
{	
	if (nullptr == InData ||
		0 == InData->_upgradeList.Num())
	{
		_currUpgradeSkillId = INVALID_SKILL_ID;
		_scrollSlot->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_currUpgradeSkillId = InData->GetSkillId();
	_scrollSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	_tempUpgradeSkillList.Empty();
	_tempUpgradeSkillList.Append(InData->_upgradeList);

	_slotHelper->RefreshAll(InData->_upgradeList.Num());
	
	_tempUpgradeSkillList.Empty();
}

void UGsUISkillDetailInfo::SelectUpgradeStep(uint8 InIndex)
{
	int32 maxCount = _slotHelper->GetItemCount();

	for (int i = maxCount - 1; i >= 0; --i)
	{
		UWidget* child = _panelRootUpgradeStep->GetChildAt(i);
		if (ESlateVisibility::Collapsed == child->GetVisibility())
		{
			continue;
		}

		if (UGsUISkillSlotEquipStep* slot = Cast<UGsUISkillSlotEquipStep>(child))
		{
			// 선택된 슬롯에 이펙트 처리
			slot->SetIsSelected((i == InIndex) ? true : false);

			bool bIsCurrent = false;
			if (const FGsSkill* skill = slot->GetSkill())
			{
				if (skill->_isLearn)
				{
					if (_currUpgradeSkillId == skill->GetSkillId())
					{
						bIsCurrent = true;
					}
				}
			}

			slot->SetIsLearned(bIsCurrent);
		}
	}
}

void UGsUISkillDetailInfo::SetRequiredPassivity(const FGsSkill* InData)
{
	if (InData && InData->_tableData)

	{
		const FGsSchemaPassivitySet* passivitySet =
			FGsSkillHelper::GetUpgradeSkillRequirePassivity(InData->GetCurrLevelSet());
		if (passivitySet)
		{
			// TEXT: 패시브 {0}를 보유하여 업그레이드된 스킬입니다.
			FText textFormat;
			if (FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Description_RequirePassivity"), textFormat))
			{
				// 이름 컬러를 위해 태그 씌우기
				FText textName;
				SetGradeTag(InData->_tableData->grade, passivitySet->nameText, textName);

				FText textMsg = FText::Format(textFormat, textName);
				_rbRequiredPassivity->SetVisibility(ESlateVisibility::HitTestInvisible);
				_rbRequiredPassivity->SetText(textMsg);

				return;
			}
		}
	}

	_rbRequiredPassivity->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISkillDetailInfo::SetGradeTag(ItemGrade InGrade, const FText& InTargetText, OUT FText& OutText)
{
	FString strGrade = FGsTextUtil::GetStringFromEnum<ItemGrade>(TEXT("ItemGrade"), InGrade);
	FString strTaged = FString::Format(TEXT("<{0}>{1}</>"), { *strGrade, *(InTargetText.ToString()) });

	OutText = FText::FromString(strTaged);
}
