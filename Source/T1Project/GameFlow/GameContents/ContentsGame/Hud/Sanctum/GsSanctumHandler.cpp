// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSanctumHandler.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsSanctumManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectSanctumBase.h"
#include "Message/MessageParam/GsSanctumMessageParam.h"
#include "DataSchema/Sanctum/GsSchemaSanctumConfig.h"
#include "DataSchema/Sanctum/GsSchemaSanctumData.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIcontent/Tray/Invade/GsUITrayTickerInvadeOneText.h"
#include "Guild/GsGuildHelper.h"
#include "Net/GsNetSendServiceWorld.h"
#include "UTIL/GsDir.h"
#include "T1Project.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"


bool FGsSanctumHandler::ShowSanctumMessageTicker(int32 InResult)
{
	PD::Result result = static_cast<PD::Result>(InResult);

	FText textMsg;
	switch (result)
	{
	case PD::Result::SANCTUM_ERROR_GUILD_GRADE:
	case PD::Result::SANCTUM_ERROR_UNAUTHORIZED:
		// TEXT: 권한이 없습니다
		FText::FindText(TEXT("GuildText"), TEXT("Alert_NoAuthority"), textMsg);
		break;
	case PD::Result::SANCTUM_ERROR_INVALID_AREA:
		// TEXT: 설치 불가능한 지역입니다
		FText::FindText(TEXT("SanctumUIText"), TEXT("SANCTUM_ERROR_INVALID_AREA"), textMsg);
		break;
	case PD::Result::SANCTUM_ERROR_SPAWN_SANCTUM:
	case PD::Result::SANCTUM_ERROR_EXIST_NEXUS:
		// TEXT: 이미 성소가 설치되어 있습니다
		FText::FindText(TEXT("SanctumUIText"), TEXT("SANCTUM_ERROR_SPAWN_SANCTUM"), textMsg);
		break;
	case PD::Result::SANCTUM_ERROR_SANCTUM_DESTROYING:
		// TEXT: 성소가 파괴되는 중입니다
		FText::FindText(TEXT("SanctumUIText"), TEXT("SANCTUM_ERROR_SANCTUM_DESTROYING"), textMsg);
		break;
	case PD::Result::SANCTUM_ERROR_ALREADY_INSTALLED:
	case PD::Result::INTERACT_ERROR_INVALID_NPC:
		// TEXT: 성소에 이미 수호탑이 존재합니다
		FText::FindText(TEXT("SanctumUIText"), TEXT("Occupy_Fail_NotEmpty"), textMsg);
		break;
	case PD::Result::SANCTUM_ERROR_BATTLE_BEGIN:
		// TEXT: 성소 점령전이 진행중이지 않습니다
		FText::FindText(TEXT("SanctumUIText"), TEXT("SANCTUM_ERROR_BATTLE_BEGIN"), textMsg);
		break;
	case PD::Result::GUILD_ERROR_INVALID_USER:
		// TEXT: 유효하지 않은 요청입니다.
		FText::FindText(TEXT("GuildText"), TEXT("GUILD_ERROR_INVALID_USER"), textMsg);
		break;
	}

	if (false == textMsg.IsEmpty())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return true;
	}

	return false;
}

void FGsSanctumHandler::Init()
{
	BindMessages();
}

void FGsSanctumHandler::Close()
{
	UnbindMessages();

	_bReserveInateraction = false;
}

void FGsSanctumHandler::OnReconnection()
{
	_bReserveInateraction = false;
}

