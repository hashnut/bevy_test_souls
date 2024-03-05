// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIWindowGuildMain.h"

#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentDungeon.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentMain.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentMember.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentOperation.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentManage.h"
//#include "UI/UIContent/Window/Guild/GsUIGuildContentItemCollection.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentQuest.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentAgit.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentDungeon.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentStorage.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Message/MessageParam/GsGuildMessageParam.h"
//#include "Message/MessageParam/GsItemCollectionMessageParam.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Net/GsNetSendServiceGuild.h"

#include "Data/GsDataContainManager.h"


void UGsUIWindowGuildMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::Guild;

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowGuildMain::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowGuildMain::OnClickCloseAllStack);

	_toggleGroupMainTab.AddToggleWidgetByParentPanel(_panelMainTab, false);
	_toggleGroupMainTab.OnSelectChanged.BindUObject(this, &UGsUIWindowGuildMain::OnClickMainTab);
	_toggleGroupMainTab.OnChangeBlocked.BindUObject(this, &UGsUIWindowGuildMain::OnClickBlockedTab);

	_contentMap.Empty();

	_contentMap.Emplace(EGsGuildMainTab::Main, _contentMain);
	_contentMap.Emplace(EGsGuildMainTab::Member, _contentMember);
	_contentMap.Emplace(EGsGuildMainTab::Dungeon, _contentDungeon);
	_contentMap.Emplace(EGsGuildMainTab::Quest, _contentQuest);
	_contentMap.Emplace(EGsGuildMainTab::Agit, _contentAgit);
	_contentMap.Emplace(EGsGuildMainTab::GuildStorage, _contentStorage);
	//_contentMap.Emplace(EGsGuildMainTab::ItemCollection, _contentItemCollection);
	_contentMap.Emplace(EGsGuildMainTab::Operation, _contentOperation);
	_contentMap.Emplace(EGsGuildMainTab::Manage, _contentManage);

	for (TPair<EGsGuildMainTab, UGsUIGuildContentBase*> itPair : _contentMap)
	{
		itPair.Value->SetMainTabIndex(static_cast<int32>(itPair.Key));
		itPair.Value->OnReqUpdateMainTabRedDot.BindUObject(this, &UGsUIWindowGuildMain::OnUpdateMainTabRedDot);
	}

	// 레드닷 체크 함수 설정
	_redDotCheckerMap.Empty();
	_redDotCheckerMap.Emplace(EGsGuildMainTab::Main, FGsRedDotChecker(_uiRedDotMain, EGsIconRedDotType::NORMAL, []()
		{
			if (FGsGuildManager* guildMgr = GGuild())
			{
				if (guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Attendance) ||
					guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Notice) ||
					guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyEnemy) ||
					guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyAlly) ||
					guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyState))
				{
					return true;
				}
			}

			return false;
		}));
	_redDotCheckerMap.Emplace(EGsGuildMainTab::Quest, FGsRedDotChecker(_uiRedDotQuest, EGsIconRedDotType::NORMAL, []()
		{
			return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Quest);
		}));
	//_redDotCheckerMap.Emplace(EGsGuildMainTab::ItemCollection, FGsRedDotChecker(_uiRedDotCollection, EGsIconRedDotType::NORMAL, []()
	//	{
	//		return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::ItemCollection);
	//	}));
	_redDotCheckerMap.Emplace(EGsGuildMainTab::Operation, FGsRedDotChecker(_uiRedDotOperation, EGsIconRedDotType::NORMAL, []()
		{
			if (FGsGuildManager* guildMgr = GGuild())
			{
				if (guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Buildings) ||
					guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Buffs) ||
					guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Sanctum))
				{
					return true;
				}
			}

			return false;
		}));
	_redDotCheckerMap.Emplace(EGsGuildMainTab::Manage, FGsRedDotChecker(_uiRedDotManage, EGsIconRedDotType::NORMAL, []()
		{
			if (FGsGuildManager* guildMgr = GGuild())
			{
				if (guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Approval))
				{
					return true;
				}
				if (IsBranch(EGsGameClientBranchType::GUILD_AUCTION))
				{
					if (guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::AcquisitionSetting))
					{
						return true;
					}
				}
			}
			return false;
		}));

	_redDotCheckerMap.Emplace(EGsGuildMainTab::GuildStorage, FGsRedDotChecker(_uiRedDotStorage, EGsIconRedDotType::NORMAL, []()
		{
			if (FGsGuildManager* guildMgr = GGuild())
			{
				if (guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Division) ||
					guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Auction) ||
					guildMgr->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::GuildCalculate))
				{
					return true;
				}
			}
			return false;
		}));

	/* 탭 별 언락컨텐츠타입 연결 */
	_unlockTypeMap.Empty();
	_unlockTypeMap.Emplace(EGsGuildMainTab::Main, EGsUnlockContentsType::GUILD_MAIN_TAB);
	_unlockTypeMap.Emplace(EGsGuildMainTab::Member, EGsUnlockContentsType::GUILD_MEMBER_TAB);
	_unlockTypeMap.Emplace(EGsGuildMainTab::Dungeon, EGsUnlockContentsType::GUILD_DUNGEON_TAB);
	_unlockTypeMap.Emplace(EGsGuildMainTab::Quest, EGsUnlockContentsType::GUILD_QUEST_TAB);
	_unlockTypeMap.Emplace(EGsGuildMainTab::Agit, EGsUnlockContentsType::GUILD_AGIT_TAB);
	_unlockTypeMap.Emplace(EGsGuildMainTab::Operation, EGsUnlockContentsType::GUILD_BUILDING_TAB);
	_unlockTypeMap.Emplace(EGsGuildMainTab::Manage, EGsUnlockContentsType::GUILD_MANAGEMENT_TAB);
	_unlockTypeMap.Emplace(EGsGuildMainTab::GuildStorage, EGsUnlockContentsType::GUILD_DIVISION_TAB);
}

