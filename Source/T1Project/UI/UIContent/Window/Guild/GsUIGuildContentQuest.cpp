// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildContentQuest.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUICurrencyRefreshCountButton.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Window/Guild/Quest/GsUIGuildQuestListEntry.h"
#include "UI/UIContent/Window/Guild/Quest/GsUIGuildQuestPointListEntry.h"
#include "UI/UIContent/Window/Guild/Quest/GsUIGuildQuestPointEffect.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "DataSchema/Guild/GsSchemaGuildLevelDetailData.h"
#include "DataSchema/Guild/GsSchemaGuildZealPointReward.h"
#include "DataSchema/Guild/GsSchemaGuildQuestConfig.h"
#include "Currency/GsCurrencyHelper.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildHelper.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"


void UGsUIGuildContentQuest::BeginDestroy()
{
	if (_slotHelperPoint)
	{
		_slotHelperPoint->OnRefreshEntry.RemoveDynamic(this, &UGsUIGuildContentQuest::OnRefreshEntryPoint);
	}
	_slotHelperPoint = nullptr;

	if (_slotHelperQuest)
	{
		_slotHelperQuest->OnRefreshEntry.RemoveDynamic(this, &UGsUIGuildContentQuest::OnRefreshEntryQuest);
	}
	_slotHelperQuest = nullptr;

	Super::BeginDestroy();
}

void UGsUIGuildContentQuest::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperPoint = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperPoint->Initialize(_pointWidgetClass, _panelRootPoint);
	_slotHelperPoint->OnRefreshEntry.AddDynamic(this, &UGsUIGuildContentQuest::OnRefreshEntryPoint);

	_slotHelperQuest = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperQuest->Initialize(_questWidgetClass, _panelRootQuest);
	_slotHelperQuest->SetEmptyListPanel(_panelRootQuestEmpty);
	_slotHelperQuest->OnRefreshEntry.AddDynamic(this, &UGsUIGuildContentQuest::OnRefreshEntryQuest);

	_btnTooltipZealPoint->OnPressed.AddDynamic(this, &UGsUIGuildContentQuest::OnPressTooltipZealPoint);
	_btnTooltipQuest->OnPressed.AddDynamic(this, &UGsUIGuildContentQuest::OnPressTooltipQuest);
	_btnRefresh->OnClickRefreshButton.BindUObject(this, &UGsUIGuildContentQuest::OnClickRefresh);
}

void UGsUIGuildContentQuest::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CheckInvalidate();

	if (0 < _checkTimeSec)
	{
		_checkTimeSec -= InDeltaTime;
		if (0 >= _checkTimeSec)
		{
			InvalidateRemainTime();
		}
	}
}

void UGsUIGuildContentQuest::SendRequestPacket()
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig())
		{
			// 기사단 퀘스트가 오픈 되었을 때만 패킷을 보냄
			// 모자랄 경우 QUEST_ERROR_NOT_ENOUGH_GUILD_LEVEL 에러가 오기 때문
			if (guildMgr->GetGuildLevel() >= guildConfig->guildQuestOpenLevel)
			{
				GGuild()->GetSendController().SendReqQuestInfo();
			}
		}
	}
}

void UGsUIGuildContentQuest::Show(bool bIsMyGuild, bool IsStackRecoverFlag)
{
	_bIsMyGuild = bIsMyGuild;
	InvalidateAll();
}

void UGsUIGuildContentQuest::Hide()
{
	if (_bIsMyGuild)
	{
		if (FGsGuildManager* guildMgr = GGuild())
		{
			guildMgr->GetRedDotCheckData().UpdateQuestCheckTime();
			guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Quest);
		}
	}
}

void UGsUIGuildContentQuest::OnResetTimeContents(ResetTimeTicket InType)
{
	if (ResetTimeTicket::RTT_GUILD_QUEST == InType ||
		ResetTimeTicket::RTT_GUILD_ZEAL_POINT_RESET == InType)
	{
		SendRequestPacket();
	}
}

void UGsUIGuildContentQuest::OnChangeGuildLevel()
{
	// 길드 레벨이 변경될 경우, 전체 갱신
	Level level = GGuild()->GetGuildLevel();
	if (level != _guildLevel)
	{
		_guildLevel = level;
		InvalidateAll();
	}
}

