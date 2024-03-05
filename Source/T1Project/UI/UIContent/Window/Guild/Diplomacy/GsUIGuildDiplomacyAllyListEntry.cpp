// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDiplomacyAllyListEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/GsUIPopupGuildDiplomacyConfirm.h"
#include "Guild/GsGuildDiplomacyData.h"
#include "Guild/GsGuildHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Net/GsNetSendServiceGuild.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"


void UGsUIGuildDiplomacyAllyListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnRemove->OnClicked.AddDynamic(this, &UGsUIGuildDiplomacyAllyListEntry::OnClickRemove);
}

void UGsUIGuildDiplomacyAllyListEntry::NativeDestruct()
{
	_allyData = nullptr;

	Super::NativeDestruct();
}

void UGsUIGuildDiplomacyAllyListEntry::SetData(const FGsGuildDiplomacyDataAlly* InData)
{
	SetBaseData(InData);

	_allyData = InData;

	_btnRemove->SetIsEnabled(FGsGuildHelper::CanDiplomacy(GGuild()->GetAuthority()));

	FDateTime serverDateTime = FGsTimeSyncUtil::ConvertToDateTime(InData->_allyTime);
	FDateTime dateTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(serverDateTime);

	FString textDate;
	FGsTimeStringHelper::GetTimeStringNYMDHM(dateTime, textDate);
	_tbAllyTime->SetText(FText::FromString(textDate));

	//_tbAllyTime->SetText(FText::AsDateTime(dateTime, EDateTimeStyle::Default, EDateTimeStyle::Default,
		//FText::GetInvariantTimeZone()));
}

void UGsUIGuildDiplomacyAllyListEntry::OnClickRemove()
{
	if (INVALID_GUILD_DB_ID == _guildDBId ||
		nullptr == _allyData)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildDiplomacyConfirm"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildDiplomacyConfirm* popup = Cast<UGsUIPopupGuildDiplomacyConfirm>(widget.Get()))
		{
			// TEXT: 위 기사단과의 동맹을 해제하시겠습니까?
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Confirm_Req_Break_Ally"), textMsg);

			GuildDBId guildDBId = _allyData->_guildDBId;
			popup->SetData(_allyData, textMsg, true, [guildDBId](bool bIsOk)
				{
					if (bIsOk)
					{
						FGsNetSendServiceGuild::SendBreakAlly(guildDBId);
					}
				});
		}
	}
}
