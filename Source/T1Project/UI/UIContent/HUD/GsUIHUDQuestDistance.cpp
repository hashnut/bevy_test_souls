// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDQuestDistance.h"

#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionMoveType.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Movement/LocalPlayer/Auto/GsWaypointWork.h"

#include "UserWidget.h"
#include "Components/Image.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "TimerManager.h"
#include "T1Project.h"

#include "UTIL/GsTableUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcFunctionData.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "AI/Quest/GsAIQuestHandler.h"


void UGsUIHUDQuestDistance::NativeConstruct()
{
	// 타이머 등록
	GetWorld()->GetTimerManager().SetTimer(_updateDistTimerHandle, this, &UGsUIHUDQuestDistance::UpdateQuestDist, _distUpdateSec, true);

	// "자동 이동 {0}m 미리 테이블에서 찾아서 들고있는다.
	FText::FindText(TEXT("QuestUIText"), TEXT("Automaticmovement"), _findText);
	FText::FindText(TEXT("QuestUIText"), TEXT("QuestAutomaticmovement"), _findQuestText);

	Super::NativeConstruct();
}

void UGsUIHUDQuestDistance::NativeDestruct()
{
	_owner = nullptr;

	//타이머 해지	
	GetWorld()->GetTimerManager().ClearTimer(_updateDistTimerHandle);

	Super::NativeDestruct();
}

void UGsUIHUDQuestDistance::UpdateQuestDist()
{
	_distanceText = FText::GetEmpty();

	if (false == _owner.IsValid())
	{
		if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			_owner = local->CastGameObject<UGsGameObjectLocalPlayer>();
		}
	}
	
	float outDist = 0.0f;
	EGsMovementAutoContentsType outType = EGsMovementAutoContentsType::None;
	if (true == GetDistAndMovementAutoContentsType(outDist, outType))
	{
		if (outDist == 0)
		{
			_distanceText = FText::GetEmpty();
			return;
		}

		int dist = int(outDist * 0.01f);		
		if (0 < dist)
		{
			if (outType == EGsMovementAutoContentsType::QuestMove)
			{
				_distanceText = FText::Format(_findQuestText, FText::AsNumber(FMath::Max(dist, 0)));
			}
			else
			{
				_distanceText = FText::Format(_findText, FText::AsNumber(FMath::Max(dist, 0)));
			}
		}
	}
}

FGsAIQuestHandler* UGsUIHUDQuestDistance::GetAIQuestHandler()
{
	FGsAIManager* aiMgr = GSAI();
	if (nullptr == aiMgr)
		return nullptr;

	return aiMgr->GetAIQuestHandler();
}

// 거리값 가져오기
bool UGsUIHUDQuestDistance::GetDistAndMovementAutoContentsType(OUT float& Out_dist, OUT EGsMovementAutoContentsType& Out_movementAutoContentsType)
{
	if (false == _owner.IsValid())
		return false;

	FGsMovementHandlerLocalPlayer* movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movement == nullptr)
	{
		return false;
	}

	Out_movementAutoContentsType = movement->GetStateAutoMove().GetMovementAutoContentsType();
	if (Out_movementAutoContentsType == EGsMovementAutoContentsType::QuestMove)
	{
		FGsQuestHandler* questHandler = _owner->GetQuestHandler();
		if (nullptr == questHandler)
		{
			return false;
		}
		
		if (questHandler->GetAutoMoveRemainDistance(Out_dist))
		{
			return true;
		}
		else
		{			
			if (GSQuest()->IsMovingToNpc())
			{
				FGsInteractionHandlerLocalPlayer* interactionHander =
					_owner->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();

				if (interactionHander == nullptr)
				{
					return false;
				}

				if (true == interactionHander->GetNpcListMoveTargetDist(Out_dist))
				{
					return true;
				}
			}
		}
	}	
	// npc list 이동인가
	else if (Out_movementAutoContentsType == EGsMovementAutoContentsType::NpcListMove)
	{
		FGsInteractionHandlerLocalPlayer* interactionHander=
			_owner->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();

		if (interactionHander == nullptr)
		{
			return false;
		}

		if (true == interactionHander->GetNpcListMoveTargetDist(Out_dist))
		{
			return true;
		}
	}
	else if (Out_movementAutoContentsType == EGsMovementAutoContentsType::RegionMapBookmarkMove ||
		Out_movementAutoContentsType == EGsMovementAutoContentsType::RegionMapPortalMove ||
		Out_movementAutoContentsType == EGsMovementAutoContentsType::TouchUIMapMove ||
		Out_movementAutoContentsType == EGsMovementAutoContentsType::FieldBossMove)
	{		
		Out_dist =
			movement->GetStateAutoMove().GetWayPointMoveTargetDist();

		return true;		
	}
	
	return false;
}

void UGsUIHUDQuestDistance::ShowNpcListMoveInfo(int In_npcTblId)
{
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(In_npcTblId);
	if (npcData == nullptr)
	{
		return;
	}

	const FGsSchemaNpcFunctionData* npcFunctionData =
		UGsTableUtil::FindRowById<UGsTableNpcFunctionData, FGsSchemaNpcFunctionData>(npcData->npcFunctionType);

	if (npcFunctionData == nullptr)
	{
		return;
	}

	UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(npcFunctionData->_autoMoveIconImagePath);
	_imgTargetNpc->SetBrushFromAtlasInterface(spriteImg);

	_textNpcName = npcData->nameText;
	_textNpcFunctionDesc = npcData->NpcFunctionDesc;

	_panelNpcListMoveInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
void UGsUIHUDQuestDistance::HideNpcListMoveInfo()
{
	_panelNpcListMoveInfo->SetVisibility(ESlateVisibility::Collapsed);
}