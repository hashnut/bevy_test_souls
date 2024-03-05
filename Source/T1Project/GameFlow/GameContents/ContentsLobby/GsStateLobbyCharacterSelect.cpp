// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyCharacterSelect.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Net/GsNetSendService.h"

#include "ActorEx/GsCharacterPlayer.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsCharacterLobbyPlayer.h"
#include "ActorEx/GsCharacterBase.h"

#include "Animation/GsAnimInstanceState.h"
#include "Animation/GsLobbyAnimInstanceState.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Customize/GsCustomizeFunc.h"


#include "GsContentsLobbyBase.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeLobby/GsLobbyDataType.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "GameFlow/Stage/StageLobby/GsStageLobbyCharacterSelect.h"
#include "GameFlow/Stage/StageLobby/GsStageManagerLobby.h"
#include "GameFlow/GsGameFlowLobby.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Window/GsUIWindowCharacterSelect.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "UI/UIContent/Popup/GsUIPopupCharacterDelete.h"
#include "UI/UIContent/Popup/GsUIPopupCharacterDeleteCancel.h"
#include "UI/UIContent/Popup/GsUIPopupOKTwoMessage.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Sound/GsSoundPlayer.h"

#include "Data/GsDataContainManager.h"

#include "DataSchema/GsSchemaEnums.h"

#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"

#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsCameraUtil.h"
#include "UTIL/GsTableUtil.h"

#include "Engine/Classes/Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageStage.h"



//#define CRASH_TEST_LOOP

FGsStateLobbyCharacterSelect::FGsStateLobbyCharacterSelect() : FGsContentsLobbyBase(FGsContentsMode::InLobby::CHARACTER_SELECT)
{
}

FGsStateLobbyCharacterSelect::~FGsStateLobbyCharacterSelect()
{
}

void FGsStateLobbyCharacterSelect::Enter()
{
	FGsContentsLobbyBase::Enter();

	_customizeHandler = new FGsCustomizeHandler();

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(FName(TEXT("WindowCharacterSelect")));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowCharacterSelect>(widget.Get());
			_windowUI->OnSelectUser.BindRaw(this, &FGsStateLobbyCharacterSelect::OnClickSelectUser);
			_windowUI->OnDeleteUser.BindRaw(this, &FGsStateLobbyCharacterSelect::OnClickDeleteUser);
			_windowUI->OnSlotClick.BindRaw(this, &FGsStateLobbyCharacterSelect::OnClickSlot);
			_windowUI->OnDeleteCancel.BindRaw(this, &FGsStateLobbyCharacterSelect::OnClickDeleteCancel);
			_windowUI->OnCustomizingTicketUse.BindRaw(this, &FGsStateLobbyCharacterSelect::OnClickCustomizingTicketUse);
		}
	}

#ifdef CRASH_TEST_LOOP
	UWorld* world = GUI()->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	FTimerDelegate timerCallback;
	timerCallback.BindLambda([] {
		if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
		{
			TArray<LobbyDataType::FGsLobbyUserListData>& listCharacter = lobbyDataMgr->GetUserList();

			int8 listNum = listCharacter.Num();
			if (0 < listNum)
			{
				FGsNetSendService::SendGatewayReqSelectUser(listCharacter[0]._id);
			}
		}
	});
	FTimerHandle testCrashTimeHandle;
	world->GetTimerManager().SetTimer(testCrashTimeHandle,
		timerCallback, 2.0f, false);

#endif

	RegisterMessages();

	if (UGsHiveManager* hiveMgr = GHive())
	{
		if (hiveMgr->IsSignIn())
		{
			hiveMgr->PromotionBANNER();
		}
	}

	if (FGsNetManager* netManager = GNet())
	{
		netManager->ResetBeforePatchPlanetWorldId();
	}
}


