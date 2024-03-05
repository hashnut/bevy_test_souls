
#pragma once

/**
* file		GsDungeonGroupBase.h
* @brief	DungeonData Base Class
* @author	PKT
* @date		2022/07/19
**/

#include "CoreMinimal.h"
#include "GsDungeonData.h"
#include "IDelegateInstance.h"
#include "Unlock/GsUnlockDefine.h"
#include "Currency/GsCostPackage.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

class FGsDungeonGroupBase
{
protected:
	// 2022/07/19 PKT - Schema Dungeon Group Data
	const struct FGsSchemaDungeonGroupData* _schemaData;

	mutable const struct FGsSchemaPassivitySet* _environmentPassivityData;

	// 2022/08/16 PKT - Dungeon Data Holder
	TArray<FGsDungeonData> _dungeonDataHolder;

	// 2023/2/21 PKT - Event On / Off
	bool _isVisible = false;

private:
	// 2022/07/19 PKT - Event Handler Delegate
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemActionDelegates;	
	TPair<MessageContentUnlock, FDelegateHandle> _unlockDelegate;
	MsgGameObjHandleArray _listGameObjectDelegate;

public:
	// 2022/07/19 PKT - Invalidate Delegate
	DECLARE_DELEGATE_OneParam(FOnInvalidate, DungeonGroupId);
	// 2022/07/19 PKT - Delegate
	FOnInvalidate _OnInvalidate;

public:
	FGsDungeonGroupBase() = default;
	virtual ~FGsDungeonGroupBase() = default;

	explicit FGsDungeonGroupBase(const struct FGsSchemaDungeonGroupData* InSchemaData);

	virtual void Initialized();
	virtual void Finalized();

	void SetVisible(bool InIsVisible);

	bool IsEventGroup() const;
	// 2023/2/28 PKT - Data는 갱신 받을 수도 안 받을 수도 있지만.. UI에서 표기 되지 않는 그룹 정보( ex : 퀘스트 던전 그룹 )
	bool IsUIHidden() const;
	// 2023/2/28 PKT - Data가 갱신 받지 않는 상태. 즉 Schema 정보만 있는 상태( ex : 비활성된 던전 그룹 )
	bool IsDisable() const;

protected:
	// 2022/07/19 PKT - 상속 받는 곳에서 재 정의(필요에 따라..)
	virtual void RegisterMessage(class FGsMessageHolder* InMessageHolder);
	virtual void UnRegisterMessage(class FGsMessageHolder* InMessageHolder);

	// 2022/08/16 PKT - Changed Level
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);
	void OnChangePlayerLevelDelegate(EGsGameObjectDataMember InMeberType);

	// 2022/08/16 PKT - ChargeItem
	void OnAddItem(struct FGsItemMessageParamBase& InParam);
	void OnUpdateItem(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItem(struct FGsItemMessageParamBase& InParam);

	// 2022/08/16 PKT - UnLock Contetns
	virtual void OnContentsUnlock(const struct IGsMessageParam* InParam);

	virtual bool IsDungeonUnLockContentsDelegate(const FGsDungeonData* InTarget, bool InShowTicker = false);

	// 2022/08/16 PKT - 충전 아이템 제외 다른 Cost Item 존재 시 정의 함.
	virtual void AddItem(struct FGsItemMessageParamBase& InParam) {/**/ }
	virtual void UpdateItem(struct FGsItemMessageParamBase& InParam) {/**/ }
	virtual void RemoveItem(struct FGsItemMessageParamBase& InParam) {/**/ }
	
protected:
	void ChangedState();

public:
	/************************************************************************/
	/* Get                                                                  */
	/************************************************************************/
	const struct FGsSchemaDungeonGroupData* GetSchemaData() const;

	DungeonGroupId GetGroupId() const;

	DungeonCategory GetCategory() const;

	int32 GetListOrder() const;

	const struct FGsSchemaPassivitySet* GetEnvPassivity() const;

	bool IsUnLockContents(bool InShowTicker = false) const;

	void LockContentsReason(FText& OutText) const;

	int32 DungeonCountAll() const;

	const FGsDungeonData* FindDungeonByMapId(MapId InMapId) const;

	const FGsDungeonData* FindDungeonByIndex(int32 InIndex) const;

	int32 FindIndexByData(const FGsDungeonData* InData) const;

	bool IsValidEnterDateTime(FText& OutMessage) const;

	ContentsBindingType ContentsBindType() const;

protected:
	TSharedRef<const FGsCostPackage> CostItemPackage(const struct FGsSchemaItemCommon* InItemData) const;
};