// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupSkillEnchant.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "PaperSprite.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Common/Skill/GsUISkillIcon.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Popup/Skill/GsUISkillEnchantIngredientListEntry.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Skill/GsSchemaSkillEnchantSet.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelInfo.h"
#include "DataSchema/Skill/Level/GsSchemaSkillCostConsumeInfo.h"
#include "DataSchema/Skill/Cost/GsSchemaSkillUpCostSet.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillHelper.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Currency/GsCurrencyHelper.h"
#include "T1Project.h"


void UGsUIPopupSkillEnchant::BeginDestroy()
{
	if (_slotHelperIngredients)
	{
		_slotHelperIngredients->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupSkillEnchant::OnRefreshEntry);
	}
	_slotHelperIngredients = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupSkillEnchant::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_enchantBtnUI->OnClickCurrencyButton.BindUObject(this, &UGsUIPopupSkillEnchant::OnClickEnchant);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupSkillEnchant::OnClickCancel);
	_btnProbability->OnClicked.AddDynamic(this, &UGsUIPopupSkillEnchant::OnClickProbability);

	_slotHelperIngredients = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperIngredients->Initialize(_entryWidgetClass, _panelIngredients);
	_slotHelperIngredients->OnRefreshEntry.AddDynamic(this, &UGsUIPopupSkillEnchant::OnRefreshEntry);
}

void UGsUIPopupSkillEnchant::NativeDestruct()
{
	_skillId = INVALID_SKILL_ID;
	_costTable = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupSkillEnchant::InvalidateAllInternal()
{
	if (const FGsSkill* skill = GSkill()->FindSkill(_skillId, CreatureWeaponType::MAX, true))
	{
		InvalidateUI(skill);
	}
}

void UGsUIPopupSkillEnchant::OnInputOk()
{
	if (false == CheckEnableEnchant(true))
	{
		return;
	}

	FText confirmText;
	// TEXT: 강화 실패시 재료 아이템과 재화는 사라집니다. 강화하시겠습니까?
	FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Notice_EnchantAttempt"), confirmText);

	// 확인 팝업 띄우기
	FGsUIHelper::PopupYesNo(confirmText, [this](bool bInYes)
		{
			if (bInYes)
			{
				// 강화 연출 시작 요청
				FGsUIMsgParamEnchantSkill param(_skillId);
				GMessage()->GetUI().SendMessage(MessageUI::SKILL_ENCHANT_EFFECT_START, &param);
			}
		});
}

void UGsUIPopupSkillEnchant::OnInputCancel()
{
	Close();
}

void UGsUIPopupSkillEnchant::SetData(const FGsSkill* InData)
{
	_skillId = InData->GetSkillId();

	InvalidateUI(InData);
}

void UGsUIPopupSkillEnchant::InvalidateUI(const FGsSkill* InData)
{
	_costTable = nullptr;

	int32 currLevel = InData->GetLevel();
	int32 nextLevel = (currLevel < InData->GetMaxEnchantLevel()) ?
		currLevel + 1 : currLevel;
	
	// 더이상 강화 불가 시 팝업 닫기(최대 레벨 도달 시)
	if (currLevel == nextLevel)
	{
		Close();
		return;
	}

	const FGsSkill* nextSkill = InData;

	if (nullptr == _costTable)
	{
		const FGsSkill* rootSkill = nullptr;
		TWeakPtr<const FGsSkill> rootSkillPtr = FGsSkillHelper::GetUpgradeRootSkill(InData);
		if (rootSkillPtr.IsValid())
		{
			rootSkill = rootSkillPtr.Pin().Get();
		}
		else
		{
			rootSkill = InData;
		}

		if (const FGsSchemaSkillLevelInfo* levelInfo = rootSkill->GetLevelInfo(currLevel))
		{
			_costTable = FGsSkillHelper::GetSkillUpCostSet(levelInfo->enchantSkillUpCostId);
		}
	}
	
	// 현재 스킬 정보 표시
	SetUISkillDefaultInfo(InData, currLevel, _uiSkillIcon, _textBlockLevel, _textBlockName);

	// 다음 스킬 정보 표시
	SetUISkillDefaultInfo(nextSkill, nextLevel, _uiSkillIconNext, _textBlockLevelNext, _textBlockNameNext);

	// 효과 텍스트 표시
	SetUIEffectList(InData->GetSkillId(), InData->GetLevelInfo(currLevel),
		nextSkill->GetSkillId(), nextSkill->GetLevelInfo(nextLevel));

	// 소모 재료 표시
	CurrencyType costCurrencyType = CurrencyType::NONE;
	Currency costAmount = 0;
	if (_costTable)
	{
		// 아이템 표시
		_slotHelperIngredients->RefreshAll(_costTable->itemList.Num());

		// 기획팀에서 첫번째 것만 사용하겠다고 답변 받음. 추후 확장 성 위해 리스트는 남겨둠.
		if (0 < _costTable->currencyList.Num())
		{
			costCurrencyType = _costTable->currencyList[0].currencyType;
			costAmount = _costTable->currencyList[0].currencyAmount;
		}
	}

	_scrollIngredients->ScrollToStart();

	// 소모 골드 표시
	Currency currGold = FGsCurrencyHelper::GetCurrencyAmount(costCurrencyType);
	_enchantBtnUI->SetData(costCurrencyType, costAmount);
	_enchantBtnUI->UpdateAmountTextColorLocalPlayer();
	
	// TEXT: 스킬 강화
	FText textTitle;
	FText::FindText(TEXT("SkillUIText"), TEXT("Skill_Enchant_Title"), textTitle);
	// TEXT: 강화하기
	FText textButton;
	FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Button_Enchant"), textButton);

	_enchantBtnUI->SetIsEnabled(InData->IsEnableEnchant());
	
	_textBlockTitle->SetText(textTitle);
	_enchantBtnUI->SetButtonName(textButton);
}

void UGsUIPopupSkillEnchant::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (nullptr == _costTable ||
		false == _costTable->itemList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUISkillEnchantIngredientListEntry* entry = Cast<UGsUISkillEnchantIngredientListEntry>(InEntry))
	{
		entry->SetData(_costTable->itemList[InIndex].itemId, _costTable->itemList[InIndex].itemCount);
	}
}

