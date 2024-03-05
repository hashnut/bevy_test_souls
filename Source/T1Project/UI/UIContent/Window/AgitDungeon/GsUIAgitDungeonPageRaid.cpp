// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIAgitDungeonPageRaid.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Classes/Engine/Texture2D.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Common/Reward/GsUIRewardIconCommonListEntry.h"
#include "UI/UIContent/Window/AgitDungeon/GsUIAgitRaidBossListEntry.h"
#include "UI/UIContent/Window/AgitDungeon/GsUIAgitRaidCountListEntry.h"
#include "UI/UIContent/Window/Guild/Dungeon/GsUIGuildDungeonRankingListEntry.h"
#include "UI/UIContent/Window/Guild/Dungeon/GsUIGuildDungeonPlayerListEntry.h"
#include "UI/UIContent/Popup/Guild/GuildDungeon/GsUIPopupGuildDungeonReward.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Guild/GsAgitDungeonRaidData.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitDungeonRaid.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitDungeonRaidConfig.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Reward/GsRewardIconDataWrapper.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Currency/GsCurrencyHelper.h"
#include "Message/MessageParam/GsGuildMessageParam.h"


void UGsUIAgitDungeonPageRaid::BeginDestroy()
{
	if (_poolRankingListItem)
	{
		_poolRankingListItem->RemovePool();
	}
	_poolRankingListItem = nullptr;

	if (_poolPlayerListItem)
	{
		_poolPlayerListItem->RemovePool();
	}
	_poolPlayerListItem = nullptr;

	if (_poolRewardListItem)
	{
		_poolRewardListItem->RemovePool();
	}
	_poolRewardListItem = nullptr;

	if (_helperRaidCount)
	{
		_helperRaidCount->OnRefreshEntry.RemoveDynamic(this, &UGsUIAgitDungeonPageRaid::OnRefreshRaidCount);
	}
	_helperRaidCount = nullptr;

	if (_helperBossList)
	{
		_helperBossList->OnRefreshEntry.RemoveDynamic(this, &UGsUIAgitDungeonPageRaid::OnRefreshBossList);
	}
	_helperBossList = nullptr;

	Super::BeginDestroy();
}

void UGsUIAgitDungeonPageRaid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_poolRankingListItem = NewObject<UGsPoolUObject>(this); // UGsGuildDungeonRankingListItem
	_poolPlayerListItem = NewObject<UGsPoolUObject>(this); // UGsGuildDungeonPlayerListItem
	_poolRewardListItem = NewObject<UGsPoolUObject>(this); // UGsRewardIconCommonListItem

	_btnRaidCountInfo->OnPressed.AddDynamic(this, &UGsUIAgitDungeonPageRaid::OnPressRaidCountInfo);
	_btnDungeonOpenInfo->OnPressed.AddDynamic(this, &UGsUIAgitDungeonPageRaid::OnPressDungeonOpenInfo);

	_btnDungeonOpen->OnClickCurrencyButton.BindUObject(this, &UGsUIAgitDungeonPageRaid::OnClickOpenDungeon);
	_btnDungeonClose->OnClicked.AddDynamic(this, &UGsUIAgitDungeonPageRaid::OnClickCloseDungeon);
	_btnInfoRewardPersonal->OnClicked.AddDynamic(this, &UGsUIAgitDungeonPageRaid::OnClickRewardPersonal);
	_btnInfoRewardGuild->OnClicked.AddDynamic(this, &UGsUIAgitDungeonPageRaid::OnClickRewardGuild);
	_btnEnter->OnClicked.AddDynamic(this, &UGsUIAgitDungeonPageRaid::OnClickEnterDungeon);

	_helperRaidCount = NewObject<UGsDynamicPanelSlotHelper>(this);
	_helperRaidCount->Initialize(_raidCountEntryClass, _scrollRaidCount);
	_helperRaidCount->OnRefreshEntry.AddDynamic(this, &UGsUIAgitDungeonPageRaid::OnRefreshRaidCount);

	_helperBossList = NewObject<UGsDynamicPanelSlotHelper>(this);
	_helperBossList->Initialize(_bossEntryClass, _scrollBossList);
	_helperBossList->OnRefreshEntry.AddDynamic(this, &UGsUIAgitDungeonPageRaid::OnRefreshBossList);

	_toggleGroupMenu.OnSelectChanged.BindUObject(this, &UGsUIAgitDungeonPageRaid::OnSelectMenu);
	_toggleGroupMenu.AddToggleWidgetByParentPanel(_panelRootMenu);

	_toggleGroupBoss.OnSelectChanged.BindUObject(this, &UGsUIAgitDungeonPageRaid::OnSelectBoss);
}

void UGsUIAgitDungeonPageRaid::NativeConstruct()
{
	Super::NativeConstruct();

	_listViewRanking->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_listViewPlayer->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_listViewRewardPersonal->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_listViewRewardGuild->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	
	// 한 번만 세팅할 것들
	
	// TEXT: 던전 공략 시작
	FText textBtnName;
	FText::FindText(TEXT("GuildText"), TEXT("Raid_StartButton_Ready"), textBtnName);
	_btnDungeonOpen->SetButtonName(textBtnName);
}

void UGsUIAgitDungeonPageRaid::NativeDestruct()
{
	_bIgnoreChangeSelect = false;
	_bIsForcedRequestBossInfo = false;
	_selectedRaidId = 0;
	_remainRaidCount = 0;
	_bossList.Empty();

	_checkBossCloseTime = 0.f;
	_bossCloseTime = 0;
	_checkResetTicketTime = 0.f;
	_resetTicketTime = 0;
	_checkDelayedResetTime = 0.f;

	_unboxingPersonal.Clear();
	_unboxingGuild.Clear();
	_lastRequestState = AgitDungeonRaidState::NONE;
	_strBossImgPath.Empty();

	Super::NativeDestruct();
}

