// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDMainMenu.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/HUD/MainMenu/GsUIMainMenuBtnSimple.h"
#include "UI/UIContent/HUD/MainMenu/GsUIMainMenuBtnNormal.h"
#include "UI/UIContent/HUD/MainMenu/GsUIMainMenuBtnInventory.h"
#include "UI/UIContent/HUD/MainMenu/GsUIMainMenuBtnBMShop.h"
#include "UI/UIContent/HUD/MainMenu/GsUIMainMenuBtnHamberger.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGame/GsBuddyManager.h"
#include "Management/ScopeGame/GsMailManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Management/ScopeGame/GsMultiLevelRankManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsMonsterCollectionManager.h"
#include "Management/ScopeGame/GsCodexManager.h"
#include "Management/ScopeGame/GsTitleManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsAchievementManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGame/GsPKBookManager.h"
#include "Management/ScopeGame/GsCommunityEventManager.h"
#include "Management/ScopeGame/GsBattlePassManager.h"

#include "Net/GsNetSendService.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"

#include "Message/GsMessageContentDungeon.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsInvenMessageParam.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Message/MessageParam/GsItemCollectionMessageParam.h"
#include "Message/GsMessageInput.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGameClientVersion.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"
#include "Item/Craft/GsCraftTarget.h"

#include "UTIL/GsGameObjectUtil.h"

#include "Guild/GsGuildHelper.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "Input/GsInputEventMsgBase.h"
#include "Message/GsMessageInput.h"
#include "Net/GsNetSendServiceBoss.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "T1Project.h"

const FTextKey POPUP_COMMON_LEAVE1 = TEXT("Popup_Common_Leave1");
const FTextKey POPUP_COMMON_LEAVE2 = TEXT("Popup_Common_Leave2");


void UGsUIHUDMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 각 메뉴 버튼에 맞는 함수를 바인딩합니다.
	InitializeMenuFunc();
	
	// 각 버튼 버튼과 관련된 위젯을 하나로 묶습니다.
	InitializeMenuWidget();

	// MenuType - UnlockContentsType 연결
	InitializeMenuUnlockType();
}

void UGsUIHUDMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// 메뉴 레드닷 설정
	InitializeMenuRedDot();

	CheckMainMenuAct();

	// 메시지 등록
	BindMessages();

	// 햄버거 메뉴 닫기 : 초기 상태
	ShowSubMenu(false, false);
}

void UGsUIHUDMainMenu::NativeDestruct()
{
	// 메시지 해제
	UnbindMessages();

	Super::NativeDestruct();
}

void UGsUIHUDMainMenu::InvalidateAllInternal()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		SetHUDMode(uiMgr->GetHUDMode());
	}

	// 컨텐츠 활성화 갱신
	InvalidateMenu(nullptr);
	// 레드닷 갱신
	UpdateRedDotAll(true);
}

bool UGsUIHUDMainMenu::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	_btnBMShop->UpdateManagerTick(InDeltaTime);

	return true;
}

