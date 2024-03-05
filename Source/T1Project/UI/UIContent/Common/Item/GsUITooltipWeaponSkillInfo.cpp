// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Common/Item/GsUITooltipWeaponSkillInfo.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"

void UGsUITooltipWeaponSkillInfo::NativeConstruct()
{
	Super::NativeConstruct();
	
	BindMessage();
}

void UGsUITooltipWeaponSkillInfo::NativeDestruct()
{
	UnbindMessage();

	Super::NativeDestruct();
}

void UGsUITooltipWeaponSkillInfo::BindMessage()
{
	// 이벤트 등록
	FGsMessageHolder* message = GMessage();

	if (nullptr == message)
	{
		GSLOG(Error, TEXT("nullptr == message"));
		return;
	}

	// 툴팁 띄우는 이벤트 바인딩
	_listItemDelegate.Emplace(
		message->GetItemContentSkill().AddUObject(MessageContentItemSkill::DETAIL_OPEN_ITEM_SKILL, this, &UGsUITooltipWeaponSkillInfo::AckClickedHyperLink));
}

void UGsUITooltipWeaponSkillInfo::UnbindMessage()
{
	// 이벤트 해지
	FGsMessageHolder* message = GMessage();

	if (nullptr == message)
	{
		GSLOG(Error, TEXT("nullptr == message"));
		return;
	}

	if (_listItemDelegate.Num() != 0)
	{
		for (auto iter : _listItemDelegate)
		{
			message->GetItemContentSkill().Remove(iter);
		}

		_listItemDelegate.Empty();
	}
}

void UGsUITooltipWeaponSkillInfo::SetData(FText&& InTitleText, FText&& InDescText, bool InIsPassive)
{
	_titleText->SetText(MoveTemp(InTitleText));
	_descText->SetText(MoveTemp(InDescText));

	if (InIsPassive)
	{
		_passiveText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		_passiveText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUITooltipWeaponSkillInfo::SetTitleColor(const FLinearColor& InColor)
{
	_titleText->SetColorAndOpacity(InColor);
}

void UGsUITooltipWeaponSkillInfo::AckClickedHyperLink(const IGsMessageParam* InParam)
{
	const FGsItemDetailSkillInfoMessage* itemDetailSkillInfo = InParam->Cast<const FGsItemDetailSkillInfoMessage>();

	if (nullptr == itemDetailSkillInfo)
	{
		GSLOG(Error, TEXT("nullptr == itemDetailSkillInfo"));
		return;
	}

	auto panel = GetOuter();

	if (nullptr == panel)
	{
		GSLOG(Error, TEXT("nullptr == panel"));
		return;
	}

	auto owner = panel->GetOuter();

	if (nullptr == owner)
	{
		GSLOG(Error, TEXT("nullptr == owner"));
		return;
	}

	if (owner->GetUniqueID() != itemDetailSkillInfo->_uniqueId)
	{
		GSLOG(Error, TEXT("owner->GetUniqueID() != itemDetailSkillInfo->_uniqueId"));
		return;
	}

	SetData(Forward<FText>(FText::FromString(itemDetailSkillInfo->_titleString)),
		Forward<FText>(FText::FromString(itemDetailSkillInfo->_descString)),
		itemDetailSkillInfo->_isPassive);

	SetTitleColor(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_VALID));

	Super::Open();
}