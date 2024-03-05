// Fill out your copyright notice in the Description page of Project Settings.


#include "GsLootingHandlerLocalPlayer.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Message/MessageParam/GsInteractionMessageParam.h"

#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Gameobject/ObjectClass/GsGameObjectDropObject.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"

#include "Item/GsItemManager.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Util/GsItemUtil.h"

// 루팅 범위 구하기
float FGsLootingHandlerLocalPlayer::GetLootRange(bool In_isOrigin)
{
	// 보스 몬스터 안에 루팅 아이템이 있으면 획득 못하는 이슈로
	// 이동시 origin(더파고 들게 하는 처리, ex) factor 값 0.6) 처리를 뺀다
	float scale =  1.f;
	float range = GData()->GetGlobalData()->_dropObjectLootingRangeFactor;
	return range * scale;
}

void FGsLootingHandlerLocalPlayer::Initialize(UGsGameObjectBase* In_owner)
{
	_local = Cast<UGsGameObjectLocalPlayer>(In_owner);

	FGsMessageHolder* msg = GMessage();
	InitializeMessageDelegateList(&msg->GetGameObject());

	AddMessageDelegate(msg->GetGameObject().AddRaw(MessageGameObject::DROPOBJECT_PICK,
			this, &FGsLootingHandlerLocalPlayer::OnDropObjectPick));
	AddMessageDelegate(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SKILL_END,
			this, &FGsLootingHandlerLocalPlayer::OnSkillEnd));
	AddMessageDelegate(msg->GetGameObject().AddRaw(MessageGameObject::DROPOBJECT_DESPAWN,
		this, &FGsLootingHandlerLocalPlayer::OnClearReserveLootInfo));
}

void FGsLootingHandlerLocalPlayer::Finalize()
{
	ClearReserveLootInfo();
	// 바인딩 된 메시지 해제
	FinalizeMessageDelegateList();
}

void FGsLootingHandlerLocalPlayer::Update(float In_delta)
{
	if (IsReserveLootng())
	{
		// 도착했으면 다시 시도
		if (_reserveLootInfo._isMoveComplete == true)
		{
			// 사라진 녀석인가 검증
			if (false == _reserveLootInfo._targetObject.IsValid())
			{
				ClearReserveLootInfo();
			}
			else
			{
				TryLooting(_reserveLootInfo._targetObject.Get(), _reserveLootInfo._isAutoLooting);
			}
		}
	}
}

// 인터랙션 시도(DropObject 타겟)
void FGsLootingHandlerLocalPlayer::TryLooting(UGsGameObjectBase* DropObject, bool isAutoLooing)
{
	if (DropObject && DropObject->IsObjectType(EGsGameObjectType::DropObject))
	{
		// 예약
		_reserveLootInfo._isMoveComplete = false;
		_reserveLootInfo._targetObject = DropObject->CastGameObject<UGsGameObjectDropObject>();
		_reserveLootInfo._isAutoLooting = isAutoLooing;		

		// Loot 대기 상태로 전환
		FGsGameObjectStateManager::ProcessEvent(_local.Get(), EGsStateBase::Looting);
	}
}

// DropObject(루팅) Pick
void FGsLootingHandlerLocalPlayer::OnDropObjectPick(const IGsMessageParam* In_param)
{
	const FGsInteractionMessageParam* castData = In_param->Cast<const FGsInteractionMessageParam>();
	if (castData == nullptr)
	{
		return;
	}

	UGsGameObjectBase* findObject = GSGameObject()->FindObject(castData->_objectType, castData->_targetGameID);
	UGsGameObjectDropObject* castDropObject = Cast<UGsGameObjectDropObject>(findObject);
	if (nullptr == castDropObject)
	{
		GSLOG(Error, TEXT("drop object is missing - game id : %lld"), castData->_targetGameID);
		return;
	}

	int32 itemId = castDropObject->GetItemId();
	ItemAmount itemAmount = castDropObject->GetItemAmount();

	// 인벤토리 무게 다차면 티커띄우고
	// 리턴	
	if (GItem()->IsInvenWeightFull() == true)
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}
	// 인벤토리 슬롯 다차면 티커띄우고
	// 리턴	
	if (false == GItem()->IsCanItemAdd(itemId, itemAmount))
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}
	
	if (findObject)
	{
		TryLooting(findObject, castData->_isAutoLooting);
	}
}

