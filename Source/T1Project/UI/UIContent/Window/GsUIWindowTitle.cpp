// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowTitle.h"

#include "T1Project.h"
#include "Net/GsNetSendService.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsPatchManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "UI/UIContent/Popup/GsUIPopupServerSelect.h"
#include "UI/UIContent/Popup/GsUIPopupDownload.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "DataSchema/GsSchemaEnums.h"
#include "Data/GsLobbyConstant.h"
#include "Data/GsResourceManager.h"

#include "MediaPlayer.h"
#include "MediaUtils/Public/MediaPlayerOptions.h"
#include "MediaAssets/Public/MediaSource.h"
#include "MediaAssets/Public/MediaTexture.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIWindowTitle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnStart->OnClicked.AddDynamic(this, &UGsUIWindowTitle::OnClickStartFunction);
	_btnOption->OnClicked.AddDynamic(this, &UGsUIWindowTitle::OnClickOptionFunction);
	_btnServerSelect->OnClicked.AddDynamic(this, &UGsUIWindowTitle::OnClickServerSelectFunction);
	_btnAccountChange->OnClicked.AddDynamic(this, &UGsUIWindowTitle::OnClickAccountChangeFunction);
	_btnSecurityRegister->OnClicked.AddDynamic(this, &UGsUIWindowTitle::OnClickDeviceRegisterFunction);

	// BSAM_PATCH_DATA_REPAIR
	btnPatchCheck->OnClicked.AddDynamic(this, &UGsUIWindowTitle::OnClickPatchCheckFunction);
}

void UGsUIWindowTitle::NativeConstruct()
{
	Super::NativeConstruct();

	_planetWorldId = 0;

//#if WITH_EDITOR
	SetVisibleOptionBtn(true);
//#else
//	SetVisibleOptionBtn(false);
//#endif

#if WITH_EDITOR
	SetVisibleDeviceRegisterBtn(false);
#else
	SetVisibleDeviceRegisterBtn(true);
#endif

#if FLOW_DUBUG_TEST
	SetVisibleAccountChangeBtn(true);
#else
#if !WITH_EDITOR
	SetVisibleAccountChangeBtn(true);
#else
	SetVisibleAccountChangeBtn(false);
#endif
#endif

	// BSAM_PATCH_DATA_REPAIR
	// 기본적으로 Off되어 있으며, 패치 프로세스 혹은 마운트 프로세스 후 캐릭터 선택창까지는 진입해야지만 활성화 된다.	
#if WITH_EDITOR
	SetVisiblePatchCheckBtn(false);
#else
	SetVisiblePatchCheckBtn(true);
#endif

	GSLOG(Log, TEXT("[TITLE_TEST] NativeConstruct"));
}

void UGsUIWindowTitle::NativeDestruct()
{
	GSLOG(Log, TEXT("[TITLE_TEST] NativeDestruct"));		

	Super::NativeDestruct();
}

bool UGsUIWindowTitle::OnBack()
{
	FGsUIHelper::PopupQuitGame();

	// 게임 나가기 팝업창이 아닌 종료창을 띄울 것이므로 Super::OnBack를 타지 않음
	return true;
}

void UGsUIWindowTitle::CloseInternal()
{
	GSLOG(Log, TEXT("[TITLE_TEST] Click Close"));


	StopMovie();

	Super::CloseInternal();
}

bool UGsUIWindowTitle::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InLobby::TITLE))
	{		
		return true;
	}

	return false;
}


void UGsUIWindowTitle::OnClickStartFunction()
{
	OnClickStart.ExecuteIfBound(_planetWorldId);
}

void UGsUIWindowTitle::OnClickOptionFunction()
{
	OnClickOption.ExecuteIfBound();
}

void UGsUIWindowTitle::OnClickServerSelectFunction()
{
	OnClickServerSelect.ExecuteIfBound();
}

void UGsUIWindowTitle::OnClickAccountChangeFunction()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click ChangeAccount"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnClickAccountChange.ExecuteIfBound();
}

// BSAM_PATCH_DATA_REPAIR
void UGsUIWindowTitle::OnClickPatchCheckFunction()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click PatchCheck"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnClickPatchCheck.ExecuteIfBound();
}

void UGsUIWindowTitle::OnClickDeviceRegisterFunction()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click kDeviceRegister"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnClickDeviceRegister.ExecuteIfBound();
}