void UGsUIWindowGuildMain::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	CheckUnlock();

	const static int32 DEFAULT_SELECTED_INDEX = 0;
	int32 selectedIndex = StaticCast<int32>(_lastTab);

	if (false == IsStackRecoverFlag())
	{		
		selectedIndex = DEFAULT_SELECTED_INDEX;
	}

	// 열고자하는 탭에 진입할 수 없을 경우 열 수 있는 가장 왼쪽 탭을 연다.
	if (_toggleGroupMainTab.IsBlockedIndex(selectedIndex))
	{
		const int32 MAX_TAB_INDEX = StaticCast<int32>(EGsGuildMainTab::Max);
		int32 index = 0;
		for (index = StaticCast<int32>(EGsGuildMainTab::Main); index < MAX_TAB_INDEX; ++index)
		{
			if (false == _toggleGroupMainTab.IsBlockedIndex(index))
			{
				selectedIndex = index;
				break;
			}
		}

		// 열 수 있는 탭이 하나도 없다면 닫는다.
		if (index == MAX_TAB_INDEX)
		{
			OnClickCloseAllStack();
			return;
		}
	}

	_toggleGroupMainTab.SetSelectedIndex(selectedIndex, true);

	// 주의: FGsGuildManager::CheckRedDot에서 이전 단계 검사에 레드닷이 있어 return true되었으면, 
	// 캐싱된 결과가 없을 수 있다. 
	// 상단 탭의 레드닷 표시를 위해 열 때 모든 레드닷 검사를 한 번 강제로 수행한다.
	GGuild()->CheckAllRedDot();

	UpdateRedDot(EGsGuildMainTab::All);
}

void UGsUIWindowGuildMain::NativeDestruct()
{
	// 페이지 확인하면 제거되는 레드닷 처리 위함(예: 퀘스트)
	if (UGsUIGuildContentBase* content = GetCurrentContent())
	{
		content->Hide();
	}

	// Exit은 파괴시에도 불리므로 널체크 필수
	if (FGsGuildManager* guildMgr = GGuild())
	{
		// 레드닷 체크에 필요한 정보를 저장한다
		guildMgr->SaveLastSettings();
	}

	// HUD 관련 레드닷 갱신 요청
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		messageMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, false);
	}

	UnbindMessages();

	_prevTab = EGsGuildMainTab::Max;

	Super::NativeDestruct();
}

bool UGsUIWindowGuildMain::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsGuild))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_GUILD);
		return true;
	}

	return false;
}

