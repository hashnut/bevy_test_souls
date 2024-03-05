// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSpaceCrackManager.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"

#include "Map/SpaceCrack/GsSchemaSpaceCrackData.h"

#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "SpaceCrack/GsSpaceCrackData.h"

const FString SPCAE_CRACK_TEXT = TEXT("SpaceCrackText");
const FTextKey NOTIC_TICKER_MESSAGE_ACTIVATION_TEXT_KEY = TEXT("Notice_TickerMessage_Activation");
const FTextKey NOTIC_SYSTEM_MESSAGE_ACTIVATION_TEXT_KEY = TEXT("Notice_SystemMessage_Activation");

void FGsSpaceCrackManager::Initialize()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
				&FGsSpaceCrackManager::OnLocalPlayerSpawnComplete));

		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_REVIVE, this,
				&FGsSpaceCrackManager::OnLocalPlayerRevive));
	}
}

void FGsSpaceCrackManager::Finalize()
{		
	_mapSpaceCrackAlarmInfo.Empty();
	_mapSpaceCrackActivationInfo.Empty();

	if (FGsMessageHolder* msg = GMessage())
	{
		for (MsgGameObjHandle& DelegatePair : _gameObjectMsg)
		{
			msg->GetGameObject().Remove(DelegatePair);
		}		

		for (const MsgSpceCrackHandle& DelegatePair : _spaceCrackMsg)
		{
			msg->GetSpaceCrack().Remove(DelegatePair);
		}		
	}	

	_gameObjectMsg.Empty();
	_spaceCrackMsg.Empty();
}

void FGsSpaceCrackManager::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	/*_mapSpaceCrackAlarmInfo.Empty();
	_mapSpaceCrackActivationInfo.Empty();*/

	// 미니맵 시공의틈새 아이콘 업데이트
	UpdateMinimapSpaceCrackIcon();
}

void FGsSpaceCrackManager::OnLocalPlayerRevive(const IGsMessageParam*)
{
	UpdateMinimapSpaceCrackIcon();
}

void FGsSpaceCrackManager::UpdateMinimapSpaceCrackIcon()
{	
	int32 currentLevelId = 0;
	UGsLevelManager* level = GLevel();
	if (nullptr == level)
	{
		return;
	}		

	currentLevelId = level->GetCurrentLevelId();

	TArray<int64> currentMapPortalIdList;
	for (auto& iter : _mapSpaceCrackAlarmInfo)
	{
		TSharedPtr<FGsSpaceCrackAlarmInfo> info = iter.Value;
		if (info.IsValid())
		{
			if (currentLevelId == info->_regionMapId)
			{
				currentMapPortalIdList.Add(iter.Key);
			}
		}
	}

	if (0 < currentMapPortalIdList.Num())
	{		
		if (false == GGameData()->IsInvadeWorld())
		{
			// 아이콘 보임
			FGsUIMsgParamSpaceCrackMinimapIcon param(true, true, false);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPACE_CRACK_BUTTON_ACTIVE, &param);
		}
		else
		{
			FGsUIMsgParamSpaceCrackMinimapIcon param(false, false, false);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPACE_CRACK_BUTTON_ACTIVE, &param);
		}
	}	
}

// 시공의 틈새 인포 추가
void FGsSpaceCrackManager::AddSpaceCrackAlarmInfo(const SpaceCrackAlarm& inAlarmInfo)
{
	AddSpaceCrackAlarmInfo(inAlarmInfo.mMapId
		, inAlarmInfo.mFenceId
		, inAlarmInfo.mTicker
		, FVector(inAlarmInfo.mX, inAlarmInfo.mY, inAlarmInfo.mZ));
}

void FGsSpaceCrackManager::AddSpaceCrackAlarmInfo(MapId inRegionMapId
	, FenceId inFenceId
	, bool inAlarm
	, const FVector& inPos)
{
	TSharedPtr<FGsSpaceCrackAlarmInfo> findInfo = _mapSpaceCrackAlarmInfo.FindRef(inRegionMapId);
	if (false == findInfo.IsValid())
	{
		findInfo = MakeShareable(new FGsSpaceCrackAlarmInfo(inRegionMapId, inFenceId, inPos));
		_mapSpaceCrackAlarmInfo.Add(inRegionMapId, findInfo);
	}
	else
	{		
		findInfo->_regionMapId = inRegionMapId;		
		findInfo->_fenceId = inFenceId;
		findInfo->_pos = inPos;
	}

	if (GGameData()->IsInvadeWorld())
	{
		return;
	}

	if (inAlarm)
	{
		FText tickerMessageText;
		FText::FindText(SPCAE_CRACK_TEXT, NOTIC_TICKER_MESSAGE_ACTIVATION_TEXT_KEY, tickerMessageText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, tickerMessageText);

		const FGsSchemaMapData* mapData = GLevel()->GetMapData(inRegionMapId);
		if (mapData)
		{
			FText format;
			FText::FindText(SPCAE_CRACK_TEXT, NOTIC_TICKER_MESSAGE_ACTIVATION_TEXT_KEY, format);
			FText chatMessageText = FText::Format(format, mapData->MapStringId);
			FGsUIHelper::AddChatMessageSystem(chatMessageText);
		}
	}	

	FGsMapSpaceCrackAlarmParam param(inRegionMapId
		, inPos);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_SPACE_CRACK_PORTAL, &param);

	// 미니맵 시공의틈새 아이콘 업데이트
	UpdateMinimapSpaceCrackIcon();
}