void FGsSanctumHandler::BindMessages()
{
	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
	{
		return;
	}

	MSanctum& msgSanctum = msgHolder->GetSanctum();

	_msgHandleSanctumList.Emplace(msgSanctum.AddRaw(MessageContentSanctum::SANCTUM_BATTLE_FIRST_NOTIFY,
		this, &FGsSanctumHandler::OnBattleFirstNotify));
	_msgHandleSanctumList.Emplace(msgSanctum.AddRaw(MessageContentSanctum::SANCTUM_BATTLE_FINAL_NOTIFY,
		this, &FGsSanctumHandler::OnBattleFinalNotify));
	_msgHandleSanctumList.Emplace(msgSanctum.AddRaw(MessageContentSanctum::SANCTUM_BATTLE_START,
		this, &FGsSanctumHandler::OnBattleStart));
	_msgHandleSanctumList.Emplace(msgSanctum.AddRaw(MessageContentSanctum::SANCTUM_BATTLE_BEFORE_END_NOTIFY,
		this, &FGsSanctumHandler::OnBattleBeforeEndNotify));
	_msgHandleSanctumList.Emplace(msgSanctum.AddRaw(MessageContentSanctum::SANCTUM_BATTLE_END,
		this, &FGsSanctumHandler::OnBattleEnd));
	_msgHandleSanctumList.Emplace(msgSanctum.AddRaw(MessageContentSanctum::SANCTUM_BATTLE_OCCUPY_SUCCESS,
		this, &FGsSanctumHandler::OnBattleOccupySuccess));
	_msgHandleSanctumList.Emplace(msgSanctum.AddRaw(MessageContentSanctum::TRY_START_CONSTRUCT,
		this, &FGsSanctumHandler::OnTryStartConstruct));
	_msgHandleSanctumList.Emplace(msgSanctum.AddRaw(MessageContentSanctum::CANCEL_RESERVED_INTERACTION,
		this, &FGsSanctumHandler::OnCancelReservedInteraction));
	_msgHandleSanctumList.Emplace(msgSanctum.AddRaw(MessageContentSanctum::TRY_START_RESERVED_INTERACTION,
		this, &FGsSanctumHandler::OnTrayStartReservedInteraction));
	_msgHandleSanctumList.Emplace(msgSanctum.AddRaw(MessageContentSanctum::INVALIDATE_SANCTUM_STATE,
		this, &FGsSanctumHandler::OnInvalidateSanctumState));
}

void FGsSanctumHandler::UnbindMessages()
{
	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
	{
		return;
	}

	MSanctum& msgSanctum = msgHolder->GetSanctum();
	for (MsgHandleSanctum& msgHandle : _msgHandleSanctumList)
	{
		msgSanctum.Remove(msgHandle);
	}
	_msgHandleSanctumList.Empty();
}

void FGsSanctumHandler::OnBattleFirstNotify(const IGsMessageParam*)
{
	// TEXT: {0} 후 성소 점령전이 시작됩니다
	FText textFormat;
	if (FText::FindText(TEXT("SanctumUIText"), TEXT("Alert_World_Before_Battle1"), textFormat))
	{
		FString strTime;
		if (const FGsSchemaSanctumConfig* sanctumConfig = FGsSanctumManager::GetTableSanctumConfig())
		{
			FTimespan timeData = FTimespan::FromSeconds(sanctumConfig->battleFirstNotifySec);
			FGsTimeStringHelper::GetTimeString(timeData, strTime);
		}

		FText textMsg = FText::Format(textFormat, FText::FromString(strTime));
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}
}

void FGsSanctumHandler::OnBattleFinalNotify(const IGsMessageParam*)
{
	if (FGsSanctumManager* sanctumMgr = GSSanctum())
	{
		sanctumMgr->PrepareBattle();
	}

	// TEXT: {0} 후 성소 점령전이 시작됩니다. 각 성소의 수호탑이 파괴됩니다.
	FText textFormat;
	if (FText::FindText(TEXT("SanctumUIText"), TEXT("Alert_World_Before_Battle2"), textFormat))
	{
		FString strTime;
		if (const FGsSchemaSanctumConfig* sanctumConfig = FGsSanctumManager::GetTableSanctumConfig())
		{
			FTimespan timeData = FTimespan::FromSeconds(sanctumConfig->battlePrepareSec);
			FGsTimeStringHelper::GetTimeString(timeData, strTime);
		}

		FText textMsg = FText::Format(textFormat, FText::FromString(strTime));
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}
}

