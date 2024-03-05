// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayEnchantItemProgress.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUITrayEnchantItemProgress::NativeConstruct()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Super::NativeConstruct();

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayEnchantItemProgress::OnClickBlock);
}

void UGsUITrayEnchantItemProgress::NativeDestruct()
{
	_confirmCallback = nullptr;
	_btnConfirm->OnClicked.RemoveDynamic(this, &UGsUITrayEnchantItemProgress::OnClickBlock);

	Super::NativeDestruct();
}

void UGsUITrayEnchantItemProgress::SetData(TFunction<void()> InConfirmCallback)
{
	_confirmCallback = InConfirmCallback;
}

void UGsUITrayEnchantItemProgress::FinishedAnim()
{
	if (nullptr != _confirmCallback)
	{
		_confirmCallback();
	}

	SetVisibility(ESlateVisibility::Collapsed);
	//Close();
}

void UGsUITrayEnchantItemProgress::OnClickBlock()
{
}