void FGsStateLobbyCharacterSelect::Exit()
{
	if (_customizeHandler)
		delete _customizeHandler;

	_customizeHandler = nullptr;

	UnregisterMessages();

	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyCharacterSelect::Close()
{
	FGsContentsLobbyBase::Close();
}

void FGsStateLobbyCharacterSelect::RegisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		_msgUiHandle = MessageHolder->GetUI().AddRaw(MessageUI::CHARACTER_SLECT_EMPTY, this, &FGsStateLobbyCharacterSelect::OnCharacterEmpty);
	}
}

void FGsStateLobbyCharacterSelect::UnregisterMessages()
{
	if (!_msgUiHandle.Value.IsValid())
	{
		return;
	}

	if (nullptr == GScope())
	{
		return;
	}

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetUI().Remove(_msgUiHandle);
		_msgUiHandle.Value.Reset();
	}
}



class AGsCharacterPlayer* FGsStateLobbyCharacterSelect::LoadChangeCharacter(const LobbyDataType::FGsLobbyUserListData& clickData)
{
	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (nullptr == characterMgr)
		return nullptr;

	FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow();
	FGsStageManagerLobby* stageManagerLobby = lobbyFlow->GetStageManagerLobby();
	auto lobbyStage = stageManagerLobby->GetCurrentState().Pin();
	
	if (nullptr == lobbyStage)
		return nullptr;

	AGsCharacterLobbyPlayer* player = lobbyStage->SetCharacter(clickData._genderType);
	if (nullptr == player)
		return nullptr;
	
	lobbyStage->ShowCharacter(true);

	UGsAnimInstanceState* anim = player->GetAnim();
	if (nullptr == anim)
	{
		return nullptr;
	}

	anim->ChangeState(static_cast<uint8>(EGsStateBase::Spawn), (int)clickData._classType, false);

	float selectZoomRate =
		GData()->GetGlobalData()->SelectInitZoomRate;
	// 각 로비 상태 마다 초기화 할것 처리

	// characterMgr->InitState(selectZoomRate);
	if (nullptr != _customizeHandler)
	{
		_customizeHandler->InitializeActor(player);
		if (clickData._classType == CreatureWeaponType::NONE ||
			clickData._classType == CreatureWeaponType::HAND)
		{
			_customizeHandler->SetWeaponChange();
		}

		_customizeHandler->SetActor(player);
		_customizeHandler->SetGenderType(clickData._genderType);
		_customizeHandler->SetCustomizeDataBonScale(clickData._faceData, clickData._bodyData);
		_customizeHandler->SetCostume(clickData._lookInfo.mTransformCostumeId, clickData._lookInfo.mCostumeId, clickData._lookInfo.mViewHelmet, true, true);
		FGsCustomizeFunc::AttachWeapon(_customizeHandler, &clickData._lookInfo, (int32)clickData._genderType, true);
	}

	

	// only once save value
	UGsGameObjectUtil::SaveCameraBoneHeight(clickData._genderType, clickData._bodyData.mHeight);

	// 인게임에서 pc 스폰전에 해당 위치 레벨 로드 되었는지 체크
	// TrySweepToLand 함수 사용하기 위해 실린더 radius, height 값 저장해놓는다	
	// FGsSchemaCreatureCommonInfo 테이블 찾을때 1, 2 가 pc 남, 여 키값인데 
	// 일단은 gender 값이랑 같아서 그걸로 찾자 (로비에서 정확히 가리키는 값있다면 그걸로 변경 필요)
	// https://jira.com2us.com/jira/browse/C2URWQ-5675

	if(const FGsSchemaCreatureCommonInfo* findCommonInfo =
		UGsTableUtil::FindRowById<UGsTableCreatureCommonInfo,
			FGsSchemaCreatureCommonInfo>((int)clickData._genderType))
	{
		if (UGsLevelManager* levelMgr = GLevel())
		{
			levelMgr->SetLocalCylinderSize(findCommonInfo->cylinderRadius, findCommonInfo->cylinderHeight);
		}
	}
	