void UGsUIAgitDungeonPageRaid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	IGsInvalidateUIInterface::CheckInvalidate();

	if (0 < _checkResetTicketTime)
	{
		_checkResetTicketTime -= InDeltaTime;
		if (0 >= _checkResetTicketTime)
		{
			InvalidateResetTime(true);
		}
	}

	if (0 < _checkBossCloseTime)
	{
		_checkBossCloseTime -= InDeltaTime;
		if (0 >= _checkBossCloseTime)
		{
			InvalidateBossRemainTime(true);
		}
	}

	if (0 < _checkDelayedResetTime)
	{
		_checkDelayedResetTime -= InDeltaTime;
		if (0 >= _checkDelayedResetTime)
		{
			_checkDelayedResetTime = 0.f;
			ForcedRefreshAll(true, true);
		}
	}
}

void UGsUIAgitDungeonPageRaid::InvalidateAllInternal()
{
	InvalidateGuildAsset();
	InvalidateRaidCount();
	InvalidateResetTime(false);

	InvalidateBossList();
	InvalidateBossInfo();
	InvalidateBossRemainTime(false);
	InvalidateOpenButton();
	
	InvalidateRanking();
	InvalidatePlayers();
	InvalidateReward();
	
	InvalidateEnterButton();
}

void UGsUIAgitDungeonPageRaid::Show()
{
	// 보스 정보 채우고 첫 항목 선택
	_selectedRaidId = 0;
	_bossList.Empty();

	// 갱신 전 기본 리스트 구성
	RefreshBossList(true);

	// 정보요청
	if (FGsGuildManager* guildMgr = GGuild())
	{
		// 즐겨찾기 정보 요청
		guildMgr->GetSendController().SendReqAgitDungeonRaidBookmarkList();

		// 레이드 기본 정보 요청
		guildMgr->GetSendController().SendReqAgitDungeonRaid();
	}
	
	_toggleGroupMenu.SetSelectedIndexNoEvent(0);
	_switcherMenu->SetActiveWidgetIndex(0);

	InvalidateAll();
}

void UGsUIAgitDungeonPageRaid::OnResetTimeContents(ResetTimeTicket InType)
{
	if (ResetTimeTicket::RTT_AGIT_DUNGEON_RAID != InType)
	{
		return;
	}

	// 주의: 이 정보를 즉시 요청하면, 서버가 초기화가 끝나지 않아 이전 정보가 내려올 수 있다. 1초 후에 요청한다.
	_checkDelayedResetTime = 1.f;
}

void UGsUIAgitDungeonPageRaid::OnChangeCurrency(CurrencyType InType)
{
	if (CurrencyType::GUILD_ASSET == InType)
	{
		InvalidateGuildAsset();
	}
}

void UGsUIAgitDungeonPageRaid::OnNotifyChangeMemberGrade(const FGsGuildMsgParamMemberGrade* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	// 내 등급 변경 정보일 경우, 확인 팝업을 모두 닫고, 강제 갱신
	if (FGsGameDataManager* gameDataMgr = GGameData())
	{
		if (const FGsNetUserData* myData = gameDataMgr->GetUserData())
		{
			if (myData->mUserName == InParam->_userName)
			{
				// 확인 팝업 닫기
				GUI()->CloseByKeyName(TEXT("PopupYesNo"));

				// 새로 뭔가를 받을 필요는 없어서 그냥 갱신
				InvalidateAll();
			}
		}
	}
}

void UGsUIAgitDungeonPageRaid::OnRcvRaidInfo()
{
	// 선택을 바꾸고 싶지 않을 때(오픈상태 변경에 사용)
	if (_bIgnoreChangeSelect)
	{
		RefreshBossList(false);
		_bIgnoreChangeSelect = false;
	}
	else
	{
		RefreshBossList(true);
	}

	InvalidateAll();
}

void UGsUIAgitDungeonPageRaid::OnRcvRaidBossInfo(int32 InRaidId)
{
	// 이미 InvalidateAll이 예약되어있으면 아래 항목 할 필요없음
	if (GetDirtyInvalidate())
	{
		return;
	}

	InvalidateBossInfo(); // 상태 영향
	InvalidateBossRemainTime(false);	// 상태 영향
	InvalidateOpenButton(); // 상태 영향
	InvalidateRanking();
	InvalidatePlayers();

	// 현재랑 같으면 보상은 변하지 않으므로 세팅 안해도됨
	if (InRaidId != _selectedRaidId)
	{
		InvalidateReward();
	}
		
	InvalidateEnterButton(); //상태에 따라 갱신해야하니까
}

void UGsUIAgitDungeonPageRaid::OnRcvRaidBookmarkList()
{
	RefreshBossList(true);

	InvalidateAll();
}

void UGsUIAgitDungeonPageRaid::OnRcvRaidBookmarkChanged()
{
	// 소팅때문에 보스 리스트는 다시 갱신한다. Select 상태는 유지함
	RefreshBossList(false);
	InvalidateBossList();

	// 스크롤 위치로 바꿔준다
	int32 index = GetSelectedBossIndex();	
	if (UWidget* selectedWidget = _scrollBossList->GetChildAt(index))
	{
		_scrollBossList->ScrollWidgetIntoView(selectedWidget);
	}
}

