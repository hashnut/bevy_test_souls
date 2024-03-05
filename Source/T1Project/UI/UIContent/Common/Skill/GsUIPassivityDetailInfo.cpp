// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPassivityDetailInfo.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/Skill/GsUIPassivityDetailInfoListEntry.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoEntry.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "DataSchema/EffectText/GsSchemaEffectTextPassivity.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "GameObject/Abnormality/GsAbnormalityDataDefine.h"
#include "UTIL/GsTableUtil.h"
#include "../../Window/Skill/GsUISkillDetailInfoRichEntry.h"


void UGsUIPassivityDetailInfo::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPassivityDetailInfo::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPassivityDetailInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootInfo);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPassivityDetailInfo::OnRefreshEntry);
}

void UGsUIPassivityDetailInfo::NativeDestruct()
{
	_bIsCollection = false;
	_passivityData = nullptr;
	_effectTextData = nullptr;
	
	_strCollectionEffect.Empty();

	Super::NativeDestruct();
}

void UGsUIPassivityDetailInfo::SetPassivityData(PassivityId InPassivityId)
{
	const FGsSchemaPassivitySet* table = UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(InPassivityId);
	SetPassivityData(table);	
}

void UGsUIPassivityDetailInfo::SetPassivityData(const FGsSchemaPassivitySet* InPassivity)
{
	SetPassivityData(InPassivity, FText::GetEmpty());
}

void UGsUIPassivityDetailInfo::SetPassivityData(const FGsSchemaPassivitySet* InPassivity, const FText& InSkillSupply)
{
	_bIsCollection = false;
	_passivityData = InPassivity;
	_effectTextData = UGsTableUtil::FindRowById<UGsTableEffectTextPassivity, FGsSchemaEffectTextPassivity>(_passivityData->id);

	if (nullptr == _passivityData ||
		nullptr == _effectTextData)
	{
		SetDescInfo(FText::GetEmpty());
		SetSkillSupplyInfo(FText::GetEmpty());
		return;
	}

	SetDescInfo(_passivityData->descText);
	SetSkillSupplyInfo(InSkillSupply);

	_slotHelper->RefreshAll(_effectTextData->effectList.Num());
}

void UGsUIPassivityDetailInfo::SetPassivityData(const FGsPassivityData* InData)
{
	SetSkillSupplyInfo(FText::GetEmpty());

	// 콜렉션이 아닐 경우, 전용 UI로 표시
	if (nullptr == InData->_collectionData)
	{
		SetPassivityData(InData->_data);
		return;
	}

	_bIsCollection = true;
	_passivityData = nullptr;
	_effectTextData = nullptr;

	SetDescInfo(InData->GetDescText());

	// 컬렉션 텍스트 얻어오기
	FGsEffectStringHelper::GetCollectionEffectString(InData->_collectionData, _strCollectionEffect);

	_slotHelper->RefreshAll(1);
}

void UGsUIPassivityDetailInfo::SetDescInfo(const FText& InText)
{
	if (InText.IsEmpty())
	{
		_infoDesc->SetVisibility(ESlateVisibility::Collapsed);
	}

	FText textTitle;
	FText::FindText(TEXT("FairyText"), TEXT("Skill_Desc"), textTitle);

	_infoDesc->SetTitle(textTitle);
	_infoDesc->SetDesc(InText);

	_infoDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UGsUIPassivityDetailInfo::SetSkillSupplyInfo(const FText& InText)
{
	if (InText.IsEmpty())
	{
		_infoSupply->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	FText textTitle;
	FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Description_Supply"), textTitle);

	_infoSupply->SetTitle(textTitle);
	_infoSupply->SetDesc(InText);

	_infoSupply->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIPassivityDetailInfo::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (UGsUIPassivityDetailInfoListEntry* entry = Cast<UGsUIPassivityDetailInfoListEntry>(InEntry))
	{
		if (_bIsCollection)
		{
			entry->SetHideEmptyCondition(true);
			entry->SetData(_strCollectionEffect);
		}
		else
		{
			if (nullptr == _effectTextData ||
				false == _effectTextData->effectList.IsValidIndex(InIndex))
			{
				return;
			}

			// 첫 슬롯이고 조건이 하나뿐일 경우(조건이 비어있는지는 entry 내부에서 체크)
			if (0 == InIndex &&
				1 == _effectTextData->effectList.Num())
			{
				entry->SetHideEmptyCondition(true);
			}
			else
			{
				entry->SetHideEmptyCondition(false);
			}

			entry->SetData(_effectTextData->effectList[InIndex]);
		}
	}
}