void UGsUIWindowGuildMain::BindMessages()
{
	MGuild& msgGuild = GMessage()->GetGuild();
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_OPEN_GUILD,
		this, &UGsUIWindowGuildMain::OnReceiveGuildInfo));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_CHANGE_MY_AUTHORITY,
		this, &UGsUIWindowGuildMain::OnChangeMyAuthority));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_MEMBER_LIST,
		this, &UGsUIWindowGuildMain::OnReceiveMemberList));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_JOIN_WAITING_USER_LIST,
		this, &UGsUIWindowGuildMain::OnReceiveJoinWaitingUserList));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::NOTIFY_JOIN_REQUEST,
		this, &UGsUIWindowGuildMain::OnNotifyJoinRequest));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_AUTHORITY_LIST,
		this, &UGsUIWindowGuildMain::OnReceiveAuthorityList));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::CHANGE_GUILD_SETTING,
		this, &UGsUIWindowGuildMain::OnReceiveChangeGuildSetting));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::CHANGE_GUILD_LEVEL,
		this, &UGsUIWindowGuildMain::OnChangeGuildLevel));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::CHANGE_CONTRIBUTION,
		this, &UGsUIWindowGuildMain::OnChangeContribution));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_CHANGE_MEMBER_GRADE,
		this, &UGsUIWindowGuildMain::OnReceiveChangeMemberGrade));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::NOTIFY_CHANGE_MEMBER_GRADE,
		this, &UGsUIWindowGuildMain::OnNotifyChangeMemberGrade));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_DAILY_ACTIVITY,
		this, &UGsUIWindowGuildMain::OnReceiveChangeDailyActivity));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::NOTIFY_JOIN_MEMBER,
		this, &UGsUIWindowGuildMain::OnNotifyJoinMember));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_REMOVE_MEMBER,
		this, &UGsUIWindowGuildMain::OnReceiveRemoveMember));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_BUILDING_LIST,
		this, &UGsUIWindowGuildMain::OnReceiveBuildingList));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_BUILDING_UPDATE,
		this, &UGsUIWindowGuildMain::OnReceiveBuildingUpdate));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::NOTIFY_CHANGE_NOTICE,
		this, &UGsUIWindowGuildMain::OnNotifyChangeNotice));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_JOIN_APPROVE_RESULT,
		this, &UGsUIWindowGuildMain::OnReceiveJoinApproveResult));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::DIPLOMACY_INVALIDATE_LIST,
		this, &UGsUIWindowGuildMain::OnDiplomacyInvalidateList));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::DIPLOMACY_RESEND_LIST,
		this, &UGsUIWindowGuildMain::OnDiplomacyResendList));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::DIPLOMACY_ENEMY_STATE_NOTIFY,
		this, &UGsUIWindowGuildMain::OnDiplomacyEnemyStateNotify));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::DIPLOMACY_ALLY_STATE_NOTIFY,
		this, &UGsUIWindowGuildMain::OnDiplomacyAllyStateNotify));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_QUEST_INFO,
		this, &UGsUIWindowGuildMain::OnReceiveQuestInfo));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::UPDATE_QUEST,
		this, &UGsUIWindowGuildMain::OnUpdateQuest));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::RECEIVE_QUEST_REFRESH,
		this, &UGsUIWindowGuildMain::OnReceiveQuestRefresh));
	//_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::ITEM_COLLECTION_UPDATE_LIST,
	//	this, &UGsUIWindowGuildMain::OnItemCollectionUpdateList));
	//_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::ITEM_COLLECTION_INVAILDATE_STAT_STATUS,
	//	this, &UGsUIWindowGuildMain::OnItemCollectionChangedStatStaus));
	//_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::ITEM_COLLECTION_CLEANUP,
	//	this, &UGsUIWindowGuildMain::OnItemCollectionCleanUp));
	//_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::ITEM_COLLECTION_INVAILDATE_ALL,
	//	this, &UGsUIWindowGuildMain::OnItemCollectionAll));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_NOTIFY_ACQUIRED,
		this, &UGsUIWindowGuildMain::OnStorageChanged));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_NOTIFY_DISTRIBUTED,
		this, &UGsUIWindowGuildMain::OnStorageChanged));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_CLOSE,
		this, &UGsUIWindowGuildMain::OnStorageClose));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::CAMP_CONSTRUCTION,
		this, &UGsUIWindowGuildMain::OnCampConstruction));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::CAMP_DESTRUCTION,
		this, &UGsUIWindowGuildMain::OnCampDestruction));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::SANCTUM_CONSTRUCTION_STATE_CHANGED,
		this, &UGsUIWindowGuildMain::OnSanctumConstructionStateChanged));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::SANCTUM_DESTRUCTION,
		this, &UGsUIWindowGuildMain::OnSanctumDestruction));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::STORAGE_INVALIDATE_LIST,
		this, &UGsUIWindowGuildMain::OnStorageInvalidateList));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::DIVISION_LIST_CHECKED,
		this, &UGsUIWindowGuildMain::OnChangedDivisionLIstCheck));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::DIVISION_INVALIDATE_REDDOT,
		this, &UGsUIWindowGuildMain::OnNotifyDivisionRedDot));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::DIVISION_REGISTER,
		this, &UGsUIWindowGuildMain::OnNotifyDivisionRegister));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::DIVISION_UPDATE_ITEM,
		this, &UGsUIWindowGuildMain::OnUpdatedDivisionItem));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::AUCTION_DETAIL_RECEIVE,
		this, &UGsUIWindowGuildMain::OnReceiveAuctionDetail));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::AUCTION_PAYBACK_UPDATED,
		this, &UGsUIWindowGuildMain::OnUpdatedAuctionPayback));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::AUCTION_INVALIDATE_REDDOT,
		this, &UGsUIWindowGuildMain::OnNotifyAuctionRedDot));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::AUCTION_UPDATE_ITEM,
		this, &UGsUIWindowGuildMain::OnUpdatedAuctionItem));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::AUCTION_REMOVE_ITEM,
		this, &UGsUIWindowGuildMain::OnRemovedAuctionItem));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::AUCTION_LIST_RESET,
		this, &UGsUIWindowGuildMain::OnResetAuctionList));
	_msgGuildHandleList.Emplace(msgGuild.AddUObject(MessageContentGuild::AGIT_STATE_CHANGED,
		this, &UGsUIWindowGuildMain::OnAgitStateChanged));

	MUserBasicInfo& msgUserInfo = GMessage()->GetUserBasicInfo();
	_msgUserInfoHandleList.Emplace(msgUserInfo.AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIWindowGuildMain::OnChangeUserCurrency));

	MSystemParam& msgSystemParam = GMessage()->GetSystemParam();
	_msgSystemParamHandle = msgSystemParam.AddUObject(MessageSystem::RESET_TIME_CONTENTS,
		this, &UGsUIWindowGuildMain::OnResetTimeContents);

	MUnlock& msgUnlock = GMessage()->GetUnlock();
	_msgUnlockHandleList.Emplace(msgUnlock.AddUObject(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,
		this, &UGsUIWindowGuildMain::OnContentsUnlock));
	_msgUnlockHandleList.Emplace(msgUnlock.AddUObject(MessageContentUnlock::INVALIDATE_ALL_UNLOCK,
		this, &UGsUIWindowGuildMain::OnContentsUnlock));

}