// 스킬 종료시 Drop 예약이면 실행
void FGsLootingHandlerLocalPlayer::OnSkillEnd(const IGsMessageParam* In_param)
{
	if (IsReserveLootng())
	{
		// 인터랙션이 스킬 예약보다 우선순위가 높다.
		// 예약된 스킬이 있다면 Clear
		if (FGsSkillHandlerLocalPlayer* localSkillHandler = _local->GetCastSkill<FGsSkillHandlerLocalPlayer>())
		{
			if (localSkillHandler->IsReserveData())
			{
				localSkillHandler->ClearReserveSkill();
			}
		}
	}
}

void FGsLootingHandlerLocalPlayer::OnClearReserveLootInfo(const IGsMessageParam* In_param)
{
	const FGsInteractionMessageParam* castData = In_param->Cast<const FGsInteractionMessageParam>();
	if (castData == nullptr)
	{
		return;
	}

	ClearReserveLootInfo(castData->_targetGameID);
}

void FGsLootingHandlerLocalPlayer::OnMoveToLoot(UGsGameObjectBase* Target)
{
	if (Target && IsReserveLootng())
	{
		AActor* targetActor = Target->GetActor();
		if (targetActor && true == targetActor->IsActorInitialized() &&
			false == targetActor->IsPendingKillPending())
		{
			FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();

			movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::LootItemMove);

			// 루팅 이동 시작때 나머지 reserve 데이터 삭제(현재는 waypoint, input handler (터치 이동) 이동만 처리)
			// https://jira.com2us.com/jira/browse/C2URWQ-6334
			_local->ClearReserveDataByReserveWork(this);

			float acceptanceRadius = GetLootRange();
			acceptanceRadius = FMath::Min(acceptanceRadius + Target->GetData()->GetScaledCapsuleRadius(), GetLootRange(true));
			movement->ChangeAutoMoveState().StartActor(_local.Get(), Target->GetActor(), acceptanceRadius,
					FGsAutoMoveCompletedDelegate::CreateRaw(this, &FGsLootingHandlerLocalPlayer::CallbackMoveToLootEnd),
					FGsAutoMoveCompletedOneDelegate::CreateRaw(this, &FGsLootingHandlerLocalPlayer::CallbackMoveToLootFailed));
			
		}
	}
}

void FGsLootingHandlerLocalPlayer::ClearReserveLootInfo(uint64 GameId)
{
	if (IsReserveLootng())
	{
		if (GameId == 0 
		|| (_reserveLootInfo._targetObject.IsValid() && 
		_reserveLootInfo._targetObject->GetGameId() == GameId))
		{
			// 이동중이었다면 Stop
			_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
			_reserveLootInfo._targetObject.Reset();
			_reserveLootInfo._isMoveComplete = false;
		}
	}
}

void FGsLootingHandlerLocalPlayer::CallbackMoveToLootFailed(const FPathFollowingResult& Result)
{
	ClearReserveLootInfo();
	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
}

void FGsLootingHandlerLocalPlayer::CallbackMoveToLootEnd()
{
	_reserveLootInfo._isMoveComplete = true;
	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movement == nullptr)
	{
		return;
	}
	movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);
}

bool FGsLootingHandlerLocalPlayer::IsReserveLootng() const
{
	return _reserveLootInfo._targetObject.IsValid();
}

UGsGameObjectDropObject* FGsLootingHandlerLocalPlayer::GetLootingTarget() const
{
	if (IsReserveLootng() && _reserveLootInfo._targetObject.IsValid())
	{
		return _reserveLootInfo._targetObject.Get();
	}
	return nullptr;
}

const FGsLootingHandlerLocalPlayer::ReserveLootingInfo& FGsLootingHandlerLocalPlayer::GetReservedLootingInfo()
{
	return _reserveLootInfo;
}

// 예약 데이터 있는가
bool FGsLootingHandlerLocalPlayer::IsReserveData()
{
	return IsReserveLootng();
}

// 재시도 해라
void FGsLootingHandlerLocalPlayer::DoRetry()
{
	if (false == _reserveLootInfo._targetObject.IsValid())
	{
		GSLOG(Error, TEXT("Looting item destroyed"));
		return;
	}

	TryLooting(_reserveLootInfo._targetObject.Get(), _reserveLootInfo._isAutoLooting);
}