void FGsSanctumHandler::OnBattleStart(const IGsMessageParam*)
{
	FSoftObjectPath iconPath;
	if (const FGsSchemaSanctumConfig* sanctumConfig = FGsSanctumManager::GetTableSanctumConfig())
	{
		iconPath = sanctumConfig->pathIconNormal;

		if (UGsSoundPlayer* soundPlayer = GSound()->GetOrCreateSoundPlayer())
		{
			soundPlayer->PlaySound2D(sanctumConfig->tickerSoundStart.GetRow());
		}
	}

	// TEXT: 성소 점령전이 시작됩니다	
	FText textMsgFx;
	FText::FindText(TEXT("SanctumUIText"), TEXT("Alert_World_BattleStart_FX"), textMsgFx);
	FGsUIHelper::TrayTickerInvadeOneText(textMsgFx, iconPath);

	// TEXT: {0} 동안 각 지역의 성소가 점령 가능한 상태로 변경됩니다.
	FText textFormat;
	if (FText::FindText(TEXT("SanctumUIText"), TEXT("Alert_World_BattleStart"), textFormat))
	{
		FString strTime;
		if (const FGsSchemaSanctumConfig* sanctumConfig = FGsSanctumManager::GetTableSanctumConfig())
		{
			FTimespan timeData = FTimespan::FromSeconds(sanctumConfig->battelDurationSec);
			FGsTimeStringHelper::GetTimeString(timeData, strTime);
		}

		FText textMsg = FText::Format(textFormat, FText::FromString(strTime));
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}
}

void FGsSanctumHandler::OnBattleBeforeEndNotify(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>();
	if (nullptr == param)
	{
		return;
	}	

	// 종료 전 알림 상황
	if (0 < param->_data)
	{
		// TEXT: {0} 후 성소 점령전이 종료됩니다
		FText textFormat;
		if (FText::FindText(TEXT("SanctumUIText"), TEXT("Alert_World_Battle_BeforeEnd"), textFormat))
		{
			FTimespan timeData = FTimespan::FromSeconds(param->_data);

			FString strTime;
			FGsTimeStringHelper::GetTimeString(timeData, strTime);

			FText textMsg = FText::Format(textFormat, FText::FromString(strTime));
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}
	}
}

void FGsSanctumHandler::OnBattleEnd(const IGsMessageParam*)
{
	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		TArray<UGsGameObjectBase*> sanctumList = gameObjectMgr->FindObjectArray(EGsGameObjectType::Sanctum);
		for (UGsGameObjectBase* obj : sanctumList)
		{
			if (obj->IsValidLowLevel())
			{
				if (UGsGameObjectSanctumBase* sanctumObj = obj->CastGameObject<UGsGameObjectSanctumBase>())
				{
					sanctumObj->OnBattleEnd();
				}
			}
		}
	}

	// 종료 된 상황. 종료 후 점령 성공 메시지가 바로 오므로 큐잉 처리
	GUI()->PushQueue(TEXT("TrayTickerInvadeOneText"), 0, [](UGsUIWidgetBase* InWidget)
		{
			if (UGsUITrayTickerInvadeOneText* tray = Cast<UGsUITrayTickerInvadeOneText>(InWidget))
			{
				FSoftObjectPath iconPath;
				if (const FGsSchemaSanctumConfig* sanctumConfig = FGsSanctumManager::GetTableSanctumConfig())
				{
					iconPath = sanctumConfig->pathIconNormal;

					if (UGsSoundPlayer* soundPlayer = GSound()->GetOrCreateSoundPlayer())
					{
						soundPlayer->PlaySound2D(sanctumConfig->tickerSoundEnd.GetRow());
					}
				}

				// TEXT: 성소 점령전이 종료되었습니다
				FText textMsg;
				FText::FindText(TEXT("SanctumUIText"), TEXT("Alert_World_BattleEnd_FX"), textMsg);

				tray->SetInvadeOneTextData(textMsg, iconPath);
			}
		});
	
}