void UGsUIGuildContentQuest::InvalidateAllInternal()
{
	FGsGuildManager* guildMgr = GGuild();

	FGsGuildQuestDataHandler* questHandler = guildMgr->GetQuestHandler();
	if (nullptr == questHandler)
	{
		return;
	}

	_guildLevel = guildMgr->GetGuildLevel();
	_maxZealPointLevel = 0;

	if (const FGsSchemaGuildLevelDetailData* levelData = FGsGuildHelper::GetGuildLevelData(_guildLevel))
	{
		_maxZealPointLevel = levelData->maxZealPointLevel;
	}

	// 열성 포인트 리스트
	_pointList.Empty();
	if (const UGsTable* table = FGsSchemaGuildZealPointReward::GetStaticTable())
	{
		table->GetAllRows(_pointList);
	}

	InvalidateGuildZealPoint();
	ScrollToCurrZealPoint();
	
	// 퀘스트 리스트
	InvalidateQuestList();
	_scrollQuest->ScrollToStart();

	// 새로고침 설정
	InvalidateRefreshInfo();

	// 남은 시간 표시
	_endPointResetSec = questHandler->GetZealPointResetTime();
	_endQuestResetSec = questHandler->GetQuestResetTime();

	InvalidateRemainTime();

	// 수행 횟수 표시
	InvalidateCompleteCount();
}

void UGsUIGuildContentQuest::InvalidateGuildZealPoint()
{
	_currZealPoint = GGuild()->GetGuildZealPoint();
	_tbWeeklyPoint->SetText(FText::AsNumber(_currZealPoint));

	_slotHelperPoint->RefreshAll(_pointList.Num());
}

void UGsUIGuildContentQuest::InvalidateRemainTime()
{
	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();

	time_t pointRemainSec = 0;
	if (serverNowSec < _endPointResetSec)
	{
		pointRemainSec = _endPointResetSec - serverNowSec;
	}
	SetUIRemainTime(FTimespan::FromSeconds(pointRemainSec), _tbRewardRemainTime);
	
	_panelRootPointRemainTime->SetVisibility(
		(0 < pointRemainSec) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);


	time_t questRemainSec = 0;
	if (serverNowSec < _endQuestResetSec)
	{
		questRemainSec = _endQuestResetSec - serverNowSec;
	}
	SetUIRemainTime(FTimespan::FromSeconds(questRemainSec), _tbResetRemainTime);

	_panelRootQuestRemainTime->SetVisibility(
		(0 < questRemainSec) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	
	// 1초에 한번씩 갱신
	_checkTimeSec = 1.f;
}

void UGsUIGuildContentQuest::InvalidateQuestList()
{
	// 길드 퀘스트가 오픈되지 않음
	int32 questOpenLevel = 0;
	if (const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig())
	{
		questOpenLevel = guildConfig->guildQuestOpenLevel;
	}

	if (_guildLevel < questOpenLevel)
	{
		SetUIQuestLock(true);

		// TEXT: 기사단 레벨 {0}부터, 기사단 퀘스트를 진행하실 수 있습니다.
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("UI_GuildQuest_LockText"), textFormat))
		{
			FText textLock = FText::Format(textFormat, questOpenLevel);
			_tbLockDesc->SetText(textLock);
		}
		else
		{
			_tbLockDesc->SetText(FText::GetEmpty());
		}

		return;
	}

	// 가입하고 하루가 지나지 않음(대신 퀘스트가 리셋되었으면 가능)
	if (IsLockedByGuildJoinDay())
	{
		SetUIQuestLock(true);

		// TEXT: 다음 초기화 시점부터, 기사단 퀘스트를 수행하실 수 있습니다.
		FText textLock;
		FText::FindText(TEXT("GuildText"), TEXT("UI_GuildQuest_CanNotDoQuest"), textLock);
		_tbLockDesc->SetText(textLock);

		return;
	}

	SetUIQuestLock(false);

	if (FGsGuildQuestDataHandler* questHandler = GGuild()->GetQuestHandler())
	{
		// 퀘스트 리스트 표시
		_questList.Empty();
		_questList.Append(questHandler->GetQuestList());
		_slotHelperQuest->RefreshAll(_questList.Num());
	}
}