void UGsUIAgitDungeonPageRaid::OnRcvRaidOpenChanged(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsGuildMsgParamAgitDungeonState* param = InParam->Cast<const FGsGuildMsgParamAgitDungeonState>();
	if (nullptr == param)
	{
		return;
	}

	// 노티일 때만 처리
	if (false == param->_bIsNotify)
	{
		_lastRequestState = AgitDungeonRaidState::NONE;
		return;
	}

	// 상태가 바뀌었다는 노티가 오면, 각종 확인 팝업들을 강제로 닫아버리도록 처리
	GUI()->CloseByKeyName(TEXT("PopupYesNo"));

	// 갑자기 닫히면 이상하니 티커 출력. 단 내가 요청한 항목이면 갱신한다는 메시지를 굳이 보여줄 필요없음
	bool bAlreadyShowRefreshTicker = false;
	if (_lastRequestState != param->_raidState)
	{
		// TEXT: 정보가 수정되어 페이지를 갱신합니다
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_Fail_And_Refresh"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		bAlreadyShowRefreshTicker = true;
	}

	if (AgitDungeonRaidState::CLOSED == param->_raidState)
	{
		// 닫을 때 입장 횟수가 변경되는데, 내 입장여부를 알 수 없으므로 강제 갱신
		if (FGsAgitDungeonRaidData* raidData = GetRaidData())
		{
			if (const FGsSchemaAgitDungeonRaidConfig* config = FGsGuildHelper::GetAgitDungeonRaidConfig())
			{
				// 티켓 소진한게 하나라도 있으면 강제 리셋
				if (raidData->GetTicketCount() < config->raidTicketCount)
				{
					// 위에서 이미 티커를 보여줬던 상황이면 보여줄 필요 없고, 내가 요청했던 항목이면 역시 보여줄 필요 없음
					bool bShowRefreshTicker = false;
					if (false == bAlreadyShowRefreshTicker)
					{
						bShowRefreshTicker = (_lastRequestState != param->_raidState) ? true : false;
					}

					_bIgnoreChangeSelect = true;
					ForcedRefreshAll(bShowRefreshTicker, false);
				}
			}
		}
	}
	else if (AgitDungeonRaidState::OPEN == param->_raidState)
	{
		// 열 때 재오픈 상황이면 기여도가 리셋되는데, 리셋 정보를 반영하기 위해 강제 갱신
		if (param->_raidId == _selectedRaidId)
		{
			if (FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData())
			{
				// 기여도가 하나라도 존재하면 강제 갱신
				if (0 < bossData->_rankingList.Num())
				{
					// 이게 켜지면 InvalidateBossList 에 의해 OnSelectBoss가 불릴 때 강제로 갱신하게 됨
					_bIsForcedRequestBossInfo = true;
				}
			}
		}
	}
	
	// 오픈 상태에 따라 정렬이 바뀌기 때문에 무조건 수행
	RefreshBossList(false);
	InvalidateBossList();

	// 해당 보스가 선택되어 있을 때만 수행
	if (param->_raidId == _selectedRaidId)
	{
		InvalidateBossInfo();
		InvalidateBossRemainTime(false);
		InvalidateOpenButton();
	}
	
	InvalidateGuildAsset();
	InvalidateEnterButton();
}

void UGsUIAgitDungeonPageRaid::OnSelectMenu(int32 InIndex)
{
	_switcherMenu->SetActiveWidgetIndex(InIndex);
	
	if (0 == InIndex)
	{
		InvalidateBossInfo();
	}
	else
	{
		InvalidateBossList();
	}
}

void UGsUIAgitDungeonPageRaid::OnSelectBoss(int32 InIndex)
{
	if (_bossList.IsValidIndex(InIndex))
	{
		_selectedRaidId = _bossList[InIndex].Pin()->GetRaidId();
	}

	if (0 < _selectedRaidId)
	{
		if (FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData())
		{
			if (_bIsForcedRequestBossInfo)
			{
				bossData->SendReqAgitDungeonRaidDetail(true);
				_bIsForcedRequestBossInfo = false;
			}
			else
			{
				bossData->SendReqAgitDungeonRaidDetail(false);
			}
		}
	}

	// 이미 InvalidateAll이 예약되어있으면 아래 항목 할 필요없음
	if (GetDirtyInvalidate())
	{
		return;
	}

	InvalidateBossRemainTime(false);
	InvalidateOpenButton();
	InvalidateRanking();
	InvalidatePlayers();
	InvalidateReward();
	InvalidateEnterButton();
}

void UGsUIAgitDungeonPageRaid::OnRefreshRaidCount(int32 InIndex, UWidget* InEntry)
{
	if (UGsUIAgitRaidCountListEntry* entry = Cast<UGsUIAgitRaidCountListEntry>(InEntry))
	{
		bool bIsActive = (_remainRaidCount > InIndex) ? true : false;
		entry->SetData(bIsActive);
	}
}
	
void UGsUIAgitDungeonPageRaid::OnRefreshBossList(int32 InIndex, UWidget* InEntry)
{
	if (_bossList.IsValidIndex(InIndex))
	{
		if (_bossList[InIndex].IsValid())
		{
			if (UGsUIAgitRaidBossListEntry* entry = Cast<UGsUIAgitRaidBossListEntry>(InEntry))
			{
				entry->SetData(_bossList[InIndex]);

				if (FGsAgitDungeonRaidData* raidData = GetRaidData())
				{
					entry->SetIsBookmark(raidData->IsBookmarked(_bossList[InIndex].Pin()->GetRaidId()));
				}
			}
		}
	}
}

