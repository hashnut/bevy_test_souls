// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowGuildStart.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/GsMessageHolder.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "Guild/GsGuildHelper.h"


void UGsUIWindowGuildStart::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCreate->OnClicked.AddDynamic(this, &UGsUIWindowGuildStart::OnClickCreate);
	_btnJoin->OnClicked.AddDynamic(this, &UGsUIWindowGuildStart::OnClickJoin);

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowGuildStart::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowGuildStart::OnClickCloseAllStack);
}

void UGsUIWindowGuildStart::NativeConstruct()
{
	Super::NativeConstruct();

	_msgGuildHandle = GMessage()->GetGuild().AddUObject(MessageContentGuild::RECEIVE_GUILD_LIST,
		this, &UGsUIWindowGuildStart::OnReceiveGuildList);
}

void UGsUIWindowGuildStart::NativeDestruct()
{
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		msgMgr->GetGuild().Remove(_msgGuildHandle);
	}
	
	_bIsReservedCreate = false;

	Super::NativeDestruct();
}

bool UGsUIWindowGuildStart::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsGuild))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_GUILD);
		return true;
	}

	return false;
}

void UGsUIWindowGuildStart::OnClickCreate()
{
	StartOpenCreate();
}

void UGsUIWindowGuildStart::OnClickJoin()
{
	StartOpenJoin();
}

void UGsUIWindowGuildStart::OnReceiveGuildList(const IGsMessageParam*)
{
	// 패킷 수령 후 창 열기 시도
	if (_bIsReservedCreate)
	{
		CheckAndOpenCreateWindow();
	}
	else
	{
		CheckAndOpenJoinWindow();
	}
}

void UGsUIWindowGuildStart::CheckAndOpenCreateWindow()
{
	// 재가입 대기시간 체크
	if (0 < GGuild()->GetReJoinRemainTime())
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

	// 생성 창 열기
	GUI()->Open(TEXT("WindowGuildCreate"));
}

void UGsUIWindowGuildStart::CheckAndOpenJoinWindow()
{
	// 가입창 열기
	GUI()->Open(TEXT("WindowGuildJoin"));
}

void UGsUIWindowGuildStart::StartOpenCreate()
{
	_bIsReservedCreate = true;

	// 주의: 길드 리스트에 재가입 대기시간 정보가 오기 때문에, UI를 막기위해 한번 받아본다.
	if (GGuild()->GetSendController().SendRequestGuildList())
	{
		return;
	}

	// 이미 받은 정보가 있으면 바로 열기 시도
	CheckAndOpenCreateWindow();
}

void UGsUIWindowGuildStart::StartOpenJoin()
{
	_bIsReservedCreate = false;

	FGsGuildManager* guildMgr = GGuild();

	// 길드 목록에 신청중인지 상태 표시를 위해 필요하다. 먼저 받는다. 
	guildMgr->GetSendController().SendRequestRequestedJoinGuildList();

	// 이미 받은 정보가 있으면 바로 열기 시도
	CheckAndOpenJoinWindow();
}