void UGsUIWindowGuildMain::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MGuild& msgGuild = msgMgr->GetGuild();
	for (MsgGuildHandle& handle : _msgGuildHandleList)
	{
		msgGuild.Remove(handle);
	}
	_msgGuildHandleList.Empty();

	MUserBasicInfo& msgUserInfo = msgMgr->GetUserBasicInfo();
	for (MsgUserInfoHandle& handle : _msgUserInfoHandleList)
	{
		msgUserInfo.Remove(handle);
	}
	_msgUserInfoHandleList.Empty();

	msgMgr->GetSystemParam().Remove(_msgSystemParamHandle);

	MUnlock& msgUnlock = msgMgr->GetUnlock();
	for (MsgHandleUnlock& handle : _msgUnlockHandleList)
	{
		msgUnlock.Remove(handle);
	}
	_msgUnlockHandleList.Empty();
}

void UGsUIWindowGuildMain::UpdateRedDot(EGsGuildMainTab InType)
{
	// Max값이면 전부 업데이트
	if (EGsGuildMainTab::All == InType)
	{
		for (TPair<EGsGuildMainTab, FGsRedDotChecker>& itPair : _redDotCheckerMap)
		{
			itPair.Value.UpdateRedDot();
		}

		return;
	}

	if (FGsRedDotChecker* checker = GetRedDotChecker(InType))
	{
		checker->UpdateRedDot();
	}
}

void UGsUIWindowGuildMain::SetUICurrentContent(int32 InIndex)
{
	// 이전 페이지 나가기 처리
	if (EGsGuildMainTab::Max != _prevTab)
	{
		if (UGsUIGuildContentBase* prevContent = GetContent(_prevTab))
		{
			prevContent->Hide();
		}

		// 이전 Hide에서 레드닷 확인에 대한 처리를 진행함
		// 확인이 되었으면 레드닷 제거를 위해 Update 호출 
		if (FGsRedDotChecker* checker = GetRedDotChecker(_prevTab))
		{
			checker->UpdateRedDot();
		}
	}

	_currentTab = static_cast<EGsGuildMainTab>(InIndex);
	_switcherContent->SetActiveWidgetIndex(InIndex);

	if (UGsUIGuildContentBase* content = GetContent(_currentTab))
	{
		content->Show(true, IsStackRecoverFlag());
		SetStackRecoverFlag(false);

		_prevTab = _currentTab;
	}
}

void UGsUIWindowGuildMain::CheckUnlock()
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	for (TPair<EGsGuildMainTab, EGsUnlockContentsType> tabUnlockPair : _unlockTypeMap)
	{
		bool bIsUnlock = unlockManager->IsContentsUnlock(tabUnlockPair.Value);
		int32 tabIndex = tabUnlockPair.Key;

		if (bIsUnlock)
		{
			// 컨텐츠를 사용할 수 있다.
			_toggleGroupMainTab.DeleteBlockedIndex(tabIndex);
		}
		else
		{
			// 컨텐츠를 사용할 수 없다.
			if (false == _toggleGroupMainTab.IsBlockedIndex(tabIndex))
			{
				_toggleGroupMainTab.AddBlockedIndex(tabIndex);
			}
		}
	}
}