void UGsUIAgitDungeonPageRaid::OnPressRaidCountInfo()
{
	_tooltipRaidCount->ToggleOpenClose();

	if (_tooltipRaidCount->IsOpen())
	{
		// TEXT: 기사단 공략을 시작하는데 사용됩니다...
		FText textDesc;
		FText::FindText(TEXT("GuildText"), TEXT("Raid_OpenTicket_ToolTip"), textDesc);
		_tooltipRaidCount->SetDescText(textDesc);
	}
}

void UGsUIAgitDungeonPageRaid::OnPressDungeonOpenInfo()
{
	_tooltipDungeonOpen->ToggleOpenClose();

	if (_tooltipDungeonOpen->IsOpen())
	{
		if (const FGsSchemaAgitDungeonRaidConfig* config = FGsGuildHelper::GetAgitDungeonRaidConfig())
		{
			// TEXT: {0} 이상 기사단원만 공략을 시작하고 종료할 수 있습니다...
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Raid_StartButton_ToolTip"), textFormat))
			{
				FText textGrade;
				FGsGuildHelper::GetMemberGradeText(config->raidOpenMinGrade, textGrade);				

				FText textDesc = FText::Format(textFormat, textGrade);
				_tooltipDungeonOpen->SetDescText(textDesc);
			}
		}
	}
}

void UGsUIAgitDungeonPageRaid::OnClickOpenDungeon()
{
	FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData();
	if (nullptr == bossData)
	{
		return;
	}

	// 이미 열려있으면 무시(다른 모양으로 보여서 들어오지 말아야 정상)
	if (AgitDungeonRaidState::OPEN == bossData->GetRaidState())
	{
		return;
	}

	// 이미 완료되었으면 무시(다른 모양으로 보여서 들어오지 말아야 정상)
	if (AgitDungeonRaidState::CLEAR == bossData->GetRaidState())
	{
		// TEXT: 공략에 성공한 던전은 초기화 시점까지 다시 공략할 수 없습니다
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Raid_Open_Already_Clear"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return;
	}

	FGsAgitDungeonRaidData* raidData = GetRaidData();
	if (nullptr == raidData)
	{
		return;
	}

	// 레이드 횟수 체크
	if (0 == raidData->GetRemainRaidCount())
	{
		// TEXT: 남은 기사단 공략 횟수가 부족해 공략을 시작할 수 없습니다
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Raid_Ticket_Empty"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return;
	}

	// 다른 활성화된 던전이 있는지 체크
	TWeakPtr<FGsAgitDungeonRaidBossData> currOpenBossData = raidData->GetCurrOpenBossData();
	if (currOpenBossData.IsValid())
	{
		if (currOpenBossData.Pin()->GetRaidId() != bossData->GetRaidId())
		{
			// TEXT: 이미 공략 중인 던전이 존재합니다
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("Raid_Open_Already_Open"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

			return;
		}
	}

	// 기사단 레벨 체크
	if (GGuild()->GetGuildLevel() < bossData->GetLevelLimit())
	{
		// TEXT: 기사단 레베이 부족해 공략을 시작할 수 없습니다
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Raid_Open_Fail_GuildLevel"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return;
	}

	// 등급 체크
	if (false == CheckOpenMemberGrade(true))
	{
		return;
	}

	const FGsSchemaAgitDungeonRaid* table = bossData->GetTableData();
	if (nullptr == table)
	{
		return;
	}

	// 운영비 체크
	if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GUILD_ASSET, table->openCost, false))
	{
		Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GUILD_ASSET);
		FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GUILD_ASSET, table->openCost - myAmount);

		return;
	}

	FText textPopup;
	// TEXT: 기사단 운영비 {0}를 사용해 {1} 공략을 시작 하시겠습니까 ?
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Raid_Open_PopUp_Desc"), textFormat))
	{
		textPopup = FText::Format(textFormat, table->openCost, bossData->GetBossName());
	}

	TWeakObjectPtr<UGsUIAgitDungeonPageRaid> weakThis(this);
	AgitDungeonRaidId raidId = static_cast<AgitDungeonRaidId>(_selectedRaidId);
	FGsUIHelper::PopupYesNo(textPopup, [raidId, weakThis](bool bIsOk)
		{
			if (bIsOk)
			{
				FGsUIHelper::ShowBlockUI();

				if (weakThis.IsValid())
				{
					weakThis->SetLastRequestState(AgitDungeonRaidState::OPEN);
				}

				FGsNetSendServiceGuild::SendReqAgitDungeonRaidOpen(raidId);
			}
		});
}

void UGsUIAgitDungeonPageRaid::OnClickCloseDungeon()
{
	FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData();
	if (nullptr == bossData)
	{
		return;
	}

	if (AgitDungeonRaidState::OPEN != bossData->GetRaidState())
	{
		// TEXT: 공략 진행 중인 던전만 입장할 수 있습니다
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Raid_Enter_Fail_Closed"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return;
	}

	// 등급 체크
	if (false == CheckOpenMemberGrade(true))
	{
		return;
	}

	FText textPopup;
	// TEXT: {0} 공략을 종료 하시겠습니까?
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Raid_Close_PopUp_Desc"), textFormat))
	{
		textPopup = FText::Format(textFormat, bossData->GetBossName());
	}

	TWeakObjectPtr<UGsUIAgitDungeonPageRaid> weakThis(this);
	AgitDungeonRaidId raidId = static_cast<AgitDungeonRaidId>(_selectedRaidId);
	FGsUIHelper::PopupYesNo(textPopup, [raidId, weakThis](bool bIsOk)
		{
			if (bIsOk)
			{
				FGsUIHelper::ShowBlockUI();

				if (weakThis.IsValid())
				{
					weakThis->SetLastRequestState(AgitDungeonRaidState::CLOSED);
				}

				FGsNetSendServiceGuild::SendReqAgitDungeonRaidClose(raidId);
			}
		});
}

