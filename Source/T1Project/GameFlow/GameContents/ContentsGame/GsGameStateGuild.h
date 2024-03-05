// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define USE_3D_GUILD_BOSS false

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"
#include "GuildDungeon/GsGuildDungeonHandler.h"
#include "UI/UIContent/Window/GsUIWindowGuildMain.h"
#include "SharedPointer.h"

struct IGsMessageParam;
struct FGsSchemaNpcData;
struct FGsSchemaGuildDungeon;
class AGsActorBossScene;
class AGsCharacterNonPlayer;

/**
 * 기사단 State
 */
class AGsActorBossScene;
class AGsCharacterNonPlayer;

class T1PROJECT_API FGsGameStateGuild : public FGsContentsGameBase
{
public:
	using MsgGuildHandle = TPair<MessageContentGuild, FDelegateHandle>;

	// 2022/06/24 PKT - 기사단 컬렉션 바로가기 Data
	struct FGsShotCutItemCollectionData
	{
		ItemId itemId = INVALID_ITEM_ID;
		FText itemName = FText::GetEmpty();
	};

protected:
	TArray<MsgGuildHandle> _msgGuildHandleList;

	// 2022/06/24 PKT - 바로가기 Tab Type
	//using MainTab = UGsUIWindowGuildMain::EGsGuildMainTab;
	//MainTab _reservedShotcutTabType;			// 2022/06/24 PKT - 바로가기 예약

	//FGsShotCutItemCollectionData _reservedShotcutCollectionData;		// 2022/06/24 PKT - 기사단 컬렉션 바로가기

public:
	TUniquePtr<FGsGuildDungeonHandler> _guildDungeonHandler;

#if USE_3D_GUILD_BOSS
	TWeakObjectPtr<AGsActorBossScene> _scene;
	TWeakObjectPtr<AGsCharacterNonPlayer> _mannequin;
#endif

	// B2: MessageContentGuild 메시지에 의해 열 경우, -stompmalloc 키워드 플레이 시 
	// Open하며 창에서 메시지 등록이 되어 invokeList가 늘어나 잠재적 크래시 요인이 되는 문제 수정
	bool _bReserveOpen = false;

public:
	FGsGameStateGuild();
	virtual ~FGsGameStateGuild();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float InDeltaTime) override;

protected:
	void RegisterMessages();
	void DeregisterMessages();

protected:
	// 가입 성공 받음
	void OnReceiveJoinGuild(const IGsMessageParam*);
	// 길드 해산 결과 받음
	void OnReceiveDisbandGuild(const IGsMessageParam*);
	// 길드 탈퇴 결과 받음
	void OnReceiveLeaveGuild(const IGsMessageParam*);
	// 다른 길드 상세 정보 보기 결과 받음
	void OnReceiveOtherGuildDetailInfo(const IGsMessageParam*);
	// 2021/10/29 PKT - 길드 아이템 컬렉션 바로가기
	//void OnShotcutItemCollection(const IGsMessageParam* InParam);	

protected:
	void OpenWindowGuildMain(bool bIsMyGuild);

public:
#if USE_3D_GUILD_BOSS
	void LoadBossScene();
	void DestroyBossScene();
	void InvalidBoss(const FGsSchemaGuildDungeon* inGuildData);
#endif
	class FGsGuildDungeonHandler* GetGuildDungeonHandler(){return _guildDungeonHandler.Get();}

};
