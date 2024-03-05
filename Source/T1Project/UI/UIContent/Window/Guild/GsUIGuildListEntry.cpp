// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildListEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildHelper.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "UTIL/GsText.h"


void UGsUIGuildListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIGuildListEntry::OnClickSlot);
	_switcherJoin->OnClicked.AddDynamic(this, &UGsUIGuildListEntry::OnClickJoin);

	// 광클방지 타임 설정
	_checkerJoin.SetIntervalSecond(1.f);
}

void UGsUIGuildListEntry::OnHide()
{
	_guildDBId = INVALID_GUILD_DB_ID;
	_joinType = GuildJoinType::MAX;
	_limitLevel = 0;
}

void UGsUIGuildListEntry::SetData(const FGsGuildData* InData)
{
	if (nullptr == InData)
	{
		// 빈슬롯 처리필요할까? 버그상황임
		return;
	}

	_guildDBId = InData->_guildDBId;
	_joinType = InData->_joinType;
	_limitLevel = InData->_joinLimitLevel;
		
	_textBlockName->SetText(FText::FromString(InData->_guildName));
	_textBlockLevel->SetText(FText::AsNumber(InData->_guildLevel));

	_textBlockMemberNum->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"), 
		{ InData->_memberNum, InData->GetMaxMemberNum() })));
	_textBlockMaster->SetText(FText::FromString(InData->_masterName));
	_textBlockLimitLevel->SetText(FText::FromString(FString::Format(TEXT("Lv.{0}") , { InData->_joinLimitLevel })));
	
	auto thiz = MakeWeakObjectPtr(this);
	FGsTextUtil::ChangeBanSentence(InData->GetIntroduction(), [thiz](bool success, FString inText, FString outText) {
			if(thiz.IsValid()) thiz->_textBlockIntroduction->SetText(FText::FromString(outText));
		});

	// 현재 상태에 따라 Join버튼 Switch	
	if (GGuild()->IsJoinWaitingGuild(InData->_guildDBId))
	{
		// 신청중
		_switcherJoin->SetSwitcherIndex(EGsJoinButtonState::WaitApproval);
		_switcherJoin->SetIsEnabled(true);
	}
	else
	{
		if (GuildJoinType::UNABLE == InData->_joinType)
		{
			_switcherJoin->SetSwitcherIndex(EGsJoinButtonState::Unable);
			_switcherJoin->SetIsEnabled(false);
		}
		else
		{
			_switcherJoin->SetSwitcherIndex((GuildJoinType::IMMEDIATE == InData->_joinType) ? 
				EGsJoinButtonState::Immediate : EGsJoinButtonState::Approval);

			// 길드가 꽉찼는가 검사
			bool bIsFull = (InData->_memberNum >= InData->_maxMemberNum) ? true : false;
			_switcherJoin->SetIsEnabled(!bIsFull);
		}
	}
}

bool UGsUIGuildListEntry::CheckJoinRequestCount()
{
	TArray<FGsGuildData*> requestGuildList = GGuild()->GetJoinWaitingGuildList();
	const FGsSchemaGuildConfig* config = FGsGuildHelper::GetGuildConfig();
	if (config)
	{
		if (requestGuildList.Num() < config->maxJoinRequestCount)
		{
			return true;
		}
	}

	// TEXT: 기사단 가입 신청은 최대 {0}개 할 수 있습니다.
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Alert_FullApplication"), textFormat))
	{
		FText textMsg = FText::Format(textFormat, config->maxJoinRequestCount);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}

	return false;
}

void UGsUIGuildListEntry::OnClickJoin()
{
	if (INVALID_GUILD_DB_ID == _guildDBId)
	{
		return;
	}

	// 광클 방지 처리
	if (false == _checkerJoin.IsTimeOver())
	{
		// TEXT: 잠시 후 다시 시도해주세요.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return;
	}

	_checkerJoin.UpdateTime();

	FGsGuildManager* guildMgr = GGuild();

	// 재가입 대기시간 체크
	if (0 < guildMgr->GetReJoinRemainTime())
	{
		const FGsSchemaGuildConfig* config = FGsGuildHelper::GetGuildConfig();
		if (config)
		{
			// TEXT: 탈퇴 후 길드 가입은 {0} 시간 지난 후에 가능합니다.
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_JoinCoolTime"), textFormat))
			{
				FText textMsg = FText::Format(textFormat, config->reJoinCooldownHour);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}

		return;
	}

	// 신청중인 길드는 취소 요청
	if (guildMgr->IsJoinWaitingGuild(_guildDBId))
	{
		FGsNetSendServiceGuild::SendReqCancelJoinGuild(_guildDBId);
		return;
	}

	if (GuildJoinType::UNABLE != _joinType)
	{
		if (GuildJoinType::APPROVAL == _joinType)
		{
			// 가입요청 가능 개수인지 체크
			if (false == CheckJoinRequestCount())
			{
				return;
			}
		}

		// 주의: 기사단 정보를 실시간 동기화하지 않기 때문에, 기사단 가입조건등을 클라가 체크하면 안됨
		// 서버가 에러를 줄 경우, 기사단 정보가 바뀌었다는 메시지 출력 
		FGsNetSendServiceGuild::SendReqJoinGuild(_guildDBId);
	}
}

void UGsUIGuildListEntry::OnClickSlot()
{
	if (0 < _guildDBId)
	{
		GGuild()->SendRequestGuildDetail(_guildDBId);
	}
}