void UGsUIWindowTitle::SetServerName(uint16 In_PlanetWorldId)
{	
	if (FGsNetManager* netManager = GNet())
	{
		const TArray<ServerElem> listServer = netManager->GetSeverList();
	
		uint16 id = In_PlanetWorldId;
		int index = listServer.IndexOfByPredicate([id](const ServerElem& data)
			{
				return data.mPlanetWorldId == id;
			});

		if (0 <= index && index < listServer.Num())
		{
			FString serverName;
			netManager->GetPlanetWorldIdName(listServer[index].mPlanetWorldId, serverName);
			_serverNameText->SetText(FText::FromString(serverName));

#if	!UE_BUILD_SHIPPING				
			const FString logString = FString::Printf(
				TEXT("[WindowTitle] Set ServerName PlanetWorldId : %d, PlanetWorldName : %s"), listServer[index].mPlanetWorldId, *serverName);
			//FVector2D textScale{ 2.f, 2.f };
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);

			GSLOG(Log, TEXT("%s"), *logString);
#endif
		}

		_planetWorldId = In_PlanetWorldId;
		netManager->SetPlanetWorldId(_planetWorldId);		
	}
}

void UGsUIWindowTitle::SetClickStartEnabled(bool inEnabled)
{
	if (_btnStart)
		_btnStart->SetIsEnabled(inEnabled);
}

void UGsUIWindowTitle::SetVisibleOptionBtn(bool In_Visible)
{
	if (_btnOption)
		_btnOption->SetVisibility(In_Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGsUIWindowTitle::SetVisibleAccountChangeBtn(bool In_Visible)
{
	if (_btnAccountChange)
		_btnAccountChange->SetVisibility(In_Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

// BSAM_PATCH_DATA_REPAIR
void UGsUIWindowTitle::SetVisiblePatchCheckBtn(bool In_Visible)
{
	if (btnPatchCheck)
		btnPatchCheck->SetVisibility(In_Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGsUIWindowTitle::SetVisibleDeviceRegisterBtn(bool In_Visible)
{
	if (_btnSecurityRegister)
		_btnSecurityRegister->SetVisibility(In_Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGsUIWindowTitle::UpdateServerName()
{
	//if (0 < _planetWorldId)
	//{

	//}
	//else
	//{
		if (FGsNetManager* netManager = GNet())
		{
			_planetWorldId = netManager->GetPlanetWorldId();

			const FString& selectPlanetWorldName = netManager->GetSelectPlanetWorldName();

			if (!selectPlanetWorldName.IsEmpty())
			{
				_serverNameText->SetText(FText::FromString(selectPlanetWorldName));

#if	!UE_BUILD_SHIPPING				
				const FString logString = FString::Printf(
					TEXT("[WindowTitle] Update ServerName PlanetWorldId : %d, PlanetWorldName : %s"), _planetWorldId, *selectPlanetWorldName);
				//FVector2D textScale{ 1.5f, 1.5f };
				//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, logString, true, textScale);

				GSLOG(Log, TEXT("%s"), *logString);
#endif
			}			
		}
	//}
}

void UGsUIWindowTitle::PlayMovie()
{
	if (nullptr == _mediaPlayer)
		return;

	if (_mediaPlayer->IsPlaying())
		return;

	if (_mediaTexture != nullptr)
	{
		_mediaTexture->ClearColor = FColor::Black;
	}

	const UGsLobbyConstant* lobbyConstant = LSLobbyData()->GetLobbyConstantData();
	if (nullptr == lobbyConstant)
		return;

	const FString mediaSourcePath = lobbyConstant->_titleMovie.ToSoftObjectPath().ToString();
	_fileName = FPaths::GetBaseFilename(mediaSourcePath);

	UObject* mediaSource = GetMediaSource(mediaSourcePath);
	if (mediaSource)
	{
		FMediaPlayerOptions option;
		option.PlayOnOpen = EMediaPlayerOptionBooleanOverride::Enabled;
		option.Loop = EMediaPlayerOptionBooleanOverride::Enabled;

		if (_mediaPlayer)
		{
			_mediaPlayer->OpenSourceWithOptions(Cast<UMediaSource>(mediaSource),
				option);
			_mediaPlayer->SetNativeVolume(1.0f);
		}
	}
}

void UGsUIWindowTitle::StopMovie()
{
	if (_mediaPlayer)
	{
		FText currFileName = _mediaPlayer->GetMediaName();
		if (_fileName == currFileName.ToString())
		{
			_mediaPlayer->Close();
		}
	}
}

UObject* UGsUIWindowTitle::GetMediaSource(const FString& inMediaSource)
{
	return GResource()->LoadSync<UObject>(FSoftObjectPath(inMediaSource));
}

void UGsUIWindowTitle::SetRegisterIcon(bool In_Visible)
{
	_swicherRegisterIcon->SetActiveWidgetIndex(In_Visible? 1 : 0);
}