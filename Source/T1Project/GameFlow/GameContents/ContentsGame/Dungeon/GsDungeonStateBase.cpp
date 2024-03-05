

#include "GsDungeonStateBase.h"
#include "Dungeon/GsDungeonGroupBase.h"
#include "Dungeon/GsDungeonHelper.h"

#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGame/GsDungeonManager.h"
#include "../Management/ScopeGame/GsUnlockManager.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../Management/ScopeGame/GsGameObjectManager.h"
#include "../Message/GsMessageContentDungeon.h"
#include "../Message/MessageParam/GsUIMessageParam.h"
#include "../UI/UILib/Base/GsUIWidgetBase.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "../Net/GsNetSendServiceWorld.h"
#include "../Net/GsNetSendServiceItem.h"

#include "T1Project.h"
#include "../GameObject/ObjectClass/GsGameObjectBase.h"
#include "../GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"


void FGsDungeonStateBase::Initialize()
{
	_groupId = INVALID_DUNGEON_GROUP_ID;
	_mapId = INVALID_MAP_ID;

	SendMessageHUDTimeMode(HUDTimeMode::Hidden);
}

void FGsDungeonStateBase::Finalize()
{
	_groupId = INVALID_DUNGEON_GROUP_ID;
	_mapId = INVALID_MAP_ID;

	SendMessageHUDTimeMode(HUDTimeMode::Hidden);
}

bool FGsDungeonStateBase::Enter(MapId InMapId)
{
	const FGsDungeonGroupBase* findData = GSDungeon()->FindGroupDataByMapId(InMapId);
	if (nullptr == findData || _category != findData->GetCategory())
	{
		GSLOG(Error, TEXT("nullptr == findData || _category != findData->GetCategory() : MapId : [%d]"), InMapId);
		return false;
	}

	_groupId = findData->GetGroupId();
	_mapId = InMapId;
	_progressState = EDungeonProgressState::EInDungeon;

	// 2022/10/31 PKT - Show HUD Time Mode
	SendMessageHUDTimeMode(GetHUDTimeMode());

	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		UnlockCategory unlockType = GetUnlockCategory();
		if (unlockType != UnlockCategory::NONE)
		{
			unlockManager->LockContentsByClient(unlockType);
		}
	}

	return true;
}

void FGsDungeonStateBase::Exit()
{
	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		UnlockCategory unlockType = GetUnlockCategory();
		if (unlockType != UnlockCategory::NONE)
		{
			unlockManager->UnlockContentsByClient(unlockType);
		}
	}

	if (_progressState != EDungeonProgressState::EInDungeon)
	{
		return;
	}

	_groupId = INVALID_DUNGEON_GROUP_ID;
	
	_mapId = INVALID_MAP_ID;
	
	_progressState = EDungeonProgressState::ENone;

	// 2022/10/31 PKT - Hidden HUD Time Mode
	SendMessageHUDTimeMode(GetHUDTimeMode());
}

void FGsDungeonStateBase::TryExit(ItemDBId InUseItemDBId)
{
	TFunction<void(bool)> ExitFunc = [InUseItemDBId](bool InIsOk)
	{
		if (false == InIsOk)
			return;

		if (INVALID_ITEM_DB_ID == InUseItemDBId)
		{	// 2022/08/05 PKT - 나가기 버튼을 통해 나가기 시도
			if (MapType::GUILD_WAR == GLevel()->GetCurrentMapType())
			{
				FGsNetSendServiceWorld::Send_ExitBattleArena();
			}
			else
			{
				FGsNetSendServiceWorld::Send_ExitDungeon();
			}			
		}
		else
		{	// 2022/08/05 PKT - Item 을 사용하여 나가기 시도(귀환 주문서)
			FGsNetSendServiceItem::SendUseItem(InUseItemDBId, ItemCategorySub::WARP_TO_TOWN_SCROLL);
		}
	};

	// 2023/9/18 PKT - 워프가 가능한 상황인가?( 파티 던전 에서는 기본적으로 워프 불가 상태지만 버튼으로 나가는건 동작되어야 해서 예외 사항을 둠. )
	if (_category != DungeonCategory::PARTY)
	{
		if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_WARP))
			{
				FText findText;
				FGsDungeonHelper::FindText(TEXT("WarpFailedAbnormality"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return;
			}
		}
	}

	FText message1 = FText::GetEmpty();
	FText message2 = FText::GetEmpty();

	// 2022/11/01 PKT - 던전 탈출에 대한 안내 팝업이 필요한가?
	bool isNotifyMessage = IsExitNotifyMessage(message1, message2);

	if (false == isNotifyMessage)
	{	// 2022/11/01 PKT - 바로 탈출
		ExitFunc(true);
	}
	else
	{
		// 2022/06/07 PKT - 안내 팝업창을 띄운다.
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
		if (!widget.IsValid())
		{
			GSLOG(Error, TEXT("!widget.IsValid()"));
			return;
		}

		if (UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(widget.Get()))
		{
			popup->SetData(message1, message2, ExitFunc);
		}
	}
}

void FGsDungeonStateBase::SendMessageHUDTimeMode(HUDTimeMode InHUDTimeMode)
{
	switch (InHUDTimeMode)
	{
	case HUDTimeMode::Play:
		GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::ON_HUD_TIME, nullptr);
		break;
	case HUDTimeMode::Pause:
		GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::PAUSE_HUD_TIME, nullptr);
		break;
	case HUDTimeMode::Hidden:
		GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::OFF_HUD_TIME, nullptr);
		break;
	}
}

FGsDungeonStateBase::HUDTimeMode FGsDungeonStateBase::GetHUDTimeMode() const
{
	if (INVALID_DUNGEON_GROUP_ID == _groupId)
	{
		return HUDTimeMode::Hidden;
	}

	return HUDTimeMode::Play;
}

// 2022/11/01 PKT - 각 던전에서 나갈 때 확인 창이 필요 한가를 묻는다.( Default - 확인창 없음. )
bool FGsDungeonStateBase::IsExitNotifyMessage(FText& InMessage1, FText& InMessage2)
{
	InMessage1 = FText::GetEmpty();
	InMessage2 = FText::GetEmpty();

	// 2022/11/01 PKT - Default 없음.
	return false;
}

// 2022/12/07 PKT - 던전 남은 시간 HUD에 출력
void FGsDungeonStateBase::SendMessageHUDTimeUpdate(FDateTime InDateTime, bool InIsLimitTime, bool InIsPause /*= false*/)
{
	FGsUIMsgParamDungeonTime param = FGsUIMsgParamDungeonTime(InDateTime, InIsLimitTime, false, InIsPause);
	GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::HUD_TIME, &param);
}