void UGsUIGuildContentQuest::InvalidateRefreshInfo()
{
	FGsGuildQuestDataHandler* questHandler = GGuild()->GetQuestHandler();
	if (nullptr == questHandler)
	{
		return;
	}

	// 새로고침 설정
	if (const FGsSchemaGuildQuestConfig* questConfig = FGsGuildHelper::GetQuestConfig())
	{
		int32 refreshStep = questHandler->GetRefreshStep(); // 1부터 시작하는 값

		const FGsSchemaGuildQuestRefreshInfo* refreshInfo = nullptr;
		for (int i = 0; i < questConfig->refreshInfoList.Num(); ++i)
		{
			if (refreshStep == questConfig->refreshInfoList[i].number)
			{
				refreshInfo = &questConfig->refreshInfoList[i];
				break;
			}
		}

		if (refreshInfo)
		{
			// refreshStep은 1부터 시작하는 값이므로 1을 빼고 넣음
			int32 remainCount = questConfig->refreshInfoList.Num() - (refreshStep - 1);
			_btnRefresh->SetData(remainCount, refreshInfo->currencyType, refreshInfo->cost);
		}
		else
		{
			// 없을 경우
			_btnRefresh->SetData(0, CurrencyType::GOLD, 0);
		}
	}
}

void UGsUIGuildContentQuest::InvalidateCompleteCount()
{
	int32 currCount = 0;
	if (FGsGuildQuestDataHandler* questHandler = GGuild()->GetQuestHandler())
	{
		currCount = questHandler->GetCompleteCount();
	}

	FText textCount;
	FGsUIStringHelper::GetTwoValueSlashedText(currCount, MAX_COMPLETE_QUEST_GUILD_COUNT, textCount);

	_tbQuestCount->SetText(textCount);
}

void UGsUIGuildContentQuest::SetUIRemainTime(const FTimespan& InTime, OUT UTextBlock* OutTextBlock)
{
	if (0 >= InTime.GetTotalSeconds())
	{
		OutTextBlock->SetText(FText::GetEmpty());
	}
	else
	{
		FString strTime;
		FGsTimeStringHelper::GetTimeStringCeil(InTime, strTime, false);
		OutTextBlock->SetText(FText::FromString(strTime));
	}
}

void UGsUIGuildContentQuest::SetUIQuestLock(bool bIsLocked)
{
	_switcherQuest->SetActiveWidgetIndex(bIsLocked ? 0 : 1);
	_btnRefresh->SetIsEnabled(!bIsLocked);
	
	// 화로연출
	//_uiPointEffect->SetPointState(bIsLocked ?
	//	UGsUIGuildQuestPointEffect::EGsGuildQuestPointState::LOCKED :
	//	UGsUIGuildQuestPointEffect::EGsGuildQuestPointState::ACTIVE);
}

void UGsUIGuildContentQuest::ScrollToCurrZealPoint()
{
	int pointScrollTargetIndex = 0;
	if (0 < _currZealPoint)
	{
		int64 prevPoint = 0;
		for (int i = 0; i < _pointList.Num(); ++i)
		{
			if (prevPoint < _currZealPoint &&
				_pointList[i]->point >= _currZealPoint)
			{
				break;
			}

			prevPoint = _pointList[i]->point;
			pointScrollTargetIndex = i;
		}
	}

	if (0 < pointScrollTargetIndex)
	{
		if (UWidget* scrollTargetWidget = _slotHelperPoint->GetEntry(pointScrollTargetIndex))
		{
			_scrollPoint->ScrollWidgetIntoView(scrollTargetWidget, true, EDescendantScrollDestination::TopOrLeft);
		}
		else
		{
			_scrollPoint->ScrollToStart();
		}
	}
	else
	{
		_scrollPoint->ScrollToStart();
	}
}

