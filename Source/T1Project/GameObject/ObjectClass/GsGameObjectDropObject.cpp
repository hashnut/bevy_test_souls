// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectDropObject.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "WidgetComponent.h"
#include "GameFramework/Actor.h"

#include "Net/GsNetSendServiceWorld.h"
#include "ActorEx/GsActorDropObject.h"

#include "GameObject/Define/EGsDropItemCategory.h"

#include "GsTableManager.h"

#include "Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsItemUtil.h"

#include "UI/UIContent/Billboard/GsUIBillboardDropObject.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "GameObject/DropObject/GsSchemaDropObject.h"
#include "GameObject/DropObject/GsSchemaDropObjectEffect.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Option/GsGameUserSettings.h"

#include "ActorComponentEx/GsWidgetComponentButton.h"

#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsAIMessageParam.h"

#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"
#include "DataSchema/Item/GsSchemaDropItemEffect.h"
#include "DataSchema/GsSchemaEnums.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"

#include "Option/GsOptionFunc.h"
#include "DrawDebugHelpers.h"


void UGsGameObjectDropObject::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectDropObject::GetObjectType());

	_isSpawnFinish = false;
	_isPlayerOwn = false;
	_isLooted = false;
	_initOwnerDBIDList.Empty();

	// 테이블 캐싱
	FGsGameObjectData* data = GetData();
	_itemId = data->GetTableId();
	EGsDropItemCategory itemCategory = UGsItemUtil::GetDropObjectCategory(data->GetTableId());
	_dropTable = UGsTableUtil::FindRowById<UGsTableDropObject, FGsSchemaDropObject>(itemCategory);
	if (_dropTable)
	{
		// User 이름은 서버에서 주지만 그외는 따로 설정해야한다.
		_itemTable = FGsItem::GetTableDataByID(data->GetTableId());
		if (_itemTable)
		{
			_gameObjectData->SetName(_itemTable->name.ToString());
		}
	}

	//bak1210 : final 에서호출
	CreateSubClass();
}

void UGsGameObjectDropObject::InitializeActor(AActor* inOwner)
{
	Super::InitializeActor(inOwner);

	if (nullptr == inOwner)
	{
		return;
	}

	_actor = Cast<AGsActorPropObject>(inOwner);
	if (nullptr == _actor)
	{
		return;
	}

	AGsActorDropObject* dropItem = Cast<AGsActorDropObject>(inOwner);
	if (nullptr == dropItem)
	{
		return;
	}

	if (dropItem->GetSphereComponent())
	{
		dropItem->GetSphereComponent()->OnInputTouchBegin.RemoveAll(this);
		dropItem->GetSphereComponent()->OnInputTouchBegin.AddDynamic(this, &UGsGameObjectDropObject::OnInputTouchLooting);

		// DropObject는 Capsule Radius정보를 읽어 올수 없으므로
		// 직접 찾아서 설정한다.
		// 실제 설정된 Radius정보가 크기 때문에 절반 값만 반영
		FGsGameObjectData* data = GetData();
		data->SetScaledCapsuleRadius(dropItem->GetSphereComponent()->GetScaledSphereRadius() * 0.5f);
	}

#if DROPOBJECT_PICKING_ACTIVE
	dropItem->SetInteractionClickCallback([this](bool In_isByWidget) {
		FGsInteractionMessageParam messageParam(GetGameId()
			, GetObjectType()
			, GetItemAmount()
			, false);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::DROPOBJECT_PICK, &messageParam);
		});
#endif

	SetDropObjectEffect(dropItem);
	SetItemName(dropItem);
}

void UGsGameObjectDropObject::ActorDespawn()
{
	UnbindDropObjectLootingEvent();

	// 삭제됨을 알림
	FGsGameObjectMessageParam messageParam(this);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::DROPOBJECT_DESPAWN, &messageParam);

	FGsAIReserveParam aiReserveClearParam(EGsAIActionType::LOOT_ITEM, this);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam);

	FGsAIReserveParam aiReserveClearParam_lootItemAutoOff(EGsAIActionType::LOOT_ITEM_AUTO_OFF, this);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam_lootItemAutoOff);

	Super::ActorDespawn();
}

