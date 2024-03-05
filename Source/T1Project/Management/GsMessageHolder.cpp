

#include "GsMessageHolder.h"
#include "../Classes/GsMessageLog.h"


//-------------------------------------------------------------------------------------------------
// Enum 타입을 문자열 변환
//-------------------------------------------------------------------------------------------------
template<typename T>
FString TypeString(FString ltype)
{
	return ltype;
}

//-------------------------------------------------------------------------------------------------
// Enum 타입별 로그출력을 위한 특성 처리
//-------------------------------------------------------------------------------------------------
#pragma region Traits 선언부
namespace EMessageLogType
{
	enum Type : uint8
	{
		LOG_NORMAL,
		LOG_WARNING,
		LOG_ERROR,
		LOG_SCREEN,
	};
}

template <typename TEnumState>
struct TGsTypeTraits;

#define MESSAGE_LOG_TYPE(Target, LogType, flag)									\
template<>																		\
struct TGsTypeTraits<Target>													\
{																				\
	static const EMessageLogType::Type	Type	= EMessageLogType::LogType;		\
	static const bool					Enable	= flag;							\
	static const FString				TName;									\
};																				\
const FString TGsTypeTraits<Target>::TName = TypeString<Target>(#Target);   												
using namespace EMessageLogType;
#pragma endregion
//-------------------------------------------------------------------------------------------------
//  타입 별 로그 설정
// 
//  인자 1: UEnum 클래스 : 메시지 키값
//  인자 2: 출력 로그 방식
//  읹자 3: 메시지 출력여부 결정
//-------------------------------------------------------------------------------------------------
#pragma region 메시지 출력 타입설정
MESSAGE_LOG_TYPE(MessageSystem,						LOG_ERROR,		true)
MESSAGE_LOG_TYPE(MessageStage,						LOG_WARNING,	true)
MESSAGE_LOG_TYPE(MessageGameObject,					LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentQuest,				LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentEvent,				LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageItem,						LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentCamera,				LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageUI,							LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContents,					LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentsLobby,				LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentHud,					LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentHUDRedDot,			LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentUnlock,				LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentItem,				LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentInven,				LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentCommonInven,			LOG_NORMAL,		true)


MESSAGE_LOG_TYPE(MessageContentEnchant,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentItemCollection,		LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentStoreBM,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentNpcShop,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentMarket,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentCraft,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageUserInfo,					LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentPlayerInfo,			LOG_NORMAL,		false)


MESSAGE_LOG_TYPE(MessageContentGuild,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentGuildDungeon,		LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentDungeon,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentRanking,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentMail,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentChat,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentSpaceCrack,			LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentOfflinePlay,			LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentCommunity,			LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentMultiLevelRank,		LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentMonsterKnowledge,	LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageFairy,						LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageCostume,					LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentAchievement,			LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentObserver,			LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageBoss,						LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentMinimap,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentSaveBattery,			LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentTitle,				LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentUserProfile,			LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentPlayableEventHud,	LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageInput,						LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentUserContext,			LOG_NORMAL,		true)
MESSAGE_LOG_TYPE(MessageContentTutorial,			LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentPersoncalTrading,	LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentSanctum,				LOG_NORMAL,		false)
MESSAGE_LOG_TYPE(MessageContentBattleArena, 		LOG_NORMAL, 	false)
#pragma endregion


#pragma region 출력 구현부

//-------------------------------------------------------------------------------------------------
// Enum 을 문자열로 추출
//-------------------------------------------------------------------------------------------------
template <typename T>
FString EnumToString(const FString& enumName, const T value, const FString& defaultValue)
{
	if (enumName.IsEmpty())
		return enumName;

	UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, *enumName, true);
	return pEnum ? pEnum->GetNameStringByIndex((int32)value) : defaultValue;
}