void UGsUIHUDMainMenu::InitializeMenuFunc()
{
	_btnExit->OnClicked.AddDynamic(this, &UGsUIHUDMainMenu::OnClickExit);

	_btnMail->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickMail);
	_btnItemCollection->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickItemCollection);
	_btnFriend->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickFriend);
	_btnQuest->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickQuest);
	_btnInventory->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickInvenOpen);
	_btnOption->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickOption);
	_btnQuickMenu->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickQuickMenu);
	
	_btnFairy->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickFairy);
	_btnGuild->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickGuild);
	_btnCostume->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickCostume);
	_btnDungeon->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickDungeon);
	_btnSkill->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickSkill);
	_btnMultiLevelRank->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnMultiLevelRank);
	_btnMarket->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickMarket);
	_btnCraft->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickCraft);
	_btnOffLinePlay->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickOfflinePlay);
	_btnCommunity->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickEvent);
	_btnBMShop->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickBMShop);
	_btnRanking->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickRanking);
	_btnAchievement->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickAchievement);
	_btnMonsterKnowledgeCollection->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickMonsterKnowledgeCollection);
	_btnTitle->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickTitle);
	_btnGameStatistics->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickGameStatistics);
	_btnUserProfile->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickUserProfile);
	_btnBoss->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickBoss);
	_btnGoToLobby->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickGoToLobby);
	_btnPKBook->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickPKBook);
	_btnInvade->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickInvade);
	_btnSeal->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickSeal);
	_btnScreenshot->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickScreenshot);
	_btnBattlePass->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickBattlePass);
	_btnSiegeWarfare->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickSiegeWarfare);
	_btnSpiritShot->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickSpiritShot);
	_btnStarLegacy->OnClickMenu.BindUObject(this, &UGsUIHUDMainMenu::OnClickStarLegacy);

	// 별의 유산 테스트용 코드 추가
	// https://jira.com2us.com/jira/browse/CHR-25775
	if (true == IsBranch(EGsGameClientBranchType::TEST_STAR_LEGACY))
	{
		_btnStarLegacy->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_btnStarLegacy->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIHUDMainMenu::InitializeMenuWidget()
{
	// 각 메뉴와 연관된 위젯을 하나로 묶는다. (메뉴타입, (버튼, New 이미지, 레드닷))
	_menuMap.Empty();

	_menuMap.Emplace(EGsMenuType::EVENT, _btnCommunity);
	_menuMap.Emplace(EGsMenuType::SKILL, _btnSkill);
	_menuMap.Emplace(EGsMenuType::INVENTORY, _btnInventory);
	_menuMap.Emplace(EGsMenuType::BMSHOP, _btnBMShop);
	_menuMap.Emplace(EGsMenuType::QUICK_MENU, _btnQuickMenu);

	_menuMap.Emplace(EGsMenuType::COSTUME, _btnCostume);
	_menuMap.Emplace(EGsMenuType::FAIRY, _btnFairy);
	_menuMap.Emplace(EGsMenuType::ITEM_COLLECTION, _btnItemCollection);
	_menuMap.Emplace(EGsMenuType::QUEST, _btnQuest);
	_menuMap.Emplace(EGsMenuType::MULTI_LEVEL_RANK, _btnMultiLevelRank);
	_menuMap.Emplace(EGsMenuType::MARKET, _btnMarket);
	_menuMap.Emplace(EGsMenuType::CRAFT, _btnCraft);
	_menuMap.Emplace(EGsMenuType::GUILD, _btnGuild);
	_menuMap.Emplace(EGsMenuType::BUDDY, _btnFriend);
	_menuMap.Emplace(EGsMenuType::MAIL, _btnMail);
	_menuMap.Emplace(EGsMenuType::OPTION, _btnOption);
	_menuMap.Emplace(EGsMenuType::OFFLINEPLAY, _btnOffLinePlay);
	_menuMap.Emplace(EGsMenuType::RANKING, _btnRanking);
	_menuMap.Emplace(EGsMenuType::ACHIEVEMENT, _btnAchievement);
	_menuMap.Emplace(EGsMenuType::MONSTER_COLLECTION, _btnMonsterKnowledgeCollection);
	_menuMap.Emplace(EGsMenuType::TITLE, _btnTitle);
	_menuMap.Emplace(EGsMenuType::GAME_STATISTICS, _btnGameStatistics);
	_menuMap.Emplace(EGsMenuType::USER_PROFILE, _btnUserProfile);
	_menuMap.Emplace(EGsMenuType::DUNGEON, _btnDungeon);
	_menuMap.Emplace(EGsMenuType::BOSS, _btnBoss);
	_menuMap.Emplace(EGsMenuType::PK_BOOK, _btnPKBook);
	_menuMap.Emplace(EGsMenuType::INVADE, _btnInvade);
	_menuMap.Emplace(EGsMenuType::SEAL, _btnSeal);
	_menuMap.Emplace(EGsMenuType::BATTLEPASS, _btnBattlePass);
	_menuMap.Emplace(EGsMenuType::SIEGE_WARFARE, _btnSiegeWarfare);
	_menuMap.Emplace(EGsMenuType::SPIRIT_SHOT, _btnSpiritShot);
}

void UGsUIHUDMainMenu::InitializeMenuUnlockType()
{
	// 각 메뉴와 연관된 언락 컨텐츠 타입을 설정한다.
	_menuMap[EGsMenuType::QUEST]->SetUnlockContentsType(EGsUnlockContentsType::QUEST);
	_menuMap[EGsMenuType::SKILL]->SetUnlockContentsType(EGsUnlockContentsType::SKILL);
	_menuMap[EGsMenuType::INVENTORY]->SetUnlockContentsType(EGsUnlockContentsType::INVENTORY);
	_menuMap[EGsMenuType::BMSHOP]->SetUnlockContentsType(EGsUnlockContentsType::BMSHOP);
	_menuMap[EGsMenuType::COSTUME]->SetUnlockContentsType(EGsUnlockContentsType::COSTUME);
	_menuMap[EGsMenuType::FAIRY]->SetUnlockContentsType(EGsUnlockContentsType::FAIRY);
	_menuMap[EGsMenuType::ITEM_COLLECTION]->SetUnlockContentsType(EGsUnlockContentsType::ITEM_COLLECTION);
	_menuMap[EGsMenuType::MULTI_LEVEL_RANK]->SetUnlockContentsType(EGsUnlockContentsType::MULTI_LEVEL_RANK);
	_menuMap[EGsMenuType::MARKET]->SetUnlockContentsType(EGsUnlockContentsType::MARKET);
	_menuMap[EGsMenuType::CRAFT]->SetUnlockContentsType(EGsUnlockContentsType::CRAFT);
	_menuMap[EGsMenuType::GUILD]->SetUnlockContentsType(EGsUnlockContentsType::GUILD);
	_menuMap[EGsMenuType::BUDDY]->SetUnlockContentsType(EGsUnlockContentsType::BUDDY);
	_menuMap[EGsMenuType::RANKING]->SetUnlockContentsType(EGsUnlockContentsType::RANKING);
	_menuMap[EGsMenuType::MAIL]->SetUnlockContentsType(EGsUnlockContentsType::MAIL);
	_menuMap[EGsMenuType::OFFLINEPLAY]->SetUnlockContentsType(EGsUnlockContentsType::OFFLINE_PLAY);
	_menuMap[EGsMenuType::EVENT]->SetUnlockContentsType(EGsUnlockContentsType::COMMUNITY);
	_menuMap[EGsMenuType::ACHIEVEMENT]->SetUnlockContentsType(EGsUnlockContentsType::ACHIEVEMENT);
	_menuMap[EGsMenuType::MONSTER_COLLECTION]->SetUnlockContentsType(EGsUnlockContentsType::MONSTER_KNOWLEDGE_COLLECTION);
	_menuMap[EGsMenuType::TITLE]->SetUnlockContentsType(EGsUnlockContentsType::TITLE);
	_menuMap[EGsMenuType::GAME_STATISTICS]->SetUnlockContentsType(EGsUnlockContentsType::GAME_STATISTICS);
	_menuMap[EGsMenuType::USER_PROFILE]->SetUnlockContentsType(EGsUnlockContentsType::USER_PROFILE);
	_menuMap[EGsMenuType::DUNGEON]->SetUnlockContentsType(EGsUnlockContentsType::DUNGEON);
	_menuMap[EGsMenuType::OPTION]->SetUnlockContentsType(EGsUnlockContentsType::OPTION);
	_menuMap[EGsMenuType::BOSS]->SetUnlockContentsType(EGsUnlockContentsType::BOSS);
	_menuMap[EGsMenuType::PK_BOOK]->SetUnlockContentsType(EGsUnlockContentsType::PK_BOOK);
	_menuMap[EGsMenuType::SEAL]->SetUnlockContentsType(EGsUnlockContentsType::SEAL);
	_menuMap[EGsMenuType::INVADE]->SetUnlockContentsType(EGsUnlockContentsType::INVADE_INFO);
	_menuMap[EGsMenuType::BATTLEPASS]->SetUnlockContentsType(EGsUnlockContentsType::BATTLEPASS);
	_menuMap[EGsMenuType::SIEGE_WARFARE]->SetUnlockContentsType(EGsUnlockContentsType::SIEGE_WARFARE);
	_menuMap[EGsMenuType::SPIRIT_SHOT]->SetUnlockContentsType(EGsUnlockContentsType::SPIRIT_SHOT);
}

void UGsUIHUDMainMenu::InitializeMenuRedDot()
{
	// 개선: 각 매니저에서 상속받으면 이 변수는 필요가 없어진다. 길드 참조
	// IGsRedDotCheck 를 상속받은 매니저 등을 사용하거나 FGsRedDotCheckFunc 변수를 생성해서 사용한다.
	_reinforceSkill.SetCheckFunction([]()
		{
			return GSkill()->CheckReinforce();
		});
	_redDotCheckCostume.SetCheckFunction([]()
		{
			return GSCostume()->IsRedDot();
		});
	_redDotCheckFairy.SetCheckFunction([]()
		{
			return GSFairy()->IsRedDot();
		});
	_redDotCheckBuddy.SetCheckFunction([]()
		{
			return GSBuddy()->IsRedDot();
		});
	_redDotCheckMail.SetCheckFunction([]()
		{
			return GSMail()->IsRedDot();
		});
	_redDotCheckEvent.SetCheckFunction([]()
		{
			return GSCommunityEvent()->IsRedDot();
		});

	_redDotCheckAchievement.SetCheckFunction([]()
		{
			return GSAchivement()->IsRedDot();
		});

	_redDotCheckBattlePass.SetCheckFunction([]()
		{
			return GSBattlePass()->IsRedDot();
		});


	_redDotCheckCraft.SetRedDot(false);
	_redDotCheckStatistics.SetRedDot(false);
	_redDotCheckOfflinePlay.SetRedDot(false);
	_redDotCheckRanking.SetRedDot(false);
	_reddotCheckBoss.SetRedDot(false);

	_menuMap[EGsMenuType::INVENTORY]->SetRedDotCheck(GSInventory());
	_menuMap[EGsMenuType::SKILL]->SetRedDotCheck(GSkill());
	_menuMap[EGsMenuType::SKILL]->SetReinforceCheck(&_reinforceSkill);	
	_menuMap[EGsMenuType::COSTUME]->SetRedDotCheck(&_redDotCheckCostume);
	_menuMap[EGsMenuType::FAIRY]->SetRedDotCheck(&_redDotCheckFairy);
	_menuMap[EGsMenuType::ITEM_COLLECTION]->SetRedDotCheck(GSItemCollection());
	_menuMap[EGsMenuType::MULTI_LEVEL_RANK]->SetRedDotCheck(GSMultiLevelRank());
	_menuMap[EGsMenuType::MARKET]->SetRedDotCheck(GSMarket());
	_menuMap[EGsMenuType::GUILD]->SetRedDotCheck(GGuild());
	_menuMap[EGsMenuType::BUDDY]->SetRedDotCheck(&_redDotCheckBuddy);
	_menuMap[EGsMenuType::MAIL]->SetRedDotCheck(&_redDotCheckMail);
	_menuMap[EGsMenuType::EVENT]->SetRedDotCheck(&_redDotCheckEvent);
	_menuMap[EGsMenuType::BMSHOP]->SetRedDotCheck(GBMShop());
	_menuMap[EGsMenuType::QUEST]->SetRedDotCheck(GSQuest());
	_menuMap[EGsMenuType::ACHIEVEMENT]->SetRedDotCheck(&_redDotCheckAchievement);
	_menuMap[EGsMenuType::MONSTER_COLLECTION]->SetRedDotCheck(GsMonsterCollection());
	//_menuMap[EGsMenuType::MONSTER_COLLECTION]->SetRedDotCheck(GsCodex());
	_menuMap[EGsMenuType::TITLE]->SetRedDotCheck(GTitle());
	_menuMap[EGsMenuType::USER_PROFILE]->SetRedDotCheck(GSUserProfile());
	_menuMap[EGsMenuType::CRAFT]->SetRedDotCheck(&_redDotCheckCraft);
	_menuMap[EGsMenuType::DUNGEON]->SetRedDotCheck(GSDungeon());
	_menuMap[EGsMenuType::GAME_STATISTICS]->SetRedDotCheck(&_redDotCheckStatistics);
	_menuMap[EGsMenuType::OFFLINEPLAY]->SetRedDotCheck(&_redDotCheckOfflinePlay);
	_menuMap[EGsMenuType::RANKING]->SetRedDotCheck(&_redDotCheckRanking);
	_menuMap[EGsMenuType::BOSS]->SetRedDotCheck(&_reddotCheckBoss);
	_menuMap[EGsMenuType::PK_BOOK]->SetRedDotCheck(GsPKBook());
	_menuMap[EGsMenuType::QUICK_MENU]->SetRedDotCheck(&_redDotCheckQuickMenu, EGsIconRedDotType::NUMBER);
	_menuMap[EGsMenuType::BATTLEPASS]->SetRedDotCheck(&_redDotCheckBattlePass);

	_redDotCheckQuickMenu.Clear();
	for (int32 i = START_QUICK_MENU_SUB; i < EGsMenuType::MAX; ++i)
	{
		if (UGsUIMainMenuBtnBase* menu = GetMenu(i))
		{			
			_redDotCheckQuickMenu.AddTarget(menu->GetRedDotCheck(), menu->GetUnlockContentsType());
		}
	}
}

void UGsUIHUDMainMenu::BindMessages()
{
	FGsMessageHolder* msg = GMessage();
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::INVEN,
		this, &UGsUIHUDMainMenu::OnInventoryRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::BUDDY,
		this, &UGsUIHUDMainMenu::OnBuddyRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::GUILD,
		this, &UGsUIHUDMainMenu::OnGuildRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::MAIL,
		this, &UGsUIHUDMainMenu::OnMailRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::ITEM_COLLECTION,
		this, &UGsUIHUDMainMenu::OnItemCollectionRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::FAIRY,
		this, &UGsUIHUDMainMenu::OnFairyRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::COSTUME,
		this, &UGsUIHUDMainMenu::OnCostumeRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::SKILL,
		this, &UGsUIHUDMainMenu::OnSkillRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::MARKET,
		this, &UGsUIHUDMainMenu::OnMarketRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::CRAFT,
		this, &UGsUIHUDMainMenu::OnCraftRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::COMMUNITY,
		this, &UGsUIHUDMainMenu::OnEventRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::BMSHOP,
		this, &UGsUIHUDMainMenu::OnBMShopRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::MULTILEVELRANK,
		this, &UGsUIHUDMainMenu::OnMultiLevelRankRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::QUEST,
		this, &UGsUIHUDMainMenu::OnQuestRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::ACHEIEVEMENT,
		this, &UGsUIHUDMainMenu::OnAchievementRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::MONSTER_COLLECTION,
		this, &UGsUIHUDMainMenu::OnMonsterCollectionRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::TITLE,
		this, &UGsUIHUDMainMenu::OnTitleRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::USER_PROFILE,
		this, &UGsUIHUDMainMenu::OnUserProfileRedDot));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::DUNGEON,
		this, &UGsUIHUDMainMenu::OnDungeon));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::PK_BOOK,
		this, &UGsUIHUDMainMenu::OnPKBook));
	_messageDelegateList.Emplace(msg->GetHudRedDot().AddUObject(MessageContentHUDRedDot::BATTLEPASS,
		this, &UGsUIHUDMainMenu::OnBattlePass));

	_inventoryDelegateList.Emplace(msg->GetInventoryEvent().AddUObject(MessageContentInven::UPDATE_ITEM_WEIGHT,
		this, &UGsUIHUDMainMenu::UpdateInvenWeightCallback));
	_inventoryDelegateList.Emplace(msg->GetInventoryEvent().AddUObject(MessageContentInven::UPDATE_ITEM_WEIGHT_PENALTY,
		this, &UGsUIHUDMainMenu::UpdateWeightPenaltyTypeCallback));

	_contentsDelegateList.Emplace(msg->GetContents().AddUObject(MessageContents::CLOSE_QUICK_MENU,
		this, &UGsUIHUDMainMenu::OnCloseQuickMenu));

	_objectDelegate = msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		this, &UGsUIHUDMainMenu::OnLocalPlayerSpawnComplete);
	_objectReviveDelegate = msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_REVIVE,
		this, &UGsUIHUDMainMenu::OnLocalPlayerSpawnComplete);

	_unlockDelegateList.Emplace(msg->GetUnlock().AddUObject(MessageContentUnlock::INVALIDATE_ALL_UNLOCK,
		this, &UGsUIHUDMainMenu::InvalidateMenu));
	_unlockDelegateList.Emplace(msg->GetUnlock().AddUObject(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,
		this, &UGsUIHUDMainMenu::OnContentsUnlock));

	_inputParamDelegate = msg->GetWindowInput().AddUObject(MessageInput::Window_keyboard_Action,
		this, &UGsUIHUDMainMenu::OnWindowKeyMapping);

	_bmShopDelegate = msg->GetBMShop().AddUObject(FGsBMShopInvalidatePopupShopProduct::MESSAGE_ID,
		this, &UGsUIHUDMainMenu::UpdateBMShopEffect);

	_contentsDelegateList.Emplace(msg->GetContents().AddUObject(MessageContents::REQ_ACTIVE_WINDOW_INVADE,
		this, &UGsUIHUDMainMenu::OnCheckAndActiveInvasion));
	
	_mainMenuActDelegateList.Emplace(msg->GetMainMenuAct().AddUObject(MessageContentHudMainMenuAct::FORCED_DISABLE, 
		this, &UGsUIHUDMainMenu::OnMainMenuForcedDisable));

	_mainMenuActDelegateList.Emplace(msg->GetMainMenuAct().AddUObject(MessageContentHudMainMenuAct::NORMAL_RULES,
		this, &UGsUIHUDMainMenu::OnMainMenuReturnToNormalRule));
}

