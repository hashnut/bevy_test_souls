// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyRepairCheck.h"

#include "T1Project.h"

#include "Net/GsNetSendService.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsPatchManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"

#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageStage.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "UI/UIContent/Window/GsUIWindowDownload.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsLobbyConstant.h"

#include "Sound/GsSoundPlayer.h"


FGsStateLobbyRepairCheck::FGsStateLobbyRepairCheck() : FGsContentsLobbyBase(FGsContentsMode::InLobby::REPAIR_CHECK)
{
}

FGsStateLobbyRepairCheck::~FGsStateLobbyRepairCheck()
{
}

void FGsStateLobbyRepairCheck::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyRepairCheck::Enter()"));

#if FLOW_DUBUG_TEST
	/*FGsLobbyFlowMsgParam param(MessageStage::COMPLETED_PATCH);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);*/
	StartTimer();
	//return;
#endif

	FText::FindText(TEXT("TitleSceneText"), TEXT("DescDownloadinfo01"), _textDownloading);
	FText::FindText(TEXT("TitleSceneText"), TEXT("DescDownloadinfo03"), _textDownloadChecking);

	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchStateQueued"), _textPatchStateQueued);
	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchStateInitializing"), _textPatchStateInitializing);
	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchStateResuming"), _textPatchStateResuming);
	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchStateInstalling"), _textPatchStateInstalling);
	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchStateMovingToInstall"), _textPatchStateMovingToInstall);
	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchRepairStateBuildVerification"), _textPatchStateBuildVerification);
	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchStateCleanUp"), _textPatchStateCleanUp);
	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchStatePrerequisitesInstall"), _textPatchStatePrerequisitesInstall);
	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchStateCompleted"), _textPatchStateCompleted);
	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchStatePaused"), _textPatchStatePaused);
	
	if(FGsNetManager * netManager = GNet())
	{		
		netManager->SetBeforePatchPlanetWorldId(netManager->GetPlanetWorldId());
	}

	if(FGsNetManager * netManager = GNet())
	{		
		netManager->SetBeforePatchPlanetWorldId(netManager->GetPlanetWorldId());
	}

	// 메세지 등록
	RegisterMessages();

	// ui 오픈
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	const UGsLobbyConstant* lobbyConstant = LSLobbyData()->GetLobbyConstantData();
	if (nullptr == lobbyConstant)
		return;
	
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidgetByPath(
		lobbyConstant->_pathWIndowDownLoad.Id, lobbyConstant->_pathWIndowDownLoad.Path);
	if (widget.IsValid())
	{
		_windowUI = Cast<UGsUIWindowDownload>(widget);
		_windowUI->SetProgressBarRatio(0.0f);
		_windowUI->SetCheckingBarRatio(0.0f);
		_windowUI->SetDownloadText(FText::FromString(TEXT("")));
		_windowUI->PlayMovie();
	}	

	// bgm play
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	GSLOG(Log, TEXT("FGsStateLobbyTitle: UGsSoundPlayer* soundPlayer = GSoundPlayer()"));
	soundPlayer->PlaySoundBgmByPath(lobbyConstant->_pathBgmDownLoad);

	GSLOG(Log, TEXT("FGsStateLobbyRepairCheck::RequestRepairCheck() Call"));

	int AssetChunkID = 11;
	int VoiceChunkID = 12;
	const UGsLocalizationConstant* LocalizationConstant = LSLobbyData()->GetLocalizationConstantData();
	if (LocalizationConstant)
	{
		AssetChunkID = LocalizationConstant->GetAssetPackageNum(GLocalization()->GetCurrentCultureName());
		VoiceChunkID = LocalizationConstant->GetVoicePackageNum(GLocalization()->GetVoiceCultureName());

		GSLOG(Log, TEXT("GetAssetPackageNum InAssetChunkID : %d, GetVoicePackageNum InVoiceChunkID : %d)"), AssetChunkID, VoiceChunkID);
	}

#if FLOW_DUBUG_TEST
#else
	_curTime = 0.0f;
	GPatch()->RepairProcessRequest(AssetChunkID, VoiceChunkID);
#endif
}

