// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyCheckPatch.h"

#include "Net/GsNetSendService.h"

#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsPatchManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/GsMessageStage.h"
#include "Message/GsMessageUI.h"

#include "Text.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupDownload.h"

#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "../Data/GsLocalizationConstant.h"
#include "../Message/MessageParam/GsStageMessageParam.h"

#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/GameContents/ContentsLobby/GsContentsManagerLobby.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

static const FString CONTENT_FOLDER = TEXT("../Content/");


FGsStateLobbyCheckPatch::FGsStateLobbyCheckPatch() : FGsContentsLobbyBase(FGsContentsMode::InLobby::PATCH_CHECK)
{
}

FGsStateLobbyCheckPatch::~FGsStateLobbyCheckPatch()
{
}

void FGsStateLobbyCheckPatch::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyCheckPatch::Enter()"));

#if FLOW_DUBUG_TEST
	FGsLobbyFlowMsgParam param(MessageStage::REQUEST_PATCH_START);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);

	return;
#endif

	if(false == GHive()->IsInit())
	{
		FGsLobbyFlowMsgParam param(MessageStage::RETRY_HIVE_INIT);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
	else if (false == GHive()->IsSignIn())
	{
		FGsLobbyFlowMsgParam param(MessageStage::RETRY_HIVE_AUTH_SINGNIN);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
	else
	{
		// 윈도우 전용 설치 폴더 선택 기능(최초 설치시, 배포된 버전에서는 최초 패치받을때 한번만 출력, 한번 선택이후에는 변경안됨, 차후에 필요하면 옵션화)
		// 기존 무조건 사용자/AppData/Local/Zenonia/Content/Patch 폴더
		// 선택에 따라 배포 클라이언트 설치 폴더에 추가 ex)C:/Program Files/Zenonia/Content/Patch 폴더
#if PLATFORM_WINDOWS && UE_BUILD_SHIPPING
		// 파일이 없다면 한번도 패치를 받지 않은 상태이다(최초 설치, 혹은 해당 클라 배포시점(해당파일은 패치받은 패치버전을 저장하는 파일로 쉬핑에서 현재 저장되지 않고 있다)
		if (false == GPatch()->IsCheckPatchInfoFile())
		{			
			/*현재 설치된(될) 패치 파일경로를 클라이언트 폴더하위로 변경 하시겠습니까 ?
			(아닐시 사용자 /AppData/Local/Zenonia 에 저장됩니다.)*/	
			FText msg;
			FText::FindText(TEXT("TitleSceneText"), TEXT("PatchInstallPathSelectDesc"), msg);
			FGsUIHelper::PopupSystemYesNo(msg, [](bool bYes)
			{
				if (bYes)
				{
					FText msg;
					FText::FindText(TEXT("TitleSceneText"), TEXT("PatchInstallPathDataMoveDesc"), msg);
					FGsUIHelper::PopupSystemMsg(msg, []()
					{
						GPatch()->ChangePatchPath(true);

						// 기존 설치된 패치폴더와 다르다면 기존에 것을 새로 변경된 패치 폴더로 이동
						if (false == GPatch()->PatchFileCleanUp())
						{
							// 그전 설치 폴더에서 현재 선택한 폴더로 복사가 실패(사용자 중단, 용량 초과등에 이유로), 그럼 다시 시도 해야됨
							// 해당 폴더(파일)에 엑세스가 불가능하거나, 용량들에 문제로 복사가 실패했습니다.(제시작 및 제부팅후 다시 시도해주세요)
							FText msg;
							FText::FindText(TEXT("TitleSceneText"), TEXT("PatchInstallPathSelectCopyDirectoryFail"), msg);
							FGsUIHelper::PopupSystemMsg(msg);
						}

						if (FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow())
						{
							if (FGsContentsManagerLobby* contents = lobbyFlow->GetContentsManagerLobby())
							{
								FGsStateLobbyCheckPatch* StateLobbyCheckPatch = contents->GetContentsStateObject<FGsStateLobbyCheckPatch>(FGsContentsMode::InLobby::PATCH_CHECK);
								if (StateLobbyCheckPatch)
									StateLobbyCheckPatch->Enter();
							}
						}
					});
				}				
				else
				{
					GPatch()->ChangePatchPath(false);

					// 기존 설치된 패치폴더와 다르다면 기존에 것을 새로 변경된 패치 폴더로 이동
					if (false == GPatch()->PatchFileCleanUp())
					{
						// 그전 설치 폴더에서 현재 선택한 폴더로 복사가 실패(사용자 중단, 용량 초과등에 이유로), 그럼 다시 시도 해야됨
						// 해당 폴더(파일)에 엑세스가 불가능하거나, 용량들에 문제로 복사가 실패했습니다.(제시작 및 제부팅후 다시 시도해주세요)
						FText msg;
						FText::FindText(TEXT("TitleSceneText"), TEXT("PatchInstallPathSelectCopyDirectoryFail"), msg);
						FGsUIHelper::PopupSystemMsg(msg);
					}

					if (FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow())
					{
						if (FGsContentsManagerLobby* contents = lobbyFlow->GetContentsManagerLobby())
						{
							FGsStateLobbyCheckPatch* StateLobbyCheckPatch = contents->GetContentsStateObject<FGsStateLobbyCheckPatch>(FGsContentsMode::InLobby::PATCH_CHECK);
							if (StateLobbyCheckPatch)
								StateLobbyCheckPatch->Enter();
						}
					}
				}				
			});
		}
		else
		{
			// 요청 후 터치를 막는다.
			Blocking();
			CheckPatch();
		}
#else
		// 요청 후 터치를 막는다.
		Blocking();
		CheckPatch();
#endif
	}	
}

void FGsStateLobbyCheckPatch::Exit()
{
	UnBlocking();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyCheckPatch::CheckPatch()
{
	GSLOG(Log, TEXT("FGsStateLobbyCheckPatch::CheckPatch() call"));
	
	_downloadSize = 0;
	int AssetChunkID = 11;
	int VoiceChunkID = 12;
	const UGsLocalizationConstant* LocalizationConstant = LSLobbyData()->GetLocalizationConstantData();
	if (LocalizationConstant)
	{		
		AssetChunkID = LocalizationConstant->GetAssetPackageNum(GLocalization()->GetCurrentCultureName());
		VoiceChunkID = LocalizationConstant->GetVoicePackageNum(GLocalization()->GetVoiceCultureName());

		GSLOG(Log, TEXT("GetAssetPackageNum InAssetChunkID : %d, GetVoicePackageNum InVoiceChunkID : %d)"), AssetChunkID, VoiceChunkID);
	}
	// 성공 시	
	// 다운로드가 필요한가?
	GPatch()->OnRequestCheckDownloadComplete().BindLambda([this](bool bCheckDownload, bool bMounted, float fDownloadSize) {
		if (bCheckDownload)
		{			
			UnBlocking();

			_downloadSize = fDownloadSize;

			TWeakObjectPtr<UGsUIWidgetBase> widget;
			if (const UGsLobbyConstant* lobbyConst = LSLobbyData()->GetLobbyConstantData())
			{
				widget = GUI()->OpenAndGetWidgetByPath(lobbyConst->_pathPopupDownload.Id, lobbyConst->_pathPopupDownload.Path);
			}

			if (widget.IsValid())
			{
				if (UGsUIPopupDownload* popup = Cast<UGsUIPopupDownload>(widget.Get()))
				{
					popup->SetData(fDownloadSize, [this](bool bInIsOk)
						{
							if (bInIsOk)
							{
								float diskFreeSpace = 0;
								if (true == GetDiskFreeSpace(diskFreeSpace))
								{
									if (_downloadSize > diskFreeSpace)
									{
										GSLOG(Log, TEXT("FGsStateLobbyCheckPatch::CheckPatch() totalDownloadedSize > diskFreeSpace : true"));

										FText errorText;
										FText::FindText(TEXT("TitleSceneText"), TEXT("DescSpaceFull"), errorText);

										FGsUIHelper::PopupSystemMsg(errorText, []()
											{
												GSLOG(Log, TEXT("FGsStateLobbyPatch::Enter() totalDownloadedSize > diskFreeSpace : true"));

												FGsLobbyFlowMsgParam param(MessageStage::CHECK_PATHC_FAILED);
												GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
											});

										return;
									}
								}

								FGsLobbyFlowMsgParam param(MessageStage::REQUEST_PATCH_START);
								GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
							}
							else
							{
								FGsLobbyFlowMsgParam param(MessageStage::REQUEST_PATHC_REJECT);
								GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
							}
						});
				}
			}
		}
		else
		{
			// BSAM_PATCH_DATA_REPAIR
			if (true == bMounted)
			{
				FGsLobbyFlowMsgParam param(MessageStage::CHECK_PATHC_COMPLETE);
				GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);				
			}
			else
			{
				FGsUIHelper::HideBlockUI();
				
				FGsLobbyFlowMsgParam param(MessageStage::CHECK_PATHC_FAILED);
				GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
			}
		}});
	GPatch()->CheckDownloadRequest(AssetChunkID, VoiceChunkID);
}

bool FGsStateLobbyCheckPatch::GetDiskFreeSpace(OUT float& outDiskFreeSpaceMB)
{
	FString InstallDir;
#if PLATFORM_WINDOWS
	InstallDir = FPaths::ProjectContentDir();
#else
	InstallDir = FPaths::ProjectPersistentDownloadDir() / CONTENT_FOLDER;
#endif

	GSLOG(Log, TEXT("FGsStateLobbyCheckPatch::GetDiskFreeSpace() InstallDir : %s"), *InstallDir);

	uint64 TotalDiskSpace = 0;
	uint64 TotalDiskFreeSpace = 0;
	if (FPlatformMisc::GetDiskTotalAndFreeSpace(InstallDir, TotalDiskSpace, TotalDiskFreeSpace))
	{
		double DiskFreeSpace = (double)TotalDiskFreeSpace;
		double DiskFreeSpaceMB = FUnitConversion::Convert(DiskFreeSpace, EUnit::Bytes, EUnit::Megabytes);
		outDiskFreeSpaceMB = (float)DiskFreeSpaceMB;

		return true;
	}

	return false;
}