void UGsUIHUDMainMenu::UnbindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();

	MHudRedDot& msgRedDot = messageMgr->GetHudRedDot();
	for (MsgHUDRedDotHandle& elemRedDot : _messageDelegateList)
	{
		msgRedDot.Remove(elemRedDot);
	}
	_messageDelegateList.Empty();

	MInventory& msgInven = messageMgr->GetInventoryEvent();
	for (MsgHandleInventory& elemInven : _inventoryDelegateList)
	{
		msgInven.Remove(elemInven);
	}
	_inventoryDelegateList.Empty();

	MContents& msgContents = messageMgr->GetContents();
	for (MsgHandleContents& elemContents : _contentsDelegateList)
	{
		msgContents.Remove(elemContents);
	}
	_contentsDelegateList.Empty();

	MUnlock& msgUnlock = messageMgr->GetUnlock();
	for (MsgHandleUnlock& elemUnlock : _unlockDelegateList)
	{
		msgUnlock.Remove(elemUnlock);
	}
	_unlockDelegateList.Empty();

	messageMgr->GetGameObject().Remove(_objectDelegate);
	messageMgr->GetGameObject().Remove(_objectReviveDelegate);
	
	messageMgr->GetWindowInput().Remove(_inputParamDelegate);

	messageMgr->GetBMShop().Remove(_bmShopDelegate);

	MHudMainMenuAct& msgMainMenuAct = messageMgr->GetMainMenuAct();
	for (MsgHUDMainMenuActHandle& mainMacuActDel : _mainMenuActDelegateList)
	{
		msgMainMenuAct.Remove(mainMacuActDel);
	}

	_mainMenuActDelegateList.Reset();
}

