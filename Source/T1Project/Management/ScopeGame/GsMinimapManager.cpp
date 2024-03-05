#include "GsMinimapManager.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Map/Minimap/MapIcon/GsMinimapIconObject.h"
#include "Map/Minimap/MapIcon/GsMinimapIconLocal.h"
#include "Map/Minimap/MapIcon/GsMinimapIconOther.h"
#include "Map/Minimap/MapIcon/GsMinimapIconNpc.h"
#include "Map/Minimap/SceneImage/GsMinimapSceneImage.h"
#include "Map/Minimap/GsMinimapFunc.h"
#include "Map/Minimap/MapIcon/GsMinimapIconHandler.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"

#include "Net/GsNetSendServiceWorld.h"

#include "DataSchema/icon/GsSchemaMinimapIconData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcFunctionData.h"

#include "UTIL/GsText.h"
#include "UTIL/GsTableUtil.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UILib/Container/GsWidgetPool.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UIContent/HUD/Minimap/GsUIMinimapIcon.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Factory/GsIconFactory.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "T1Project.h"

void UGsMinimapManager::Initialize()
{
	InitializeMessage();

	_sceneImage = NewObject<UGsMinimapSceneImage>();
	_sceneImage->Initialize();

	_iconHandler = NewObject<UGsMinimapIconHandler>();
	_iconHandler->Initialize();

}
void UGsMinimapManager::Finalize()
{
	FinalizeMessage();

	if (_sceneImage != nullptr)
	{
		_sceneImage->Finalize();
		_sceneImage = nullptr;
	}

	if (_iconHandler != nullptr)
	{
		_iconHandler->Finalize();
		_iconHandler = nullptr;
	}

}
void UGsMinimapManager::Update(float In_delta)
{

	bool isLocalMoved = false;
	if (_centerPlayer != nullptr)
	{
		FVector nowPos = _centerPlayer->GetLocation();
		isLocalMoved = !nowPos.Equals(_oldLocalPos);
		if (isLocalMoved)
		{
			_oldLocalPos = nowPos;
		}
	}

	// scene imange not init size 
	// 1. init image size
	// 2. update icon

	bool sceneImageInitSize = true;
	// 지도 갱신
	// 지도는 로컬이 이동할때만 갱신
	if (_sceneImage != nullptr)		
	{
		sceneImageInitSize = _sceneImage->GetIsInitSize();
		if (isLocalMoved == true ||
			sceneImageInitSize == false)
		{
			_sceneImage->Tick(In_delta);
		}
	}

	_iconHandler->Update(In_delta, (isLocalMoved|| false == sceneImageInitSize));

}
void UGsMinimapManager::InitializeMessage()
{

}
void UGsMinimapManager::FinalizeMessage()
{
}

// 캐릭터 세팅
void UGsMinimapManager::SetCenterObject(UGsGameObjectBase* In_centerObj)
{
	_centerPlayer = In_centerObj;
}

// 캐릭터 삭제될때
void UGsMinimapManager::RemoveCenterObject()
{
	_centerPlayer = nullptr;
}

// 이벤트 시작
void UGsMinimapManager::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	// TEXT: {0} 채널로 이동하시겠습니까?
	FText messageFormat;
	FText::FindText(TEXT("UICommonText"), TEXT("ChannelChangeConfirm"), messageFormat);

	// 채널 이동 팝업 띄움
	FGsUIHelper::PopupYesNo(FText::Format(messageFormat, _reserveChannelId), [this](bool bYes)
		{
			if (bYes)
			{
				SendRequestSelectChannel();
			}
		}, [this](bool bIsCallEventProcessFinish)
		{
			FGsEventProgressManager* eventProgressManager = GSEventProgress();
			if (eventProgressManager != nullptr)
			{
				if (bIsCallEventProcessFinish == false)
				{
					// 성공하면 예약 이벤트 클리어
					// 워프가야되어서 다음꺼 진행하면 안됨
					eventProgressManager->ClearAllData();
				}
				else
				{
					// 이벤트 진행 순서에 전송(다음 이벤트 진행)
					eventProgressManager->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_YES_NO_CHNNEL_MOVE);
				}
			}
		}
		);
}
// 이벤트 종료
void UGsMinimapManager::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	GUI()->CloseByKeyName(TEXT("PopupYesNo"));
}

void UGsMinimapManager::ReserveChannelMove(int In_channelId, 
	bool In_reserveIsCurrentChannel,
	ChannelCongestionStatus In_reserveChannelState)
{
	_reserveChannelId = In_channelId;
	_reserveIsCurrentChannel = In_reserveIsCurrentChannel;
	_reserveChannelState = In_reserveChannelState;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_YES_NO_CHNNEL_MOVE, this);
}
void UGsMinimapManager::SendRequestSelectChannel()
{
	// 채널 이동 확인 팝업을 오랫동안 띄워놓고 있으면 상태가 바뀔 수 있으므로 한 번 더 체크한다
	if (false == CheckCanChangeChannel())
	{
		return;
	}

	// 채널이동 패킷 전송
	FGsNetSendServiceWorld::SendRequestSelectChannel(_reserveChannelId);
}
bool UGsMinimapManager::CheckCanChangeChannel()
{
	// 현재 채널일 경우 버튼 막기
	if (_reserveIsCurrentChannel)
	{
		return false;
	}

	// 꽉참일 경우 이동 불가 팝업 띄움
	if (ChannelCongestionStatus::FULL == _reserveChannelState)
	{
		// TEXT: 해당 채널에 유저가 많아서 이동할 수 없습니다.
		FText message;
		FText::FindText(TEXT("UICommonText"), TEXT("ChannelChangeDeny"), message);

		FGsUIHelper::PopupOK(message);
		return false;
	}

	// 쿨타임 체크. 쿨타임은 팝업에서 보여주고 있으므로 별도 안내가 필요 없음
	if (false == GGameData()->CanChangeChannel())
	{
		return false;
	}

	// 내 캐릭터가 평화 상태인지 체크
	if (false == IsPeaceState())
	{
		// TEXT: 전투 중에는 채널을 이동할 수 없습니다.
		FText message;
		FText::FindText(TEXT("UICommonText"), TEXT("ChannelCombatDeny"), message);

		FGsUIHelper::PopupOK(message);
		return false;
	}

	return true;
}
bool UGsMinimapManager::IsPeaceState()const
{
	if (UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>())
		{
			// 전투상태면 불가
			if (local->IsBattleMode())
			{
				return false;
			}
		}
	}

	return true;
}

