// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageInput.h"
#include "Message/GsMessageContentDungeon.h"
#include "Message/GsMessagePKBook.h"
#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"


class UGsUITraySaveBattery;
class FGsStatInfo;
struct IGsMessageParam;
struct FGsItemMessageParamBase;
struct FGsInvenMessageParamBase;

/**
 * 절전모드 로직
 */
class T1PROJECT_API FGsSaveBatteryHandler : public IGsEventProgressInterface
{
private:
	const FName TRAY_SAVEBATTERY_NAME = TEXT("TraySaveBattery");
	const FName TRAY_MOVIE_NAME = TEXT("TrayMovie");
	const FName NPC_DIALOG_NAME = TEXT("NpcDialog");
	const FName OFFLINE_PLAY_NAME = TEXT("TrayOfflinePlayCount");

private:
	enum class SaveBatteryExitType
	{
		Normal,
		ReturnScroll,
		OfflinePlay,
	};

public:
	using MsgHandleContents = TPair<MessageContents, FDelegateHandle>;	
	using MsgHandleItem = TPair<MessageItem, FDelegateHandle>;
	using MsgHandleInventory = TPair<MessageContentInven, FDelegateHandle>;
	using MsgHandleSaveBattery = TPair<MessageContentSaveBattery, FDelegateHandle>;
	using MsgHandleUserInfo = TPair<MessageUserInfo, FDelegateHandle>;
	using MsgHandleUnlock = TPair<MessageContentUnlock, FDelegateHandle>;

private:
	TArray<MsgHandleContents> _contentsHandlerList;
	MsgGameObjHandleArray _objectIDHandlerList;
	MsgHudHandleArray _hudHandlerList;
	TArray<MsgHandleItem> _itemHandlerList;
	TArray<MsgHandleInventory> _inventoryHandlerList;
	TArray<MsgHandleSaveBattery> _saveBatteryHandlerList;
	TArray<MsgHandleUserInfo> _userInfoHandlerList;
	TArray<MsgHandleUnlock> _msgHandleUnlockList;
	TArray<MsgInputHandle> _inputHandlerList;
	MsgDungeonHUDTimeHandleArray _dungeonHUDTimeHandlerList;
	MsgGuildDungeonHandleArray _guildDungeonHandlerList;
	MsgPKBookArray _pkBookHandlerList;

	TWeakObjectPtr<UGsUITraySaveBattery> _trayUI;

	FDelegateHandle _changeStatHandle;

protected:
	bool _requestEvent = false;
	bool _clickedBeforeStartEvent = false;
	int32 _tick = 0;
	SaveBatteryExitType _exitType = SaveBatteryExitType::Normal;

protected:
	bool _isReservedCinematic = false;

private:
	double _lastInteractionTime;
	float _time = 0.f;
	TArray<float> _enterTimeList;
	bool _isEntered = false;
	float _prevFrameRate = 0.f;

	// 던전 시간 정보 (GsUIHUDDungeonTime 참조)
protected:
	bool _isDungeonTimeShow = false;
	bool _isDungeonTimeLimit = true;
	bool _isDungeonTimeShowSecond = false;
	FDateTime _dungeonEndTime;

public:
	FGsSaveBatteryHandler() = default;
	virtual ~FGsSaveBatteryHandler();

	/************************************************************************/
	/* 시작 및 종료                                                          */
	/************************************************************************/
public:
	void Init();
	void Close();
	void Update(float InDeltaTime);

public:
	void Enter();
	void Exit();
	void OnReconectionEnd();

private:
	void RegisterMessages();
	void RemoveMessage();

private:
	void BindEvents();
	void UnBindEvents();

private:
	void InitEnterTime();

	bool CanEnterSaveBattery();

	/************************************************************************/
	/* 메세지 핸들 함수                                                      */
	/************************************************************************/
private:
	void OnActiveSaveBattery();
	void OnKeyboardInput(const IGsMessageParam* InParam);

	void InvalidateAll();
	void InvalidateComsumable();
	void InvalidatePlayerInfo();
	void InvalidateSystemInfo();
	void InvalidateCounterAttack();

	void OnChangeLocalPlayerLevel(uint64 InLevel);
	void OnChangePlayerExp(uint64 InExp);
	void OnIncreasePlayerExp(uint64 InExp);
	void OnIncreaseGold(uint64 InAmount);

	void OnAddItemList(FGsItemMessageParamBase& InParam);
	void OnUpdateItem(FGsItemMessageParamBase& InParam);
	void OnRemoveItem(FGsItemMessageParamBase& InParam);
	void OnRemoveItemList(FGsItemMessageParamBase& InParam);

	void OnUpdateItemWeight(const IGsMessageParam* InParam);
	void OnUpdateWeightPenaltyType(const IGsMessageParam* InParam);

	void InvalidateMode(const IGsMessageParam* InParam);
	void OnHitLocal(const IGsMessageParam* InParam);
	void OnLocalPlayerDie(const IGsMessageParam* InParam);

	void OnContentsUnlock(const IGsMessageParam* InParam);

	void OnDungeonTimeSetData(const IGsMessageParam* InParam);
	void OnDungeonTimePlay(const IGsMessageParam* InParam);
	void OnDungeonTimePause(const IGsMessageParam* InParam);
	void OnDungeonTimeStop(const IGsMessageParam* InParam);

	void OnInvalidateDetectionTargetList(const IGsMessageParam* InParam);

public:
	void OnClose(const IGsMessageParam* InParam = nullptr);
	void OnCloseAll(const IGsMessageParam* InParam = nullptr);
	void OnCloseUI(const IGsMessageParam * InParam = nullptr);
	void InvalidateInventory(const IGsMessageParam* InParam = nullptr);

	/************************************************************************/
	/* 바인딩 함수                                                           */
	/************************************************************************/
	void OnSetLocalPlayerStat(const FGsStatInfo& InStatInfo);

	/************************************************************************/
	/* 이벤트 우선순위                                                       */
	/************************************************************************/
public:
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;
	void RequestOfflinePlay();
	void RequestReturn();

	/************************************************************************/
	/* 기타 함수                                                             */
	/************************************************************************/
	// 월드 렌더링 On/Off
	void SetEnableWorldRendering(const bool bInEnable);
	// 프레임 성능 제한
	void SetFrameRateLimit(float InFrameRate);


	bool IsDungeonTimeShow() { return _isDungeonTimeShow; }
	bool IsDungeonTimeLimit() { return _isDungeonTimeLimit; }
	bool IsDungeonTimeShowSecond() { return _isDungeonTimeShowSecond; }
	FDateTime GetDungeonEndTime() { return _dungeonEndTime; }
};