void UGsUIHUDMainMenu::OnLocalPlayerSpawnComplete(const struct IGsMessageParam*)
{
	GSInventory()->UpdateItemWeightRate();
	FGsBMShopManager::SendPopupShopProduct();
}

void UGsUIHUDMainMenu::UpdateBMShopEffect(FGsSharedParam& InMessageParam)
{
	_btnBMShop->UpdateBMShopEffect(InMessageParam);
}

void UGsUIHUDMainMenu::SetHUDMode(EGsUIHUDMode InMode)
{
	// EGsMenuType 0 ~ QUICK_MENU 까지의 항목만 하면 되며, 퀵메뉴 안에 들어가는 것은 하지 말 것
	switch (InMode)
	{
	case EGsUIHUDMode::NORMAL:
	{	
		_panelTopMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		_btnExit->SetVisibility(IsEnableExit() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		//UGsUIManager* uiMgr = GUI();
		//if (nullptr == uiMgr)
		//	return;

		//if ((EGsUIHUDMode::MENU != uiMgr->GetHUDMode()))
		//	ShowSubMenu(false, false);
	}
	break;
	case EGsUIHUDMode::BATTLE:
	{
		_panelTopMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		_btnExit->SetVisibility(IsEnableExit() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	break;
	case EGsUIHUDMode::CAMERA:
	{
		_panelTopMenu->SetVisibility(ESlateVisibility::Collapsed);

		ShowSubMenu(false, false);
	}
	break;
	//case EGsUIHUDMode::MENU:
	//{
	//	SetHUDMode(_prevMode);
	//}
	//break;
	}
}

void UGsUIHUDMainMenu::UpdateRedDotAll(bool bUpdate)
{
	for (int32 i = 0; i < EGsMenuType::QUICK_MENU; ++i)
	{
		if (UGsUIMainMenuBtnBase* menu = GetMenu(i))
		{
			menu->UpdateRedDot(bUpdate);
		}
	}

	for (int32 i = START_QUICK_MENU_SUB; i < EGsMenuType::MAX; ++i)
	{
		if (UGsUIMainMenuBtnBase* menu = GetMenu(i))
		{
			menu->UpdateRedDot(bUpdate);
		}
	}

	// 퀵메뉴 갱신
	if (UGsUIMainMenuBtnBase* menu = GetMenu(EGsMenuType::QUICK_MENU))
	{
		menu->UpdateRedDot(false);
	}
}

void UGsUIHUDMainMenu::UpdateRedDot(EGsMenuType InType, bool bUpdate)
{
	if (EGsMenuType::QUICK_MENU == InType)
	{
		// 퀵 메뉴일 경우, 자신 업데이트
		UpdateRedDotQuickMenu(bUpdate);
	}
	else if (EGsMenuType::QUICK_MENU < InType)
	{
		if (UGsUIMainMenuBtnBase* menu = GetMenu(InType))
		{
			menu->UpdateRedDot(bUpdate);
		}

		if (UGsUIMainMenuBtnBase* menu = GetMenu(EGsMenuType::QUICK_MENU))
		{
			menu->UpdateRedDot(false);
		}
	}
	else
	{
		// 퀵 메뉴 안에 들어가지 않는 항목들
		if (UGsUIMainMenuBtnBase* menu = GetMenu(InType))
		{
			menu->UpdateRedDot(bUpdate);
		}
	}
}

void UGsUIHUDMainMenu::UpdateRedDotQuickMenu(bool bUpdate)
{
	for (int32 i = START_QUICK_MENU_SUB; i < EGsMenuType::MAX; ++i)
	{
		if (UGsUIMainMenuBtnBase* menu = GetMenu(i))
		{
			menu->UpdateRedDot(bUpdate);
		}
	}

	if (UGsUIMainMenuBtnBase* menu = GetMenu(EGsMenuType::QUICK_MENU))
	{
		menu->UpdateRedDot(false);
	}
}

void UGsUIHUDMainMenu::CloseInven()
{
	if (UGsInventoryManager* inventoryMgr = GSInventory())
	{
		// 인벤 닫기
		inventoryMgr->CloseInventoryPopup(false,false);
	}
	
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		// 상세창 닫기
		msgMgr->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, nullptr);

		// 강화창 닫기
		msgMgr->GetItemEnchant().SendMessage(MessageContentEnchant::CLOSE, nullptr);
	}	
}

void UGsUIHUDMainMenu::OnCloseQuickMenu()
{
	if (true == _isOpen)
	{
		ShowSubMenu(false);
	}
}

bool UGsUIHUDMainMenu::CanClickMenu(EGsMenuType InMenuType)
{
	if (UGsUIMainMenuBtnBase* menu = GetMenu(InMenuType))
	{
		bool canClick = menu->OnCheckCanClick();
		if (!canClick)
		{
			// New로 인한 메뉴 카운팅 갱신
			if (UGsUIMainMenuBtnBase* quickMenu = GetMenu(EGsMenuType::QUICK_MENU))
			{
				quickMenu->UpdateRedDot(false);
			}
		}
		return canClick;
	}
	return true;
}

void UGsUIHUDMainMenu::OnClickInvenOpen()
{
	if (CanClickMenu(EGsMenuType::INVENTORY))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_INVENTORY);
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::CLOSE_ABORT);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);

	ShowSubMenu(false, false);
}