void UGsUIWindowGuildMain::OnClickMainTab(int32 InIndex)
{
	_lastTab = static_cast<EGsGuildMainTab>(InIndex);

	// 일단 화면을 바꾼 후
	SetUICurrentContent(InIndex);

	// 해당 화면에서 필요한 패킷을 요청
	if (UGsUIGuildContentBase* content = GetCurrentContent())
	{
		content->SendRequestPacket();
	}

	bool bIsUnlocked = true;
	if (_unlockTypeMap.Contains(_currentTab))
	{
		if (FGsUnlockManager* unlockMgr = GSUnlock())
		{
			// 언락 상태인지 체크함
			bIsUnlocked = unlockMgr->IsContentsUnlock(_unlockTypeMap[_currentTab]);
		}
	}

	// 언락 상태일 때만 튜토리얼 메시지 보냄
	if (bIsUnlocked)
	{
		if (UGsUIGuildContentBase* content = GetContent(_lastTab))
		{
			FGsTutorialMsgParamOpenUI tutorialParam(GetTableKey(), content->GetTabName());
			GMessage()->GetTutorial().SendMessage(MessageContentTutorial::OPEN_UI_CHECK_PARAM, &tutorialParam);
		}
	}
}

void UGsUIWindowGuildMain::OnClickBlockedTab(int32 InIndex)
{
	EGsGuildMainTab clickedTab = StaticCast<EGsGuildMainTab>(InIndex);
	if (_unlockTypeMap.Contains(clickedTab))
	{
		GSUnlock()->IsContentsUnlock(_unlockTypeMap[clickedTab], true);
	}
}

void UGsUIWindowGuildMain::OnReceiveGuildInfo(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Main == GetCurrentTab())
	{
		_contentMain->OnReceiveGuildInfo();
	}
}

void UGsUIWindowGuildMain::OnChangeMyAuthority(const IGsMessageParam*)
{
	if (UGsUIGuildContentBase* currContent = GetCurrentContent())
	{
		currContent->OnChangeMyAuthority();
	}

	UpdateRedDot(EGsGuildMainTab::Manage);
}

void UGsUIWindowGuildMain::OnReceiveAuthorityList(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Manage == GetCurrentTab())
	{
		_contentManage->OnReceiveAuthorityList();
	}
}

void UGsUIWindowGuildMain::OnReceiveMemberList(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Member == GetCurrentTab())
	{
		_contentMember->OnRecieveMemberList();
	}
}

void UGsUIWindowGuildMain::OnReceiveJoinWaitingUserList(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Manage == GetCurrentTab())
	{
		_contentManage->OnReceiveJoinWaitingUserList();
	}

	UpdateRedDot(EGsGuildMainTab::Manage);
}

void UGsUIWindowGuildMain::OnNotifyJoinRequest(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Manage == GetCurrentTab())
	{
		_contentManage->OnNotifyJoinRequest();
	}

	UpdateRedDot(EGsGuildMainTab::Manage);
}

void UGsUIWindowGuildMain::OnReceiveChangeGuildSetting(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamSetting* param = InParam->Cast<const FGsGuildMsgParamSetting>())
	{
		if (UGsUIGuildContentBase* content = GetCurrentContent())
		{
			content->OnReceiveChangeGuildSetting(param);
		}

		if (FGsGuildMsgParamSetting::EGsGuildSettingType::Notice == param->_settingType)
		{
			UpdateRedDot(EGsGuildMainTab::Main);
		}
		if (FGsGuildMsgParamSetting::EGsGuildSettingType::Acquisition == param->_settingType)
		{
			UpdateRedDot(EGsGuildMainTab::Manage);
		}
	}
}

void UGsUIWindowGuildMain::OnChangeGuildLevel(const IGsMessageParam*)
{
	if (UGsUIGuildContentBase* content = GetCurrentContent())
	{
		content->OnChangeGuildLevel();
	}
}

void UGsUIWindowGuildMain::OnChangeContribution(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Main == GetCurrentTab())
	{
		_contentMain->OnChangeContribution();
	}
}

void UGsUIWindowGuildMain::OnReceiveChangeMemberGrade(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamUser* param = InParam->Cast<const FGsGuildMsgParamUser>())
	{
		if (EGsGuildMainTab::Member == GetCurrentTab())
		{
			_contentMember->OnReceiveChangeMemberGrade(param->_userDBId);
		}
	}
}

