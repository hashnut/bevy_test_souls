// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupDownload.h"
#include "RichTextBlock.h"
#include "TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "../Helper/GsUIHelper.h"
#include "../Helper/GsUIStringHelper.h"

void UGsUIPopupDownload::NativeConstruct()
{
	Super::NativeConstruct();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupDownload::OnClickCancel);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupDownload::OnClickOk);
}

void UGsUIPopupDownload::NativeDestruct()
{
	_buttonCallback = nullptr;	
	_btnCancel->OnClicked.RemoveDynamic(this, &UGsUIPopupDownload::OnClickCancel);
	_btnOk->OnClicked.RemoveDynamic(this, &UGsUIPopupDownload::OnClickOk);

	Super::NativeDestruct();
}

bool UGsUIPopupDownload::OnBack()
{
	return true;
}

void UGsUIPopupDownload::SetData(float inDownloadSize, TFunction<void(bool)> InCallback)
{
	_buttonCallback = InCallback;

	//FString fileSize;
	//FGsUIStringHelper::GetPatchSizeText(inDownloadSize, fileSize);

	FString fileSize = FString::Printf(TEXT("%.2f MB"), inDownloadSize);

	FText findText;
	FText::FindText(TEXT("TitleSceneText"), TEXT("UIDownloadVolume"), findText);
	FText text = FText::Format(findText, FText::FromString(fileSize));
	
	_downloadSizeText->SetText(FText::FromString(fileSize));
}

void UGsUIPopupDownload::OnClickOk()
{
	// Ã¢ ´Ý±â
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}
}

void UGsUIPopupDownload::OnClickCancel()
{
	// Ã¢ ´Ý±â
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}
}