// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDiplomacyEnemyListEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/GsUIPopupGuildDiplomacyConfirm.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Guild/GsGuildHelper.h"
#include "Guild/GsGuildDiplomacyData.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Net/GsNetSendServiceGuild.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"


void UGsUIGuildDiplomacyEnemyListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIGuildDiplomacyEnemyListEntry::OnClickCancel);
}

void UGsUIGuildDiplomacyEnemyListEntry::NativeDestruct()
{
	_enemyData = nullptr;

	Super::NativeDestruct();
}

void UGsUIGuildDiplomacyEnemyListEntry::SetData(const FGsGuildDiplomacyDataEnemy* InData)
{
	_enemyData = InData;

	SetBaseData(InData);

	_btnCancel->SetIsEnabled(FGsGuildHelper::CanDiplomacy(GGuild()->GetAuthority()));

	FDateTime serverDateTime = FGsTimeSyncUtil::ConvertToDateTime(InData->_enemyTime);
	FDateTime dateTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(serverDateTime);

	FString textDate;
	FGsTimeStringHelper::GetTimeStringNYMDHM(dateTime, textDate);
	_tbEnemyTime->SetText(FText::FromString(textDate));

	//_tbEnemyTime->SetText(FText::AsDateTime(dateTime, EDateTimeStyle::Default, EDateTimeStyle::Default,
	//	FText::GetInvariantTimeZone()));
}

void UGsUIGuildDiplomacyEnemyListEntry::OnClickCancel()
{
	if (INVALID_GUILD_DB_ID == _guildDBId ||
		nullptr == _enemyData)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildDiplomacyConfirm"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildDiplomacyConfirm* popup = Cast<UGsUIPopupGuildDiplomacyConfirm>(widget.Get()))
		{
			// TEXT: 위 기사단과의 적대를 해제하시겠습니까?
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Diplomacy_Confirm_Req_Enemy"), textMsg);

			GuildDBId guildDBId = _enemyData->_guildDBId;
			popup->SetData(_enemyData, textMsg, false, [guildDBId](bool bIsOk)
				{
					if (bIsOk)
					{
						FGsNetSendServiceGuild::SendHostileDelete(guildDBId);
					}
				});
		}
	}
}