void UGsUIWindowGuildMain::OnNotifyChangeMemberGrade(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamMemberGrade* param = InParam->Cast<const FGsGuildMsgParamMemberGrade>())
	{
		if (UGsUIGuildContentBase* content = GetCurrentContent())
		{
			content->OnNotifyChangeMemberGrade(param);
		}
	}
}

void UGsUIWindowGuildMain::OnReceiveChangeDailyActivity(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Main == GetCurrentTab())
	{
		_contentMain->OnReceiveChangeDailyActivity();
	}

	UpdateRedDot(EGsGuildMainTab::Main);
}

void UGsUIWindowGuildMain::OnNotifyJoinMember(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Member == GetCurrentTab())
	{
		_contentMember->OnNotifyJoinMember();
	}
	else if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
	{
		_contentStorage->OnNotifyJoinMember();
	}
}

void UGsUIWindowGuildMain::OnReceiveRemoveMember(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Member == GetCurrentTab())
	{
		_contentMember->OnReceiveRemoveMember();
	}
	else if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
	{
		_contentStorage->OnReceiveRemoveMember();
	}
}

void UGsUIWindowGuildMain::OnReceiveBuildingList(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Operation == GetCurrentTab())
	{
		_contentOperation->OnReceiveBuildingList();
		UpdateRedDot(EGsGuildMainTab::Operation);
	}
}

void UGsUIWindowGuildMain::OnReceiveBuildingUpdate(const IGsMessageParam* InParam)
{
	if (UGsUIGuildContentBase* content = GetCurrentContent())
	{
		const FGsGuildMsgParamInt* param = InParam->Cast<const FGsGuildMsgParamInt>();
		content->OnUpdateBuilding(static_cast<GuildBuildingId>(param->_data));
	}

	UpdateRedDot(EGsGuildMainTab::Operation);
}

void UGsUIWindowGuildMain::OnNotifyChangeNotice(const IGsMessageParam*)
{
	if (UGsUIGuildContentBase* content = GetCurrentContent())
	{
		content->OnNotifyChangeNotice();
	}

	UpdateRedDot(EGsGuildMainTab::Main);
}

void UGsUIWindowGuildMain::OnReceiveJoinApproveResult(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Manage == GetCurrentTab())
	{
		_contentManage->OnReceiveJoinApproveResult();
	}

	UpdateRedDot(EGsGuildMainTab::Manage);
}

void UGsUIWindowGuildMain::OnDiplomacyInvalidateList(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Main == GetCurrentTab())
	{
		_contentMain->OnDiplomacyInvalidateList();
	}

	UpdateRedDot(EGsGuildMainTab::Main);
}

void UGsUIWindowGuildMain::OnDiplomacyResendList(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Main == GetCurrentTab())
	{
		_contentMain->OnDiplomacyResendList();
	}
}

void UGsUIWindowGuildMain::OnDiplomacyEnemyStateNotify(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamDiplomacyNotify* param = InParam->Cast<const FGsGuildMsgParamDiplomacyNotify>())
	{
		if (EGsGuildMainTab::Main == GetCurrentTab())
		{
			_contentMain->OnDiplomacyEnemyStateNotify(param);
		}
	}
	UpdateRedDot(EGsGuildMainTab::Main);
}

void UGsUIWindowGuildMain::OnDiplomacyAllyStateNotify(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamDiplomacyNotify* param = InParam->Cast<const FGsGuildMsgParamDiplomacyNotify>())
	{
		if (EGsGuildMainTab::Main == GetCurrentTab())
		{
			_contentMain->OnDiplomacyAllyStateNotify(param);
		}
	}
	UpdateRedDot(EGsGuildMainTab::Main);
}

void UGsUIWindowGuildMain::OnReceiveQuestInfo(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Quest == GetCurrentTab())
	{
		_contentQuest->OnReceiveQuestInfo();
		UpdateRedDot(EGsGuildMainTab::Quest);
	}
}

void UGsUIWindowGuildMain::OnUpdateQuest(const IGsMessageParam* InParam)
{
	if (EGsGuildMainTab::Quest == GetCurrentTab())
	{
		_contentQuest->OnUpdateQuest(InParam);
		UpdateRedDot(EGsGuildMainTab::Quest);
	}
}

void UGsUIWindowGuildMain::OnReceiveQuestRefresh(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Quest == GetCurrentTab())
	{
		_contentQuest->OnReceiveQuestRefresh();
	}
}

void UGsUIWindowGuildMain::OnCampConstruction(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Operation == GetCurrentTab())
	{
		_contentOperation->OnCampConstruction();
	}
}

void UGsUIWindowGuildMain::OnCampDestruction(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Operation == GetCurrentTab())
	{
		_contentOperation->OnCampDestruction();
	}
}

