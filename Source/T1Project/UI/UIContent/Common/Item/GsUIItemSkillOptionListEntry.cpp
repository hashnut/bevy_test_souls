// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIContent/Common/Item/GsUIItemSkillOptionListEntry.h"
#include "Components/RichTextBlock.h"

#include "T1Project.h"
#include "Management/ScopeGame/GsPassivityManager.h"

void UGsUIItemSkillOptionListEntry::NativeDestruct()
{
	FGsPassivityManager* passivityManager = GSPassivity();

	if (nullptr == passivityManager)
	{
		GSLOG(Error, TEXT("nullptr == passivityManager"));
		return;
	}

	passivityManager->UnRegisterHyperLinkDelegate(_skillOptionRichText, GetOuter()->GetOuter());

	Super::NativeDestruct();
}

void UGsUIItemSkillOptionListEntry::SetOptionText(FText InText)
{
	// 텍스트 채우기
	_skillOptionRichText->SetText(InText);
}

void UGsUIItemSkillOptionListEntry::SetOptionText(const FGsSchemaPassivitySet* passivitySet)
{
	FGsPassivityManager* passivityManager = GSPassivity();

	if (nullptr == passivityManager)
	{
		GSLOG(Error, TEXT("nullptr == passivityManager"));
		return;
	}

	FText skillText = passivityManager->MakeRichText(passivitySet);

	// 텍스트 채우기
	_skillOptionRichText->SetText(skillText);

	// 하이퍼링크 달기 (UGsUIItemDetailInfo 를 함께 보낸다)
	passivityManager->RegisterHyperLinkDelegate(_skillOptionRichText, GetOuter()->GetOuter());
}