EGsGameObjectType UGsGameObjectDropObject::GetObjectType() const
{
	return EGsGameObjectType::DropObject;
}

void UGsGameObjectDropObject::OnInputTouchLooting(ETouchIndex::Type In_type, UPrimitiveComponent* In_component)
{
	// 터치됨을 알림	
	FGsInteractionMessageParam messageParam(GetGameId()
		, GetObjectType()
		, GetItemAmount()
		, false);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::DROPOBJECT_PICK, &messageParam);
}

void UGsGameObjectDropObject::OnSpawnFinish(const IGsMessageParam* inParam)
{
#if CANT_LOOT_WHEN_DROP_OBJECT_SPAWN
	if (inParam)
	{
		if (const FDropObjectSpawnFinishMessage* message = inParam->Cast<const FDropObjectSpawnFinishMessage>())
		{
			if (message->_dropObject == GetActor())
			{
				_isSpawnFinish = true;
			}
		}
	}
#endif
}

void UGsGameObjectDropObject::SetDropObjectEffect(AGsActorDropObject* inDropObject)
{
	FSoftObjectPath effectPath;
	if (TryGetGradeParticlePath(effectPath, _gameObjectData->GetTableId()))
	{
		UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(effectPath);
		if (nullptr == load)
		{
			return;
		}

		UParticleSystem* particle = Cast<UParticleSystem>(load);
		if (nullptr == particle)
		{
			return;
		}

		if (inDropObject->_gradeParticleSystemComponent)
		{
			inDropObject->_gradeParticleSystemComponent->SetTemplate(particle);
		}
	}
	else
	{
		if (inDropObject->_gradeParticleSystemComponent)
		{
			inDropObject->_gradeParticleSystemComponent->SetTemplate(nullptr);
		}
	}
}

void UGsGameObjectDropObject::SetItemName(AGsActorDropObject* inDropObject)
{
	if (nullptr == inDropObject
		|| nullptr == inDropObject->GetWidget())
	{
		return;
	}

	UGsUIBillboardDropObject* widget = Cast<UGsUIBillboardDropObject>(inDropObject->GetWidget()->GetUserWidgetObject());
	if (nullptr == widget)
	{
		return;
	}

	FString itemName;
	if (TryGetItemName(itemName, _gameObjectData->GetTableId()))
	{
		widget->SetItemNameText(FText::FromString(itemName));
	}
}

void UGsGameObjectDropObject::UnbindDropObjectLootingEvent()
{
	if (AGsActorDropObject* dropItem = Cast<AGsActorDropObject>(_actor))
	{
		dropItem->GetSphereComponent()->OnInputTouchBegin.RemoveAll(this);
	}

	if (SpawnFinishHandle.Value.IsValid())
	{
		GMessage()->GetGameObject().Remove(SpawnFinishHandle);
	}
}

bool UGsGameObjectDropObject::TryGetItemName(FString& outName, int32 inItemId)
{
	const FGsSchemaItemCommon* item = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(inItemId);
	if (nullptr == item)
	{
		return false;
	}

	outName = item->name.ToString();

	return true;
}

bool UGsGameObjectDropObject::TryGetGradeParticlePath(FSoftObjectPath& outPath, int32 inItemId)
{
	const FGsSchemaItemCommon* item = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(inItemId);
	if (nullptr == item)
	{
		return false;
	}

	ItemGrade grade = item->grade;

	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaDropObjectEffect::StaticStruct());
	if (nullptr == table)
	{
		return false;
	}

	const UGsTableDropObjectEffect* dropObjectEffectTable = Cast<const UGsTableDropObjectEffect>(table);
	if (nullptr == dropObjectEffectTable)
	{
		return false;
	}

	const FGsSchemaDropObjectEffect* find;
	if (false == dropObjectEffectTable->FindRowById(grade, find))
	{
		return false;
	}

	outPath = find->particle;

	return true;
}

