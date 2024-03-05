// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowAgitDungeon.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/AgitDungeon/GsUIAgitDungeonPageRaid.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


void UGsUIWindowAgitDungeon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowAgitDungeon::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowAgitDungeon::OnClickCloseAllStack);

	_toggleGroupMainTab.AddToggleWidgetByParentPanel(_panelMainTab);
	_toggleGroupMainTab.OnSelectChanged.BindUObject(this, &UGsUIWindowAgitDungeon::OnSelectMainTab);

	_pageMap.Emplace(EGsAgitMainTab::Raid, _pageRaid);
}

void UGsUIWindowAgitDungeon::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	// TEXT: 공략던전
	FText textRaidTab;
	FText::FindText(TEXT("GuildText"), TEXT("AgitDungeon_Title_Raid"), textRaidTab);
	_btnTabRaid->SetTitleText(textRaidTab);

	// 첫 탭 세팅 후 OnSelectMainTab 강제 호출
	_toggleGroupMainTab.SetSelectedIndex(0, true);
}

void UGsUIWindowAgitDungeon::NativeDestruct()
{
	UnbindMessages();

	Super::NativeDestruct();
}

bool UGsUIWindowAgitDungeon::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsAgitDungeon))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_AGIT_DUNGEON);
		return true;
	}

	return false;
}

void UGsUIWindowAgitDungeon::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	_msgHandleGuildList.Empty();

	MGuild& msgGuild = msgMgr->GetGuild();
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::NOTIFY_CHANGE_MEMBER_GRADE,
		this, &UGsUIWindowAgitDungeon::OnNotifyChangeMemberGrade));
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::AGIT_DUNGEON_RAID_INFO,
		this, &UGsUIWindowAgitDungeon::OnRcvRaidInfo));
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::AGIT_DUNGEON_RAID_DETAIL_INFO,
		this, &UGsUIWindowAgitDungeon::OnRcvRaidDetailInfo));
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::AGIT_DUNGEON_RAID_BOOKMARK_LIST,
		this, &UGsUIWindowAgitDungeon::OnRcvRaidBookmarkList));
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::AGIT_DUNGEON_RAID_BOOKMARK_CHANGED,
		this, &UGsUIWindowAgitDungeon::OnRcvRaidBookmarkChanged));
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::AGIT_DUNGEON_RAID_OPEN_CHANGED,
		this, &UGsUIWindowAgitDungeon::OnRcvRaidOpenChanged));

	_msgHandleSystemParam = msgMgr->GetSystemParam().AddUObject(MessageSystem::RESET_TIME_CONTENTS,
		this, &UGsUIWindowAgitDungeon::OnResetTimeContents);

	_msgHandleUserInfo = msgMgr->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIWindowAgitDungeon::OnChangeUserCurrency);
}

void UGsUIWindowAgitDungeon::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MGuild& msgGuild = msgMgr->GetGuild();
	for (MsgHandleGuild& handle : _msgHandleGuildList)
	{
		msgGuild.Remove(handle);
	}
	_msgHandleGuildList.Empty();

	msgMgr->GetSystemParam().Remove(_msgHandleSystemParam);

	msgMgr->GetUserBasicInfo().Remove(_msgHandleUserInfo);
}

void UGsUIWindowAgitDungeon::OnNotifyChangeMemberGrade(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamMemberGrade* param = InParam->Cast<const FGsGuildMsgParamMemberGrade>())
	{
		TWeakObjectPtr<UGsUIAgitDungeonPageBase> page = GetPage(GetCurrentTab());
		if (page.IsValid())
		{
			page->OnNotifyChangeMemberGrade(param);
		}
	}
}

void UGsUIWindowAgitDungeon::OnRcvRaidInfo(const IGsMessageParam*)
{
	if (EGsAgitMainTab::Raid == GetCurrentTab())
	{
		_pageRaid->OnRcvRaidInfo();
	}
}

void UGsUIWindowAgitDungeon::OnRcvRaidDetailInfo(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamInt* param = InParam->Cast<const FGsGuildMsgParamInt>())
	{
		if (EGsAgitMainTab::Raid == GetCurrentTab())
		{
			_pageRaid->OnRcvRaidBossInfo(param->_data);
		}
	}
}

void UGsUIWindowAgitDungeon::OnRcvRaidBookmarkList(const IGsMessageParam*)
{
	if (EGsAgitMainTab::Raid == GetCurrentTab())
	{
		_pageRaid->OnRcvRaidBookmarkList();
	}
}

void UGsUIWindowAgitDungeon::OnRcvRaidBookmarkChanged(const IGsMessageParam*)
{
	if (EGsAgitMainTab::Raid == GetCurrentTab())
	{
		_pageRaid->OnRcvRaidBookmarkChanged();
	}
}

void UGsUIWindowAgitDungeon::OnRcvRaidOpenChanged(const IGsMessageParam* InParam)
{
	if (EGsAgitMainTab::Raid == GetCurrentTab())
	{
		_pageRaid->OnRcvRaidOpenChanged(InParam);
	}
}	

void UGsUIWindowAgitDungeon::OnResetTimeContents(const IGsMessageParam* InParam)
{
	if (const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>())
	{
		TWeakObjectPtr<UGsUIAgitDungeonPageBase> page = GetPage(GetCurrentTab());
		if (page.IsValid())
		{
			page->OnResetTimeContents(static_cast<ResetTimeTicket>(param->_data));
		}
	}
}

void UGsUIWindowAgitDungeon::OnChangeUserCurrency(uint64 InType)
{
	TWeakObjectPtr<UGsUIAgitDungeonPageBase> page = GetPage(GetCurrentTab());
	if (page.IsValid())
	{
		CurrencyType currencyType = static_cast<CurrencyType>(InType);
		page->OnChangeCurrency(currencyType);
	}
}

void UGsUIWindowAgitDungeon::OnSelectMainTab(int32 InIndex)
{
	_switcherPage->SetActiveWidgetIndex(InIndex);

	TWeakObjectPtr<UGsUIAgitDungeonPageBase> page = GetPage(static_cast<EGsAgitMainTab>(InIndex));
	if (page.IsValid())
	{
		page->Show();
	}
}

TWeakObjectPtr<UGsUIAgitDungeonPageBase> UGsUIWindowAgitDungeon::GetPage(EGsAgitMainTab InTabType)
{
	if (TWeakObjectPtr<UGsUIAgitDungeonPageBase>* page = _pageMap.Find(InTabType))
	{
		return *page;
	}

	return nullptr;
}

UGsUIWindowAgitDungeon::EGsAgitMainTab UGsUIWindowAgitDungeon::GetCurrentTab() const
{
	return static_cast<EGsAgitMainTab>(_toggleGroupMainTab.GetSelectedIndex());
}