void FGsStateLobbyRepairCheck::OnRepairCheckFailed()
{
	// 다운로드 실패
	GSLOG(Log, TEXT("FGsStateLobbyRepairCheck::OnRepairCheckFailed"));

	// 패치 인스톨 실패가 아닐때는 기존 로직
	if (GPatch()->IsRepairFail() == false)
	{
		FGsLobbyFlowMsgParam param(MessageStage::FAILED_REPAIR_CHECK);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
	else
	{
		switch (GPatch()->GetRepairFailError())
		{
			// 등록된 오류가 없습니다.
			// 설치된 스테이징 폴더 삭제 팝업, 다시 받기
			case EBuildPatchInstallError::NoError:
			{
				/*
				* 1. Zenonia\Saved\PersistentDownloadDir\PatchStaging 폴더 내용 삭제
				* 2. 설치시 오류가 발생했습니다. 패치를 다시 시도해주세요. 팝업 출력 확인하면 초기화면으로가서 패치를 다시받게
				*/				
				GPatch()->DeletePatchStagingDataDirectory();

				FGsLobbyFlowMsgParam param(MessageStage::FAILED_REPAIR_CHECK);
				GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);

				FText msg;
				FText::FindText(TEXT("TitleSceneText"), TEXT("PatchInstallErrorNoError"), msg);
				FGsUIHelper::PopupSystemMsg(msg);

			} break;

			// 다운로드 요청이 실패했으며 허용된 재시도 횟수를 초과했습니다.
			// 네트웍 연결 제확인 팝업
			case EBuildPatchInstallError::DownloadError:
			{
				/*
				* 1. 네트웍 연결에 문제가 발생했습니다. 패치를 다시 시도해주세요. 팝업 출력 확인하면 초기화면으로가서 패치를 다시받게
				*/
				FGsLobbyFlowMsgParam param(MessageStage::FAILED_REPAIR_CHECK);
				GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);

				FText msg;
				FText::FindText(TEXT("TitleSceneText"), TEXT("PatchInstallErrorDownloadError"), msg);
				FGsUIHelper::PopupSystemMsg(msg);
			} break;

			// 파일을 제대로 구성하지 못했습니다.
			// https://forums.unrealengine.com/t/unable-to-install-marketplace-content/483966
			// 해당 사례는 캐시파일 삭제, 불량섹터등의 문제로 다시 패치 시도를 할수 있는 상태로
			// 설치된 스테이징 폴더 삭제 팝업, 다시 받기
			case EBuildPatchInstallError::FileConstructionFail:
			{
				/*
				* 1. Zenonia\Saved\PersistentDownloadDir\PatchStaging 폴더 내용 삭제
				* 2. 설치파일에 문제가 있어, 파일을 제대로 구성하지 못했습니다. 패치를 다시 시도해주세요. 팝업 출력 확인하면 초기화면으로가서 패치를 다시받게				
				*/
				GPatch()->DeletePatchStagingDataDirectory();

				FGsLobbyFlowMsgParam param(MessageStage::FAILED_REPAIR_CHECK);
				GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);

				FText msg;
				FText::FindText(TEXT("TitleSceneText"), TEXT("PatchInstallErrorFileConstructionFail"), msg);
				FGsUIHelper::PopupSystemMsg(msg);
			} break;

			// 파일을 설치 위치로 이동하는 동안 오류가 발생했습니다.
			// 기존에 패치 파일엑세스를 할수 없을때 주로 발생한다. (이미 게임실행중 마운트되어 있는등)
			// 해결 방법은 제시작 요청 팝업 출력 후 확인 버튼 누르면 종료 후 제시작
			case EBuildPatchInstallError::MoveFileToInstall:
			{
				/*
				* 1. 파일을 설치 위치로 이동하는 동안 오류가 발생했습니다.(엑세스 위반) 게임을 종료 후 다시 실행해주세요.(확인을 누르시면 종료됩니다.) 팝업 출력 확인하면 종료? 그냥 두기?
				*/
				FText msg;
				FText::FindText(TEXT("TitleSceneText"), TEXT("PatchInstallErrorMoveFileToInstall"), msg);
				FGsUIHelper::PopupSystemMsg(msg, []()
				{
					GHive()->ExitGame();
				});				
			} break;

			// 설치된 빌드를 확인하지 못했습니다.
			// 설치 파일이 손상, 패치 받은 파일을 지우고 다시 시도하는게 맞을거 같다.
			case EBuildPatchInstallError::BuildVerifyFail:
			{
				/*
				* 1. Zenonia\Saved\PersistentDownloadDir\PatchStaging 폴더 내용 삭제
				* 2. 설치된 빌드 검증에 실패했습니다. 패치를 다시 시도해주세요. 팝업 출력 확인하면 초기화면으로가서 패치를 다시받게
				* https://thedroidguy.com/fix-fortnite-error-is-bv04-build-verification-failed-windows-10-epic-games-1157327
				* 파일 자체가 문제 있는거로 다시 설치를 추천하고 있다.
				*/				
				GPatch()->DeletePatchStagingDataDirectory();
				GPatch()->DeletePatchDataDirectory();

				FGsLobbyFlowMsgParam param(MessageStage::FAILED_REPAIR_CHECK);
				GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);

				FText msg;
				FText::FindText(TEXT("TitleSceneText"), TEXT("PatchInstallErrorBuildVerifyFail"), msg);
				FGsUIHelper::PopupSystemMsg(msg);
			} break;

			// 사용자 또는 일부 프로세스가 응용 프로그램을 닫았습니다.
			// 사용자가 종료했으니 여기 올일도 없을거 같지만
			// 앱이 종료되어 종료처리
			case EBuildPatchInstallError::ApplicationClosing:
			{
				GHive()->ExitGame();
			} break;

			// 모듈 로드 실패와 같은 애플리케이션 오류.
			// 알수없는 문제로 종료처리
			case EBuildPatchInstallError::ApplicationError:
			{
				GHive()->ExitGame();
			} break;			

			// 디스크에 남은 공간이 부족하여 파일을 만드는 중 오류가 발생했습니다.
			// 설치전에 체크를 하지만 설치하는 도중에 다른 프로그램 다운로드등으로 이슈 발생 공간확보 메세지박스 출력하기, 그리고 다시 시도할수 있는 상태로
			// 타이틀 보내기
			case EBuildPatchInstallError::OutOfDiskSpace:
			{
				// 설치할 디스크에 용량이 부족합니다. 용량 확보 후 제시도 해주세요.
				/*
				* 1. 설치할 디스크에 용량이 부족합니다. 용량 확보 후 제시도 해주세요. 팝업 출력 확인하면 초기화면으로가서 패치를 다시받게
				*/				
				FGsLobbyFlowMsgParam param(MessageStage::FAILED_REPAIR_CHECK);
				GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);

				FText msg;
				FText::FindText(TEXT("TitleSceneText"), TEXT("PatchInstallErrorOutOfDiskSpace"), msg);
				FGsUIHelper::PopupSystemMsg(msg);
			} break;			

			default:
			{
				FGsLobbyFlowMsgParam param(MessageStage::FAILED_REPAIR_CHECK);
				GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
			} break;
		}
	}
}

