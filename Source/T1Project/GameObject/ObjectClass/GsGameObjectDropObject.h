// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/ObjectClass/GsGameObjectPropObject.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsGameObjectDropObject.generated.h"

/**
 *
 */

UENUM()
enum class EDropObjectState : uint8
{
	Spawn,
	Unlootable,
	Lootable,
	Looted,
	Max
};

struct FDropObjectSpawnFinishMessage : public IGsMessageParam
{
public:
	class AGsActorDropObject* _dropObject;

	FDropObjectSpawnFinishMessage() = default;
	explicit FDropObjectSpawnFinishMessage(class AGsActorDropObject* inTarget) :
		_dropObject(inTarget)
	{
	}
};

UCLASS()
class T1PROJECT_API UGsGameObjectDropObject final : public UGsGameObjectPropObject
{
	GENERATED_BODY()

private:
	MsgGameObjHandle SpawnFinishHandle;
	TArray<UserDBId> _initOwnerDBIDList;

private:
	//스폰이 끝나 룻팅할 수 있는가?
	UPROPERTY()
		bool _isSpawnFinish = false;
	//플레이어 소유의 아이템인가?
	UPROPERTY()
		bool _isPlayerOwn = false;
	//룻팅이 끝났는가?
	UPROPERTY()
		bool _isLooted = false;
	//타유저 소유의 아이템이 소유권이 만료 되었는가?
	UPROPERTY()
		bool _isExpired = false;
	UPROPERTY()
		int32 _itemId;
	UPROPERTY()
		int64 _itemAmount;

	const struct FGsSchemaDropObject* _dropTable = nullptr;
	const struct FGsSchemaItemCommon* _itemTable = nullptr;

public:
	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;

protected:
	virtual void InitializeActor(class AActor* Owner) override;
	virtual void ActorDespawn() override;

public:
	virtual EGsGameObjectType GetObjectType() const override;

	// 위젯 이름 show/hide 옵션 갱신
	virtual void UpdateWidgetShowNameOption() override;

public:
	UFUNCTION()
		void OnInputTouchLooting(ETouchIndex::Type In_type, UPrimitiveComponent* In_component);
	void OnSpawnFinish(const IGsMessageParam* inParam);

public:
	void UnbindDropObjectLootingEvent();

private:
	void SetDropObjectEffect(class AGsActorDropObject* inDropObject);
	void SetItemName(class AGsActorDropObject* inDropObject);

public:
	void StartSpawn(const FVector& inDest, const FVector& inStart);

public:
	//스폰시 루팅할 수 없다. 그리스 스폰시엔 루팅은 가능하다
	void SetSpawnFinish(const bool inIsSpawnFinish) { _isSpawnFinish = inIsSpawnFinish; }
	bool IsSpawnFinish() const { return _isSpawnFinish; }
	//Set player ownership
	void SetPlayerOwn(const bool isPlayerOwn) { _isPlayerOwn = isPlayerOwn; }
	bool IsPlayerOwn() const { return _isPlayerOwn; }
	//If you send loot packet to server, Set true
	void SetLooted(const bool inLooted) { _isLooted = inLooted; }
	bool IsLooted() const { return _isLooted; }
	bool TryGetItemName(FString& outName, int32 inItemId);
	bool TryGetGradeParticlePath(FSoftObjectPath& outPath, int32 inItemId);	
	void SetItemAmount(int64 inAmount){ _itemAmount = inAmount;}
	int64 GetItemAmount(){return _itemAmount;}
	int32 GetItemWeight();
	// 2022/08/02 PKT - Sub Effect
	bool PlayDropItemEffect(DropItemEffect InDropItemEffect);

public:
	const struct FGsSchemaDropObject* GetDropTable() const { return _dropTable; }
	const EDropObjectState GetState() const;
	const struct FGsSchemaItemCommon* GetItemTable() const { return _itemTable; }
	const int32 GetItemId(){return _itemId;}
	void SetInitOwnerDBIdArray(const TArray<UserDBId>& inDBIDlist){_initOwnerDBIDList = inDBIDlist;}
	TArray<UserDBId> GetInitOwnerDBIdArray(){return _initOwnerDBIDList;}
	bool IsInitOwnerDBId(UserDBId In_dbId);
	void SetExpiredOwnerShip(bool inOwnerShip){ _isExpired  = inOwnerShip;}
	bool GetExpriedOwnerShip() {return _isExpired;}
	void Print(class UWorld* inWorld);
};