void FGsSanctumHandler::OnBattleOccupySuccess(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsSanctumMsgParamSanctumId* param = InParam->Cast<const FGsSanctumMsgParamSanctumId>();
	if (nullptr == param)
	{
		return;
	}

	// 레드닷 체크를 위해 시간 저장 후 갱신 요청
	GGuild()->SetSanctumOccupySuccessTime();
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, false);

	int32 sanctumId = param->_sanctumId;

	// 종료 후 점령 성공 메시지가 바로 오므로 큐잉 처리
	GUI()->PushQueue(TEXT("TrayTickerInvadeOneText"), 1, [sanctumId](UGsUIWidgetBase* InWidget)
		{
			if (UGsUITrayTickerInvadeOneText* tray = Cast<UGsUITrayTickerInvadeOneText>(InWidget))
			{
				if (const FGsSchemaSanctumData* sanctumData = FGsSanctumManager::GetTableSanctumData(sanctumId))
				{
					FSoftObjectPath iconPath;
					if (const FGsSchemaSanctumConfig* sanctumConfig = FGsSanctumManager::GetTableSanctumConfig())
					{
						iconPath = sanctumConfig->pathIconWin;

						if (UGsSoundPlayer* soundPlayer = GSound()->GetOrCreateSoundPlayer())
						{
							soundPlayer->PlaySound2D(sanctumConfig->tickerSoundWin.GetRow());
						}
					}

					// TEXT: {0} 점령에 성공했습니다
					FText textMsg;
					FText textFormat;
					if (FText::FindText(TEXT("SanctumUIText"), TEXT("Alert_World_BattleEnd_Guild_FX"), textFormat))
					{
						textMsg = FText::Format(textFormat, sanctumData->nameText);
					}

					tray->SetInvadeOneTextData(textMsg, iconPath);
				}				
			}
		});
}

