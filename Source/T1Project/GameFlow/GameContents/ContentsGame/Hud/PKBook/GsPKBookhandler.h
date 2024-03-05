#pragma once

#include "CoreMinimal.h"
#include "../Message/GsMessagePKBook.h"
#include "../Message/GsMessageGameObject.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "EventProgress/GsEventProgressInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"



class FGsMockeryReserve : public IGsEventProgressInterface
{
	FText _attacker;
	FText _Dead;
	FText _customeMessage;

protected:
	void Done();
public:
	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

	void PushData(const FText& InAttacker, const FText& InDead, const FText& InCustomeMessage);
};

class FGsPKInfoGuildNotifyReserve : public IGsEventProgressInterface
{
	GuildEmblemId _killerGuilEmblemdId;
	GuildEmblemId _deadGuildEmblemdId;
	FText _killerName;	
	FText _deadName;

protected:
	void Done();
public:
	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

	void PushData(GuildEmblemId InKillerGuilEmblemdId, const FText& InKillerName, GuildEmblemId InDeadGuildEmblemdId, const FText& InDeadName);
};

class T1PROJECT_API FGsPKBookHandler
{
protected:
	// 2022/09/30 PKT - PK Book Event Delegate
	MsgPKBookArray _listPKBookDelegate;
	// 2022/09/29 PKT - Game Object Event Delegate
	MsgGameObjHandleArray _listGameObjectDelegate;

	float _elapsedTime = 0.f;
	bool _isDoneAlert = true;

	FGsMockeryReserve _reserveMockery;
	FGsPKInfoGuildNotifyReserve _reserveNofifyPKGuild;

public:
	FGsPKBookHandler() {/**/ }
	virtual ~FGsPKBookHandler() {/**/ }

protected:
	// 2022/09/30 PKT - On Alert
	void OnCloseTargetDectionAlert(const struct IGsMessageParam* InParam);
	// 2022/09/29 PKT - Local Player Spawn
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);
	// 2022/09/29 PKT - User Spawn
	void OnSpawnRemotePlayer(const struct IGsMessageParam* InParam);
	// 2022/09/29 PKT - User DeSpawn
	void OnDeSpawnRemotePlyaer(const struct IGsMessageParam* InParam);

public:
	void Init();
	void Close();
	void Update(float InDeltaTime);

	// 2022/09/29 PKT - PK 결과에 따라 세팅되어진 메세지를 보낸다.
	void SendChatMessagePKBookResult(PKBookResult InResult);
	// 2022/09/29 PKT - 조롱하기를 통해... 오는거
	void PKBookTeaseActionMessage(const FText& InAttacker, const FText& InDead, const FText& InCustomeMessage);
	// 2022/09/28 PKT - 경계 대상 로그인 메세지
	void DetectionTargetLogOnMessage(UserDBId InUserId);
	// 2022/09/30 PKT - 경계 대상이 추가 되었음.
	void AddDetectionTarget(UserDBId InUserDBId, int64 InGameId);
	// 2022/09/30 PKT - 경계 대상이 해제 되었음.
	void ClearDetectionTarget(int64 InGameId);

	// 2022/10/06 PKT - PK Book Window Option Save
	bool SavePKSettingMessage(bool InIsActive, const FText& InTeaseMessage, const FText& InRevengeMessage);

	bool SavedMockerMessage(bool InIsUsed, const FText& InMessage);

	void PKGuildNotify(GuildEmblemId InKillerGuilEmblemdId, const FText& InKillerName, GuildEmblemId InDeadGuildEmblemdId, const FText& InDeadName);
};