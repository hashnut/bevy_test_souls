#include "GsUIGuildContentMain.h"

#include "Runtime/UMG/Public/Components/ListView.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "UI/UIContent/Window/Guild/Diplomacy/GsUIGuildDiplomacyPage.h"
#include "UI/UIContent/Popup/GsUIPopupSanctumNexusBuffInfo.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildHelper.h"
#include "Guild/GsGuildData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "DataSchema/Guild/GsSchemaGuildLevelDetailData.h"
#include "DataSchema/Guild/GsSchemaGuildSnsShortcutUrl.h"
#include "Net/GsNetSendServiceGuild.h"
#include "UI/UIContent/Window/Guild/Sns/GsUIGuildSnsIconListEntry.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Currency/GsCurrencyHelper.h"
#include "PaperSprite.h"
#include "UTIL/GsText.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Classes/SecureVar/GsSecureVar.h"

#include "Data/GsDataContainManager.h"

#include "T1Project.h"

void UGsUIGuildContentMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnDonation->OnClicked.AddDynamic(this, &UGsUIGuildContentMain::OnClickDonation);
	_btnAttendance->OnClicked.AddDynamic(this, &UGsUIGuildContentMain::OnClickAttendance);	
	_btnStorage->OnClicked.AddDynamic(this, &UGsUIGuildContentMain::OnClickStorage);
	_btnTooltipCamp->OnClicked.AddDynamic(this, &UGsUIGuildContentMain::OnClickTooltipCamp);
	
	_redDotAttendance.Initialize(_uiRedDotAttendance, EGsIconRedDotType::NORMAL, []()
		{
			return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Attendance);
		});

	_redDotNotice.Initialize(_uiRedDotNotice, EGsIconRedDotType::NORMAL, []()
		{
			return GGuild()->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Notice);
		});	

	_redDotStorage.Initialize(_uiRedDotStorage, EGsIconRedDotType::NORMAL, []()
		{
			return false;
		});

	_uiDiplomacy->OnChangeDiplomacyTab.BindUObject(this, &UGsUIGuildContentMain::OnChangeDiplomacyTab);
}

void UGsUIGuildContentMain::InvalidateAllInternal()
{
	const FGsGuildDataDetail* guildData = GGuild()->GetGuildDetailData(_bIsMyGuild);
	if (nullptr == guildData)
	{
		return;
	}

	// 길드 문장	
	SetUIGuildEmblem(guildData->_emblemId);

	// 정보출력
	_textBlockName->SetText(FText::FromString(guildData->_guildName));

	FDateTime createdServerDateTime = FGsTimeSyncUtil::ConvertToDateTime(guildData->_createdTime);
	FDateTime createdDateTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(createdServerDateTime);

	FString textDate;
	FGsTimeStringHelper::GetTimeStringNYMDHM(createdDateTime, textDate);
	_textBlockCreateDate->SetText(FText::FromString(textDate));

	SetUIMasterName(guildData->_masterName);
	SetUIGuildMember(guildData->_memberNum, guildData->GetMaxMemberNum());
	SetUIGuildLevel(guildData->_guildLevel, guildData->_campLevel);

	SetUIGuildLimitLevel(guildData->_joinLimitLevel);
	Currency GuildAsset = 0;
	guildData->_guildAsset.Get(GuildAsset);
	SetUIGuildAsset(GuildAsset);
	SetUIGuildAttendance(guildData);
	SetUIRank(guildData->_donationRank, guildData->_dungeonClearRank);

	auto thiz = MakeWeakObjectPtr(this);
	if (_bIsMyGuild)
	{
		// 내 길드이면 공지글 출력
		FGsTextUtil::ChangeBanSentence(guildData->GetNotice(), [thiz](bool success, FString inText, FString outText) {
				if(thiz.IsValid()) thiz->SetUIGuildNotice(outText);
			});

		// 공헌도 progressBar 세팅
		SetUIConribution(guildData->_guildLevel, guildData->GetContribution());

		_redDotAttendance.UpdateRedDot();
		_redDotNotice.UpdateRedDot();
	}
	else
	{
		// 타 길드이면 소개글 출력
		FGsTextUtil::ChangeBanSentence(guildData->GetIntroduction(), [thiz](bool success, FString inText, FString outText) {
			if (thiz.IsValid()) thiz->SetUIGuildNotice(outText);
			});
		
		_redDotAttendance.SetIsRedDot(false);
		_redDotNotice.SetIsRedDot(false);
	}

	SetUIGuildFrame(_bIsMyGuild);

	_uiDiplomacy->Show(_bIsMyGuild);

	// SNS 링크
	SetSnsLinkIcons(GGuild()->MakeSnsLinks(_bIsMyGuild));
}