//-------------------------------------------------------------------------------------------------
// 메시지를 특성에 맞추어 출력하기위한 로직 (Callby MessageHandler)
//-------------------------------------------------------------------------------------------------
template<typename T>
void PrintTypeLog(T MessageId, FString& tagName)
{
	if (false == TGsTypeTraits<T>::Enable)
		return;

	FString enumName = EnumToString<T>(TGsTypeTraits<T>::TName, MessageId, TEXT("Error"));

	switch (TGsTypeTraits<T>::Type)
	{
	case Type::LOG_NORMAL:
		GS_MSSAGE_LOG(Log, TEXT("%s ▬ [%s]::[%s]"), *tagName, *TGsTypeTraits<T>::TName, *enumName);
		break;
	case Type::LOG_WARNING:
		GS_MSSAGE_LOG(Warning, TEXT("%s ▬ [%s]::[%s]"), *tagName, *TGsTypeTraits<T>::TName, *enumName);
		break;
	case Type::LOG_ERROR:
		GS_MSSAGE_LOG(Error, TEXT("%s ▬ [%s]::[%s]"), *tagName, *TGsTypeTraits<T>::TName, *enumName);
		break;
	case Type::LOG_SCREEN:
		break;
	}
}
#pragma endregion
//-------------------------------------------------------------------------------------------------
// 맴버 초기화 Helper
// 
// 인자1: 맴버변수 이름
// 인자2: (Tag 기능) : 메시지 출력시 맨 앞에 출력되는 문자열 
// - 로그 검색시 특정 문자열을 검색할때 활용 
// - \t 를 활용해서 깊이표현을 한다던지..
//-------------------------------------------------------------------------------------------------
#define INIT_MSG(member, Tag)		\
member.SetTag(Tag);					\
member.SetLogger(PrintTypeLog)	

