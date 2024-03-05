// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Message/GsMessageItem.h"
#include "Message/GsMessageContents.h"


class FGsItem;
/**
 * 
 */
class T1PROJECT_API FGsQuickSlotHandler
{
public:
	enum QuickSlotMode : uint8
	{
		Normal = 0,
		Edit,

		Max,
	};
	
	struct FGsAutoDelayData
	{
		QuickSlotId _slotId = INVALID_QUICK_SLOT_ID;
		uint64 _itemDBId = 0;
		bool _bUseAuto = false;
		float _remainSec = 0.f;

	public:
		bool Update(float InDeltaTime);
	};

private:
	QuickSlotMode _slotMode = QuickSlotMode::Normal;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDelegate;
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_listQuickSlotDelegate;

	// 0.2초간 딜레이를 주어 동작을 모았다가 현재 오토상태와 다를때만 보내는 로직 추가
	TMap<QuickSlotId, FGsAutoDelayData> _reqAutoMap;
	// 결과 패킷을 못받았을 경우에 갱신하기 위함
	TMap<QuickSlotId, float> _rcvAutoCheckMap;

public:
	FGsQuickSlotHandler();
	virtual ~FGsQuickSlotHandler();

private:
	void RegisterMessage();
	void RemoveMsg();

private:
	void OnChagneQuickSlotState(const struct IGsMessageParam* InParamData);
	void RegisterQuickSlot(const struct IGsMessageParam* InParamData);
	void SwapQuickSlot(const struct IGsMessageParam* InParamData);
	void RemoveQuickSlotIcon(const struct IGsMessageParam* InParamData);
	void QuickSlotUseItem(const struct IGsMessageParam* InParamData);
	void OnChangeQuickSlotAutoState(const struct IGsMessageParam* InParamData);
	void OnReceiveAutoState(const struct IGsMessageParam* InParamData);

private:
	void AddItemListMessage(struct FGsItemMessageParamBase& InParam);
	void UpdateItemMessage(struct FGsItemMessageParamBase& InParam);
	void RemoveItemMessage(struct FGsItemMessageParamBase& InParam);

private:
	void UpdateItemMessage(const ItemDBId InItemDbid);

public:
	void Init();
	void Close();
	void OnClickRemoveSlotItem(const QuickSlotId InSlotID);
	void OnClickUseItem(const QuickSlotId InSlotID);
	void OnChangeAutoState(const QuickSlotId InSlotID, const uint64 InItemDBID, const bool InFlag);

	void RegisterQuickSlot(const uint64 InInvenItemDBID , const QuickSlotId InQuickSlotID);
	void SwapQuickSlot(const QuickSlotId InQuickSlotID1 , const QuickSlotId InQuickSlotID2);

	void RemoveAutoCheckData(QuickSlotId InQuickSlotId);

public:
	void Enter();
	void OnReconection();
	void Update(float InDeltaTime);
};