void UGsUIGuildContentMain::SendRequestPacket()
{
	GGuild()->GetSendController().SendRequestOpenGuild();
}

void UGsUIGuildContentMain::Show(bool bIsMyGuild, bool IsStackRecoverFlag /*= false*/)
{
	_bIsMyGuild = bIsMyGuild;

	InvalidateAll();

	_scrollNotice->ScrollToStart();
}

void UGsUIGuildContentMain::Hide()
{
	if (_bIsMyGuild)
	{
		if (FGsGuildManager* guildMgr = GGuild())
		{
			guildMgr->GetRedDotCheckData().UpdateNoticeCheckTime();
			guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Notice);
		}

		_uiDiplomacy->Hide();
	}
}

void UGsUIGuildContentMain::SetUIGuildFrame(bool bIsMyGuild)
{
	// 기능 버튼, 운영비, 공지사항 제외 출력
	ESlateVisibility visibilityHit = bIsMyGuild ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	ESlateVisibility visibilityNoHit = bIsMyGuild ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;

	_textBlockGuildAssetTitle->SetVisibility(visibilityNoHit);
	_textBlockGuildAsset->SetVisibility(visibilityNoHit);
	_imgIconGuildAsset->SetVisibility(visibilityNoHit);

	_barContribution->SetVisibility(visibilityNoHit);
	_btnTooltipCamp->SetVisibility(visibilityNoHit);

	_btnDonation->SetVisibility(visibilityHit);
	_btnAttendance->SetVisibility(visibilityHit);

	if (IsBranch(EGsGameClientBranchType::GUILD_AUCTION))
	{
		_btnStorage->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_btnStorage->SetVisibility(visibilityHit);
	}
}

void UGsUIGuildContentMain::OnResetTimeContents(ResetTimeTicket InType)
{
	const FGsGuildDataMine* guildData = GGuild()->GetMyGuildData();
	if (guildData)
	{
		SetUIGuildAttendance(guildData);
	}
}

void UGsUIGuildContentMain::OnChangeMyAuthority()
{
	if (_bIsMyGuild)
	{
		_uiDiplomacy->OnChangeMyAuthority();
	}
}

void UGsUIGuildContentMain::OnReceiveChangeGuildSetting(const FGsGuildMsgParamSetting* InParam)
{
	// null일 수 없는값이라 널체크 안함.
	const FGsGuildDataMine* guildData = GGuild()->GetMyGuildData();
	switch (InParam->_settingType)
	{	
	case FGsGuildMsgParamSetting::EGsGuildSettingType::Notice:
	{
		auto thiz = MakeWeakObjectPtr(this);
		FGsTextUtil::ChangeBanSentence(guildData->GetNotice(), [thiz](bool success, FString inText, FString outText) 
			{
				if (thiz.IsValid())
				{
					thiz->SetUIGuildNotice(outText);
				}
			});
	}
	break;
	case FGsGuildMsgParamSetting::EGsGuildSettingType::LimitLevel:
	{
		SetUIGuildLimitLevel(guildData->_joinLimitLevel);
	}
	break;
	case FGsGuildMsgParamSetting::EGsGuildSettingType::Emblem:
	{
		SetUIGuildEmblem(guildData->_emblemId);
	}
	break;
	case FGsGuildMsgParamSetting::EGsGuildSettingType::SNSLink:
	{
		SetSnsLinkIcons(GGuild()->MakeSnsLinks(_bIsMyGuild));
	}
	break;
	}
}

void UGsUIGuildContentMain::OnChangeCurrency(CurrencyType InType)
{
	if (CurrencyType::GUILD_ASSET == InType)
	{
		SetUIGuildAsset(FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GUILD_ASSET));
	}
}

void UGsUIGuildContentMain::OnChangeGuildLevel()
{
	if (_bIsMyGuild)
	{
		if (const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData())
		{
			SetUIGuildLevel(myGuildData->_guildLevel, myGuildData->_campLevel);
			SetUIConribution(myGuildData->_guildLevel, myGuildData->_contribution);
		}
	}
}