void UGsUIAgitDungeonPageRaid::OnClickRewardPersonal()
{
	OpenRewardPopup(false);
}

void UGsUIAgitDungeonPageRaid::OnClickRewardGuild()
{
	OpenRewardPopup(true);
}

void UGsUIAgitDungeonPageRaid::OnClickEnterDungeon()
{
	FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData();
	if (nullptr == bossData)
	{
		return;
	}
	
	// 던전 열려있는지 체크
	if (AgitDungeonRaidState::OPEN != bossData->GetRaidState())
	{
		return;
	}

	FText textPopup;
	// TEXT: {0} 공략 던전에 입장하시겠습니까? ...
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Raid_Enter_PopUp_Desc"), textFormat))
	{
		textPopup = FText::Format(textFormat, bossData->GetBossName());
	}

	AgitDungeonRaidId raidId = static_cast<AgitDungeonRaidId>(_selectedRaidId);
	FGsUIHelper::PopupYesNo(textPopup, [raidId](bool bIsOk)
		{
			if (bIsOk)
			{
				// 주의: 팝업 열고있는 사이에 종료될 경우, 서버 에러코드로 처리해야 할 것
				FGsUIHelper::ShowBlockUI();
				FGsNetSendServiceGuild::SendReqAgitDungeonRaidEnter(raidId);
			}
		});
}

void UGsUIAgitDungeonPageRaid::RefreshBossList(bool bSelectFirst)
{
	_bossList.Empty();
	if (FGsAgitDungeonRaidData* raidData = GetRaidData())
	{
		_resetTicketTime = raidData->GetResetTime();

		raidData->GetSortedBossList(_bossList);
	}

	if (bSelectFirst)
	{
		if (0 < _bossList.Num())
		{
			if (_bossList[0].IsValid())
			{
				_selectedRaidId = _bossList[0].Pin()->GetRaidId();
			}
		}

		_bIsForcedRequestBossInfo = true;
	}
}

void UGsUIAgitDungeonPageRaid::InvalidateGuildAsset()
{
	Currency guildAssetAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GUILD_ASSET);
	_textBlockGuildAsset->SetText(FText::AsNumber(guildAssetAmount));
}

void UGsUIAgitDungeonPageRaid::InvalidateRaidCount()
{
	const FGsAgitDungeonRaidData* raidData = GetRaidData();
	if (nullptr == raidData)
	{
		return;
	}

	_remainRaidCount = raidData->GetRemainRaidCount();

	FString strCount = FString::Format(TEXT("{0}/{1}"), { _remainRaidCount, raidData->GetMaxRaidCount() });
	_textBlockRaidCount->SetText(FText::FromString(strCount));
	
	_helperRaidCount->RefreshAll(raidData->GetMaxRaidCount());
}

void UGsUIAgitDungeonPageRaid::InvalidateResetTime(bool bCheckZero)
{
	time_t remainTime = _resetTicketTime - FGsTimeSyncUtil::GetServerNowSecond();
	if (0 >= remainTime)
	{
		_checkResetTicketTime = 0;
		_textBlockResetTime->SetText(FText::FromString(TEXT("-")));

		// 주의: 서버에서 ResetTimeTicket을 보내줄거라 해당 패킷을 받아 OnResetTimeContents 여기 통해서 서 갱신 수행
		//if (bCheckZero)
		//{
		//	ForcedRefreshAll(false, false);
		//}
		return;
	}

	FString strTime;
	FTimespan timespan = FTimespan::FromSeconds(remainTime);
	FGsTimeStringHelper::GetTimeStringCeil(timespan, strTime, false);

	_textBlockResetTime->SetText(FText::FromString(strTime));

	// 1초에 한번씩 갱신
	_checkResetTicketTime = 1.f;
}

void UGsUIAgitDungeonPageRaid::InvalidateBossList()
{
	_helperBossList->RefreshAll(_bossList.Num());

	int selectedIndex = 0;
	if (0 < _selectedRaidId)
	{
		for (int i = 0; i < _bossList.Num(); ++i)
		{
			if (_bossList[i].IsValid())
			{
				if (_selectedRaidId == _bossList[i].Pin()->GetRaidId())
				{
					selectedIndex = i;
				}
			}
		}
	}

	_toggleGroupBoss.Clear();
	_toggleGroupBoss.AddToggleWidgetByParentPanel(_scrollBossList);
	_toggleGroupBoss.SetSelectedIndex(selectedIndex, true);
}

