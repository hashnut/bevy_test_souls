// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupAbnormalityDetailInfo.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconAbnormality.h"
#include "UI/UIContent/Common/Skill/GsUIPassivityDetailInfo.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoEntry.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Abnormality/GsAbnormalityDataDefine.h"


void UGsUIPopupAbnormalityDetailInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupAbnormalityDetailInfo::OnClickClose);
}

void UGsUIPopupAbnormalityDetailInfo::NativeConstruct()
{
	Super::NativeConstruct();

	// 스크롤 리셋
	_scrollBox->ScrollToStart();
}

void UGsUIPopupAbnormalityDetailInfo::NativeDestruct()
{
	_iconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUIPopupAbnormalityDetailInfo::OnInputCancel()
{
	Close();
}

void UGsUIPopupAbnormalityDetailInfo::SetAbnormalityData(AbnormalityId InAbnormalityId, int InLevel)
{
	_switcherInfo->SetActiveWidgetIndex(0);

	const FGsSchemaAbnormalitySet* tableData = nullptr;
	const UGsTableAbnormalitySet* table = Cast<UGsTableAbnormalitySet>(FGsSchemaAbnormalitySet::GetStaticTable());
	if (table)
	{
		if (false == table->FindRowById(InAbnormalityId, tableData))
		{
			return;
		}
	}
	else
	{
		return;
	}

	// 아이콘
	if (UGsUIIconAbnormality* icon = GetIcon())
	{
		icon->SetData(tableData);
	}

	// 이름
	_textBlockName->SetText(tableData->ingameName);
	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorNameTextAbnormality(tableData->isBuff));

	// 설명글
	if (tableData->ingameDesc.IsEmpty())
	{
		_infoDesc->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_infoDesc->SetVisibility(ESlateVisibility::HitTestInvisible);

		// TEXT: 설명
		FText textTitle;
		FText::FindText(TEXT("UICommonText"), TEXT("AbnormalTitle_desc"), textTitle);

		_infoDesc->SetTitle(textTitle);
		_infoDesc->SetDesc(tableData->ingameDesc);
	}

	// 버프/디버프 표시
	if (tableData->isBuff)
	{
		// TEXT: 버프
		FText::FindText(TEXT("AbnormalityText"), TEXT("CategoryBuff"), _textCategory);
	}
	else
	{
		// TEXT: 디버프
		FText::FindText(TEXT("AbnormalityText"), TEXT("CategoryDebuff"), _textCategory);
	}

	// 효과 스탯 텍스트 리스트 표시
	SetUITimeText(InAbnormalityId, InLevel);
	SetUIStatText(InAbnormalityId, InLevel);
}

void UGsUIPopupAbnormalityDetailInfo::SetPassivityData(const FGsPassivityData* InData)
{
	_switcherInfo->SetActiveWidgetIndex(1);

	// 아이콘
	if (UGsUIIconAbnormality* icon = GetIcon())
	{
		icon->SetData(InData);
	}

	// 이름
	_textBlockName->SetText(InData->GetNameText());

	// 환경 디버프를 다른 색으로 찍기 위한 로직
	PassivityOwnerType ownerType = InData->IsEnvironment() ? PassivityOwnerType::MAP : PassivityOwnerType::MAX;
	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorNameTextPassivity(InData->_data->isBuff, ownerType));

	// TEXT: 패시브
	FText::FindText(TEXT("UICommonText"), TEXT("AbnormalInfo_Passive"), _textCategory);

	// 패시브 전용 UI로 변경	
	_uiInfoPassivity->SetPassivityData(InData);
}

void UGsUIPopupAbnormalityDetailInfo::SetPassivitySet(const FGsSchemaPassivitySet* InSchema, PassivityOwnerType InOwnerType)
{
	_switcherInfo->SetActiveWidgetIndex(1);

	// 아이콘
	if (UGsUIIconAbnormality* icon = GetIcon())
	{
		icon->SetData(InSchema);
	}

	// 이름
	_textBlockName->SetText(InSchema->nameText);

	// 환경 디버프를 다른 색으로 찍기 위한 로직
	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorNameTextPassivity(InSchema->isBuff, InOwnerType));

	// TEXT: 패시브
	FText::FindText(TEXT("UICommonText"), TEXT("AbnormalInfo_Passive"), _textCategory);

	_uiInfoPassivity->SetPassivityData(InSchema);
}

void UGsUIPopupAbnormalityDetailInfo::SetUIStatText(AbnormalityId InAbnormalityId, int InLevel)
{
	// 스탯일 경우 시간정보 떼고 출력
	int32 effectTextOption = 0;
	FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_STAT_TIME, effectTextOption);

	FString strEffect;
	GSEffectText()->GetEffectTextAbnormality(InAbnormalityId, strEffect, InLevel, effectTextOption);

	if (strEffect.IsEmpty())
	{
		_infoStat->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		// TEXT: 효과
		FText textTitle;
		FText::FindText(TEXT("UICommonText"), TEXT("AbnormalTitle_EffectList"), textTitle);

		_infoStat->SetTitle(textTitle);
		_infoStat->SetDesc(FText::FromString(strEffect));

		_infoStat->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UGsUIPopupAbnormalityDetailInfo::SetUITimeText(AbnormalityId InAbnormalityId, int InLevel)
{
	FString strTime;
	if (GSEffectText()->GetStringAbnormalityTime(InAbnormalityId, strTime, InLevel))
	{
		// TEXT: 시간
		FText textTitle;
		FText::FindText(TEXT("TooltipText"), TEXT("Abnormality_durationTime"), textTitle);

		_infoTime->SetTitle(textTitle);
		_infoTime->SetDesc(FText::FromString(strTime));

		_infoTime->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		_infoTime->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UGsUIIconAbnormality* UGsUIPopupAbnormalityDetailInfo::GetIcon()
{
	// 아이콘 세팅
	if (0 == _iconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelector->CreateIcon();
		if (newIcon)
		{
			_iconSelector->AddChild(newIcon);
		}
	}

	// 아이콘을 하나만 만들기 때문에 0번째 아이콘 리턴
	return Cast<UGsUIIconAbnormality>(_iconSelector->GetChildAt(0));
}

void UGsUIPopupAbnormalityDetailInfo::OnClickClose()
{
	OnInputCancel();
}
