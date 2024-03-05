// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowDownload.h"
#include "Net/GsNetSendService.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UMG/Public/Components/ProgressBar.h"
#include "UMG/Public/Components/TextBlock.h"

#include "Data/GsLobbyConstant.h"
#include "Data/GsResourceManager.h"

#include "MediaPlayer.h"
#include "MediaAssets/Public/MediaSource.h"
#include "MediaUtils/Public/MediaPlayerOptions.h"
#include "MediaAssets/Public/MediaTexture.h"

#include "DataSchema/GsSchemaEnums.h"


void UGsUIWindowDownload::NativeConstruct()
{
	Super::NativeConstruct();	

	_audioId = 0;
	_donloadProgressBar->SetPercent(0.0f);	
	_isShowDonloadProgressBar = true;
	_isShowDonloadCheckingBar = false;
	ShowDonloadProgressBar(false);
	ShowDonloadCheckingBar(true);
}

void UGsUIWindowDownload::NativeDestruct()
{
	StopMovie();

	Super::NativeDestruct();
}

void UGsUIWindowDownload::SetDownloadText(const FText& inText)
{
	if (_donloadStateText)
	{
		_donloadStateText->SetText(inText);
	}
}

void UGsUIWindowDownload::SetDownloadCoundText(const FText& inText)
{
	if (_donloadCoundText)
	{
		_donloadCoundText->SetText(inText);
	}
}

void UGsUIWindowDownload::SetProgressBarRatio(float inRatio)
{
	if (_donloadProgressBar)
	{
		_donloadProgressBar->SetPercent(inRatio);
	}
}

void UGsUIWindowDownload::SetCheckingBarRatio(float inRatio)
{
	if (_donloadCheckingBar)
	{
		_donloadCheckingBar->SetPercent(inRatio);
	}
}

bool UGsUIWindowDownload::OnBack()
{
	// 뒤로가기 막기
	return true;
}

void UGsUIWindowDownload::ShowDonloadProgressBar(bool inIsShow)
{
	if (_isShowDonloadProgressBar == inIsShow)
		return;

	// 상태값 저장
	_isShowDonloadProgressBar = inIsShow;

	// UI Show/Hide
	if (inIsShow)
	{
		_donloadCoundText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_donloadProgressBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_donloadCoundText->SetVisibility(ESlateVisibility::Collapsed);
		_donloadProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIWindowDownload::ShowDonloadCheckingBar(bool inIsShow)
{
	if (_isShowDonloadCheckingBar == inIsShow)
		return;

	// 상태값 저장
	_isShowDonloadCheckingBar = inIsShow;

	// UI Show/Hide
	if (inIsShow)
	{
		_donloadCheckingBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_donloadCheckingBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIWindowDownload::PlayMovie()
{
	if (_mediaTexture != nullptr)
	{
		_mediaTexture->ClearColor = FColor::Black;
	}

	const UGsLobbyConstant* lobbyConstant = LSLobbyData()->GetLobbyConstantData();
	if (nullptr == lobbyConstant)
		return;

	const FString mediaSourcePath = lobbyConstant->_downloadMovie.ToSoftObjectPath().ToString();
	_fileName = FPaths::GetBaseFilename(mediaSourcePath);

	if (_mediaPlayer)
	{
		_mediaPlayer->Close();

		UObject* mediaSource = GetMediaSource(mediaSourcePath);
		if (mediaSource)
		{
			FMediaPlayerOptions option;
			option.PlayOnOpen = EMediaPlayerOptionBooleanOverride::Enabled;
			option.Loop = EMediaPlayerOptionBooleanOverride::Enabled;
			option.SeekTime = 0;

			if (_mediaPlayer)
			{
				_mediaPlayer->OpenSourceWithOptions(Cast<UMediaSource>(mediaSource),
					option);
				_mediaPlayer->SetNativeVolume(1.0f);
				_mediaPlayer->Rewind();
			}
		}
	}
}

void UGsUIWindowDownload::StopMovie()
{
	FText currFileName = _mediaPlayer->GetMediaName();
	if (_fileName == currFileName.ToString())
	{
		_mediaPlayer->Close();
	}
}

UObject* UGsUIWindowDownload::GetMediaSource(const FString& inMediaSource)
{
	return StaticLoadObject(UMediaSource::StaticClass(), NULL, *inMediaSource);
}