void UGsUIHUDMainMenu::OnClickOption()
{
	if (_btnOption->IsEnableButton())
	{
		CloseInven();

		GOption()->SetInItPage();
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_OPTION);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickQuest()
{
	if (CanClickMenu(EGsMenuType::QUEST))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_QUEST);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickQuickMenu()
{
	if (false == _isOpen && true == _panelSubMenu->IsVisible())
		return;

	_isOpen = !_panelSubMenu->IsVisible();

	if (UGsUIManager* uiMgr = GUI())
	{
		if (_isOpen)
		{
			_prevMode = uiMgr->GetHUDMode();
			//uiMgr->SetHUDMode(EGsUIHUDMode::MENU);
			GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_QUICK_MENU);

			CloseInven();
		}
	}

	ShowSubMenu(_isOpen);

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnInputCloseQuickMenu()
{
	OnBack();
}

void UGsUIHUDMainMenu::OnClickMail()
{
	if (CanClickMenu(EGsMenuType::MAIL))
	{
		// TODO: 구현
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MAIL);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickItemCollection()
{
	if (CanClickMenu(EGsMenuType::ITEM_COLLECTION))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_ITEM_COLLECTION);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickFriend()
{
	if (CanClickMenu(EGsMenuType::BUDDY))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_POPUP_BUDDY);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickFairy()
{
	// https://jira.com2us.com/jira/browse/C2URWQ-4903
	// 대화 연출 서버 응답 대기중 페어리 창열때 막기
	if (false == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE) &&
		CanClickMenu(EGsMenuType::FAIRY))
	{
		GSEventProgress()->OnReqStartEvent(EGsEventProgressType::FAIRY_ROOM, this, GSTutorial()->IsActiveTutorial());
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickGuild()
{
	if (CanClickMenu(EGsMenuType::GUILD))
	{
		// 이미 길드가 있으면 레벨체크 필요없음 
		if (false == GGuild()->IsGuild())
		{
			// 길드메뉴 입장 가능한지 체크
			if (const FGsNetUserData* userData = GGameData()->GetUserData())
			{
				if (false == FGsGuildHelper::CanEnterGuildContent(userData->mLevel, true))
				{
					return;
				}
			}
		}

		// 길드메뉴 입장 가능한지 체크
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_GUILD);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickCostume()
{
	// https://jira.com2us.com/jira/browse/C2URWQ-4903
	// 대화 연출 서버 응답 대기중 코스튬 창열때 막기
	if (false == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE) &&
		CanClickMenu(EGsMenuType::COSTUME))
	{
		GSEventProgress()->OnReqStartEvent(EGsEventProgressType::COSTUME_ROOM, this, GSTutorial()->IsActiveTutorial());
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickDungeon()
{
	if (CanClickMenu(EGsMenuType::DUNGEON))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_DUNGEON);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickSkill()
{
	// https://jira.com2us.com/jira/browse/C2URWQ-4903
	// 대화 연출 서버 응답 대기중 스킬 창열때 막기
	if (false == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE) &&
		CanClickMenu(EGsMenuType::SKILL))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SKILL);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnMultiLevelRank()
{
	if (CanClickMenu(EGsMenuType::MULTI_LEVEL_RANK))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MULTI_LEVEL_RANK);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickMarket()
{
	if (CanClickMenu(EGsMenuType::MARKET))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MARKET);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickCraft()
{
	if (CanClickMenu(EGsMenuType::CRAFT))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_CRAFT);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickOfflinePlay()
{
	if (CanClickMenu(EGsMenuType::OFFLINEPLAY))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_OFFLINEPLAY);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickEvent()
{
	// https://jira.com2us.com/jira/browse/C2URWQ-3491
	// 1. 대화 연출 이동중 목적지 도달하여 서버 전송(FGsNetSendServiceWorld::SendTryInteract)순간
	// 2. 커뮤니티 소녀 메뉴 클릭시 발생
	// 3. 서버 전송시 block input 하는데  커뮤니티 소녀 버튼 클릭시 그값 체크하자
	// https://jira.com2us.com/jira/browse/C2URWQ-3994
	if (false == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE) &&
		CanClickMenu(EGsMenuType::EVENT))
	{
		GSEventProgress()->OnReqStartEvent(EGsEventProgressType::COMMUNITY_ROOM, this);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickBMShop()
{
	if (CanClickMenu(EGsMenuType::BMSHOP))
	{
		GMessage()->GetBMShop().SendMessage(FGsBMShopOpenBMShopHud::MESSAGE_ID, FGsSharedParam(nullptr));
		//GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickRanking()
{
	if (CanClickMenu(EGsMenuType::RANKING))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_RANKING);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickAchievement()
{
	if (CanClickMenu(EGsMenuType::ACHIEVEMENT))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_ACHIEVEMENT);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickMonsterKnowledgeCollection()
{
	if (CanClickMenu(EGsMenuType::MONSTER_COLLECTION))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MONSTER_COLLECTION);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickTitle()
{
	if (CanClickMenu(EGsMenuType::TITLE))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_TITLE);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickGameStatistics()
{
	if (CanClickMenu(EGsMenuType::GAME_STATISTICS))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_GAME_STATISTICS);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickUserProfile()
{
	// https://jira.com2us.com/jira/browse/C2URWQ-4903
	// 민수님이 보스창에서도 나온다고 제보
	if (false == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE) &&
		CanClickMenu(EGsMenuType::USER_PROFILE))
	{
		const FGsNetUserData* userData = GGameData()->GetUserData();
		if (nullptr == userData)
			return;

		FGsNetSendService::SendReqUserProfile(userData->mUserDBId);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickBoss()
{
	// https://jira.com2us.com/jira/browse/C2URWQ-4903
	// 민수님이 보스창에서도 나온다고 제보
	if (false == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE) &&
		CanClickMenu(EGsMenuType::BOSS))
	{
		GSEventProgress()->OnReqStartEvent(EGsEventProgressType::BOSS_ROOM, this);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickGoToLobby()
{
	if (CanClickMenu(EGsMenuType::GOTO_LOBBY))
	{
		GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickPKBook()
{
	if (CanClickMenu(EGsMenuType::PK_BOOK))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_PK_BOOK);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickInvade()
{
	if (CanClickMenu(EGsMenuType::INVADE))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_INVADE);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickSeal()
{
	if (CanClickMenu(EGsMenuType::SEAL))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SEAL);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickScreenshot()
{
	GUI()->SetHUDMode(EGsUIHUDMode::CAMERA);
}

void UGsUIHUDMainMenu::OnClickBattlePass()
{
	bool isUnlock = GSUnlock() ? GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::BATTLEPASS) : false;
	if (CanClickMenu(EGsMenuType::BATTLEPASS))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_BATTLEPASS);
	}
	else if (isUnlock && _btnBattlePass->IsForcedDisable())
	{
		FText BattlepassEmpty_Alert; // "진행 중인 시즌패스가 없습니다."
		FText::FindText(TEXT("UICommonText"), TEXT("BattlepassEmpty_Alert"), BattlepassEmpty_Alert);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, BattlepassEmpty_Alert);

		return;
	}

	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickSiegeWarfare()
{
	if (CanClickMenu(EGsMenuType::SIEGE_WARFARE))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SIEGE_WARFARE);
	}

	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickSpiritShot()
{
	if (CanClickMenu(EGsMenuType::SPIRIT_SHOT))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SPIRIT_SHOT);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::OnClickExit()
{
	UGsLevelManager* levelMgr = GLevel();
	if (nullptr == levelMgr)
	{
		return;
	}

	MapType mapType = levelMgr->GetCurrentMapType();
	switch (mapType)
	{
	case MapType::GUILD_DUNGEON:
	{
		GMessage()->GetGuildDungeon().SendMessage(MessageContentGuildDungeon::EXIT, nullptr);
		return;
	}
	break;
	case MapType::INSTANCE_SINGLE:
	{
		if (levelMgr->IsSpaceCrackMapContents())
		{
			GMessage()->GetSpaceCrack().SendMessage(MessageContentSpaceCrack::EXIT_BUTTON_CLICK, nullptr);
			return;
		}
	}
	break;
	case MapType::PARTY_DUNGEON:
	case MapType::ELITE_DUNGEON_1:
	case MapType::GUILD_WAR:
	{
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::TRY_EXIT_DUNGEON, nullptr);
		return;
	} 
	break;
	case MapType::WORLD_BOSS:
	{
		FText textLeave1;
		FText textLeave2;
		FText::FindText(WORLD_BOSS_TEXT_KEY, POPUP_COMMON_LEAVE1, textLeave1);
		FText::FindText(WORLD_BOSS_TEXT_KEY, POPUP_COMMON_LEAVE2, textLeave2);

		FGsUIHelper::PopupYesNoTwoMessage(textLeave1, textLeave2, [](bool isExit) {
			if (isExit)
			{
				FGsNetSendServiceBoss::SendReqWorldBossMapLeave();
			}});

		return;
	}
	case MapType::AGIT:
	{
		GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_LEAVE, nullptr);
		return;
	}
	break;
	case MapType::AGIT_DUNGEON_RAID:
	{
		GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_LEAVE, nullptr);
		return;
	}
	break;
	}

	// 위의 특수 타입 타지 않은 경우 아래 로직을 탐
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupYesNo"));
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get());
	if (popup)
	{
		// TEXT: 이벤트 공간에서 퇴장하는 경우, ... 퇴장하시겠습니까?
		FText textDesc;
		FText::FindText(TEXT("UICommonText"), TEXT("PopupExitEventChannelDesc"), textDesc);

		popup->SetData(textDesc, [](bool bYes)
			{
				if (bYes)
				{
					// 이벤트 채널 나가기 패킷 전송					
					FGsNetSendServiceWorld::SendRequestLeaveInstanceSingleMap();
				}
			});
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMainMenu::ShowUpdateMessage()
{
	//GSLOG(Log , TEXT("UGsUIHUDMainMenu::OnClickBtns ~~~~~~~~~~~~~~~~~~~"));
	// 시스템메세지 Widget Push
	GMessage()->GetSystem().SendMessage(MessageSystem::SHOW_DEVELOP_MESSAGE);
}

// 스택되어있는 아이템일때, 습득하면 메세지 안옴
void UGsUIHUDMainMenu::OnInventoryRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::INVENTORY, true);
}

void UGsUIHUDMainMenu::OnBMShopRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::BMSHOP, true);
}

void UGsUIHUDMainMenu::OnMultiLevelRankRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::MULTI_LEVEL_RANK, false);
}