void UGsUIGuildContentMain::OnUpdateBuilding(GuildBuildingId InBuildingId)
{
	// 길드 건물이 뭐인지 체크하고 갱신하는 것보다 그냥 갱신이 빠른듯 하여 바로 갱신
	const FGsGuildDataMine* guildData = GGuild()->GetMyGuildData();
	if (nullptr == guildData)
	{
		return;
	}	

	SetUIGuildMember(guildData->_memberNum, guildData->GetMaxMemberNum());
}

void UGsUIGuildContentMain::OnNotifyChangeMemberGrade(const FGsGuildMsgParamMemberGrade* InParam)
{
	// 길드 마스터 변경 시
	if (GuildMemberGradeType::CAPTAIN == InParam->_memberGrade)
	{
		SetUIMasterName(InParam->_userName);
	}
}

void UGsUIGuildContentMain::OnNotifyChangeNotice()
{
	const FGsGuildDataMine* guildData = GGuild()->GetMyGuildData();
	if (guildData)
	{
		auto thiz = MakeWeakObjectPtr(this);
		FGsTextUtil::ChangeBanSentence(guildData->GetNotice(), [thiz](bool success, FString inText, FString outText) {
				if(thiz.IsValid()) thiz->SetUIGuildNotice(outText);
			});

		_redDotNotice.UpdateRedDot();
	}
}

void UGsUIGuildContentMain::OnReceiveGuildInfo()
{
	if (_bIsMyGuild)
	{
		InvalidateAll();
	}
}

void UGsUIGuildContentMain::OnChangeContribution()
{
	if (_bIsMyGuild)
	{
		if (const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData())
		{
			SetUIConribution(myGuildData->_guildLevel, myGuildData->_contribution);
		}
	}
}

void UGsUIGuildContentMain::OnReceiveChangeDailyActivity()
{
	SetUIGuildAttendance(GGuild()->GetMyGuildData());
}

void UGsUIGuildContentMain::OnUpdateStorage()
{
	if (_bIsMyGuild)
	{
		_redDotStorage.UpdateRedDot();
	}
}

void UGsUIGuildContentMain::OnDiplomacyResendList()
{
	if (_bIsMyGuild)
	{
		_uiDiplomacy->OnDiplomacyResendList();
	}
}

void UGsUIGuildContentMain::OnDiplomacyInvalidateList()
{
	if (_bIsMyGuild)
	{
		_uiDiplomacy->OnDiplomacyInvalidateList();
	}
}

void UGsUIGuildContentMain::OnDiplomacyEnemyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam)
{
	if (false == _bIsMyGuild)
	{
		return;
	}

	// 노티로 새로 추가될 경우, 일부 정보(길드레벨, 인원 수, 시간정보 등)가 누락되어 있어 추가 정보를 받기 위해 패킷 전송
	if (false == InParam->_bIsDelete)
	{
		if (FGsGuildManager* guildMgr = GGuild())
		{
			if (FGsGuildDataMine* myGuildData = guildMgr->GetMyGuildDataNoConst())
			{
				if (FGsGuildDiplomacyDataEnemy* enemyData = myGuildData->GetEnemyData(InParam->_guildDBId))
				{
					if (false == enemyData->IsValidData())
					{
						// 쿨타임을 강제로 리셋 후 리스트를 다시 요청한다.
						guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::EnemyList);
						guildMgr->GetSendController().SendReqEnemyList();
						return;
					}
				}
			}
		}
	}

	_uiDiplomacy->OnDiplomacyEnemyStateNotify(InParam);
}

void UGsUIGuildContentMain::OnDiplomacyAllyStateNotify(const FGsGuildMsgParamDiplomacyNotify* InParam)
{
	if (false == _bIsMyGuild)
	{
		return;
	}

	_uiDiplomacy->OnDiplomacyAllyStateNotify(InParam);
}

void UGsUIGuildContentMain::OnClickDonation()
{
	if (CheckIsJoinedDay())
	{
		return;
	}

	GUI()->Open(TEXT("PopupGuildDonation"));
}

