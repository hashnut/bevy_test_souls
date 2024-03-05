// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLocalPlayerLooting.h"
#include "GsStateLocalPlayerHeader.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "GameObject/Looting/GsLootingHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "../../Quest/GsQuestHandler.h"
#include "../../ObjectClass/GsGameObjectLocalPlayer.h"
#include "Option/GsOptionFunc.h"

#include "Management/ScopeGlobal/GsSoundManager.h"

#include "Management/ScopeGame/GsAIManager.h"
#include "Item/GsItemManager.h"

#include "Sound/GsSoundPlayer.h"

#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"

#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"


uint8 FGsStateLocalPlayerLooting::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Looting);
}

FName FGsStateLocalPlayerLooting::GetStateName() const
{
	return TEXT("StateLocalPlayerLooting");
}

FName FGsStateLocalPlayerLooting::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerLooting::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_MOVE:
		GS_STATE_GROUP_ACTION:
		GS_STATE_GROUP_ACTION_END:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Idle:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::Warp:
			return true;
		default:
			return false;
		}
	}
	return false;
}

bool FGsStateLocalPlayerLooting::Ready(UGsGameObjectBase* Owner)
{
	if (Super::Ready(Owner))
	{
		UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
		if (FGsLootingHandlerLocalPlayer* lootingHandler = local->GetLootingHandler())
		{
			if (UGsGameObjectDropObject* lootTarget = lootingHandler->GetLootingTarget())
			{
				if (IsInsideInteractionRange(Owner, lootTarget))
				{
					// 타겟 방향으로 회전한다.			
					Owner->LookAtTarget(lootTarget, true);
				}
				else
				{
					lootingHandler->OnMoveToLoot(lootTarget);
					return false;
				}
			}
		}
	}
	return true;
}

void FGsStateLocalPlayerLooting::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	Enter(Owner);
}

void FGsStateLocalPlayerLooting::Enter(UGsGameObjectBase* _owner)
{
	Super::Enter(_owner);

	UGsGameObjectLocalPlayer* local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsGameObjectStateManager* fsm = _owner->GetBaseFSM();

	/* m6 버그 중
	*	A몬스터 3마리 처치*
		B몬스터 3마리 처치*
		이경우 A몬스터를 3마리째 처치하면 자동으로 B몬스터의 사냥터로 이동하는데
		현재는 A몬스터 3마리째 처치하는 순간 아이템이 드랍되면
		해당 아이템을 줍지않고 바로 B몬스터를 향해 자동진행의 이슈로 해당 코드를 주석함
		FGsQuestHandler* questHandler = local->GetQuestHandler();
		questHandler->SetAutoMoveStart();
	*/

	FGsLootingHandlerLocalPlayer* lootingHandler = local->GetLootingHandler();
	if (UGsGameObjectDropObject* dropObject = lootingHandler->GetLootingTarget())
	{
		const FGsLootingHandlerLocalPlayer::ReserveLootingInfo& lootingInfo = lootingHandler->GetReservedLootingInfo();

		bool isAutoLootPossible = true;
		if (lootingInfo._isAutoLooting == true)
		{
			bool isAutoLootWeightPossible = FGsOptionFunc::IsPossibleLootByWeightOption(dropObject->GetItemWeight());
			
			bool isAutoLootInvenSlotPossible = false;

			UGsItemManager* itemManager = GItem();
			if (itemManager != nullptr)
			{
				// check slot			
				isAutoLootInvenSlotPossible =
					itemManager->IsCanItemAdd(dropObject->GetItemId(), dropObject->GetItemAmount());
			}

			// check grade
			bool isAutoLootGradePossible = false;
			const FGsSchemaItemCommon* itemTable = dropObject->GetItemTable();
			if (itemTable != nullptr)
			{
				isAutoLootGradePossible = FGsOptionFunc::IsPossibleLootByGradeOption(itemTable->type, itemTable->grade);
			}
			isAutoLootPossible = (isAutoLootWeightPossible && isAutoLootInvenSlotPossible && isAutoLootGradePossible);
		}

		//If you can loot
		//player own drop object
		//client not notify to server that drop object is not looted
		//send loot drop object packet to server
		if (isAutoLootPossible == true)
		{
			if (dropObject->GetState() == EDropObjectState::Lootable)
			{
				float length = (_owner->GetLocation2D() - dropObject->GetLocation2D()).SizeSquared();
				float dropObjectLootingRangeFactor = FGsLootingHandlerLocalPlayer::GetLootRange(true);
				if (length < FMath::Square(dropObjectLootingRangeFactor))
				{
#if DROP_OBJECT_DEBUG								
					GSLOG(Log, TEXT("FGsStateLocalPlayerLooting::Enter - send loot drop object - game id  : %lld\towner : %s\texpire : %s")
						, dropObject->GetGameId()
						, (dropObject->IsPlayerOwn() ? TEXT("true") : TEXT("false"))
						, (dropObject->GetExpriedOwnerShip() ? TEXT("true") : TEXT("false")));
#endif

					dropObject->UnbindDropObjectLootingEvent();
					dropObject->SetLooted(true);

					FGsGameObjectData* gameObjData = dropObject->GetData();
					FGsNetSendServiceWorld::SendDropObjectLoot(gameObjData->GetGameId()
						, lootingInfo._isAutoLooting);

					// Prevent auto rooting until server response
					GSAI()->GetContentsConditionDefault()->SetIsLootServerResponseWait(true);

					// 루팅 사운드 출력
					if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
					{
						soundPlayer->PlaySound2D(TEXT("UI_PickupItem"));
					}
				}
				else
				{
					//Player can't loot because of distance
					FText text;
					if (FText::FindText(TEXT("DropObjectText"), TEXT("LootDistanceInvalid"), text))
					{
						FGsUIHelper::TrayMessageTicker(text);
					}
				}
			}
			else
			{
#if DROP_OBJECT_DEBUG								
				GSLOG(Log, TEXT("FGsStateLocalPlayerLooting::Enter - loot fail - game id  : %lld\towner : %s\texpire : %s")
					, dropObject->GetGameId()
					, (dropObject->IsPlayerOwn() ? TEXT("true") : TEXT("false"))
					, (dropObject->GetExpriedOwnerShip() ? TEXT("true") : TEXT("false")));
#endif

				//Player can't loot because of ownership
				FText text;
				if (FText::FindText(TEXT("DropObjectText"), TEXT("NotPlayerOwn"), text))
				{
					FGsUIHelper::TrayMessageTicker(text);
				}
			}
		}

		lootingHandler->ClearReserveLootInfo();
		fsm->DelayProcessEvent(GData()->GetGlobalData()->_dropObjectLootingDuration, EGsStateBase::Looting, EGsStateBase::Idle);
	}
	else
	{
		fsm->DelayProcessEvent(GData()->GetGlobalData()->_dropObjectLootingDuration, EGsStateBase::Looting, EGsStateBase::Idle);
	}
}

void FGsStateLocalPlayerLooting::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

bool FGsStateLocalPlayerLooting::IsInsideInteractionRange(UGsGameObjectBase* In_owner, UGsGameObjectBase* In_target)
{
	if (In_owner != nullptr && In_target != nullptr)
	{
		float distanceSq = FVector::DistSquared2D(In_target->GetLocation(), In_owner->GetLocation());
		float dropObjectLootingRangeFactor = FGsLootingHandlerLocalPlayer::GetLootRange(true);
		if (distanceSq <= dropObjectLootingRangeFactor * dropObjectLootingRangeFactor)
		{
			return true;
		}
	}

	return false;
}