void UGsUIPopupSkillEnchant::OnClickEnchant()
{
	OnInputOk();
}

void UGsUIPopupSkillEnchant::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupSkillEnchant::OnClickProbability()
{
	// 확률 정보 표시
	FGsUIHelper::OpenHelpPage(EGsHelpContents::Skill);
}

bool UGsUIPopupSkillEnchant::CheckEnableEnchant(bool bInShowTicker)
{
	// 정보없을 경우
	if (INVALID_SKILL_ID == _skillId)
	{
		GSLOG(Error, TEXT("[SkillEnchant] Invalid Skill Id"));
		return false;
	}

	if (nullptr == _costTable)
	{
		GSLOG(Error, TEXT("[SkillEnchant] _costTable is null"));
		return false;
	}
	
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
		GSLOG(Error, TEXT("[SkillEnchant] itemMgr is null"));
		return false;
	}

	// 재화 체크
	if (const FGsSchemaSkillUpCostSet* costSet = FGsSkillHelper::GetSkillUpCostSet(_costTable->id))
	{
		for (const FGsSchemaSkillUpCurrencyInfo& skillUpCurrency : costSet->currencyList)
		{
			if (false == FGsCurrencyHelper::CheckCurrency(skillUpCurrency.currencyType, skillUpCurrency.currencyAmount, false))
			{
				Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(skillUpCurrency.currencyType);

				FGsCurrencyHelper::OpenLackCurrencyPopup(skillUpCurrency.currencyType,
					skillUpCurrency.currencyAmount - myAmount);

				return false;
			}
		}
	}

	// 재료 아이템 체크
	if (false == FGsSkillHelper::CheckSkillUpCost(_costTable->id, false, true))
	{
		return false;
	}
	
	return true;
}

void UGsUIPopupSkillEnchant::SetUISkillDefaultInfo(const FGsSkill* InSkill, int32 InLevel,
	OUT UGsUISkillIcon* OutIcon, OUT UTextBlock* OutTbLevel, OUT UTextBlock* OutTbName)
{	
	if (nullptr == InSkill ||
		nullptr == InSkill->_userSkillSetData ||
		nullptr == InSkill->_tableData)
	{
		return;
	}

	OutIcon->SetData(InSkill);

	// 스킬 정보 표시
	OutTbLevel->SetText(FText::AsNumber(InLevel));

	OutTbName->SetText(InSkill->_userSkillSetData->nameText);
	OutTbName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InSkill->_tableData->grade));
}