void FGsSanctumHandler::OnTryStartConstruct(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsSanctumMsgParamSanctumId* param = InParam->Cast<const FGsSanctumMsgParamSanctumId>();
	if (nullptr == param)
	{
		return;
	}

	if (INVALID_SANCTUM_AREA_ID == param->_sanctumId ||
		0 == param->_gameId)
	{
		return;
	}
	
	// 권한 체크
	if (false == CheckAuthority(true))
	{
		return;
	}

	// 성소 요구레벨 체크
	FText textSanctumName;
	if (const FGsSchemaSanctumData* sanctumData = FGsSanctumManager::GetTableSanctumData(param->_sanctumId))
	{
		if (sanctumData->needNexusLevel > GGuild()->GetCampLevel())
		{
			// TEXT: 수호탑 레벨이 부족해 성소 점령에 실패했습니다
			FText textMsg;
			FText::FindText(TEXT("SanctumUIText"), TEXT("Occupy_Fail_NeedLevel"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

			return;
		}

		textSanctumName = sanctumData->nameText;
	}

	// TEXT: 성소 점령을 시작하시겠습니까?
	FText textMsg;
	FText::FindText(TEXT("SanctumUIText"), TEXT("Occupy_Begin_PopUp"), textMsg);

	SanctumAreaId sanctumId = param->_sanctumId;
	int64 gameId = param->_gameId;

	FGsUIHelper::PopupYesNoTwoMessage(textSanctumName, textMsg, [this, sanctumId, gameId](bool bIsOk)
		{
			if (bIsOk)
			{
				SendStartConstruction(sanctumId, gameId);
			}
		});
}

void FGsSanctumHandler::OnCancelReservedInteraction(const IGsMessageParam*)
{
	_bReserveInateraction = false;
}

void FGsSanctumHandler::OnTrayStartReservedInteraction(const IGsMessageParam* InParam)
{
	// 예약이 없거나 파라미터 정보가 없으면 무시
	if (false == _bReserveInateraction ||
		nullptr == InParam)
	{
		return;
	}

	const FGsSanctumMsgParamSanctumId* param = InParam->Cast<const FGsSanctumMsgParamSanctumId>();
	if (nullptr == param)
	{
		return;
	}

	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		if (UGsGameObjectBase* sanctumObjBase = gameObjectMgr->FindObject(EGsGameObjectType::Sanctum, param->_gameId))
		{
			if (UGsGameObjectSanctumBase* sanctumObj = sanctumObjBase->CastGameObject<UGsGameObjectSanctumBase>())
			{
				// 예약 정보를 돌려놓고 인터렉션 시도
				sanctumObj->OnClickInteract(false);
			}
		}
	}
}

void FGsSanctumHandler::OnInvalidateSanctumState(const IGsMessageParam* InParam)
{
	UGsGameObjectManager* gameObjectMgr = GSGameObject();
	if (nullptr == gameObjectMgr)
	{
		return;
	}

	TArray<UGsGameObjectBase*> sanctumList = gameObjectMgr->FindObjectArray(EGsGameObjectType::Sanctum);
	for (UGsGameObjectBase* obj : sanctumList)
	{
		if (UGsGameObjectSanctumBase* sanctumObj = obj->CastGameObject<UGsGameObjectSanctumBase>())
		{
			sanctumObj->UpdateInteractionState();
			sanctumObj->UpdateWidgetShowHPBar();
		}
	}
}

void FGsSanctumHandler::SendStartConstruction(SanctumAreaId InSanctumId, int64 InGameId)
{
	// 권한 체크
	if (false == CheckAuthority(true))
	{
		return;
	}

	UGsGameObjectManager* gameObjectMgr = GSGameObject();
	if (nullptr == gameObjectMgr)
	{
		return;
	}

	UGsGameObjectBase* localBase = gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == localBase)
	{
		return;
	}

	UGsGameObjectBase* sanctumObj = gameObjectMgr->FindObject(EGsGameObjectType::Sanctum, InGameId);
	if (nullptr == sanctumObj)
	{
		// TEXT : 성소에 이미 수호탑이 존재합니다
		FText textMsg;
		FText::FindText(TEXT("SanctumUIText"), TEXT("Occupy_Fail_NotEmpty"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return;
	}

	// 거리 체크
	if (false == CheckDistance(localBase, sanctumObj, true))
	{
		return;
	}

	// 설치 후 인터렉션 예약
	_bReserveInateraction = true;

	// 설치 요청
	FVector dir = localBase->GetRotation().Vector();
	FGsNetSendServiceWorld::SendReqSanctumNexusInstall(InSanctumId, InGameId,
		localBase->GetLocation(), FGsDir::dirToAngle(dir));
}

bool FGsSanctumHandler::CheckAuthority(bool bShowMsg)
{
	if (false == FGsGuildHelper::CanManageCamp(GGuild()->GetAuthority()))
	{
		if (bShowMsg)
		{
			// TEXT: 권한이 없습니다
			FText textMsg;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_NoAuthority"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}

		return false;
	}
	
	return true;
}

bool FGsSanctumHandler::CheckDistance(UGsGameObjectBase* InLocalPlayerObj, UGsGameObjectBase* InSanctumObj, bool bShowMsg)
{
	int32 range = 0;
	if (UGsGameObjectSanctumBase* sanctumBase = Cast<UGsGameObjectSanctumBase>(InSanctumObj))
	{
		range = sanctumBase->GetInteractionRange();
	}

	float rangeSquared = static_cast<float>(range * range);
	float distSquared = FVector::DistSquared2D(InLocalPlayerObj->GetLocation(), InSanctumObj->GetLocation());
	if (rangeSquared < distSquared)
	{
		if (bShowMsg)
		{
			// TEXT: 거리가 멀어 점령이 불가능합니다
			FText textMsg;
			if (FText::FindText(TEXT("SanctumUIText"), TEXT("Alert_Impossible_Range"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}

		return false;
	}

	return true;
}
