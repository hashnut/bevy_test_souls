
#include "GsDungeonReserveWarpHandler.h"

#include "GsDungeonGroupBase.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGlobal/GsGameDataManager.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../Management/ScopeGlobal/GsQuestManager.h"
#include "../Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/Define/GsGameObjectDefine.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Input/GsInputBindingLocalPlayer.h"

#include "../Currency/GsCostPackage.h"
#include "../Currency/GsCostOption.h"
#include "../ContentsEvent/GsContentsEventTeleportCost.h"

#include "../UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "../UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"

#include "../Net/GsNetSendService.h"
#include "../Net/GsNetSendServiceWorld.h"

#include "UTIL/GsGameObjectUtil.h"






void FGsDungeonReserveWarpHandler::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	if (nullptr == _destGroupData)
	{
		// 2023/8/1 PKT - _destGroupData 이 nullptr이 상황이 있을 수 있음. 시네마틱 등의 상황에서 어드민 툴로 시련의 전당을 금지 시켰을 경우 시네마틱이 끝남과 동시에 
		//				  이쪽으로 콜 하게 되는데 이때 Data가 nullptr일수 있다. 때문에 로그 남기는 것은 삭제 하도록 함.
		//GSLOG(Error, TEXT("nullptr == _destGroupData"));
		return;
	}

	auto OKButtonClickedDelegate = [this](bool InIsOK)
	{
		if (!InIsOK)
			return;

		FText outErrorString;
		if (false == this->_destGroupData->IsValidEnterDateTime(outErrorString))
		{
			if (false == outErrorString.IsEmpty())
			{
				FGsUIHelper::PopupOK(outErrorString);
			}
			return;
		}

		// https://jira.com2us.com/jira/browse/C2URWQ-2476
		// check input lock(quest dialog server send -> try warp)
		if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
		{
			return;
		}

		int32 level = GGameData()->GetUserData()->mLevel;
		if (UGsLevelManager::IsCanSpotWarp(_mapId, _spotId, level/*, questId*/))
		{
			GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
			FGsNetSendServiceWorld::SendPortalWarp(_mapId, _spotId);
			GUI()->CloseAllStack();
		}
	};

	if (_destGroupData && DungeonCategory::ARENA == _destGroupData->GetCategory())
	{
		if (UGsUIPopupYesNoTwoMessage* popup =
				Cast<UGsUIPopupYesNoTwoMessage>(GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"))))
		{
			MapId warpToMapId = _mapId;
			popup->SetData(_enterMapText, _warpText, [warpToMapId](bool InIsOk)
				{
					if (InIsOk)
					{	// 2023/11/24 PKT - 기존 'OKButtonClickedDelegate'이것을 살릴려고 했으나 충돌되는 지점이 있고 해서 그냥 복사 붙여 넣기...
						if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
						{
							return;
						}

						FGsNetSendService::SendReqBattleArenaEnter(warpToMapId);
						
						GSEventProgress()->ClearAllData();
					}
					else
					{	// 이벤트 진행 순서에 전송(다음 이벤트 진행)
						GSEventProgress()->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_COMMON_PAY_DUNGEON_WARP);
					}
				}
			);
		}
	}
	else
	{
		if (_costType == CurrencyType::NONE)
		{
			if (UGsUIPopupYesNoTwoMessage* popup =
				Cast<UGsUIPopupYesNoTwoMessage>(GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"))))
			{
				popup->SetData(_warpText, FText::GetEmpty(), OKButtonClickedDelegate);
			}
		}
		else
		{
			if (UGsUIPopupCommonPay* popupCommonPay =
				Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay"))))
			{
				TSharedRef<FGsCostPackage> CostPackage(FGsCostPackage::CreateSimple(_costType, _costAmount));

				TSharedPtr<FGsCostOption> CostOption = FGsContentsEventTeleportCost::CreateActivatedTeleportCostOption();
				if (CostOption.IsValid())
				{
					// 텔레포트 비용 할인 객체가 있을 경우, 추가
					CostPackage->AddCostOption(CostOption.ToSharedRef());
				}

				popupCommonPay->SetData(_warpText, MoveTemp(CostPackage), OKButtonClickedDelegate,
					[this](bool bIsCallEventProcessFinish)
					{
						if (bIsCallEventProcessFinish == false)
						{
							// 성공하면 예약 이벤트 클리어
							// 워프가야되어서 다음꺼 진행하면 안됨
							GSEventProgress()->ClearAllData();
						}
						else
						{
							// 이벤트 진행 순서에 전송(다음 이벤트 진행)
							GSEventProgress()->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_COMMON_PAY_DUNGEON_WARP);
						}
					});
			}
		}
	}
}

void FGsDungeonReserveWarpHandler::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	GUI()->CloseByKeyName(TEXT("PopupCommonPay"));
	GUI()->CloseByKeyName(TEXT("PopupYesNoTwoMessage"));

	Clear();	
}

void FGsDungeonReserveWarpHandler::Clear()
{
	_mapId = INVALID_MAP_ID;
	_spotId = 0;
	_costType = CurrencyType::NONE;
	_costAmount = 0;
	_enterMapText = FText::GetEmpty();
	_warpText = FText::GetEmpty();
	_destGroupData = nullptr;
}

void FGsDungeonReserveWarpHandler::ReserveWarp(int32 InMapId, int32 InSpotId, CurrencyType InCostType, int32 InCostAmount, FText InWarpText 
	, const class FGsDungeonGroupBase* InDestGroupData)
{
	Clear();

	_mapId = InMapId;
	_spotId = InSpotId;
	_costType = InCostType;
	_costAmount = InCostAmount;
	_warpText = InWarpText;
	_destGroupData = InDestGroupData;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_COMMON_PAY_DUNGEON_WARP, this);
}

void FGsDungeonReserveWarpHandler::ReserveWarp(int32 InMapId, FText InTitle, FText InWarpText, const class FGsDungeonGroupBase* InDestGroupData)
{
	Clear();

	_mapId = InMapId;
	_enterMapText = InTitle;
	_warpText = InWarpText;
	_destGroupData = InDestGroupData;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_COMMON_PAY_DUNGEON_WARP, this);
}

void FGsDungeonReserveWarpHandler::ReserveCancel()
{
	GUI()->CloseByKeyName(TEXT("PopupCommonPay"));

	Clear();
}