void UGsUIWindowGuildMain::OnSanctumConstructionStateChanged(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Operation == GetCurrentTab())
	{
		_contentOperation->OnSanctumConstructionStateChanged();
	}
}

void UGsUIWindowGuildMain::OnSanctumDestruction(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Operation == GetCurrentTab())
	{
		_contentOperation->OnSanctumDestruction();
	}
}

void UGsUIWindowGuildMain::OnStorageInvalidateList(const IGsMessageParam*)
{
	if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
	{
		_contentStorage->OnInvalidateList();
	}
}

void UGsUIWindowGuildMain::OnChangedDivisionLIstCheck(const IGsMessageParam*)
{
	if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
	{
		_contentStorage->OnChangedDivisionListCheck();
	}
}

void UGsUIWindowGuildMain::OnReceiveAuctionDetail(const IGsMessageParam*)
{
	if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
	{
		_contentStorage->OnReceiveStorageDetail();
	}
}

void UGsUIWindowGuildMain::OnUpdatedAuctionPayback(const IGsMessageParam*)
{
	if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
	{
		_contentStorage->OnUpdatedAuctionPayback();
	}
}

void UGsUIWindowGuildMain::OnNotifyDivisionRedDot(const IGsMessageParam*)
{
	UpdateRedDot(EGsGuildMainTab::GuildStorage);

	if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
	{
		_contentStorage->InvalidateRedDot();
	}
}

void UGsUIWindowGuildMain::OnNotifyDivisionRegister(const IGsMessageParam*)
{
	if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
	{
		_contentStorage->OnRegisterDivision();
	}
}

void UGsUIWindowGuildMain::OnUpdatedDivisionItem(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamUInt64* param = InParam->Cast<const FGsGuildMsgParamUInt64>())
	{
		if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
		{
			_contentStorage->OnUpdatedDivisionItem();
		}
	}
}

void UGsUIWindowGuildMain::OnNotifyAuctionRedDot(const IGsMessageParam*)
{
	UpdateRedDot(EGsGuildMainTab::GuildStorage);
	
	if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
	{
		_contentStorage->InvalidateRedDot();
	}
}

void UGsUIWindowGuildMain::OnResetAuctionList(const IGsMessageParam*)
{
	if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
	{
		_contentStorage->OnResetAuctionList();
	}
}

void UGsUIWindowGuildMain::OnUpdatedAuctionItem(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamUInt64* param = InParam->Cast<const FGsGuildMsgParamUInt64>())
	{
		if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
		{
			_contentStorage->OnUpdatedAuctionItem(param->_data);
		}
	}
}

void UGsUIWindowGuildMain::OnRemovedAuctionItem(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamUInt64* param = InParam->Cast<const FGsGuildMsgParamUInt64>())
	{
		if (EGsGuildMainTab::GuildStorage == GetCurrentTab())
		{
			_contentStorage->OnRemovedAuctionItem(param->_data);
		}
	}
}

void UGsUIWindowGuildMain::OnAgitStateChanged(const IGsMessageParam*)
{
	if (EGsGuildMainTab::Agit == GetCurrentTab())
	{
		_contentAgit->OnAgitStateChanged();
	}
}

void UGsUIWindowGuildMain::OnChangeUserCurrency(uint64 InType)
{
	if (UGsUIGuildContentBase* content = GetCurrentContent())
	{
		CurrencyType currencyType = static_cast<CurrencyType>(InType);
		content->OnChangeCurrency(currencyType);
	}
}

void UGsUIWindowGuildMain::OnResetTimeContents(const IGsMessageParam* InParam)
{
	if (const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>())
	{
		if (UGsUIGuildContentBase* content = GetCurrentContent())
		{
			content->OnResetTimeContents(static_cast<ResetTimeTicket>(param->_data));
		}
	}

	UpdateRedDot(EGsGuildMainTab::Main);
	UpdateRedDot(EGsGuildMainTab::Quest);
}