//-------------------------------------------------------------------------------------------------
// FGsMessageHolder 구현부
//-------------------------------------------------------------------------------------------------
FGsMessageHolder::FGsMessageHolder()
{
	INIT_MSG(_system, TEXT("♠"));
	INIT_MSG(_systemParam, TEXT("♠"));
	INIT_MSG(_systemInt, TEXT("♠1"));
	INIT_MSG(_stage, TEXT("♣"));
	INIT_MSG(_stageParam, TEXT("♣"));
	INIT_MSG(_quest, TEXT("\t ▼"));
	INIT_MSG(_event, TEXT("\t ▼"));
	INIT_MSG(_gameObject, TEXT("\t ▼"));
	INIT_MSG(_contentsLobby, TEXT("MContentsLobby"));
	INIT_MSG(_contents, TEXT("MContents"));
	INIT_MSG(_contentsHud, TEXT("MHud"));
	INIT_MSG(_contentsPlayableEventHud, TEXT("MPlayableEventHud"));
	INIT_MSG(_input, TEXT("MInput"));
	INIT_MSG(_enemyHudInfo, TEXT("MEnemyHud"));
	INIT_MSG(_skillReserveAction, TEXT("MReserveSkillAction"));
	INIT_MSG(_uiMsg, TEXT("MUI"));
	INIT_MSG(_areaTriggerEvent, TEXT("MAreaTriggerEvent"));
	INIT_MSG(_inventory, TEXT("MInventory"));
	INIT_MSG(_hudRedDot, TEXT("MHudRedDot"));
	INIT_MSG(_itemEnchant, TEXT("MItemEnchant"));
	INIT_MSG(_playerDetailInfo, TEXT("MPlayerDetailInfo"));
	INIT_MSG(_item, TEXT("MItem"));
	INIT_MSG(_itemContents, TEXT("MItemContents"));
	INIT_MSG(_npcStore, TEXT("MNpcShop"));
	INIT_MSG(_bmShop, TEXT("MBMShop"));
	INIT_MSG(_userInfoMsg, TEXT("MUserBasicInfo"));
	INIT_MSG(_uiMinimapSpawnType, TEXT("MUIMinimapSpawnType"));
	INIT_MSG(_uiMinimapIcon, TEXT("MUIMinimapIcon"));
	INIT_MSG(_uiMinimapIconWidget, TEXT("MUIMinimapIconWidget"));
	INIT_MSG(_uiMinimapSceneImage, TEXT("MUIMinimapSceneImage"));
	INIT_MSG(_uiMinimapIconPos, TEXT("MUIMinimapIconPos"));
	INIT_MSG(_uiMinimapIconTeam, TEXT("MUIMinimapIconTeam"));
	INIT_MSG(_camera, TEXT("MCamera"));
	INIT_MSG(_guild, TEXT("MGuild"));
	INIT_MSG(_mail, TEXT("MMail"));
	INIT_MSG(_itemCollection, TEXT("MItemCollection"));
	INIT_MSG(_chat, TEXT("MChat"));
	INIT_MSG(_dungeon, TEXT("MDungeon"));
	INIT_MSG(_guildDungeon, TEXT("MGuildDungeon"));
	INIT_MSG(_market, TEXT("MMarket"));
	INIT_MSG(_spaceCrack, TEXT("MSpaceCrack"));
	INIT_MSG(_multiLevelRank, TEXT("MMultiLevelRank"));
	INIT_MSG(_offlinePlay, TEXT("MOfflinePlay"));
	INIT_MSG(_craft, TEXT("MCraft"));	
	INIT_MSG(_community, TEXT("MCommunity"));
	INIT_MSG(_ranking, TEXT("MRanking"));
	INIT_MSG(_saveBattery, TEXT("MSaveBattery"));
	INIT_MSG(_observer, TEXT("MObserver"));
	INIT_MSG(_fairy, TEXT("MFairy"));
	INIT_MSG(_costume, TEXT("MCostume"));
	INIT_MSG(_unlock, TEXT("MUnlock"));
	INIT_MSG(_tutorial, TEXT("MTutorial"));
	INIT_MSG(_userProfile, TEXT("MUserProfile"));
	INIT_MSG(_userContext, TEXT("MUserContext"));
	INIT_MSG(_personalTrading, TEXT("MPersonalTrading"));
	INIT_MSG(_sanctum, TEXT("MSanctum"));
	INIT_MSG(_battleArena, TEXT("MBattleArena"));
}

/** [Tip] SendAsyncMessage 함수를 사용하고 싶을 때는, 아래 함수에서 MessageHandler 가 update 를 호출하도록 만들어야 한다. */
void FGsMessageHolder::Update(float inTick)
{
#pragma todo("bak1210 : Message Update Sequence Set Need!")
	_system.Update();
	_systemParam.Update();
	_systemInt.Update();

	_stage.Update();
	_stageParam.Update();

	_gameObject.Update();
	

	_item.Update();
	_itemContents.Update();
	_contents.Update();
	_contentsHud.Update();
	_contentsPlayableEventHud.Update();
	_input.Update();
	_enemyHudInfo.Update();
	_skillReserveAction.Update();
	_quest.Update();
	_event.Update();
	_uiMsg.Update();
	_inventory.Update();
	_areaTriggerEvent.Update();

	_multiLevelRank.Update();
	_offlinePlay.Update();

	_userInfoMsg.Update();
	_hudRedDot.Update();
	_itemEnchant.Update();
	_playerDetailInfo.Update();

	_fairy.Update(); 
	_npcStore.Update();
	_guild.Update();
	_guildDungeon.Update();
	_market.Update();
	_craft.Update();
	_bmShop.Update();
	_ranking.Update();
	_saveBattery.Update();
	_monsterKnowledgeCollection.Update();
	_title.Update();
	_unlock.Update();
	_tutorial.Update();
	_userProfile.Update();
	_userContext.Update();
	_personalTrading.Update();
	_sanctum.Update();
	_battleArena.Update();

	_hiveEvent.Update();
}