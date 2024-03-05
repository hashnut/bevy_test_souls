// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISanctumBuffInfoBase.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUIGuildIcon.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Common/Entry/GsUISimpleTwoTextEntry.h"
#include "Management/ScopeGame/GsSanctumManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"


void UGsUISanctumBuffInfoBase::BeginDestroy()
{
	if (_slotHelperSanctumBuff)
	{
		_slotHelperSanctumBuff->OnRefreshEntry.RemoveDynamic(this, &UGsUISanctumBuffInfoBase::OnRefreshSanctumBuff);
	}
	_slotHelperSanctumBuff = nullptr;

	if (_slotHelperNexusBuff)
	{
		_slotHelperNexusBuff->OnRefreshEntry.RemoveDynamic(this, &UGsUISanctumBuffInfoBase::OnRefreshNexusBuff);
	}
	_slotHelperNexusBuff = nullptr;

	Super::BeginDestroy();
}

void UGsUISanctumBuffInfoBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnTipSanctumBuff->OnPressed.AddDynamic(this, &UGsUISanctumBuffInfoBase::OnPressTipSanctumBuff);
	_btnTipNexusBuff->OnPressed.AddDynamic(this, &UGsUISanctumBuffInfoBase::OnPressTipNexusBuff);

	_slotHelperSanctumBuff = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperSanctumBuff->Initialize(_entryWidgetClass, _panelSanctumBuffList);
	_slotHelperSanctumBuff->OnRefreshEntry.AddDynamic(this, &UGsUISanctumBuffInfoBase::OnRefreshSanctumBuff);

	_slotHelperNexusBuff = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperNexusBuff->Initialize(_entryWidgetClass, _panelNexusBuffList);
	_slotHelperNexusBuff->OnRefreshEntry.AddDynamic(this, &UGsUISanctumBuffInfoBase::OnRefreshNexusBuff);
	_slotHelperNexusBuff->SetEmptyListPanel(_panelEmptyNexusBuff);
}

void UGsUISanctumBuffInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	// 고정 텍스트라 열었을 때 한 번만 텍스트 세팅 진행

	// TEXT: 성소 점령 후 기사단원 모두에게 바로 적용되는 버프 효과
	FText textTipSanctumBuff;
	FText::FindText(TEXT("SanctumUIText"), TEXT("Info_Buff_ToolTip"), textTipSanctumBuff);
	_tooltipSanctumBuff->SetDescText(textTipSanctumBuff);

	// TEXT: 수호탑 설치 완료 후 기사단원 모두에게 바로 적용되는 버프 효과
	FText textTipNexusBuff;
	FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_ConstructionBuff"), textTipNexusBuff);
	_tooltipNexusBuff->SetDescText(textTipNexusBuff);
}

void UGsUISanctumBuffInfoBase::SetData(SanctumAreaId InSanctumId)
{
	TWeakPtr<FGsSanctumData> sanctumDataPtr = GSSanctum()->GetSanctumData(InSanctumId);
	if (false == sanctumDataPtr.IsValid())
	{
		return;
	}

	const FGsSanctumData* sanctumData = sanctumDataPtr.Pin().Get();
	if (nullptr == sanctumData)
	{
		return;
	}

	// 성소 이름
	_textBlockSanctumName->SetText(sanctumData->GetSanctumName());
	
	// 성소 버프 리스트 표시
	_sanctumBuffTextList.Empty();
	GetEffectTextList(sanctumData->GetSanctumPassivityId(), _sanctumBuffTextList);
	_slotHelperSanctumBuff->RefreshAll(_sanctumBuffTextList.Num());
	/*
	// 점령 기사단 정보 유무에 따른 처리
	_nexusBuffTextList.Empty();
	if (FGsSanctumData::EGsSanctumState::NONE != sanctumData->GetSanctumState())
	{
		GetEffectTextList(sanctumData->GetNexusPassivityId(sanctumData->GetGuildNexusLevel()), _nexusBuffTextList);
	}

	// 수호탑 버프 리스트 표시
	_slotHelperNexusBuff->RefreshAll(_nexusBuffTextList.Num());
	*/
}

void UGsUISanctumBuffInfoBase::OnPressTipSanctumBuff()
{
	_tooltipSanctumBuff->ToggleOpenClose();
}

void UGsUISanctumBuffInfoBase::OnPressTipNexusBuff()
{
	_tooltipNexusBuff->ToggleOpenClose();
}

void UGsUISanctumBuffInfoBase::OnRefreshSanctumBuff(int32 InIndex, UWidget* InEntry)
{
	if (_sanctumBuffTextList.IsValidIndex(InIndex))
	{
		if (UGsUISimpleTwoTextEntry* entry = Cast<UGsUISimpleTwoTextEntry>(InEntry))
		{
			entry->SetTextFront(_sanctumBuffTextList[InIndex].Key);
			entry->SetTextBack(_sanctumBuffTextList[InIndex].Value);
		}
	}
}

void UGsUISanctumBuffInfoBase::OnRefreshNexusBuff(int32 InIndex, UWidget* InEntry)
{
	if (_nexusBuffTextList.IsValidIndex(InIndex))
	{
		if (UGsUISimpleTwoTextEntry* entry = Cast<UGsUISimpleTwoTextEntry>(InEntry))
		{
			entry->SetTextFront(_nexusBuffTextList[InIndex].Key);
			entry->SetTextBack(_nexusBuffTextList[InIndex].Value);
		}
	}
}

void UGsUISanctumBuffInfoBase::SetNexusBuff(PassivityId InPassivityId)
{
	// 점령 기사단 정보 유무에 따른 처리
	_nexusBuffTextList.Empty();

	if (INVALID_PASSIVITY_ID != InPassivityId)
	{
		GetEffectTextList(InPassivityId, _nexusBuffTextList);
	}

	// 수호탑 버프 리스트 표시
	_slotHelperNexusBuff->RefreshAll(_nexusBuffTextList.Num());
}

void UGsUISanctumBuffInfoBase::GetEffectTextList(PassivityId InPassivityId,	OUT TArray<TPair<FText, FText>>& OutList)
{
	if (INVALID_PASSIVITY_ID != InPassivityId)
	{
		if (FGsEffectTextManager* effectTextMgr = GSEffectText())
		{
			int32 effectTextOption = 0;
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_STAT_TIME, effectTextOption);

			effectTextMgr->GetEffectTextListPassivity(InPassivityId, OutList, effectTextOption);
		}
	}
}
