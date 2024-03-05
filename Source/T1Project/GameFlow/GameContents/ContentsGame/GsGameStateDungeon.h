// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Dungeon/GsDungeonStateHandler.h"
#include "Message/GsMessageContentDungeon.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageBattleArena.h"


class UGsUIWindowDungeon;
struct IGsMessageParam;
/**
 * 
 */
class T1PROJECT_API FGsGameStateDungeon : public FGsContentsGameBase
{
private:
	
private:
	FGsDungeonStateHandler _stateHandler;

	MsgDungeonHandleArray _listDungeonDelegate;
	MsgContentBattleArena _battleArenaDelegate;

private:
	int _dungeonId = 0;
	TWeakObjectPtr<class UGsUIWindowDungeon>	_windowDungeonUI;

public:
	FGsGameStateDungeon();
	virtual ~FGsGameStateDungeon();

public:
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

	// 2022/07/27 PKT - UI 갱신
	void OnInvalidate(const IGsMessageParam* InParam);
	// 2022/07/27 PKT - Window 바로가기
	void OnShotcutDungeonMapId(const IGsMessageParam* InParam);
	// 2022/07/27 PKT - Window 바로가기(파티 던전은 방법이 약간 다름)
	void OnShotcutPartyDungeon(const IGsMessageParam* InParam);
	// 2022/07/27 PKT - 파티 던전 슬롯 리스트 갱신
	void OnInvalidatePartyInfoSet(const IGsMessageParam* InParam);
	// 2022/07/27 PKT - Window 강제 닫기
	void OnForceWindowClose(const IGsMessageParam* InParam);	
	// 2022/10/26 PKT - Window가 열려 있는지 체크( 던전 리셋 시 화면 갱신을 위해 )
	void OnCheckActivateWindow(const IGsMessageParam* InParam);	

	// 2023/11/21 PKT - Battle Arena
	void OnInvalidateBattleArena(const IGsMessageParam* InParam);

public:
	FGsDungeonStateHandler* StateHandler();
};
