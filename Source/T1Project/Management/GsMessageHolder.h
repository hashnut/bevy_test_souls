#pragma once


#include "Management/GsScopeHolder.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageInput.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageFairy.h"
#include "Message/GsMessageCostume.h"
#include "Message/GsMessageBoss.h"
#include "Message/GsMessageContentDungeon.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessagePKBook.h"
#include "Message/GsMessageInvasion.h"
#include "Message/GsMessageSound.h"
#include "Message/GsMessageBattleArena.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Item/GsItem.h"


// Global
using	MQuest = TGsMessageHandlerOneParam<MessageContentQuest, const struct IGsMessageParam*>;
using	MEvent = TGsMessageHandlerOneParam<MessageContentEvent, const struct IGsMessageParam&>;
using	MContentsEvent = TGsMessageHandlerOneParam<MessageContentContentsEvent, const struct IGsMessageParam&>;
using	MItem = TGsMessageHandlerOneParam<MessageItem, struct  FGsItemMessageParamBase&>;
using	MCamera = TGsMessageHandler<MessageContentCamera>;
using	MSound = TGsMessageHandlerOneParam<MessageSound, const struct IGsMessageParam*>;

using	MContents = TGsMessageHandler<MessageContents>;
using	MContentsLobby = TGsMessageHandler<MessageContentsLobby>;

using	MUnlock = TGsMessageHandlerOneParam<MessageContentUnlock, const struct IGsMessageParam*>;
using	MOpenTabContents = TGsMessageHandlerOneParam<MessageContents, uint8>;// 오픈과 동시에 탭에 대한 정보가 필요한 타입들 정의(ex: 인벤토리,상점 등)
using	MHiveEvent = TGsMessageHandlerOneParam<MessageContentHiveEvent, const struct IGsMessageParam*>;

using	MContentsMigration = TGsMessageHandlerOneParam<MessageContentsMigration, const struct IGsMessageParam*>;


// Item 컨텐츠
using	MItemContents = TGsMessageHandlerOneParam<MessageContentItem, const struct IGsMessageParam*>;
using	MInventory = TGsMessageHandlerOneParam<MessageContentInven, const struct IGsMessageParam*>;
using	MInventoryCommon = TGsMessageHandlerOneParam<MessageContentCommonInven, const struct IGsMessageParam*>;
using	MItemEnchant = TGsMessageHandlerOneParam<MessageContentEnchant, const struct IGsMessageParam*>;
using	MItemCollection = TGsMessageHandlerOneParam<MessageContentItemCollection, const struct IGsMessageParam*>;
using	MItemContentSkill = TGsMessageHandlerOneParam<MessageContentItemSkill, const struct IGsMessageParam*>;
using	MBMShop = TGsMessageHandlerOneParam<MessageContentStoreBM, struct FGsSharedParam&&>;
using	MNpcShop = TGsMessageHandlerOneParam<MessageContentNpcShop, const struct IGsMessageParam*>;
using	MMarket = TGsMessageHandlerOneParam<MessageContentMarket, const struct IGsMessageParam*>;
using	MCraft = TGsMessageHandlerOneParam<MessageContentCraft, const struct IGsMessageParam*>;

//유저정보
using	MUserBasicInfo = TGsMessageHandlerOneParam<MessageUserInfo, uint64>;
using	MPlayerDetailInfo = TGsMessageHandlerOneParam<MessageContentPlayerInfo, struct FGsPlayerDetailInfoMessageParamBase*>;

// 인게임 시스템
using	MGuild = TGsMessageHandlerOneParam<MessageContentGuild, const struct IGsMessageParam*>;


using	MRanking = TGsMessageHandlerOneParam<MessageContentRanking, const struct IGsMessageParam*>;
using	MMail = TGsMessageHandlerOneParam<MessageContentMail, const struct IGsMessageParam*>;
using	MChat = TGsMessageHandlerOneParam<MessageContentChat, const struct IGsMessageParam*>;