void UGsUIPopupSkillEnchant::SetUIEffectList(SkillId InSkillId, const FGsSchemaSkillLevelInfo* InLevelInfo,
	SkillId InSkillIdNext, const FGsSchemaSkillLevelInfo* InLevelInfoNext)
{
	if (nullptr == InLevelInfo)
	{
		return;
	}

	// 현재 효과 텍스트 얻기
	FString strCurrEffect;
	GSEffectText()->GetEffectTextSkill(InSkillId, InLevelInfo->level, strCurrEffect);
	
	FString strNextEffect;
	if (InLevelInfoNext)
	{
		GSEffectText()->GetEffectTextSkill(InSkillIdNext, InLevelInfoNext->level, strNextEffect);

		// 쿨타임이 다르면 추가 표시
		if (InLevelInfo->cooldownTime != InLevelInfoNext->cooldownTime)
		{
			AppendCoolTime(InLevelInfo->cooldownTime, strCurrEffect);
			AppendCoolTime(InLevelInfoNext->cooldownTime, strNextEffect);
		}

		// 소모(MP등) 정보가 다르면 추가 표시
		if (false == IsSameConsumeInfo(InLevelInfo, InLevelInfoNext))
		{
			AppendConsumeInfo(InLevelInfo, strCurrEffect);
			AppendConsumeInfo(InLevelInfoNext, strNextEffect);
		}
	}
	else
	{
		// 다음 정보가 없으면 그냥 현재 정보랑 동일하게 찍는다
		strNextEffect = strCurrEffect;
	}	
	
	_textBlockEffect->SetText(FText::FromString(strCurrEffect));
	_textBlockEffectNext->SetText(FText::FromString(strNextEffect));
	_scrollBoxEffect->ScrollToStart();
}

void UGsUIPopupSkillEnchant::AppendCoolTime(float InCoolTime, OUT FString& OutData)
{
	// TEXT: 쿨타임
	FText textCoolTime;
	FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Description_Cooltime"), textCoolTime);

	FString strTime = FGsTimeStringHelper::GetSecondString(InCoolTime);

	// 이미 내용이 있으면 줄바꿈
	if (false == OutData.IsEmpty())
	{
		OutData.Append(TEXT("\n"));
	}
	OutData.Append(textCoolTime.ToString());
	OutData.Append(TEXT(" "));
	OutData.Append(strTime);
}

void UGsUIPopupSkillEnchant::AppendConsumeInfo(const FGsSchemaSkillLevelInfo* InInfo, OUT FString& OutData)
{
	const FGsSchemaSkillCostConsumeInfo& costInfo = InInfo->costConsumeInfo;
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

	// TEXT: 소모
	FText textCost;
	FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Description_Consume"), textCost);

	// 이미 내용이 있으면 줄바꿈
	if (false == OutData.IsEmpty())
	{
		OutData.Append(TEXT("\n"));
	}
	OutData.Append(textCost.ToString());
	OutData.Append(TEXT(" "));
	OutData.Append(strCost);
}

bool UGsUIPopupSkillEnchant::IsSameConsumeInfo(const FGsSchemaSkillLevelInfo* InCurr, 
	const FGsSchemaSkillLevelInfo* InNext)
{
	if (InCurr->costConsumeInfo.requireItemList.Num() != InNext->costConsumeInfo.requireItemList.Num())
	{
		return false;
	}

	int32 sameItemCount = 0;
	for (const FGsSchemaSkillCostItemInfo& iterCurr : InCurr->costConsumeInfo.requireItemList)
	{
		for (const FGsSchemaSkillCostItemInfo& iterNext : InNext->costConsumeInfo.requireItemList)
		{
			if (iterCurr.itemId == iterNext.itemId)
			{
				++sameItemCount;
				if (iterCurr.itemAmount != iterNext.itemAmount)
				{
					return false;
				}
			}
		}
	}

	if (sameItemCount != InNext->costConsumeInfo.requireItemList.Num())
	{
		return false;
	}

	int32 sameStatCount = 0;
	for (const FGsSchemaSkillStatInfo& iterCurr : InCurr->costConsumeInfo.statInfoList)
	{
		for (const FGsSchemaSkillStatInfo& iterNext : InNext->costConsumeInfo.statInfoList)
		{
			if (iterCurr.type == iterNext.type)
			{
				++sameStatCount;
				if (iterCurr.value != iterNext.value)
				{
					return false;
				}
			}
		}
	}

	if (sameStatCount != InNext->costConsumeInfo.statInfoList.Num())
	{
		return false;
	}

	return true;
}