	/*UCapsuleComponent* capsuleComponent = player->GetCapsuleComponent();
	UGsGameObjectUtil::MeshRelativeLocation(clickData._genderType
		, clickData._bodyData.mHeight
		, capsuleComponent->GetScaledCapsuleHalfHeight()
		, player);*/

	// obbyStage->ReCalcCapsuleSize();

	// init camera fov& position offset
	if (UCameraComponent* cameraComponent = player->GetFollowCamera())
	{
		UGsCameraUtil::SetContentsTypeCameraData(EGsContentsCameraType::LobbyCharacterSelect, cameraComponent);
	}

	//https://jira.com2us.com/jira/browse/C2URWQ-1685
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->HideLoading();
	}

	return player;
	
}

void FGsStateLobbyCharacterSelect::OnClickSelectUser(const UserDBId inUserDBId)
{
	// 유저 선택 패킷 전송
	FGsNetSendService::SendGatewayReqSelectUser(inUserDBId);
}

LobbyDataType::FGsLobbyUserListData* FGsStateLobbyCharacterSelect::GetUserData(const UserDBId inUserDBId)
{	
	TArray<LobbyDataType::FGsLobbyUserListData>& listCharacter = LSLobbyData()->GetUserList();
	int8 listNum = listCharacter.Num();
	if (0 < listNum)
	{
		for (int i = 0; i < listNum; ++i)
		{
			if (inUserDBId != listCharacter[i]._id)
				continue;

			return &listCharacter[i];
		}
	}

	return nullptr;
}

void FGsStateLobbyCharacterSelect::OnClickDeleteUser(const UserDBId inUserDBId)
{
	LobbyDataType::FGsLobbyUserListData* userData = GetUserData(inUserDBId);
	if (nullptr == userData)
		return;
	
	if (false == CheckDeleteLevel(userData))
		return;

	if (false == CheckDeleteGuild(userData))
		return;

	FString userName = userData->_userName;
	TWeakObjectPtr<UGsUIWidgetBase> widget1 = GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
	if (!widget1.IsValid())
		return;

	int day = LSLobbyData()->GetDeletionCooldownHour() / 24;
	
	if (UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(widget1.Get()))
	{
		// TEXT: 선택하신 캐릭터를 삭제하시겠습니까??
		FText textMsg1;
		FText::FindText(TEXT("LobbyText"), TEXT("Delete_1stPopup_Title"), textMsg1);
		// TEXT: 캐릭터 삭제 신청시 유예기간과 상관없이...
		FText textMsg2;
		FText::FindText(TEXT("LobbyText"), TEXT("Delete_1stPopup_Text"), textMsg2);
		textMsg2 = FText::Format(textMsg2, day, day);

		LSLobbyData()->SetIsDeleteUser(true);	

		popup->SetData(textMsg1, textMsg2, [inUserDBId, userName](bool InIsOk)
		{
			if (!InIsOk)
				return;
			
			TWeakObjectPtr<UGsUIWidgetBase> widget2 = GUI()->OpenAndGetWidget(TEXT("PopupCharacterDelete"));
			if (!widget2.IsValid())
				return;
			
			if (UGsUIPopupCharacterDelete* popupCharacterDelete = Cast<UGsUIPopupCharacterDelete>(widget2.Get()))
			{
				popupCharacterDelete->SetData(userName, [inUserDBId, userName](bool InIsCharacterDeleteOk)
				{
					if (!InIsCharacterDeleteOk)
						return;

					// 유저 선택 패킷 전송
					FGsNetSendService::SendGatewayReqDeleteUser(inUserDBId);
				});
			}			
		});
	}	
}