using	MOfflinePlay = TGsMessageHandlerOneParam<MessageContentOfflinePlay, const struct IGsMessageParam*>;
using	MCommunity = TGsMessageHandlerOneParam<MessageContentCommunity, const struct IGsMessageParam*>;
using	MMultiLevelRank = TGsMessageHandlerOneParam<MessageContentMultiLevelRank, const struct IGsMessageParam*>;
using	MMonsterKnowledgeCollection = TGsMessageHandlerOneParam<MessageContentMonsterKnowledge, const struct IGsMessageParam*>;
using	MFairy = TGsMessageHandlerOneParam<MessageFairy, const struct IGsMessageParam*>;
using	MCostume = TGsMessageHandlerOneParam<MessageCostume, const struct IGsMessageParam*>;
using	MAchievement = TGsMessageHandlerOneParam<MessageContentAchievement, const struct IGsMessageParam*>;
using	MObserver = TGsMessageHandlerOneParam<MessageContentObserver, const struct IGsMessageParam*>;
using	MUIMinimapSpawnType = TGsMessageHandlerOneParam<MessageGameObject, int32>;
using	MSaveBattery = TGsMessageHandlerOneParam<MessageContentSaveBattery, const struct IGsMessageParam*>;
using	MTitle = TGsMessageHandlerOneParam<MessageContentTitle, const struct IGsMessageParam*>;
using	MTutorial = TGsMessageHandlerOneParam<MessageContentTutorial, const struct IGsMessageParam*>;
using	MUserProfile = TGsMessageHandler<MessageContentUserProfile>;
using	MUserContext = TGsMessageHandlerOneParam<MessageContentUserContext, const struct IGsMessageParam*>;
using	MPKBook = TGsMessageHandlerOneParam<MessageContentPKBook, const struct IGsMessageParam*>;
using	MArena = TGsMessageHandlerOneParam<MessageContentArena, const struct IGsMessageParam*>;
using	MInvasion = TGsMessageHandlerOneParam<MessageInvasion, const struct IGsMessageParam*>;
using	MBattlePass = TGsMessageHandlerOneParam<MessageContentBattlePass, const struct IGsMessageParam*>;
using	MMaterial = TGsMessageHandlerOneParam<MessageContentMaterial, const struct IGsMessageParam*>;
using	MPersonalTrading = TGsMessageHandler<MessageContentPersoncalTrading>;
using	MSeal = TGsMessageHandlerOneParam<MessageContentSeal, const struct IGsMessageParam*>;
using	MSpiritShot = TGsMessageHandlerOneParam<MessageContentSpiritShot, const struct IGsMessageParam*>;
using	MItemFusion = TGsMessageHandlerOneParam<MessageContentsFusion, const struct IGsMessageParam*>;
using	MSanctum = TGsMessageHandlerOneParam<MessageContentSanctum, const struct IGsMessageParam*>;

// Handle 관리
using	MsgBossHandle						= TPair<MessageBoss, FDelegateHandle>;

class T1PROJECT_API FGsMessageHolder 
{
public:
	FGsMessageHolder();

private:
	MSystem						_system;
	MSystemParam				_systemParam;
	MSystemInt					_systemInt;

	MStage						_stage;
	MStageParam					_stageParam;
	