float UGsUIGuildContentQuest::GetZealPointProgressRate(int32 InIndex)
{
	if (0 >= _currZealPoint)
	{
		return 0.f;
	}

	// 기본 정보
	const FGsSchemaGuildZealPointReward* currPointData = _pointList[InIndex];

	// 열성 진행도 퍼센트
	float progressRate = 0.f;

	// 주의: point가 ProgressBar의 중앙이어서 다음의 연산을 수행
	if (currPointData->point > _currZealPoint)
	{
		// 현재 단계 포인트보다 작을 경우
		const FGsSchemaGuildZealPointReward* prevPointData = _pointList.IsValidIndex(InIndex - 1) ?
			_pointList[InIndex - 1] : nullptr;

		if (prevPointData)
		{
			// 이전 슬롯의 데이터보다 클 경우
			if (prevPointData->point <= _currZealPoint)
			{
				int32 currPointDiff = _currZealPoint - prevPointData->point;
				int32 slotPointDiff = currPointData->point - prevPointData->point;
				if (0 != currPointDiff && 0 != slotPointDiff)
				{
					// 바의 중앙이 점수 부분이라 0.5를 뺌
					float currPercent = static_cast<float>(currPointDiff) / slotPointDiff;
					if (0.5f < currPercent)
					{
						progressRate = currPercent - 0.5f;
					}
					else
					{
						progressRate = 0.f;
					}
				}
			}
		}
		else
		{
			// 이전이 없는 맨 처음 슬롯. 바의 중앙이 점수부분 이므로 0.5를 곱함
			if (0 != _currZealPoint && 0 != currPointData->point)
			{
				progressRate = static_cast<float>(_currZealPoint) / currPointData->point * 0.5f;
			}
		}
	}
	else if (currPointData->point < _currZealPoint)
	{
		// 현재 단계 포인트보다 클 경우
		const FGsSchemaGuildZealPointReward* nextPointData = _pointList.IsValidIndex(InIndex + 1) ?
			_pointList[InIndex + 1] : nullptr;

		// 다음 단계가 있을 경우
		if (nextPointData)
		{
			int32 currPointDiff = _currZealPoint - currPointData->point;
			int32 slotPointDiff = nextPointData->point - currPointData->point;

			if (0 != currPointDiff && 0 != slotPointDiff)
			{
				float currPercent = static_cast<float>(currPointDiff) / slotPointDiff;
				if (0.5f > currPercent)
				{
					progressRate = currPercent + 0.5f;
				}
				else
				{
					progressRate = 1.f;
				}
			}
		}
		else
		{
			// 다음이 없는 맨 마지막 슬롯
			progressRate = 0.5f;
		}
	}
	else
	{
		// 현재 단계 포인트와 같은 경우
		progressRate = 0.5f;
	}

	return progressRate;
}

bool UGsUIGuildContentQuest::IsLockedByGuildJoinDay()
{
	FGsGuildManager* guildMgr = GGuild();

	bool bIsJoinedDay = true;
	if (const FGsGuildDataMine* guildData = guildMgr->GetMyGuildData())
	{
		bIsJoinedDay = guildData->IsJoinedDay();

		if (bIsJoinedDay)
		{
			if (FGsGuildQuestDataHandler* questHandler = guildMgr->GetQuestHandler())
			{
				// 마지막 퀘스트 갱신날짜가 가입날짜를 넘었으면 무조건 가능
				if (guildData->_joinedTime < questHandler->GetLastQuestResetTime())
				{
					bIsJoinedDay = false;
				}
			}
		}
	}

	return bIsJoinedDay;
}

void UGsUIGuildContentQuest::OnRefreshEntryPoint(int32 InIndex, UWidget* InEntry)
{
	if (false == _pointList.IsValidIndex(InIndex))
	{
		return;
	}	

	if (UGsUIGuildQuestPointListEntry* entry = Cast<UGsUIGuildQuestPointListEntry>(InEntry))
	{
		// 기본 정보
		const FGsSchemaGuildZealPointReward* currPointData = _pointList[InIndex];

		// 상태 판정
		UGsUIGuildQuestPointListEntry::EGsGuildQuestPointState slotState =
			UGsUIGuildQuestPointListEntry::EGsGuildQuestPointState::LOCKED;

		if (currPointData->level <= _maxZealPointLevel)
		{
			slotState = (currPointData->point <= _currZealPoint) ?
				UGsUIGuildQuestPointListEntry::EGsGuildQuestPointState::REWARDED :
				UGsUIGuildQuestPointListEntry::EGsGuildQuestPointState::ACTIVE;
		}				

		float progressRate = GetZealPointProgressRate(InIndex);

		entry->SetData(currPointData);
		entry->SetState(slotState);
		entry->SetProgressBar(progressRate);
	}
}