void FGsStateLobbyRepairCheck::Exit()
{
	// 메세지 삭제
	UnregisterMessages();

#if FLOW_DUBUG_TEST
	// 타이머 삭제
	StopTimer();
#else	
	// 패치 취소
	GPatch()->CancelInstall();
#endif	

	if (_windowUI.IsValid())
	{
		_windowUI->Close();
	}	

	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyRepairCheck::Update(float In_deltaTime)
{
	FGsContentsLobbyBase::Update(In_deltaTime);
	
	FString PatchStateString = GPatch()->GetStateString();

	BuildPatchServices::EBuildPatchState PatchState = GPatch()->GetState();	

	if (_windowUI.IsValid())
	{	
		switch (PatchState)
		{
			// 패치 프로세스가 다른 설치를 기다리고 있습니다.
			case BuildPatchServices::EBuildPatchState::Queued:
			{
				_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s"), *_textPatchStateQueued.ToString())));
			} break;

			// 패치 프로세스를 초기화하는 중입니다.
			case BuildPatchServices::EBuildPatchState::Initializing:
			{
				_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s"), *_textPatchStateInitializing.ToString())));
			} break;

			// 패치 프로세스가 기존의 스테이징된 데이터를 열거하고 있습니다.(중간에 패치를 받다가 종료한뒤 다시 시도시여기를 먼저 탄다)
			// 이거는 Downloading 와 같은 텍스트를 출력해야 다운로드 게이지가 갑자기 올라가는(정상적이지만) 표현이 자연스럽게 보이게 연출을 할수 있다
			case BuildPatchServices::EBuildPatchState::Resuming:
			{
				_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s"), *_textPatchStateResuming.ToString())));
			} break;

			// 패치 프로세스가 패치 데이터를 다운로드하는 중입니다.
			case BuildPatchServices::EBuildPatchState::Downloading:
			{
				float totalDownloadedSize = GPatch()->GetTotalDownloadedSize();
				float downloadSize = GPatch()->GetDownloadSize();
				float ratio = GPatch()->GetInstallProgress();

				float downloadSpeed = GPatch()->GetDownloadSpeed();

				FString fileSize = FString::Printf(TEXT("%.2f MB"), downloadSize);
				FString totalSize = FString::Printf(TEXT("%.2f MB"), downloadSize * ratio);
				FString speed = FString::Printf(TEXT("%.2f MB/s"), downloadSpeed * 0.1f);

				// 다운로드를 받는 중
				_windowUI->ShowDonloadProgressBar(true);
				_windowUI->SetProgressBarRatio(ratio);
				_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s (%s)"), *_textDownloading.ToString(), *speed)));
				_windowUI->SetDownloadCoundText(FText::FromString(FString::Printf(TEXT("%s / %s"), *totalSize, *fileSize)));
			} break;

			// 패치 프로세스가 파일을 설치하는 중입니다.
			case BuildPatchServices::EBuildPatchState::Installing:
			{
				_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s"), *_textPatchStateInstalling.ToString())));
			} break;

			// 패치 프로세스가 준비된 파일을 설치폴더로 이동하고 있습니다.
			case BuildPatchServices::EBuildPatchState::MovingToInstall:
			{
				_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s"), *_textPatchStateMovingToInstall.ToString())));
			} break;

			// 패치 프로세스에서 파일을 검증하고 있습니다.
			case BuildPatchServices::EBuildPatchState::BuildVerification:
			{
				// 연출
				float downloadSize = GPatch()->GetDownloadSize(); // 총받아야될 용량
				FString fileSize = FString::Printf(TEXT("%.2f MB"), downloadSize);
				FString totalSize = FString::Printf(TEXT("%.2f MB"), downloadSize);
				_windowUI->SetProgressBarRatio(1.0f);
				_windowUI->SetDownloadCoundText(FText::FromString(FString::Printf(TEXT("%s / %s"), *totalSize, *fileSize)));

				_curTime += In_deltaTime;
				if (_curTime > 4.0f)
					_curTime = 0.0f;

				if (0.0f < _curTime && _curTime <= 1.0f)
				{
					_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s"), *_textPatchStateBuildVerification.ToString())));
				}
				else if (1.0f < _curTime && _curTime <= 2.0f)
				{
					_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s."), *_textPatchStateBuildVerification.ToString())));
				}
				else if (2.0f < _curTime && _curTime <= 3.0f)
				{
					_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s.."), *_textPatchStateBuildVerification.ToString())));
				}
				else if (3.0f < _curTime && _curTime <= 4.0f)
				{
					_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s."), *_textPatchStateBuildVerification.ToString())));
				}
				
			} break;

			// 패치 프로세스가 임시 파일을 정리하고 있습니다.
			case BuildPatchServices::EBuildPatchState::CleanUp:
			{
				_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s"), *_textPatchStateCleanUp.ToString())));
			} break;

			// 패치 프로세스에서 필수 구성 요소를 설치하고 있습니다.
			case BuildPatchServices::EBuildPatchState::PrerequisitesInstall:
			{
				_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s"), *_textPatchStatePrerequisitesInstall.ToString())));
			} break;

			// 패치 프로세스가 완료되어 다음 프로세스를 준비중입니다.			
			case BuildPatchServices::EBuildPatchState::Completed:
			{
				_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s"), *_textPatchStateCompleted.ToString())));
			} break;

			// 패치 프로세스가 일시 중지로 설정되었습니다.
			case BuildPatchServices::EBuildPatchState::Paused:
			{
				_windowUI->SetDownloadText(FText::FromString(FString::Printf(TEXT("%s"), *_textPatchStatePaused.ToString())));
			} break;

			// 다만 빈채로 두는건 Default로 가면 안되기 때문이다.
			// 출력할 필요 없는 상태로 보임
			// 배열 크기에 대한 상태 수를 보유합니다.
			case BuildPatchServices::EBuildPatchState::NUM_PROGRESS_STATES:
			{
				GSLOG(Log, TEXT("Holds the number of states, for array sizes"));
			} break;

			// 다만 빈채로 두는건 Default로 가면 안되기 때문이다.
			// 출력할 필요 없는 상태로 보임
			// 패치 프로세스가 빌드에 속성을 설정하고 있습니다.
			case BuildPatchServices::EBuildPatchState::SettingAttributes:
			{		
				GSLOG(Log, TEXT("The patch process is setting up attributes on the build"));
			} break;

			default:
			{
				_windowUI->ShowDonloadProgressBar(false);
				_windowUI->SetCheckingBarRatio(1.0f);
			} break;
		}
	}
}

void FGsStateLobbyRepairCheck::Init()
{
	FGsContentsLobbyBase::Init();	
}

void FGsStateLobbyRepairCheck::Close()
{
	FGsContentsLobbyBase::Close();
}

void FGsStateLobbyRepairCheck::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MSystemParam& mLobbySystem = messageMgr->GetSystemParam();

	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::REPAIR_FALSE,
		this, &FGsStateLobbyRepairCheck::RepairCheckResultFailed));
	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::REPAIR_TRUE,
		this, &FGsStateLobbyRepairCheck::RepairCheckResultSucceeded));
}