	MQuest						_quest;
	MEvent						_event;
	MContentsEvent				_contentsEvent;
	MGameObject					_gameObject;
	MContentsLobby				_contentsLobby;
	MContents					_contents;
	MHud						_contentsHud;
	MPlayableEventHud			_contentsPlayableEventHud;
	MInput						_input;
	MWindowInput				_windowInput;
	MEnemyHud					_enemyHudInfo;
	MReserveSkillAction			_skillReserveAction;
	MUI							_uiMsg;
	MAreaTriggerEvent			_areaTriggerEvent;
	MInventory					_inventory;
	MHudRedDot					_hudRedDot;
	MItemEnchant				_itemEnchant;
	MPlayerDetailInfo			_playerDetailInfo;
	MItem						_item;
	MItemContents				_itemContents;
	MItemContentSkill			_itemContentSkill;
	MNpcShop					_npcStore;
	MBMShop						_bmShop;
	MUserBasicInfo				_userInfoMsg;
	MUIMinimapSpawnType			_uiMinimapSpawnType;
	MUIMinimapIcon				_uiMinimapIcon;
	MUIMinimapIconWidget		_uiMinimapIconWidget;
	MUIMinimapSceneImage		_uiMinimapSceneImage;
	MUIMinimapIconPos			_uiMinimapIconPos;
	MUIMinimapIconTeam			_uiMinimapIconTeam;
	MCamera						_camera;
	MSound						_sound;
	MGuild						_guild;
	MMail						_mail;
	MItemCollection				_itemCollection;
	MChat						_chat;
	MDungeon					_dungeon;
	MDungeonHUDTime				_dungeonHUDTime;
	MGuildDungeon				_guildDungeon;
	MMarket						_market;
	MSpaceCrack					_spaceCrack;
	MMultiLevelRank				_multiLevelRank;
	MOfflinePlay				_offlinePlay;
	MCraft						_craft;
	MCommunity					_community;
	MRanking					_ranking;
	MSaveBattery				_saveBattery;
	MMonsterKnowledgeCollection	_monsterKnowledgeCollection;
	MTitle						_title;
	MObserver					_observer;
	MFairy						_fairy;
	MCostume					_costume;
	MBoss						_boss;
	MInventoryCommon			_InventoryCommon;
	MAchievement				_achievement;
	MUnlock						_unlock;
	MTutorial					_tutorial;
	MUserProfile				_userProfile;
	MUserContext				_userContext;
	MPKBook						_pkBook;
	MArena						_arena;
	MInvasion					_invasion;
	MBattlePass					_battlePass;
	MMaterial					_material;
	MPersonalTrading			_personalTrading;
	MSeal						_seal;
	MHiveEvent					_hiveEvent;
	MSpiritShot					_spiritShot;
	MHudMainMenuAct				_mainMenuAct;
	MItemFusion					_itemFusion;
	MSanctum					_sanctum;
	MBattleArena				_battleArena;
	MBattleArenaRank			_battleArenaRank;
	MContentsMigration			_migration;

	

public:
	virtual ~FGsMessageHolder() = default;

public:
	//IGsManager
	void Initialize()	{};
	void Finalize()		{};
	void Update(float inTick);

public:
	// 시스템 메시지
	MSystem&						GetSystem()							{ return _system; }
	MSystemParam&					GetSystemParam()					{ return _systemParam; }
	MSystemInt&						GetSystemInt()						{ return _systemInt; }
	MStage&							GetStage()							{ return _stage; }
	MStageParam&					GetStageParam()						{ return _stageParam; }

public:
	//GameObject 타입별메시지		
	MGameObject&					GetGameObject()						{ return _gameObject; }

public:
	MContentsLobby&					GetContentsLobby()					{ return _contentsLobby; }	
	MContents&						GetContents()						{ return _contents; }	
	MHud&							GetContentsHud()					{ return _contentsHud; }
	MPlayableEventHud&				GetContentsPlayableEventHud()		{ return _contentsPlayableEventHud; }
	MInput&							GetInput()							{ return _input; }
	MWindowInput&					GetWindowInput()					{ return _windowInput; }
	MEnemyHud&						GetEnemyHudInfo()					{ return _enemyHudInfo; }
	MReserveSkillAction&			GetSkillRserveActiveState()			{ return _skillReserveAction; }
	MQuest&							GetQuest()							{ return _quest; }
	MEvent&							GetEvent()							{ return _event; }
	MContentsEvent&					GetContentsEvent()					{ return _contentsEvent; }
	MUI&							GetUI()								{ return _uiMsg; }
	MInventory&						GetInventoryEvent()					{ return _inventory; }
	MAreaTriggerEvent&				GetAreaTriggerEvent()				{ return _areaTriggerEvent; }
	
	MHudRedDot&						GetHudRedDot()						{ return _hudRedDot; }	
	MItemEnchant&					GetItemEnchant()					{ return _itemEnchant; }
	MPlayerDetailInfo&				GetPlayerDetailInfo()				{ return _playerDetailInfo; }