void UGsUIGuildContentQuest::OnRefreshEntryQuest(int32 InIndex, UWidget* InEntry)
{
	if (false == _questList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIGuildQuestListEntry* entry = Cast<UGsUIGuildQuestListEntry>(InEntry))
	{
		entry->SetData(_questList[InIndex]);
	}
}

void UGsUIGuildContentQuest::OnPressTooltipZealPoint()
{
	if (_tooltipZealPoint->IsOpen())
	{
		_tooltipZealPoint->Close();
		return;
	}

	// TEXT: 각 기사단원들이 개인 목표를 클리어했을 때, ...
	FText textDesc;
	if (FText::FindText(TEXT("GuildText"), TEXT("UI_GuildQuest_InfoIcon_GuildReward"), textDesc))
	{
		_tooltipZealPoint->SetDescText(textDesc);
	}

	_tooltipZealPoint->Open();
}

void UGsUIGuildContentQuest::OnPressTooltipQuest()
{
	if (_tooltipQuest->IsOpen())
	{
		_tooltipQuest->Close();
		return;
	}

	// TEXT: 개인 목표란, 기사단에 소속된 자만이 수행할 수 있는 퀘스트 입니다.
	FText textDesc;
	if (FText::FindText(TEXT("GuildText"), TEXT("UI_GuildQuest_InfoIcon_KnightQuest"), textDesc))
	{
		_tooltipQuest->SetDescText(textDesc);
	}

	_tooltipQuest->Open();
}

void UGsUIGuildContentQuest::OnClickRefresh()
{
	// 퀘스트 최대 횟수 상태이면 새로고침 할 필요가 없으므로 막음
	if (FGsGuildQuestDataHandler* questHandler = GGuild()->GetQuestHandler())
	{
		if (MAX_COMPLETE_QUEST_GUILD_COUNT <= questHandler->GetCompleteCount())
		{
			// TEXT: 퀘스트 수행 가능 횟수가 부족합니다.
			FText textMsg;
			if (FText::FindText(TEXT("GuildText"), TEXT("Ticker_NotEnough_QuestCount"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}

			return;
		}
	}

	FGsGuildSendCoolTimeController& sendController = GGuild()->GetSendController();
	if (false == sendController.IsCoolTimeOver(FGsGuildSendCoolTimeController::EGsCoolTimeType::QuestRefresh))
	{
		// TEXT: 잠시 후 다시 시도해주세요.
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		return;
	}

	CurrencyType currencyType = _btnRefresh->GetCurrencyType();

	// 재화 검사 및 재화 부족 팝업 연결
	if (false == FGsCurrencyHelper::CheckCurrency(currencyType, _btnRefresh->GetAmount(), false))
	{
		Currency lackAmount = _btnRefresh->GetAmount() - FGsCurrencyHelper::GetCurrencyAmount(currencyType);

		FGsCurrencyHelper::OpenLackCurrencyPopup(currencyType, lackAmount);
		return;
	}

	sendController.SendReqQuestRefresh();
}

void UGsUIGuildContentQuest::OnReceiveQuestInfo()
{
	InvalidateAll();
}

void UGsUIGuildContentQuest::OnUpdateQuest(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamQuest* param = InParam->Cast<const FGsGuildMsgParamQuest>())
	{
		for (int i = 0; i < _questList.Num(); ++i)
		{
			if (param->_storyId == _questList[i]->_storyId &&
				param->_questId == _questList[i]->_questId)
			{
				_slotHelperQuest->RefreshByIndex(i);
			}
		}

		if (param->_bIsReward)
		{
			InvalidateGuildZealPoint();
			
			// 화로연출
			//_uiPointEffect->SetPointState(UGsUIGuildQuestPointEffect::EGsGuildQuestPointState::BOOST);
		}
	}
	
	InvalidateRefreshInfo();
	InvalidateCompleteCount();
}

void UGsUIGuildContentQuest::OnReceiveQuestRefresh()
{
	InvalidateQuestList();
	InvalidateRefreshInfo();
}

void UGsUIGuildContentQuest::TestSetZealPoint(int32 InPoint)
{
	_currZealPoint = InPoint;

	InvalidateGuildZealPoint();
	ScrollToCurrZealPoint();
}