bool FGsStateLobbyCharacterSelect::CheckDeleteLevel(LobbyDataType::FGsLobbyUserListData* inUserData)
{
	int deleteUserMinLevel = LSLobbyData()->GetDeleteUserMinLevel();
	if (deleteUserMinLevel <= inUserData->_lv)
		return true;

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupOKTwoMessage"));
	if (!widget.IsValid())
		return false;

	if (UGsUIPopupOKTwoMessage* popup = Cast<UGsUIPopupOKTwoMessage>(widget.Get()))
	{
		// TEXT: 선택한 캐릭터는 삭제 할 수 없습니다.
		FText textMsg1;
		FText::FindText(TEXT("LobbyText"), TEXT("DeleteError_Popup_Title"), textMsg1);
		// TEXT: 기사단 가입 상태 및 {0}레벨 .... {1}레벨 미만인 캐릭터 입니다.
		FText textMsg2;
		FText::FindText(TEXT("LobbyText"), TEXT("DeleteError_Popup_Level"), textMsg2);
		textMsg2 = FText::Format(textMsg2, deleteUserMinLevel, deleteUserMinLevel);

		popup->SetData(textMsg1, textMsg2);
	}

	return false;
}

bool FGsStateLobbyCharacterSelect::CheckDeleteGuild(LobbyDataType::FGsLobbyUserListData* inUserData)
{
	if (inUserData->_guildName.IsEmpty())
	{
		return true;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupOKTwoMessage"));
	if (!widget.IsValid())
		return false;

	

	if (UGsUIPopupOKTwoMessage* popup = Cast<UGsUIPopupOKTwoMessage>(widget.Get()))
	{
		// TEXT: 선택한 캐릭터는 삭제 할 수 없습니다.
		FText textMsg1;
		FText::FindText(TEXT("LobbyText"), TEXT("DeleteError_Popup_Title"), textMsg1);
		// TEXT: 기사단 가입 상태 및 {0}레벨 .... 기사단에 가입되어있는 캐릭터입니다.
		FText textMsg2;
		FText::FindText(TEXT("LobbyText"), TEXT("DeleteError_Popup_Head"), textMsg2);
		int deleteUserMinLevel = LSLobbyData()->GetDeleteUserMinLevel();
		textMsg2 = FText::Format(textMsg2, deleteUserMinLevel);

		popup->SetData(textMsg1, textMsg2);
	}

	return false;
}

// 캐릭터 삭제 철회
void FGsStateLobbyCharacterSelect::OnClickDeleteCancel(const UserDBId inUserDBId)
{
	FString userName;
	TArray<LobbyDataType::FGsLobbyUserListData>& listCharacter = LSLobbyData()->GetUserList();
	int8 listNum = listCharacter.Num();
	if (0 < listNum)
	{
		for (int i = 0; i < listNum; ++i)
		{
			if (inUserDBId != listCharacter[i]._id)
				continue;

			userName = listCharacter[i]._userName;
			break;
		}
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCharacterDeleteCancel"));
	if (!widget.IsValid())
		return;

	if (UGsUIPopupCharacterDeleteCancel* popupCharacterDeleteCancel = Cast<UGsUIPopupCharacterDeleteCancel>(widget.Get()))
	{
		popupCharacterDeleteCancel->SetData(userName, [inUserDBId, userName](bool InIsOk)
		{
			if (!InIsOk)
				return;

			// 유저 삭제 철회 패킷 전송
			FGsNetSendService::SendGatewayReqDeleteUserCancel(inUserDBId);
		});
	}
}

void FGsStateLobbyCharacterSelect::OnClickSlot(const UserDBId inUserDBId)
{
	if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	{
		TArray<LobbyDataType::FGsLobbyUserListData> listCharacter = lobbyDataMgr->GetUserList();
		for (auto& iter : listCharacter)
		{
			if (iter._id != inUserDBId)
				continue;

			LoadChangeCharacter(iter);
		}
	}
}

void FGsStateLobbyCharacterSelect::OnCharacterEmpty(const IGsMessageParam*)
{
	GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTER_EMPTY_STATE);
}


void FGsStateLobbyCharacterSelect::OnClickCustomizingTicketUse(const UserDBId inUserDBId)
{
	if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	{
		lobbyDataMgr->SetCustomizingUserDBId(inUserDBId);
	}

	GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERCREATE_STAGE);
}