	MContentsMigration&				GetMigration()						{ return _migration; }

public:
	MItem&							GetItem()							{ return _item; }
	MItemContents&					GetItemContents()					{ return _itemContents; }
	MItemContentSkill&				GetItemContentSkill()				{ return _itemContentSkill; };

public:
	MNpcShop&						GetNpcShop()						{ return _npcStore;}
	MBMShop&						GetBMShop()							{ return _bmShop; }

public:
	MUserBasicInfo&					GetUserBasicInfo()					{ return _userInfoMsg; }

public:
	MUIMinimapSpawnType&			GetUIMinimapSpawnType()				{ return _uiMinimapSpawnType; }
	MUIMinimapIcon&					GetUIMinimapIcon()					{ return _uiMinimapIcon; }
	MUIMinimapIconWidget&			GetUIMinimapIconWidget()			{ return _uiMinimapIconWidget; }
	MUIMinimapSceneImage&			GetUIMinimapSceneImage()			{ return _uiMinimapSceneImage; }
	MUIMinimapIconPos&				GetUIMinimapIconPos()				{ return _uiMinimapIconPos; }
	MUIMinimapIconTeam&				GetUIMinimapIconTeam()				{ return _uiMinimapIconTeam; }

public:
	MCamera&						GetCamera()							{ return _camera; }
	MSound&							GetSound()							{ return _sound; }

public:
	MGuild&							GetGuild()							{ return _guild; }
	MMail&							GetMail()							{ return _mail; }
	MItemCollection&				GetItemCollection()					{ return _itemCollection; }
	MChat&							GetChat()							{ return _chat; }
	MDungeon&						GetDungeon()						{ return _dungeon; }
	MDungeonHUDTime&				GetDungeonHUDTime()					{ return _dungeonHUDTime; }
	MGuildDungeon&					GetGuildDungeon()					{ return _guildDungeon;}
	MMarket&						GetMarket()							{ return _market; }
	MSpaceCrack&					GetSpaceCrack()						{ return _spaceCrack; }
	MMultiLevelRank&				GetMultiLevelRank()					{ return _multiLevelRank; }
	MOfflinePlay&					GetOfflinePlay()					{ return _offlinePlay; }
	MCraft&							GetCraft()							{ return _craft; }	
	MCommunity&						GetCommunity()						{ return _community; }
	MRanking&						GetRanking()						{ return _ranking; }
	MSaveBattery&					GetSaveBattery()					{ return _saveBattery; }
	MMonsterKnowledgeCollection&	GetMonsterKnowledgeCollection()		{ return _monsterKnowledgeCollection; }
	MTitle&							GetTitle()							{ return _title; }
	MObserver&						GetObserver()						{ return _observer; }

	MFairy&							GetFairy()							{ return _fairy; }
	MCostume&						GetCostume()						{ return _costume; }
	MBoss&							GetBoss()							{ return _boss; }
	MInventoryCommon&				GetInventoryCommon()				{ return _InventoryCommon; }
	MAchievement&					GetAchievement()					{ return _achievement; }
	MUnlock&						GetUnlock()							{ return _unlock; }
	MTutorial&						GetTutorial()						{ return _tutorial; }
	MUserProfile&					GetUserProfile()					{ return _userProfile; }
	MUserContext&					GetUserContext()					{ return _userContext; }
	MPKBook&						GetPKBook()							{ return _pkBook;}
	MArena&							GetArena()							{ return _arena; }
	MInvasion&						GetInvasion()						{ return _invasion;}
	MBattlePass&					GetBattlePass()						{ return _battlePass;}
	MMaterial&						GetMaterial()						{ return _material; }
	MPersonalTrading&				GetPersonalTrading()				{ return _personalTrading; }
	MSeal&							GetSeal()							{ return _seal; }
	MHiveEvent&						GetHiveEvent()						{ return _hiveEvent; }
	MSpiritShot&					GetSpiritShot()						{ return _spiritShot; }
	MHudMainMenuAct&				GetMainMenuAct()					{ return _mainMenuAct; }
	MItemFusion&					GetItemFusion()						{ return _itemFusion;}
	MSanctum&						GetSanctum()						{ return _sanctum; }
	MBattleArena&					GetBattleArena()					{ return _battleArena; }
	MBattleArenaRank&				GetBattleArenaRank()				{ return _battleArenaRank; }
};

#define GMessage() GScope()->GetMessageHolder()