// 시공의 틈새 인포 삭제
void FGsSpaceCrackManager::DelSpaceCrackPortalInfo(MapId inMapId)
{
	TSharedPtr<FGsSpaceCrackAlarmInfo> findInfo = _mapSpaceCrackAlarmInfo.FindRef(inMapId);
	if (false == findInfo.IsValid())
	{
		return;
	}

	int mapId = 0;
	int channelId = 0;	

	if (findInfo.IsValid())
	{
		mapId = findInfo->_regionMapId;		
	}
	_mapSpaceCrackAlarmInfo.Remove(inMapId);

	FGsMapSpaceCrackAlarmParam param(inMapId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_SPACE_CRACK_PORTAL, &param);	
	
	// 미니맵 시공의틈새 아이콘 업데이트
	UpdateMinimapSpaceCrackIcon();	
}

void FGsSpaceCrackManager::AddSpaceCrackActivationInfo(const SpaceCrackActivation& inActivate)
{
	TSharedPtr<FGsSpaceCrackActivationInfo> findInfo = _mapSpaceCrackActivationInfo.FindRef(inActivate.mMapId);
	if (false == findInfo.IsValid())
	{
		findInfo = MakeShareable(new FGsSpaceCrackActivationInfo(inActivate));
		_mapSpaceCrackActivationInfo.Add(inActivate.mMapId, findInfo);
	}
	else
	{
		findInfo.Get()->Initialize(inActivate);
	}
}

void FGsSpaceCrackManager::DelSpaceCrackActivationInfo(MapId inMapId)
{
	TSharedPtr<FGsSpaceCrackActivationInfo> findInfo = _mapSpaceCrackActivationInfo.FindRef(inMapId);
	if (false == findInfo.IsValid())
	{
		return;
	}

	_mapSpaceCrackActivationInfo.Remove(inMapId);
}

void FGsSpaceCrackManager::UpdateSpaceCrackActivationInfo(const SpaceCrackActivation& inActivate)
{
	TSharedPtr<FGsSpaceCrackActivationInfo> findInfo = _mapSpaceCrackActivationInfo.FindRef(inActivate.mMapId);
	if (false == findInfo.IsValid())
	{
		return;
	}

	findInfo->Initialize(inActivate);
}

const FGsSpaceCrackAlarmInfo* FGsSpaceCrackManager::FindSpaceCrackAlarmInfoByMapId(MapId inMapId)
{
	if (_mapSpaceCrackAlarmInfo.Contains(inMapId))
	{
		return _mapSpaceCrackAlarmInfo.FindRef(inMapId).Get();
	}

	return nullptr;
}

bool FGsSpaceCrackManager::IsSpaceCrackSpawn(MapId inMapId)
{
	return (nullptr != FindSpaceCrackAlarmInfoByMapId(inMapId));
}

const TMap<MapId, TSharedPtr<FGsSpaceCrackAlarmInfo>>& FGsSpaceCrackManager::GetSpaceCrackAlarmInfoMap()
{
	return _mapSpaceCrackAlarmInfo;
}

const FGsSpaceCrackActivationInfo* FGsSpaceCrackManager::FindSpaceCrackActivationInfoByMapId(MapId inMapId)
{
	if (_mapSpaceCrackActivationInfo.Contains(inMapId))
	{
		return _mapSpaceCrackActivationInfo.FindRef(inMapId).Get();
	}

	return nullptr;
}

int64 FGsSpaceCrackManager::GetCurrentActivateSpaceCrack()
{
	//get current level
	int32 currentLevel = GLevel()->GetCurrentLevelId();

	if (false == _mapSpaceCrackAlarmInfo.Contains(currentLevel))
	{
		return 0;
	}

	return _mapSpaceCrackAlarmInfo[currentLevel]->_fenceId;
}

bool FGsSpaceCrackManager::IsCurrentMapActiveSpaceCrack()
{
	int32 currentLevel = GLevel()->GetCurrentLevelId();
	return _mapSpaceCrackAlarmInfo.Contains(currentLevel);
}

bool FGsSpaceCrackManager::TryGetAllSpaceCrackByMapId(MapId inMapId, OUT TArray<FGsSchemaMapTerritorySpaceCrack>& outSpaceCrackArray)
{
	const FGsSchemaMapData* findMapData = GLevel()->GetMapData(inMapId);
	if (nullptr == findMapData)
	{
		return false;
	}

	outSpaceCrackArray = findMapData->territoryInfo.GetRow()->spaceCrackZoneList;

	return true;
}

const TMap<MapId, TSharedPtr<class FGsSpaceCrackActivationInfo>>& FGsSpaceCrackManager::GetSpaceCrackActivationInfoMap()
{
	return _mapSpaceCrackActivationInfo;
}