const EDropObjectState UGsGameObjectDropObject::GetState() const
{
	if (_isLooted)
	{
		return EDropObjectState::Looted;
	}
#if CANT_LOOT_WHEN_DROP_OBJECT_SPAWN
	else if (false == _isSpawnFinish)
	{
		return EDropObjectState::Spawn;
	}
#endif
	else if (_isPlayerOwn || _isExpired)
	{
		return EDropObjectState::Lootable;
	}

	return EDropObjectState::Unlootable;
}

void UGsGameObjectDropObject::StartSpawn(const FVector& inDest, const FVector& inStart)
{
	AGsActorDropObject* dropObjectActor = Cast<AGsActorDropObject>(GetActor());
	if (nullptr == dropObjectActor)
	{
		return;
	}

#if CANT_LOOT_WHEN_DROP_OBJECT_SPAWN
	_isSpawnFinish = false;
#else
	_isSpawnFinish = true;
#endif

	if (SpawnFinishHandle.Value.IsValid())
	{
		GMessage()->GetGameObject().Remove(SpawnFinishHandle);
	}

	SpawnFinishHandle = GMessage()->GetGameObject().AddUObject(MessageGameObject::DROPOBJECT_SPAWN_FINISH, this, &UGsGameObjectDropObject::OnSpawnFinish);
	dropObjectActor->StartSpawn(inStart, inDest);

	// 퀘스트 목표 완료후 이동시 못먹는 아이템 루팅 처리
	// 1. 퀘스트진행
	// 2. 소유주 내꺼인 아이템


	// not check quest move
	// kill count quest in progress -> drop item
	// https://jira.com2us.com/jira/browse/CHR-16728
	if (true == _isPlayerOwn &&
		true == GSQuest()->IsAutoProgressing())
	{
		if (FGsGameObjectData* data = GetData())
		{
			if (FGsAIManager* aiManager = GSAI())
			{
				aiManager->GetContentsConditionDefault()->AddMissingMyDropObjectId(data->GetGameId());
			}
		}
	}

	UGsItemManager* itemManager = GItem();
	if (itemManager == nullptr)
	{
		return;
	}

	// check slot
	bool invenSlotPossible = itemManager->IsCanItemAdd(GetItemId(), GetItemAmount());


	// check weight 
	bool isAutoLootWeightPossible = FGsOptionFunc::IsPossibleLootByWeightOption(GetItemWeight());

	// https://jira.com2us.com/jira/browse/C2URWQ-2788
	// check grade
	bool isAutoLootGradePossible = false;
	if (_itemTable != nullptr)
	{
		isAutoLootGradePossible = FGsOptionFunc::IsPossibleLootByGradeOption(_itemTable->type, _itemTable->grade);
	}


	// 드랍 오브젝트 스폰시에도
	// 기존 ai 조건들 검사하기
	// 1. IS_IN_SEARCH_RANGE_ITEM() == true 
	// 1-1: GetAutoLootDropItemGrade 체크: (위에서 함, isAutoLootGradePossible)
	// 1-2: AUTO_ITEM_LOOT_OTHER_USERS 체크: (남의 아이템 주울지 옵션 체크)
	// 1-3: GetAutoDropItemWeight 체크: (무게 옵션 체크 위에서 함: isAutoLootWeightPossible)
	// 1-4: GetReturnDist 체크: (획득 거리 안인지 체크)
	// 1-5: IsPossibleNoneEquipItemLootByGradeOption 체크: (비장착 아이템 줍고 옵션 체크)
	// 2. IS_AUTO_LOOT_COOL_TIME == false(루팅 쿨타임인지)
	// 3. IS_SERVER_MOVE_LOCK == false(서버 이동 락걸려있는지)
	// https://jira.com2us.com/jira/browse/C2URWQ-7826


	// ai on
	if (true == _isPlayerOwn &&
		true == GSAI()->IsAIOn() &&
		isAutoLootWeightPossible == true &&
		invenSlotPossible == true &&
		isAutoLootGradePossible == true)
	{
		// add index(second index)
		// 자동 이동중이면 index 0으로 넣자


		// 1-2: AUTO_ITEM_LOOT_OTHER_USERS 체크: (남의 아이템 주울지 옵션 체크)
		bool isAutoItemLootOtherUsers = true;
		if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
		{
			const float isAutoItemLootOtherUsersFloat =
				gameUserSettings->GetAutoSetting(EGsOptionAuto::AUTO_ITEM_LOOT_OTHER_USERS);

			isAutoItemLootOtherUsers = (0.0f == isAutoItemLootOtherUsersFloat) ? false : true;
		}
		// 타유저 꺼 안줍는 옵션이면 
		// 로컬 db로 체크

		const FGsGameDataManager* gameDataManager = GGameData();
		if (gameDataManager == nullptr)
		{
			return;
		}

		const FGsNetUserData* netUserData = gameDataManager->GetUserData();
		if (netUserData == nullptr)
		{
			return;
		}

		UserDBId localDBId = netUserData->mUserDBId;

		if((isAutoItemLootOtherUsers == false) &&
			(false == IsInitOwnerDBId(localDBId)))
		{
			return;
		}

		// 1-4: GetReturnDist 체크: (획득 거리 안인지 체크)
		float returnDist = GSAI()->GetReturnDist();

		UGsGameObjectManager* gameObjectManager = GSGameObject();
		if (gameObjectManager == nullptr)
		{
			return;
		}

		UGsGameObjectBase* local = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer);
		if (local == nullptr)
		{
			return;
		}

		AActor* localActor = local->GetActor();
		if (localActor == nullptr)
		{
			return;
		}

		float distanceToTarget =
			localActor->GetHorizontalDistanceTo(GetActor());

		// 시야 밖이면 실패
		if (distanceToTarget > returnDist)
		{
			return;
		}

		// 1-5: IsPossibleNoneEquipItemLootByGradeOption 체크: (비장착 아이템 줍고 옵션 체크)
		const FGsSchemaItemCommon* itemTbl = GetItemTable();
		// 아이템 테이블이 없다면 문제있음 넘어가자
		if (itemTbl == nullptr)
		{
			return;
		}
		// 자동일때 비장착 아이템 줍기 옵션 체크
		if ( ItemType::EQUIP != itemTbl->type &&
			false == FGsOptionFunc::IsPossibleNoneEquipItemLootByGradeOption(itemTbl))
		{
			return;
		}
		// 2. IS_AUTO_LOOT_COOL_TIME == false(루팅 쿨타임인지)
		if (true == GSAI()->GetIsAutoLootCoolTime())
		{
			return;
		}
		// 3. IS_SERVER_MOVE_LOCK == false(서버 이동 락걸려있는지)
		if (true == GSAI()->IsServerMoveLock())
		{
			return;
		}
		bool isLocalAutoMove = false;		
		if (FGsGameObjectStateManager* fsm = local->GetBaseFSM())
		{
			// 내 캐릭터가 죽어있는 경우 체크
			if (true == fsm->IsState(EGsStateBase::AutoMove))
			{
				isLocalAutoMove = true;
			}
		}
		


		FGsAIReserveParam aiReserveAddParam(EGsAIActionType::LOOT_ITEM, this, (isLocalAutoMove == true)?  0: 1);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_ADD_RESERVE_JOB, &aiReserveAddParam);
	}
}

