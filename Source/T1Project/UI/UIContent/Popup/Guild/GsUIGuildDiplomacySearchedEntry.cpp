// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDiplomacySearchedEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupGuildDiplomacyConfirm.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildHelper.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"


void UGsUIGuildDiplomacySearchedEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIGuildDiplomacySearchedEntry::OnClickSlot);
	_btnRequest->OnClicked.AddDynamic(this, &UGsUIGuildDiplomacySearchedEntry::OnClickRequest);
}

void UGsUIGuildDiplomacySearchedEntry::OnHide()
{
	_guildDBId = INVALID_GUILD_DB_ID;
	_bIsAlly = false;
	_bIsOtherServer = false;
	_guildData = nullptr;
}

void UGsUIGuildDiplomacySearchedEntry::SetData(const FGsGuildData* InData, bool bInIsAlly)
{
	if (nullptr == InData)
	{
		return;
	}

	_guildDBId = InData->_guildDBId;
	_bIsAlly = bInIsAlly;
	_guildData = InData;
	_bIsOtherServer = false;

	WorldId worldId = InData->GetWorldId();
	if (FGsGameDataManager* gameDataMgr = GGameData())
	{
		if (const FGsNetUserData* userData = gameDataMgr->GetUserData())
		{
			if (worldId != userData->mHomeWorldId)
			{
				_bIsOtherServer = true;
			}
		}
	}

	FText textGuildName;
	FGsGuildHelper::GetFormattedGuildName(InData->_guildName, InData->GetWorldId(), false, textGuildName);
	_textBlockName->SetText(textGuildName);

	// 타서버일 경우 레벨, 인원 정보 비공개(메인화면에서 안보이는 정보는 서버가 취급하지 않음)
	if (_bIsOtherServer)
	{
		FText textUnknown = FText::FromString(TEXT("?"));

		_textBlockLevel->SetText(textUnknown);
		_textBlockMemberNum->SetText(textUnknown);
	}
	else
	{
		_textBlockLevel->SetText(FText::AsNumber(InData->_guildLevel));
		_textBlockMemberNum->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"),
			{ InData->_memberNum, InData->GetMaxMemberNum() })));
	}
	
	_textBlockMaster->SetText(FText::FromString(InData->_masterName));	
	
	FText textBtnName;
	if (bInIsAlly)
	{
		// TEXT: 동맹 신청
		FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Req_Ally"), textBtnName);
	}
	else
	{
		// TEXT: 적대 신청
		FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Req_Enemy"), textBtnName);
	}

	_textBlockButton->SetText(textBtnName);
}

void UGsUIGuildDiplomacySearchedEntry::OnClickSlot()
{
	if (_bIsOtherServer)
	{
		// TEXT: 다른 서버 기사단의 세부 정보는 확인할 수 없습니다.		
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Search_InterServer_GuildInfo"), textMsg))
		{
			FGsUIHelper::TrayMessageTicker(textMsg);
		}

		return;
	}

	if (0 < _guildDBId)
	{
		GGuild()->SendRequestGuildDetail(_guildDBId);
	}
}

void UGsUIGuildDiplomacySearchedEntry::OnClickRequest()
{
	if (INVALID_GUILD_DB_ID == _guildDBId ||
		nullptr == _guildData)
	{
		return;
	}

	if (false == FGsGuildHelper::CheckDiplomacyGuildDBId(_guildData->_guildDBId, true))
	{
		return;
	}

	const FGsSchemaGuildConfig* config = FGsGuildHelper::GetGuildConfig();
	if (nullptr == config)
	{
		return;
	}

	const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData();
	if (nullptr == myGuildData)
	{
		return;
	}
	
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildDiplomacyConfirm"));
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupGuildDiplomacyConfirm* popup = Cast<UGsUIPopupGuildDiplomacyConfirm>(widget.Get());
	if (nullptr == popup)
	{
		return;
	}

	FGsGuildDiplomacyDataBase data;
	data._guildDBId = _guildData->_guildDBId;
	data._emblemId = _guildData->_emblemId;
	data._guildName = _guildData->_guildName;
	data._masterName = _guildData->_masterName;
	data._homePlanetWorldId.value = _guildData->_homePlanetWorldId.value;

	GuildDBId guildDBId = _guildDBId;
	FText textFormat;

	if (_bIsAlly)
	{
		// TEXT: 위 기사단에 동맹을 요청하시겠습니까? (등록가능 {0}/{1})
		FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Confirm_Req_Ally"), textFormat);
		FText textMsg = FText::Format(textFormat, myGuildData->_allyList.Num(), config->maxAllyCount);

		popup->SetData(&data, textMsg, true, [guildDBId](bool bIsOk)
			{
				if (bIsOk)
				{
					FGsNetSendServiceGuild::SendAllyAddList(guildDBId);
					GUI()->CloseByKeyName(TEXT("PopupGuildDiplomacySearch"));
				}
			});
	}
	else
	{
		// TEXT: 위 기사단을 적대 기사단으로 등록 하시겠습니까? (등록가능 {0}/{1})
		FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Confirm_Req_Hostile"), textFormat);
		FText textMsg = FText::Format(textFormat, myGuildData->_enemyList.Num(), config->maxEnemyCount);

		popup->SetData(&data, textMsg, false, [guildDBId](bool bIsOk)
			{
				if (bIsOk)
				{
					FGsNetSendServiceGuild::SendHostileAdd(guildDBId);
					GUI()->CloseByKeyName(TEXT("PopupGuildDiplomacySearch"));
				}
			});
	}
}