void UGsUIGuildContentMain::OnClickAttendance()
{
	if (CheckIsJoinedDay())
	{
		return;
	}

	FGsNetSendServiceGuild::SendReqDailyActivity(GuildDailyActivityType::ATTENDANCE);
}

void UGsUIGuildContentMain::OnClickStorage()
{
	FGsNetSendServiceGuild::SendReqOpenGuildStorage();
}

void UGsUIGuildContentMain::OnClickTooltipCamp()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupSanctumNexusBuffInfo"));
	if (widget.IsValid())
	{
		if (UGsUIPopupSanctumNexusBuffInfo* popup = Cast<UGsUIPopupSanctumNexusBuffInfo>(widget.Get()))
		{
			uint8 campLevel = 0;
			if (_bIsMyGuild)
			{
				if (const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData())
				{
					campLevel = myGuildData->_campLevel;
				}
			}

			popup->SetData(campLevel);
		}
	}
}

void UGsUIGuildContentMain::OnClickSnsIcon(const int32 InIndex)
{
	const FGsGuildManager* GuildManager = GGuild();
	if (nullptr == GuildManager)
	{
		GSLOG(Warning, TEXT("nullptr == GuildManager"));
		return;
	}

	const TArray<FString> SnsLinks = GuildManager->MakeSnsLinks(_bIsMyGuild);
	if (!SnsLinks.IsValidIndex(InIndex))
	{
		GSLOG(Warning, TEXT("!SnsLinks.IsValidIndex(%d)"), InIndex);
		return;
	}

	GuildManager->TryToActionSnsLink(SnsLinks[InIndex]);
}

void UGsUIGuildContentMain::OnChangeDiplomacyTab()
{
	// 탭확인으로 레드닷이 지워질 경우, 상단 탭이 갱신되어야 하므로, 레드닷 업데이트 이벤트 연결
	OnReqUpdateMainTabRedDot.ExecuteIfBound(GetMainTabIndex());
}

void UGsUIGuildContentMain::SetUIGuildLevel(GuildLevel InLevel, int32 InCampLevel)
{
	_textBlockLevel->SetText(FText::AsNumber(InLevel));
	_textBlockCampLevel->SetText(FText::AsNumber(InCampLevel));
}

void UGsUIGuildContentMain::SetUIGuildMember(int32 InCurrMember, int32 InMaxMember)
{
	_textBlockMember->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { 
		InCurrMember, InMaxMember })));
}

void UGsUIGuildContentMain::SetUIGuildEmblem(GuildEmblemId InEmblemId)
{
	if (const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(InEmblemId))
	{
		_imgEmblem->SetBrushFromTexture(FGsUIHelper::LoadTextureFromPath(emblemData->iconPathBig));
	}
}

void UGsUIGuildContentMain::SetUIGuildAttendance(const FGsGuildDataDetail* InData)
{
	_textBlockAttendance->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"),
		{ InData->_attendanceCount, InData->_memberNum })));
	
	if (_bIsMyGuild)
	{
		// 출첵 했으면 비활성화
		_btnAttendance->SetIsEnabled(!InData->IsAttandance());

		_redDotAttendance.UpdateRedDot();
	}
}

void UGsUIGuildContentMain::SetUIGuildAsset(Currency InAmount)
{
	_textBlockGuildAsset->SetText(FText::AsNumber(InAmount));
}

void UGsUIGuildContentMain::SetUIGuildLimitLevel(Level InLevel)
{
	_textBlockLimitLevel->SetText(FText::FromString(FString::Format(TEXT("Lv.{0}"), { InLevel })));
}

void UGsUIGuildContentMain::SetUIMasterName(const FString& InUserName)
{
	_textBlockCaptain->SetText(FText::FromString(InUserName));
}