// 위젯 이름 show/hide 옵션 갱신
void UGsGameObjectDropObject::UpdateWidgetShowNameOption()
{
	if (nullptr == _actor)
	{
		return;
	}

	AGsActorDropObject* dropItem = Cast<AGsActorDropObject>(_actor);
	if (nullptr == dropItem
		|| nullptr == dropItem->GetWidget()
		|| nullptr == dropItem->GetWidget()->GetUserWidgetObject())
	{
		return;
	}

	UGsUIBillboardDropObject* widget = Cast<UGsUIBillboardDropObject>(dropItem->GetWidget()->GetUserWidgetObject());
	if (nullptr == widget)
	{
		return;
	}

	widget->UpdateShowNameOption();
}

bool UGsGameObjectDropObject::IsInitOwnerDBId(UserDBId In_dbId)
{
	// 리스트 비어있으면 모두의 것
	if (_initOwnerDBIDList.Num() == 0)
	{
		return true;
	}

	return _initOwnerDBIDList.Contains(In_dbId);
}

int32 UGsGameObjectDropObject::GetItemWeight()
{
	if (_itemTable == nullptr)
	{
		return 0;
	}

	int32 totalWeight = _itemTable->weight * _itemAmount;
	return totalWeight;
}

bool UGsGameObjectDropObject::PlayDropItemEffect(DropItemEffect InDropItemEffect)
{
	UParticleSystem* particle = nullptr;
	do
	{
		if (DropItemEffect::NORAM_TYPE == InDropItemEffect || DropItemEffect::MAX == InDropItemEffect)
		{
			break;
		}

		const FGsSchemaDropItemEffect* dropEffectData
			= UGsTableUtil::FindRowById<UGsTableDropItemEffect, FGsSchemaDropItemEffect>(InDropItemEffect);
		if (nullptr == dropEffectData)
		{
			GSLOG(Error, TEXT("nullptr == DropEffectData"));
			break;
		}

		if (dropEffectData->path.IsNull())
		{
			GSLOG(Error, TEXT("dropEffectData->path.IsNull()"));
			break;
		}

		UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(dropEffectData->path);
		if (nullptr == load)
		{
			GSLOG(Error, TEXT("nullptr == load"));
			break;
		}

		particle = Cast<UParticleSystem>(load);
		if (nullptr == particle)
		{
			GSLOG(Error, TEXT("nullptr == particle"));
			break;
		}

	} while (0);

	AGsActorDropObject* dorpActor = Cast<AGsActorDropObject>(this->GetActor());
	if (dorpActor && dorpActor->_dropItemEffectParticleSystemComponent)
	{
		dorpActor->_dropItemEffectParticleSystemComponent->SetTemplate(particle);
	}
	else
	{
		particle = nullptr;
	}

	return (particle != nullptr);
}