void UGsUIAgitDungeonPageRaid::InvalidateBossInfo()
{
	FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData();
	if (nullptr == bossData)
	{
		return;
	}

	const FGsSchemaAgitDungeonRaid* raidTable = bossData->GetTableData();
	if (nullptr == raidTable)
	{
		return;
	}

	const FGsSchemaNpcData* npcTableData = raidTable->bossId.GetRow();
	if (nullptr == npcTableData)
	{
		return;
	}

	// 보스 이미지. 깜빡임 방지를 위해 이전과 동일하면 수행 안함
	if (_strBossImgPath != raidTable->bossImg.ToString())
	{
		_strBossImgPath = raidTable->bossImg.ToString();

		_imgBoss->SetVisibility(ESlateVisibility::Hidden);
		TWeakObjectPtr<UGsUIAgitDungeonPageRaid> weakThis(this);
		FGsUIHelper::AsyncLoadTextureFromPath(raidTable->bossImg,
			FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* InRes)
				{
					if (weakThis.IsValid())
					{
						if (UGsUIAgitDungeonPageRaid* strongThis = weakThis.Get())
						{
							if (UTexture2D* loadedTexture = Cast<UTexture2D>(InRes))
							{
								strongThis->SetBossImage(loadedTexture);
							}
						}
					}
				}
		));
	}

	// 보스 이름, 별칭
	_textBlockBossName->SetText(npcTableData->nameText);
	_textBlockBossSubName->SetText(raidTable->bossNickName);

	// 던전 상태에 따른 처리
	switch (bossData->GetRaidState())
	{
	case AgitDungeonRaidState::NONE:
	case AgitDungeonRaidState::CLOSED:
	{
		SetBossHp(0.f);
		 
		// TEXT: 공략 대기중
		FText textState;
		FText::FindText(TEXT("GuildText"), TEXT("Raid_Status_Ready_Desc"), textState);
		_textBlockBossState->SetText(textState);

		_bossCloseTime = 0;

		_panelBossClear->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	case AgitDungeonRaidState::OPEN:
	{
		SetBossHp(bossData->GetBossHpPercent());

		// TEXT: 공략중. 남은시간:
		FText textState;
		FText::FindText(TEXT("GuildText"), TEXT("Raid_Status_OnGoing_Desc"), textState);
		_textBlockBossState->SetText(textState);

		// 남은시간 활성
		_bossCloseTime = bossData->GetCloseTime();

		_panelBossClear->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	case AgitDungeonRaidState::CLEAR:
	{
		SetBossHp(0.f);

		FString strCompleteTime;
		FDateTime completeDateTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(bossData->GetCompleteTime());
		FGsTimeStringHelper::GetTimeStringNYMDHMS(completeDateTime, strCompleteTime);		

		// TEXT: {0} 처치 됨
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Raid_Status_Clear_Desc"), textFormat))
		{
			FText textState = FText::Format(textFormat, FText::FromString(strCompleteTime));
			_textBlockBossState->SetText(textState);
		}

		_bossCloseTime = 0;

		_panelBossClear->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	}

	// 보스 남은 시간 UI
	InvalidateBossRemainTime(false);
}

void UGsUIAgitDungeonPageRaid::InvalidateBossRemainTime(bool bCheckZero)
{
	if (0 == _bossCloseTime)
	{
		_checkBossCloseTime = 0.f;
		_textBlockBossRemainTime->SetText(FText::GetEmpty());

		return;
	}

	time_t remainSec = _bossCloseTime - FGsTimeSyncUtil::GetServerNowSecond();
	if (0 > remainSec)
	{
		_checkBossCloseTime = 0.f;
		_textBlockBossRemainTime->SetText(FText::FromString(TEXT("-")));

		// 서버에서 정보를 보내줄지 모르겠으므로 클라가요청함
		if (bCheckZero)
		{
			if (FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData())
			{
				bossData->SendReqAgitDungeonRaidDetail(true);
			}
		}

		return;
	}

	FTimespan remainTime = FTimespan::FromSeconds(remainSec);
	FString strTime;
	FGsTimeStringHelper::GetTimeStringHMS(remainTime, strTime);

	_textBlockBossRemainTime->SetText(FText::FromString(strTime));

	_checkBossCloseTime = 1.f;
}

void UGsUIAgitDungeonPageRaid::InvalidateOpenButton()
{
	FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData();
	if (nullptr == bossData)
	{
		return;
	}

	switch (bossData->GetRaidState())
	{
	case AgitDungeonRaidState::NONE:
	case AgitDungeonRaidState::CLOSED:
	{
		_switcherDungeonBtn->SetActiveWidgetIndex(0);

		// 비용 세팅
		if (const FGsSchemaAgitDungeonRaid* table = bossData->GetTableData())
		{
			_btnDungeonOpen->SetData(CurrencyType::GUILD_ASSET, table->openCost);
		}
	}	
		break;
	case AgitDungeonRaidState::OPEN:
		_switcherDungeonBtn->SetActiveWidgetIndex(1);
		break;
	case AgitDungeonRaidState::CLEAR:
		_switcherDungeonBtn->SetActiveWidgetIndex(2);
		break;
	}
}

void UGsUIAgitDungeonPageRaid::InvalidateRanking()
{	
	_poolRankingListItem->ReleaseAll();	
	_listViewRanking->ClearListItems();

	UGsGuildDungeonRankingListItem* myListItem = nullptr;
	TArray<UGsGuildDungeonRankingListItem*> rankingListItems;

	if (FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData())
	{
		FString strMyName;
		if (const FGsNetUserData* userData = GGameData()->GetUserData())
		{
			strMyName = userData->mUserName;
		}

		for (TSharedPtr<FGsAgitRaidRankingData> iter : bossData->_rankingList)
		{
			UGsGuildDungeonRankingListItem* listItem = _poolRankingListItem->Claim<UGsGuildDungeonRankingListItem>();
			listItem->SetData(iter->_rank, iter->_strName, iter->_point);
			rankingListItems.Emplace(listItem);

			if (strMyName == iter->_strName)
			{
				myListItem = listItem;
			}
		}
	}	

	// 랭킹 리스트
	_listViewRanking->SetListItems(rankingListItems);

	_panelEmptyRankngList->SetVisibility((0 == rankingListItems.Num()) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	// 내 랭킹 표시
	_myRankingUI->InvalidateAll(myListItem);

	// 강제 갱신 호출
	const TArray<UUserWidget*> entryList = _listViewRanking->GetDisplayedEntryWidgets();
	for (UUserWidget* widget : entryList)
	{
		if (UGsUIGuildDungeonRankingListEntry* listEntry = Cast<UGsUIGuildDungeonRankingListEntry>(widget))
		{
			listEntry->RefreshAll();
		}
	}
}

void UGsUIAgitDungeonPageRaid::InvalidatePlayers()
{
	_poolPlayerListItem->ReleaseAll();
	_listViewPlayer->ClearListItems();

	TArray<UGsGuildDungeonPlayerListItem*> playerListItems;

	if (FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData())
	{
		for (TSharedPtr<FGsAgitRaidPlayerData> iter : bossData->_playerList)
		{
			UGsGuildDungeonPlayerListItem* listItem = _poolPlayerListItem->Claim<UGsGuildDungeonPlayerListItem>();
			listItem->SetData(iter->_level, iter->_strName);

			playerListItems.Emplace(listItem);
		}
	}

	_listViewPlayer->SetListItems(playerListItems);

	_panelEmptyPlayerList->SetVisibility((0 == playerListItems.Num()) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	// TEXT: 현재 입장 {0}명
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Dungeon_CurrentPersons"), textFormat))
	{
		FText textCount = FText::Format(textFormat, playerListItems.Num());
		_textBlockPlayerCount->SetText(textCount);
	}
	else
	{
		_textBlockPlayerCount->SetText(FText::GetEmpty());
	}

	// 강제 갱신 호출
	const TArray<UUserWidget*> entryList = _listViewPlayer->GetDisplayedEntryWidgets();
	for (UUserWidget* widget : entryList)
	{
		if (UGsUIGuildDungeonPlayerListEntry* listEntry = Cast<UGsUIGuildDungeonPlayerListEntry>(widget))
		{
			listEntry->RefreshAll();
		}
	}
}

void UGsUIAgitDungeonPageRaid::InvalidateReward()
{
	_poolRewardListItem->ReleaseAll();
	_listViewRewardPersonal->ClearListItems();
	_listViewRewardGuild->ClearListItems();

	FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData();
	if (nullptr == bossData)
	{
		return;
	}

	const FGsSchemaAgitDungeonRaid* raidTable = bossData->GetTableData();
	if (nullptr == raidTable)
	{
		return;
	}

	_unboxingPersonal.Clear();
	_unboxingPersonal._bUseCurrencyIcon = true;
	CollectReward(raidTable, false, _unboxingPersonal);
	SetRewardListView(_unboxingPersonal, _listViewRewardPersonal);
		
	_unboxingGuild.Clear();
	_unboxingGuild._bUseCurrencyIcon = true;
	CollectReward(raidTable, true, _unboxingGuild);
	SetRewardListView(_unboxingGuild, _listViewRewardGuild);
}

void UGsUIAgitDungeonPageRaid::InvalidateEnterButton()
{
	FGsAgitDungeonRaidData* raidData = GetRaidData();
	if (nullptr == raidData)
	{
		_btnEnter->SetIsEnabled(false);
		return;
	}

	FGsAgitDungeonRaidBossData* bossData = GetSelectedBossData();
	if (nullptr == bossData)
	{
		_btnEnter->SetIsEnabled(false);
		return;
	}	

	if (bossData->IsReEnter())
	{
		_btnEnter->SetIsEnabled(true);

		// TEXT: 재입장
		FText textBtn;
		FText::FindText(TEXT("GuildText"), TEXT("Raid_ReEnterButton"), textBtn);
		_textBlockEnterRemainCount->SetText(textBtn);
		return;
	}

	const FGsSchemaAgitDungeonRaidConfig* config = FGsGuildHelper::GetAgitDungeonRaidConfig();
	if (nullptr == config)
	{
		return;
	}
	
	// 주의: 던전에 입장 했으면 하나 차감 되어있고 재입장 상태. 던전이 닫히면 복구됨.
	bool bEnterBtnEnable = false;
	if (AgitDungeonRaidState::OPEN == bossData->GetRaidState())
	{
		if (0 < raidData->GetTicketCount())
		{
			bEnterBtnEnable = true;
		}
	}

	_btnEnter->SetIsEnabled(bEnterBtnEnable);

	// 던전 입장 ({0}/{1})
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Dungeon_EnterButton"), textFormat))
	{
		FText textBtn = FText::Format(textFormat, raidData->GetTicketCount(), config->raidTicketCount);
		_textBlockEnterRemainCount->SetText(textBtn);
	}
}

void UGsUIAgitDungeonPageRaid::CollectReward(const FGsSchemaAgitDungeonRaid* InTable, bool bIsGuildReward,
	OUT FGsRewardUnboxingData& OutData)
{
	if (nullptr == InTable)
	{
		return;
	}

	if (bIsGuildReward)
	{
		for (int i = 0; i < InTable->rewardGuildList.Num(); ++i)
		{
			if (const FGsSchemaRewardData* rewardData = InTable->rewardGuildList[i].GetRow())
			{
				FGsRewardUnboxingData unboxingData;
				FGsRewardHelper::UnboxingRewardData(rewardData, unboxingData);
				OutData.AddData(unboxingData);
			}
		}
	}
	else
	{
		for (int i = 0; i < InTable->rewardPersonList.Num(); ++i)
		{
			if (const FGsSchemaRewardData* rewardData = InTable->rewardPersonList[i].GetRow())
			{
				FGsRewardUnboxingData unboxingData;
				FGsRewardHelper::UnboxingRewardData(rewardData, unboxingData);
				OutData.AddData(unboxingData);
			}
		}
	}

	OutData.ItemSortCurrencyItemPriority();
}

void UGsUIAgitDungeonPageRaid::SetRewardListView(FGsRewardUnboxingData& InData, OUT UListView* OutListView)
{
	TArray<FGsRewardIconDataWrapper> dataList;
	FGsRewardHelper::GetIconDataList(&InData, dataList);

	TArray<UGsRewardIconCommonListItem*> listItemList;
	for (FGsRewardIconDataWrapper& iter : dataList)
	{
		UGsRewardIconCommonListItem* listItem = _poolRewardListItem->Claim<UGsRewardIconCommonListItem>();

		listItem->SetData(&iter, true, false);
		listItemList.Emplace(listItem);
	}
	
	OutListView->SetListItems(listItemList);
	OutListView->ScrollToTop();

	// 강제 갱신 호출
	const TArray<UUserWidget*> entryList = OutListView->GetDisplayedEntryWidgets();
	for (UUserWidget* widget : entryList)
	{
		if (UGsUIRewardIconCommonListEntry* listEntry = Cast<UGsUIRewardIconCommonListEntry>(widget))
		{
			listEntry->RefreshAll();
		}
	}
}

void UGsUIAgitDungeonPageRaid::OpenRewardPopup(bool bIsGuildReward)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildDungeonReward"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildDungeonReward* popup = Cast<UGsUIPopupGuildDungeonReward>(widget.Get()))
		{
			FText textTitle;
			if (bIsGuildReward)
			{
				// TEXT: 개인 획득 보상
				FText::FindText(TEXT("GuildText"), TEXT("Dungeon_RewardGuild_List"), textTitle);
				popup->SetData(textTitle, _unboxingGuild._itemDataList, _unboxingGuild);
			}
			else
			{
				// TEXT: 기사단 획득 보상
				FText::FindText(TEXT("GuildText"), TEXT("Dungeon_RewardPerson_List"), textTitle);
				popup->SetData(textTitle, _unboxingPersonal._itemDataList, _unboxingPersonal);
			}
		}
	}
}

void UGsUIAgitDungeonPageRaid::ForcedRefreshAll(bool bShowTicker, bool bClosePopup)
{
	// 무조건 서버에 정보요청을 한 번해서 새 정보 받아 전체 갱신 수행
	if (FGsGuildManager* guildMgr = GGuild())
	{
		// 확인 팝업 닫기
		if (bClosePopup)
		{
			GUI()->CloseByKeyName(TEXT("PopupYesNo"));
		}

		if (bShowTicker)
		{
			// TEXT: 정보가 수정되어 페이지를 갱신합니다
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("Alert_Fail_And_Refresh"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		// 무조건 갱신하기 위해 쿨타임 리셋
		guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::AgitDungeonRaid);
		// 레이드 기본 정보 요청
		guildMgr->GetSendController().SendReqAgitDungeonRaid();
	}
}

void UGsUIAgitDungeonPageRaid::SetBossHp(float InPercent)
{
	if (0 >= InPercent)
	{
		_panelRootBossHp->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	_panelRootBossHp->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FString strPercent = FString::Printf(TEXT("%.2f%%"), InPercent);
	_textBlockHp->SetText(FText::FromString(strPercent));

	// 주의: 들어온 값은 0~100 값이므로 0~1 사이값으로 바꿔야 한다
	_barBossHp->SetPercent(InPercent * 0.01f);
}

void UGsUIAgitDungeonPageRaid::SetBossImage(UTexture2D* InTexture)
{
	_imgBoss->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_imgBoss->SetBrushFromSoftTexture(InTexture);
}

bool UGsUIAgitDungeonPageRaid::CheckOpenMemberGrade(bool bShowTicker) const
{
	GuildMemberGradeType myGrade = GGuild()->GetMemeberGrade();
	if (const FGsSchemaAgitDungeonRaidConfig* config = FGsGuildHelper::GetAgitDungeonRaidConfig())
	{
		if (myGrade < config->raidOpenMinGrade)
		{
			if (bShowTicker)
			{
				// TEXT: {0} 이상만 공략을 시작/종료할 수 있습니다
				FText textFormat;
				if (FText::FindText(TEXT("GuildText"), TEXT("Raid_Open_NoAuthor"), textFormat))
				{
					FText textGrade;
					FGsGuildHelper::GetMemberGradeText(config->raidOpenMinGrade, textGrade);

					FText textMsg = FText::Format(textFormat, textGrade);
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}
			}

			return false;
		}
	}

	return true;
}

FGsAgitDungeonRaidData* UGsUIAgitDungeonPageRaid::GetRaidData() const
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			return agitController->GetRaidData();
		}
	}

	return nullptr;
}

FGsAgitDungeonRaidBossData* UGsUIAgitDungeonPageRaid::GetSelectedBossData() const
{
	if (0 == _selectedRaidId)
	{
		if (0 < _bossList.Num())
		{
			if (_bossList[0].IsValid())
			{
				return _bossList[0].Pin().Get();
			}
		}
	}

	return GetBossData(_selectedRaidId);
}

FGsAgitDungeonRaidBossData* UGsUIAgitDungeonPageRaid::GetBossData(int32 InBossId) const
{
	for (int i = 0; i < _bossList.Num(); ++i)
	{
		if (_bossList[i].IsValid())
		{
			if (_bossList[i].Pin()->GetRaidId() == InBossId)
			{
				return _bossList[i].Pin().Get();
			}
		}
	}

	return nullptr;
}

int32 UGsUIAgitDungeonPageRaid::GetSelectedBossIndex() const
{
	// 0이면 가장 처음 것을 리턴
	if (0 < _selectedRaidId)
	{
		for (int i = 0; i < _bossList.Num(); ++i)
		{
			if (_bossList[i].IsValid())
			{
				if (_bossList[i].Pin()->GetRaidId() == _selectedRaidId)
				{
					return i;
				}
			}
		}
	}

	return 0;
}