void UGsUIGuildContentMain::SetUIGuildNotice(const FString& InNotice)
{
	_textBlockNotice->SetText(FText::FromString(InNotice));

	_panelNoticeEmpty->SetVisibility(InNotice.IsEmpty() ? 
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIGuildContentMain::SetUIConribution(GuildLevel InLevel, GuildContribution InContribution)
{
	// 기사단 레벨 최대치일 경우 표시
	if (const FGsSchemaGuildLevelDetailData* maxLevelData = FGsGuildHelper::GetGuildMaxLevelData())
	{
		if (maxLevelData->level < InLevel ||
			static_cast<GuildContribution>(maxLevelData->contributionNextLevel) <= InContribution)
		{
			_barContribution->SetPercent(1.f);

			// TEXT: MAX
			FText textProgress;
			FText::FindText(TEXT("MarketText"), TEXT("UI_Market_Numpad_MAX"), textProgress);
			_barContribution->SetProgressText(textProgress);

			return;
		}
	}

	// 현재 레벨 기준으로 공헌도 ProgressBar 표시
	if (const FGsSchemaGuildLevelDetailData* levelData = FGsGuildHelper::GetGuildLevelData(InLevel))
	{
		int64 currLevelMinContribution = 0;
		if (const FGsSchemaGuildLevelDetailData* prevLevelData = FGsGuildHelper::GetGuildLevelData(InLevel - 1))
		{
			currLevelMinContribution = prevLevelData->contributionNextLevel;
		}

		GuildContribution currLevelContribution = InContribution - currLevelMinContribution;
		GuildContribution currLevelMaxContribution = levelData->contributionNextLevel - currLevelMinContribution;

		float percent = 0.f;
		if (0 < currLevelContribution &&
			0 < currLevelMaxContribution)
		{
			double percentlf = static_cast<double>(currLevelContribution) / static_cast<double>(currLevelMaxContribution);
			percent = static_cast<float>(percentlf);			
		}

		_barContribution->SetPercent(percent);

		FText textProgress;
		FGsUIStringHelper::GetTwoValueSlashedText(currLevelContribution, currLevelMaxContribution, textProgress);
		_barContribution->SetProgressText(textProgress);

		return;
	}

	// 그 외의 예외 상황
	_barContribution->SetPercent(0.f);
	_barContribution->SetProgressText(FText::FromString(TEXT("0/0")));
}

void UGsUIGuildContentMain::SetUIRank(int32 InDonationRank, int32 InDungeonClearRank)
{
	// 0이면 순위에 없는 항목. - 로 표시
	FText textDonationRank = (0 == InDonationRank) ? 
		FText::FromString(TEXT("-")) : FText::AsNumber(InDonationRank);
	_textBlockDonationRank->SetText(textDonationRank);

	FText textDungeonClearRank = (0 == InDungeonClearRank) ?
		FText::FromString(TEXT("-")) : FText::AsNumber(InDungeonClearRank);	
	_textBlockDungeonClearRank->SetText(textDungeonClearRank);
}

void UGsUIGuildContentMain::SetSnsLinkIcons(const TArray<FString>& InSnsLinks)
{
	const FGsGuildManager* GuildManager = GGuild();
	if (nullptr == GuildManager)
	{
		GSLOG(Error, TEXT("nullptr == GuildManager"));
		return;
	}

	const bool IsShowShortcuts = (0 < InSnsLinks.Num());
	_snsLinkIconListView->SetVisibility(IsShowShortcuts ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (IsShowShortcuts)
	{
		TArray<UGsUIGuildSnsIconListItem*> ListItems;

		for (int32 i = 0; i < InSnsLinks.Num(); ++i)
		{
			const FString& SnsLink = InSnsLinks[i];
			const FGsSchemaGuildSnsShortcutUrl* SnsShortcutUrl = GuildManager->FindSnsLinkInfo(SnsLink);

			UGsUIGuildSnsIconListItem* ListItem = NewObject<UGsUIGuildSnsIconListItem>();
			ListItem->_index = i;
			ListItem->_hasUrl = !SnsLink.IsEmpty();
			ListItem->_onClickIcon.BindUObject(this, &UGsUIGuildContentMain::OnClickSnsIcon);

			if (nullptr != SnsShortcutUrl && !SnsShortcutUrl->platform.IsNull())
			{
				ListItem->_iconIndex = SnsShortcutUrl->platform.GetRow()->iconIndex;
			}

			ListItems.Emplace(ListItem);
		}

		_snsLinkIconListView->SetListItems(ListItems);
	}
}

bool UGsUIGuildContentMain::CheckIsJoinedDay()
{
	const FGsGuildDataMine* guildData = GGuild()->GetMyGuildData();
	if (guildData)
	{
		if (guildData->IsJoinedDay())
		{
			// TEXT: 가입 당일에는 출석과 기부를 할 수 없습니다.
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotAttendanceDonation_JoinDay"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

			return true;
		}
	}

	return false;
}