void UGsGameObjectDropObject::Print(UWorld* inWorld)
{
	FString log;

#if WITH_EDITOR
	log = FString::Printf(TEXT("label : %s\n"), *GetActor()->GetActorLabel());
#endif

	log += FString::Printf(TEXT("game id : %lld\n"), GetGameId());
	log += FString::Printf(TEXT("isSpawnFinish : %s\n"), (_isSpawnFinish ? TEXT("true") : TEXT("false")));
	log += FString::Printf(TEXT("player own : %s\n"), (_isPlayerOwn ? TEXT("true") : TEXT("false")));
	log += FString::Printf(TEXT("looted : %s\n"), (_isLooted ? TEXT("true") : TEXT("false")));
	log += FString::Printf(TEXT("expried : %s\n"), (_isExpired ? TEXT("true") : TEXT("false")));

	const FGsSchemaItemCommon* item = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(_itemId);
	if (item)
	{
		log += FString::Printf(TEXT("item id : %d\tname : %s\n"), _itemId, *item->name.ToString());
		log += FString::Printf(TEXT("item count : %d\n"), _itemAmount);
	}

	GSLOG(Log, TEXT("%s"), *log);

	DrawDebugString(inWorld, GetActor()->GetActorLocation(), log, NULL, FColor::White, 300);
	DrawDebugSphere(inWorld, GetActor()->GetActorLocation(), 100.0f, 24, FColor::Red, false, 300);
}
