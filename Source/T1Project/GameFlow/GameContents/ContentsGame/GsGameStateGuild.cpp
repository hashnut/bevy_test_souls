// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateGuild.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Message/MessageParam/GsItemCollectionMessageParam.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Net/GsNetSendServiceGuild.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "ActorEx/GsActorBossScene.h"
#include "ActorEx/GsCharacterNonPlayer.h"
#include "Guild/GsGuildHelper.h"


FGsGameStateGuild::FGsGameStateGuild() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsGuild)
{
	_guildDungeonHandler = MakeUnique<FGsGuildDungeonHandler>();

	//_reservedShotcutTabType = MainTab::Max;

	//_reservedShotcutCollectionData.itemId = INVALID_ITEM_ID;
	//_reservedShotcutCollectionData.itemName = FText::GetEmpty();

}

FGsGameStateGuild::~FGsGameStateGuild()
{
	_guildDungeonHandler.Reset();
}

void FGsGameStateGuild::Enter()
{
	FGsContentsGameBase::Enter();

	RegisterMessages();

	FGsGuildManager* guildMgr = GGuild();

	// 길드가 있으면 패킷 요청 후 메인페이지를 열고, 없으면 생성/가입 페이지 
	if (guildMgr->IsGuild())
	{
		//// 정리되지 않았을 정보 초기화
		//if (FGsGuildAuctionData* auctionData = guildMgr->GetMyGuildAuctionData())
		//{
		//	auctionData->ResetData();
		//}

		// 패킷 요청해서 상세정보 받은 후 메인페이지 열기
		guildMgr->GetSendController().SendRequestOpenGuild();

		OpenWindowGuildMain(true);
	}
	else
	{
		// 시작 페이지 열기
		GUI()->Open(TEXT("WindowGuildStart"));
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 길드코인)
	SetNavigationBarItemId(FGsGuildHelper::GetGuildCoinItemId());

	_guildDungeonHandler.Get()->Initialize();
}

void FGsGameStateGuild::Exit()
{
	DeregisterMessages();

	// 기사단 창고가 열려있으면 닫는다.
	GUI()->CloseByKeyName(TEXT("TrayGuildStorage"));

	_guildDungeonHandler.Get()->Clear();

	//if (FGsGuildManager* guildMgr = GGuild())
	//{
	//	if (FGsGuildAuctionData* auctionData = guildMgr->GetMyGuildAuctionData())
	//	{
	//		auctionData->ResetData();
	//	}
	//}

	FGsContentsGameBase::Exit();
}

void FGsGameStateGuild::Update(float InDeltaTime)
{
	if (_bReserveOpen)
	{
		_bReserveOpen = false;
		OpenWindowGuildMain(true);
	}
}

void FGsGameStateGuild::RegisterMessages()
{
	MGuild& msgGuild = GMessage()->GetGuild();
	_msgGuildHandleList.Emplace(msgGuild.AddRaw(MessageContentGuild::RECEIVE_JOIN_SUCCESS,
		this, &FGsGameStateGuild::OnReceiveJoinGuild));
	_msgGuildHandleList.Emplace(msgGuild.AddRaw(MessageContentGuild::RECEIVE_DISBAND,
		this, &FGsGameStateGuild::OnReceiveDisbandGuild));
	_msgGuildHandleList.Emplace(msgGuild.AddRaw(MessageContentGuild::RECEIVE_LEAVE,
		this, &FGsGameStateGuild::OnReceiveLeaveGuild));
	_msgGuildHandleList.Emplace(msgGuild.AddRaw(MessageContentGuild::RECEIVE_OTHER_GUILD_DETAIL_INFO,
		this, &FGsGameStateGuild::OnReceiveOtherGuildDetailInfo));
	//_msgGuildHandleList.Emplace(msgGuild.AddRaw(MessageContentGuild::ITEM_COLLECTION_OPEN_SHORTCUT,
	//	this, &FGsGameStateGuild::OnShotcutItemCollection));
}

void FGsGameStateGuild::DeregisterMessages()
{
	FGsMessageHolder* message = GMessage();

	MGuild& msgGuild = message->GetGuild();
	for (MsgGuildHandle& msgGuildHandle : _msgGuildHandleList)
	{
		msgGuild.Remove(msgGuildHandle);
	}
	_msgGuildHandleList.Empty();
}