void UGsUIHUDMainMenu::OnBuddyRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::BUDDY, true);
}

void UGsUIHUDMainMenu::OnGuildRedDot(bool bInActive)
{
	// 길드는 데이터 정리다 되어서 다시 갱신할 필요가 없어 false로 보냄
	UpdateRedDot(EGsMenuType::GUILD, false);
}

void UGsUIHUDMainMenu::OnMailRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::MAIL, true);
}

void UGsUIHUDMainMenu::OnItemCollectionRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::ITEM_COLLECTION, true);
}

void UGsUIHUDMainMenu::OnFairyRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::FAIRY, true);
}

void UGsUIHUDMainMenu::OnCostumeRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::COSTUME, true);
}

void UGsUIHUDMainMenu::OnSkillRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::SKILL, true);
}

void UGsUIHUDMainMenu::OnMarketRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::MARKET, true);
}

void UGsUIHUDMainMenu::OnCraftRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::CRAFT, true);
}

void UGsUIHUDMainMenu::OnEventRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::EVENT, bInActive);
}

void UGsUIHUDMainMenu::OnQuestRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::QUEST, true);
}

void UGsUIHUDMainMenu::OnAchievementRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::ACHIEVEMENT, true);
}

void UGsUIHUDMainMenu::OnMonsterCollectionRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::MONSTER_COLLECTION, true);
}

