// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISanctumBuffInfoRegionMap.h"
#include "Components/TextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUIGuildIcon.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Common/Entry/GsUISimpleTwoTextEntry.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsSanctumManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"


void UGsUISanctumBuffInfoRegionMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTipRequiredNexusLevel->OnPressed.AddDynamic(this, &UGsUISanctumBuffInfoRegionMap::OnPressTipRequiredNexusLevel);
}

void UGsUISanctumBuffInfoRegionMap::NativeConstruct()
{
	Super::NativeConstruct();

	// 고정 텍스트라 열었을 때 한 번만 텍스트 세팅 진행
	
	// TEXT: 해당 성소 점령에 필요한 수호탑 최소 레벨입니다. 성소는 도잇에 최대 한 개만 점령할 수 있습니다.
	FText textTipRequiredNexusLevel;
	FText::FindText(TEXT("SanctumUIText"), TEXT("Info_Occupy_NeedLevel_Tooltip"), textTipRequiredNexusLevel);
	_tooltipRequiredNexusLevel->SetDescText(textTipRequiredNexusLevel);	

	InitializeMessage();
}
void UGsUISanctumBuffInfoRegionMap::NativeDestruct()
{
	FinalizeMessage();
	Super::NativeDestruct();
}

void UGsUISanctumBuffInfoRegionMap::SetData(SanctumAreaId InSanctumId)
{
	Super::SetData(InSanctumId);

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

	// 요구 레벨 표시
	int32 requiredNexusLevel = sanctumData->GetRequiredNexusLevel();
	SetUIRequiredNexusLevel(requiredNexusLevel);

	// 점령 기사단 정보 유무에 따른 처리
	PassivityId nexusPassivityId = INVALID_PASSIVITY_ID;
	if (false== sanctumData->IsOccupied())
	{
		_textBlockGuildName->SetText(FText::FromString(TEXT("-")));
		_iconGuild->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_textBlockGuildName->SetText(FText::FromString(sanctumData->_guildName));
		_iconGuild->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_iconGuild->SetEmblemId(sanctumData->_guildEmblemId, false);

		nexusPassivityId = sanctumData->GetNexusPassivityId(sanctumData->GetGuildNexusLevel());
	}

	SetNexusBuff(nexusPassivityId);

	_sanctumAreaId = InSanctumId;
}

void UGsUISanctumBuffInfoRegionMap::OnPressTipRequiredNexusLevel()
{
	_tooltipRequiredNexusLevel->ToggleOpenClose();
}

void UGsUISanctumBuffInfoRegionMap::SetUIRequiredNexusLevel(int32 InLevel)
{
	FText textRequiredLevel;
	if (0 < InLevel)
	{
		// TEXT: 수호탑 Lv.{0}
		FText textFormat;
		if (FText::FindText(TEXT("SanctumUIText"), TEXT("Info_Occupy_NeedLevel"), textFormat))
		{
			textRequiredLevel = FText::Format(textFormat, InLevel);
		}
	}

	if (textRequiredLevel.IsEmpty())
	{
		_textBlockRequiredNexusLevel->SetText(FText::FromString(TEXT("-")));
	}
	else
	{
		_textBlockRequiredNexusLevel->SetText(textRequiredLevel);
	}
}
// 선택 된게 있는지
void UGsUISanctumBuffInfoRegionMap::SetSelectInfo(bool In_isSelected)
{
	// 0: 선택된게 있음
	// 1: 선택된게 없음
	_switcherInfoType->SetActiveWidgetIndex((In_isSelected == true)? 0: 1);
}

void UGsUISanctumBuffInfoRegionMap::InitializeMessage()
{
	FGsMessageHolder* msg = GMessage();

	if (msg == nullptr)
	{
		return;
	}
	MUI& msgUI = msg->GetUI();
	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::UPDATED_SANCTUM_INFO,
		this, &UGsUISanctumBuffInfoRegionMap::OnUpdatedSanctumInfoUpdated));
}
void UGsUISanctumBuffInfoRegionMap::FinalizeMessage()
{
	FGsMessageHolder* msg = GMessage();

	if (msg == nullptr)
	{
		return;
	}
	if (_msgUIHandlerList.Num() != 0)
	{
		for (auto iter : _msgUIHandlerList)
		{
			msg->GetUI().Remove(iter);
		}
		_msgUIHandlerList.Empty();
	}
}

// 성소 정보 갱신됨
void UGsUISanctumBuffInfoRegionMap::OnUpdatedSanctumInfoUpdated(const IGsMessageParam*)
{
	SetData(_sanctumAreaId);
}