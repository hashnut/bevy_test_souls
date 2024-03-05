// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildMemberGradeEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Guild/GsGuildHelper.h"


void UGsUIGuildMemberGradeEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIGuildMemberGradeEntry::OnClickSlot);
}

void UGsUIGuildMemberGradeEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_imgSelected->SetVisibility(_bIsSelected ? 
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIGuildMemberGradeEntry::SetData(GuildMemberGradeType InGrade, int32 InMaxCount, int32 InCurrCount)
{
	FText textGrade;
	FGsGuildHelper::GetMemberGradeText(InGrade, textGrade);
	_textBlockGrade->SetText(textGrade);

	FString strCount = FString::Format(TEXT("{0}/{1}"), { InCurrCount, InMaxCount });
	_textBlockCount->SetText(FText::FromString(strCount));	
}

void UGsUIGuildMemberGradeEntry::OnClickSlot()
{
	// ToggleGroup에 콜백 전달. 스왑을 위해 현재 상태에서 바꿔서 전달
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