void FGsGameStateGuild::OnReceiveJoinGuild(const IGsMessageParam*)
{
	FGsGuildManager* guildMgr = GGuild();

	// 첫 가입시엔 월드 입장 시 받는 건물정보가 없으므로, 레드닷 표시에 문제가 있어 건물 리스트를 요청한다.
	guildMgr->GetSendController().SendRequestBuildingList();

	// 가입 성공하자마자 바로 내 길드 상세정보 요청을 보낸다
	guildMgr->GetSendController().SendRequestOpenGuild();

	// 전부 닫아도 HUDState로 돌아가지 않게 false값 입력
	GUI()->CloseAllStack(nullptr, false);
	
	_bReserveOpen = true;
}

void FGsGameStateGuild::OnReceiveDisbandGuild(const IGsMessageParam*)
{
	// 전부 닫음 (전부닫으면 자동으로 HUD state로 감)
	GUI()->CloseAllStack();
}

void FGsGameStateGuild::OnReceiveLeaveGuild(const IGsMessageParam*)
{
	// 전부 닫음 (전부닫으면 자동으로 HUD state로 감)
	GUI()->CloseAllStack();
}

void FGsGameStateGuild::OnReceiveOtherGuildDetailInfo(const IGsMessageParam*)
{
	if (FGsGuildHelper::CanOpenGuildMainOther())
	{
		GUI()->Open(TEXT("PopupGuildMainOther"));
	}
}

void FGsGameStateGuild::OpenWindowGuildMain(bool bIsMyGuild)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("WindowGuildMain"));
	UGsUIWindowGuildMain* window = (widget.IsValid()) ? Cast<UGsUIWindowGuildMain>(widget.Get()) : nullptr;
	if (nullptr == window)
	{
		GSLOG(Error, TEXT("nullptr == window"));
		return;
	}

	//if (MainTab::Max != _reservedShotcutTabType)
	//{	// 2022/06/24 PKT - 바로가기 예약된 Tab 있으면 바로가기 한다.
	//	if (MainTab::ItemCollection == _reservedShotcutTabType)
	//	{
	//		window->OnShotcutItemCollection(_reservedShotcutCollectionData.itemId, _reservedShotcutCollectionData.itemName);

	//		_reservedShotcutCollectionData.itemId = INVALID_ITEM_ID;
	//		_reservedShotcutCollectionData.itemName = FText::GetEmpty();
	//	}
	//	
	//	// 2022/06/24 PKT - Clear
	//	_reservedShotcutTabType = MainTab::Max;
	//}
}

#if USE_3D_GUILD_BOSS
void FGsGameStateGuild::LoadBossScene()
{
	// bak1210 
		//지형 꺼짐현상 제거
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (player)
	{
		GLevel()->HoldStreamingLevelByLocation(player->GetLocation());
	}

	UWorld* world = GEngine->GameViewport->GetWorld();

	if (_scene.IsValid())
	{
		world->DestroyActor(_scene.Get());		
		_scene.Reset();
	}	

	UClass* loadClass = LoadObject<UClass>(nullptr, *GUILD_BOSS_SCENE_PATH);
	if (nullptr == loadClass)
	{
		GSLOG(Error, TEXT("%s"), TEXT("Boss scene load fail"));
		return;
	}

	FTransform tr = FTransform::Identity;
	FVector pos = tr.GetLocation();
	if (player)
	{
		pos = player->GetLocation();
	}

	pos.Z = 1000000.f;
	tr.SetLocation(pos);

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AGsActorBossScene* bossScene = world->SpawnActor<AGsActorBossScene>(loadClass, tr.GetLocation(), tr.Rotator());

	if (bossScene)
	{
		_scene = bossScene;
		_scene.Get()->InitAction();		
	}
}

void FGsGameStateGuild::DestroyBossScene()
{

	if (nullptr == GEngine
		|| nullptr == GEngine->GameViewport)
	{
		return;
	}

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (world)
	{
		if (_scene.IsValid())
		{
			_scene.Get()->Clear();
			world->DestroyActor(_scene.Get());
		}

		_scene.Reset();
		_mannequin.Reset();
	}

	GLevel()->UnHoldStreamingLevel();
}

void FGsGameStateGuild::InvalidBoss(const FGsSchemaGuildDungeon* inGuildData)
{
	_mannequin.Reset();

	if (_scene.IsValid())
	{
		_scene->SetMannequin(inGuildData);
		_mannequin = Cast<AGsCharacterNonPlayer>(_scene->GetMannequin());
	}
}
#endif

//void FGsGameStateGuild::OnShotcutItemCollection(const IGsMessageParam* InParam)
//{
//	const FGsItemCollectionMessageOpenParam* param = StaticCast<const FGsItemCollectionMessageOpenParam*>(InParam);
//	if (param)
//	{
//		_reservedShotcutTabType = MainTab::ItemCollection;
//		_reservedShotcutCollectionData.itemId = param->_itemTId;
//		_reservedShotcutCollectionData.itemName = param->_Name;
//	}	
//}