void UGsUIHUDMainMenu::OnTitleRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::TITLE, true);
}

void UGsUIHUDMainMenu::OnUserProfileRedDot(bool bInActive)
{
	UpdateRedDot(EGsMenuType::USER_PROFILE, true);
}

void UGsUIHUDMainMenu::OnDungeon(bool bInActive)
{
	UpdateRedDot(EGsMenuType::DUNGEON, bInActive);
}

void UGsUIHUDMainMenu::OnPKBook(bool bInActive)
{
	UpdateRedDot(EGsMenuType::PK_BOOK, bInActive);
}

void UGsUIHUDMainMenu::OnBattlePass(bool bInActive)
{
	UpdateRedDot(EGsMenuType::BATTLEPASS, bInActive);
}

void UGsUIHUDMainMenu::ShowSubMenu(bool bInOpen, bool bInPlayAnimation)
{
	_isOpen = bInOpen;

	// 상단 버튼에 이벤트
	for (int32 i = 0; i <= EGsMenuType::QUICK_MENU; ++i)
	{
		if (UGsUIMainMenuBtnBase* menu = GetMenu(i))
		{
			menu->SetMenuOpen(bInOpen);
		}
	}

	if (_isOpen)
	{
		SetSubMenuVisibility(ESlateVisibility::SelfHitTestInvisible);

		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::CLOSE_ABORT);
	}
	else
	{
		if (bInPlayAnimation)
		{
			// 애니메이션 재생할 땐 서서히 사라지는 모습 후 종료 이벤트에서 보이지 않게 설정
			SetSubMenuVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			// 애니메이션 재생안할 땐 바로 보이지 않게 설정
			SetSubMenuVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (bInPlayAnimation)
	{
		PlayOpenAnimation(bInOpen);
	}
}

void UGsUIHUDMainMenu::SetSubMenuVisibility(ESlateVisibility InVisibility)
{
	_panelSubMenu->SetVisibility(InVisibility);
	_panelSubMenuBg->SetVisibility(InVisibility);
}

bool UGsUIHUDMainMenu::IsEnableExit() const
{
	// 인스턴스 맵에서 나가기 버튼 보이도록 함
	UGsLevelManager* levelManager = GLevel();
	if (nullptr == levelManager)
	{
		return false;
	}

	TArray<MapType> isEnableList = { MapType::GUILD_DUNGEON 
		, MapType::INSTANCE_SINGLE 
		, MapType::PARTY_DUNGEON
		, MapType::ELITE_DUNGEON_1
		, MapType::WORLD_BOSS 
		, MapType::GUILD_WAR
		, MapType::AGIT_DUNGEON_RAID
		, MapType::AGIT
	};
	MapType mapType = levelManager->GetCurrentMapType();

	bool isEnable = false;
	for (MapType type : isEnableList)
	{
		if (mapType == type)
		{
			isEnable = true;
			break;
		}
	}

	return isEnable;

	/* https://jira.gamevilcom2us.com/jira/projects/CHR/issues/CHR-6839?filter=myopenissues
	// 요청으로 체크 해제
	else if (GLevel()->IsDungeonMap())
	{
		if (_btnExit->OnClicked.IsBound())
		{
			if (_btnExit->OnClicked.Contains(this, TEXT("OnClickSpaceCrackMapExit")))
			{
				_btnExit->OnClicked.RemoveDynamic(this, &UGsUIHUDMainMenu::OnClickSpaceCrackMapExit);
				_btnExit->OnClicked.AddDynamic(this, &UGsUIHUDMainMenu::OnClickExit);
			}
		}

		return true;
	}*/	
}

void UGsUIHUDMainMenu::UpdateInvenWeightCallback(const IGsMessageParam* InParam)
{
	_btnInventory->UpdateInvenWeight(InParam);
}

void UGsUIHUDMainMenu::UpdateWeightPenaltyTypeCallback(const IGsMessageParam* InParam)
{
	_btnInventory->UpdateWeightPenaltyType(InParam);
}

void UGsUIHUDMainMenu::OnExitCloseAnimation()
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
		return;

	// 닫는 애니메이션 중간에 메뉴가 다시 열렸다.
	if (true == _isOpen)
		return;

	SetSubMenuVisibility(ESlateVisibility::Collapsed);

	// 메뉴를 열기 전 상태로 되돌린다.
	uiMgr->SetHUDMode(_prevMode);
}
// 이벤트 시작
void UGsUIHUDMainMenu::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	// 절전모드에서 이전 이벤트로 돌아가는 것은 처리 안함
	if ((In_startType == EGsEventProgressType::COSTUME_ROOM ||
		In_startType == EGsEventProgressType::FAIRY_ROOM ||
		In_startType == EGsEventProgressType::COMMUNITY_ROOM) &&
		In_preType == EGsEventProgressType::SAVE_BATTERY)
	{
		return;
	}

	// 뽑기방에서 코스튬 룸, 페어리 룸에서  가는거면 처리 안함
	if ((In_startType == EGsEventProgressType::COSTUME_ROOM ||
		In_startType == EGsEventProgressType::FAIRY_ROOM) &&
		In_preType == EGsEventProgressType::SUMMON_ROOM)
	{
		return;
	}

	if (In_startType == EGsEventProgressType::FAIRY_ROOM)
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_FAIRY);
	}
	else if (In_startType == EGsEventProgressType::COSTUME_ROOM)
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_COSTUME);
	}
	else if (In_startType == EGsEventProgressType::COMMUNITY_ROOM)
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_COMMUNITY);
	}
	else if (In_startType == EGsEventProgressType::BOSS_ROOM)
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_BOSS);
		FGsGameStateBoss::GetInstance()->SelectFirstFieldBoss();
	}	
}
// 이벤트 종료
void UGsUIHUDMainMenu::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	// 코스튬, 페어리 룸에서 절전 모드가 켜진다면 안끔
	if ((In_finishType == EGsEventProgressType::COSTUME_ROOM ||
		In_finishType == EGsEventProgressType::FAIRY_ROOM ||
		In_finishType == EGsEventProgressType::COMMUNITY_ROOM) &&
		In_nextType == EGsEventProgressType::SAVE_BATTERY)
	{
		return;
	}

	// 코스튬 룸, 페어리 룸에서 뽑기방 가는거면 안끔
	if ((In_finishType == EGsEventProgressType::COSTUME_ROOM ||
		In_finishType == EGsEventProgressType::FAIRY_ROOM) &&
		In_nextType == EGsEventProgressType::SUMMON_ROOM)
	{
		return;
	}

	// 커뮤니티 소녀, 코스튬 window, 페어리 window 깨짐현상
	// https://jira.com2us.com/jira/browse/C2URWQ-5168
	if (In_finishType == EGsEventProgressType::COMMUNITY_ROOM)
	{
		GMessage()->GetCommunity().SendMessage(MessageContentCommunity::FINISH_EVENT_PROGRESS, nullptr);
	}
	else if (In_finishType == EGsEventProgressType::COSTUME_ROOM)
	{
		GUI()->CloseByKeyName(TEXT("WindowCostume"));
	}
	else if (In_finishType == EGsEventProgressType::FAIRY_ROOM)
	{
		GUI()->CloseByKeyName(TEXT("WindowFairy"));
	}
	
	// main hud로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	
}