//void UGsUIWindowGuildMain::OnItemCollectionUpdateList(const IGsMessageParam* InParam)
//{
//	if (EGsGuildMainTab::ItemCollection == GetCurrentTab())
//	{
//		/*UGsUIGuildContentItemCollection* currentContent = StaticCast<UGsUIGuildContentItemCollection*>(GetCurrentContent());
//		if (nullptr != currentContent && nullptr != InParam)
//		{
//			const FGsItemCollectionMessageParam* Param = StaticCast<const FGsItemCollectionMessageParam*>(InParam);
//			if (nullptr != Param)
//			{
//				currentContent->OnUpdateItemCollectionList(Param->_type, Param->_updateList, Param->_isInvalidInventory);
//			}
//		}
//
//		UpdateRedDot(EGsGuildMainTab::ItemCollection);*/
//	}
//}
//
//void UGsUIWindowGuildMain::OnItemCollectionChangedStatStaus(const IGsMessageParam*)
//{
//	if (EGsGuildMainTab::ItemCollection == GetCurrentTab())
//	{
//		UGsUIGuildContentItemCollection* currentContent = StaticCast<UGsUIGuildContentItemCollection*>(GetCurrentContent());
//		if (nullptr != currentContent)
//		{
//			currentContent->OnInvalidateStatStatus();
//		}
//
//		UpdateRedDot(EGsGuildMainTab::ItemCollection);
//	}
//}
//
//void UGsUIWindowGuildMain::OnItemCollectionCleanUp(const IGsMessageParam*)
//{
//	if (EGsGuildMainTab::ItemCollection == GetCurrentTab())
//	{
//		UGsUIGuildContentItemCollection* currentContent = StaticCast<UGsUIGuildContentItemCollection*>(GetCurrentContent());
//		if (nullptr != currentContent)
//		{
//			currentContent->OnCleanUp();
//		}
//
//		UpdateRedDot(EGsGuildMainTab::ItemCollection);
//	}
//}
//
//void UGsUIWindowGuildMain::OnItemCollectionAll(const IGsMessageParam*)
//{
//	if (EGsGuildMainTab::ItemCollection == GetCurrentTab())
//	{
//		UGsUIGuildContentItemCollection* currentContent = StaticCast<UGsUIGuildContentItemCollection*>(GetCurrentContent());
//		if (nullptr != currentContent)
//		{
//			currentContent->OnInvalidateItemCollectionListAll();
//		}
//
//		UpdateRedDot(EGsGuildMainTab::ItemCollection);
//	}
//}

void UGsUIWindowGuildMain::OnStorageChanged(const IGsMessageParam* InParam)
{
	if (EGsGuildMainTab::Main == GetCurrentTab())
	{
		_contentMain->OnUpdateStorage();
	}

	UpdateRedDot(EGsGuildMainTab::Main);
}

void UGsUIWindowGuildMain::OnStorageClose(const IGsMessageParam* InParam)
{
	if (EGsGuildMainTab::Main == GetCurrentTab())
	{
		_contentMain->OnUpdateStorage();
	}

	UpdateRedDot(EGsGuildMainTab::Main);
}

void UGsUIWindowGuildMain::OnContentsUnlock(const IGsMessageParam* InParam)
{
	CheckUnlock();

	// 보고 있던 탭이 잠겼다면 닫는다.
	int32 currentTabIndex = StaticCast<int32>(_currentTab);
	if (_toggleGroupMainTab.IsBlockedIndex(currentTabIndex))
	{
		OnClickCloseAllStack();
	}
}

//void UGsUIWindowGuildMain::OnShotcutItemCollection(ItemId InItemId, const FText& InItemName)
//{
//	int32 itemCollectionTabIndex = StaticCast<int32>(EGsGuildMainTab::ItemCollection);
//	_toggleGroupMainTab.SetSelectedIndex(itemCollectionTabIndex, true);
//
//	TWeakObjectPtr<UWidget> widget = _switcherContent->GetWidgetAtIndex(itemCollectionTabIndex);
//	TWeakObjectPtr<UGsUIGuildContentItemCollection> itemCollectionUI 
//		= (widget.IsValid()) ? Cast<UGsUIGuildContentItemCollection>(widget.Get()) : nullptr;
//	if (itemCollectionUI.IsValid())
//	{
//		itemCollectionUI->OnShotcut(InItemId, InItemName);
//	}
//}

void UGsUIWindowGuildMain::OnUpdateMainTabRedDot(int32 InMainTabIndex)
{
	UpdateRedDot(static_cast<EGsGuildMainTab>(InMainTabIndex));
}

UGsUIGuildContentBase* UGsUIWindowGuildMain::GetContent(EGsGuildMainTab InType) const
{
	if (UGsUIGuildContentBase*const* content = _contentMap.Find(InType))
	{
		return *content;
	}
	
	return nullptr;
}

UGsUIGuildContentBase* UGsUIWindowGuildMain::GetCurrentContent() const
{
	return GetContent(_currentTab);
}

FGsRedDotChecker* UGsUIWindowGuildMain::GetRedDotChecker(EGsGuildMainTab InType)
{
	if (FGsRedDotChecker* checker = _redDotCheckerMap.Find(InType))
	{
		return checker;
	}

	return nullptr;
}

FString UGsUIWindowGuildMain::GetCurrentTabString() const
{
	if (UGsUIGuildContentBase* content = GetCurrentContent())
	{
		return content->GetTabName();
	}

	return TEXT("");
}