void FGsStateLobbyRepairCheck::UnregisterMessages()
{
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		MSystemParam& mLobbySystem = messageMgr->GetSystemParam();
		for (MsgSystemHandle& handleLobbySystem : _msgHandleLobbySystemList)
		{
			mLobbySystem.Remove(handleLobbySystem);
		}
	}
	
	_msgHandleLobbySystemList.Empty();
}

// 패치 복구 실패
void FGsStateLobbyRepairCheck::RepairCheckResultFailed(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyRepairCheck::RepairCheckResultFailed"));
	OnRepairCheckFailed();
}

// 패치 복구 성공
void FGsStateLobbyRepairCheck::RepairCheckResultSucceeded(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyRepairCheck::RepairCheckResultSucceeded"));

	// BSAM_PATCH_DATA_REPAIR
	FGsLobbyFlowMsgParam Msgparam(MessageStage::COMPLETED_PATCH);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &Msgparam);

	// 패치 성공했을 때 Adjust 로 datapatch_complete 메시지 보냄
	FGsHiveEventMsg specialMsg(EGsHiveActionType::SPECIAL, 2);    // 인자는 테이블에 정의된 id 값을 따릅니다
	GMessage()->GetHiveEvent().SendMessage(MessageContentHiveEvent::SEND_ANALYTICS_EVENT, &specialMsg);
}

void FGsStateLobbyRepairCheck::StartTimer()
{
	FGsContentsLobbyBase::StartTimer();

	const UWorld* world = GHive()->GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}

	if (nullptr != world)
	{
		world->GetTimerManager().SetTimer(_timerHandle,
			FTimerDelegate::CreateRaw(this, &FGsStateLobbyRepairCheck::EndTimer),
			5.0f, false);
	}
}

void FGsStateLobbyRepairCheck::EndTimer()
{
	FGsContentsLobbyBase::EndTimer();

#if FLOW_DUBUG_TEST
	FGsLobbyFlowMsgParam Msgparam(MessageStage::CHECK_PATHC_COMPLETE);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &Msgparam);
#else
#endif
}

void FGsStateLobbyRepairCheck::StopTimer()
{
	FGsContentsLobbyBase::StopTimer();

	const UWorld* world = GHive()->GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}
}