void UGsUIHUDMainMenu::InvalidateMenu(const IGsMessageParam* InParam)
{
	for (int32 i = 0; i < EGsMenuType::QUICK_MENU; ++i)
	{
		if (UGsUIMainMenuBtnBase* menu = GetMenu(i))
		{
			menu->UpdateContentLock();
			menu->UpdateRedDot(true);
		}
	}

	for (int32 i = START_QUICK_MENU_SUB; i < EGsMenuType::MAX; ++i)
	{
		if (UGsUIMainMenuBtnBase* menu = GetMenu(i))
		{
			menu->UpdateContentLock();
			menu->UpdateRedDot(true);
		}
	}

	if (UGsUIMainMenuBtnBase* menu = GetMenu(EGsMenuType::QUICK_MENU))
	{
		menu->UpdateRedDot(false);
	}
}

void UGsUIHUDMainMenu::OnContentsUnlock(const IGsMessageParam* InParam)
{
	InvalidateAll();
}

void UGsUIHUDMainMenu::OnWindowKeyMapping(const struct IGsMessageParam* InParam)
{
	const FGsInputEventMsgWindowKey* inputEventMsgWindowKey = InParam->Cast<const FGsInputEventMsgWindowKey>();

	switch (static_cast<EGsKeyMappingType>(inputEventMsgWindowKey->WindowKeyType))
	{
	case EGsKeyMappingType::KEY_MAPPING_EVENT:
		OnClickEvent();
		break;
	case EGsKeyMappingType::KEY_MAPPING_BMSHOP:
		OnClickBMShop();
		break;
	case EGsKeyMappingType::KEY_MAPPING_INVENTORY:
		OnClickInvenOpen();
		break;
	case EGsKeyMappingType::KEY_MAPPING_SKILL:
		OnClickSkill();
		break;
	//case EGsKeyMappingType::KEY_MAPPING_QUEST:
	//	OnClickQuest();
	//	break;
	case EGsKeyMappingType::KEY_MAPPING_MENU:
		OnClickQuickMenu();
		break;
	case EGsKeyMappingType::KEY_MAPPING_NO:
		OnInputCloseQuickMenu();
		break;
	default:
		break;
	}
}

void UGsUIHUDMainMenu::OnCheckAndActiveInvasion()
{
	OnClickInvade();
}

void UGsUIHUDMainMenu::CheckMainMenuAct()
{
	if (GSBattlePass()->IsOpenBattlePass())
	{
		OnMainMenuReturnToNormalRule(static_cast<int>(EGsMenuType::BATTLEPASS));
	}
	else
	{
		OnMainMenuForcedDisable(static_cast<int>(EGsMenuType::BATTLEPASS));
	}
}

void UGsUIHUDMainMenu::OnMainMenuForcedDisable(int menuType)
{
	auto eMenuType = static_cast<EGsMenuType>(menuType);
	if (auto menuIter = _menuMap.Find(eMenuType))
	{
		(*menuIter)->SetForcedDisable(true);

		_disabledMenu.Emplace(eMenuType);

		InvalidateMenu(nullptr);
	}
}

void UGsUIHUDMainMenu::OnMainMenuReturnToNormalRule(int menuType)
{
	auto eMenuType = static_cast<EGsMenuType>(menuType);
	if (auto menuIter = _menuMap.Find(eMenuType))
	{
		(*menuIter)->SetForcedDisable(false);

		_disabledMenu.Remove(eMenuType);

		InvalidateMenu(nullptr);
	}
}

UGsUIMainMenuBtnBase* UGsUIHUDMainMenu::GetMenu(EGsMenuType InType) const
{
	if (const TWeakObjectPtr<UGsUIMainMenuBtnBase>* menu = _menuMap.Find(InType))
	{
		if (menu->IsValid())
		{
			return menu->Get();
		}
	}

	return nullptr;
}

UGsUIMainMenuBtnBase* UGsUIHUDMainMenu::GetMenu(int32 InTypeNum) const
{
	return GetMenu(static_cast<EGsMenuType>(InTypeNum));
}

bool UGsUIHUDMainMenu::OnBack()
{
	if (_isOpen)
	{
		ShowSubMenu(false, true);
		return true;
	}

	return false;
}
void UGsUIHUDMainMenu::OnClickStarLegacy()
{	
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_STAR_LEGACY);
	
	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}