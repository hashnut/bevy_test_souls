// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupTeleport.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"

#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UTIL/GsLevelUtil.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"
#include "T1Project.h"
#include "Quest/GsQuestData.h"
#include "Sequence/GsSequencePlayer.h"


void UGsUIPopupTeleport::SetData(TSharedRef<FGsCostPackage> InCostPackage, QuestKey inQuestKey, QuestObjectiveId inObjectiveId, bool inIsAccpeted)
{
	_costPackage = MoveTemp(InCostPackage);

	const FGsCostElement* CostElements = _costPackage->GetFirstCostElement();
	if (nullptr == CostElements || 0 == CostElements->GetCandidateCount())
	{
		GSLOG(Error, TEXT("nullptr == CostElements || 0 == CostElements->GetCandidateCount()"));
		return;
	}

	if (nullptr != _currencyTeleportGold)
	{
		_currencyTeleportGold->SetData(*CostElements);
		_currencyTeleportGold->UpdateAmountTextColorLocalPlayer();
	}

	_currencyLocalGold->SetData(CostElements->GetCurrencyType(), CostElements->PlayerHasAmountMax());

	_questKey = inQuestKey;
	_objectiveId = inObjectiveId;
	_isAccpeted = inIsAccpeted;

	_isCallEventProcessFinish = true;
}

void UGsUIPopupTeleport::OnInputOk()
{
	if (!_costPackage->PlayerHasEnough())
	{
		// 창 닫기
		Close(true);

		// R2: 구매 유도 팝업
		FGsCurrencyHelper::OpenLackCostPackagePopup(*_costPackage);
		return;
	}

	// 시네마틱 플레이 중이면?
	if (true == UGsSequencePlayer::IsSequencePlaying())
	{
		FText findText;
		FText::FindText(TEXT("NetText"), TEXT("WARP_ERROR_CINEMATIC_CANCLE"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		// 창 닫기
		Close(true);
		return;
	}

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
	{
		// 창 닫기
		Close(true);
		return;
	}

	if (FGsQuestHandler* questHandler = questManager->GetQuestHandler())
	{
		if (false == questHandler->IsPossibleQuestWarp(_questKey))
		{
			FText result;
			FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorTeleportInvalidState"), result);
			FGsUIHelper::TrayMessageTicker(result);

			// 창 닫기
			Close(true);
			return;
		}
	}

	FGsNetSendServiceWorld::SendQuestWarp(_questKey._questId, _questKey._questIndex, _objectiveId, _isAccpeted);

	questManager->SetWarpQuestId(_questKey);
	questManager->SetIsSpawnMeCompleteStartProgress(true);

	_isCallEventProcessFinish = false;

	// 창 닫기
	Close();
}

void UGsUIPopupTeleport::OnInputCancel()
{
	// 창 닫기
	Close();
}

void UGsUIPopupTeleport::OnClickOk()
{
	OnInputOk();
}

void UGsUIPopupTeleport::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupTeleport::NativeDestruct()
{
	FGsEventProgressManager* eventProgressManager = GSEventProgress();

	if(eventProgressManager != nullptr)
	{ 
		if (_isCallEventProcessFinish == false)
		{
			// 성공하면 예약 이벤트 클리어
			// 워프가야되어서 다음꺼 진행하면 안됨
			eventProgressManager->ClearAllData();
		}
		else
		{
			// 이벤트 진행 순서에 전송
			eventProgressManager->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_TELEPORT);
		}
	}

	Super::NativeDestruct();
}