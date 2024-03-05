// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRankingPersonalListEntry.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"

#include "DataSchema/Ranking/GsSchemaRankingTypeObjective.h"

#include "Guild/GsGuildData.h"
#include "Guild/GsGuildHelper.h"

#include "Management/ScopeGame/GsRankingManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Reward/GsRewardHelper.h"

#include "Ranking/GsRankingHelper.h"

#include "UI/UIContent/Window/Ranking/GsRankingListItem.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupChatUserInfo.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "PaperSprite.h"


void UGsUIRankingPersonalListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnUserName->OnClicked.AddDynamic(this, &UGsUIRankingPersonalListEntry::OnClickUserName);
}

void UGsUIRankingPersonalListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_textBlockName->SetText(FText::GetEmpty());
	_textBlockGuildOrServer->SetText(FText::GetEmpty());
	_textBlockValue->SetText(FText::GetEmpty());
	_textBlockPercent->SetText(FText::GetEmpty());
}

void UGsUIRankingPersonalListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	UGsRankingListItem* listItem = Cast<UGsRankingListItem>(ListItemObject);
	if (nullptr == listItem)
		return;

	OnClickListItem = listItem->OnClickListItem;
}

void UGsUIRankingPersonalListEntry::SetData(const FGsRankingData* InRankingData)
{
	Super::SetData(InRankingData);

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	const FGsSchemaRankingTypeObjective* subTabData = rankingManager->GetCurrentSubTabData();
	if (nullptr == subTabData)
		return;

	if (nullptr == InRankingData)
		return;

	// 표시한 랭킹에 배지 정보가 존재하는가?
	const FSoftObjectPath* tempBadge = FGsRankingHelper::GetBadgeIconPath(1, subTabData->rankingType);
	ESlateVisibility visibility = (nullptr != tempBadge && tempBadge->IsValid()) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	_spacerBadge->SetVisibility(visibility);
	_boxBadge->SetVisibility(visibility);

	// 내 랭킹에 배지 정보가 존재하는가?
	const FSoftObjectPath* badgeIconPath = FGsRankingHelper::GetBadgeIconPath(InRankingData->_ranking, subTabData->rankingType);
	if (nullptr == badgeIconPath)
	{
		_switcherBadge->SetActiveWidgetIndex(1);
	}
	else
	{
		_switcherBadge->SetActiveWidgetIndex(1);
		TWeakObjectPtr<UImage> weakThis(_iconBadge);
		TWeakObjectPtr<UWidgetSwitcher> weakSwitcher(_switcherBadge);
		FGsUIHelper::AsyncLoadSpriteFromPath(*badgeIconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, weakSwitcher](UObject* inRes)
			{
				if (UImage* strongThis = weakThis.Get())
				{
					if (UPaperSprite* badgeTexture = Cast<UPaperSprite>(inRes))
					{
						strongThis->SetBrushFromAtlasInterface(badgeTexture);
					}
				}

				if (UWidgetSwitcher* strongSwitcher = weakSwitcher.Get())
				{
					strongSwitcher->SetActiveWidgetIndex(0);
				}
			}));
	}

	EGsRankingDataType rankingDataType = rankingManager->GetRankingDataType(subTabData->rankingType);
	if (EGsRankingDataType::Local == rankingDataType)
	{
		SetLocalData(InRankingData);
	}
	else if (EGsRankingDataType::World == rankingDataType)
	{
		SetWorldData(InRankingData);
	}

	// 이전 순위 설정이 필요한가?
	if (rankingManager->IsLevelDataType(subTabData->rankingType))
	{
		if ((int64)InRankingData->_previousRank > (int64)subTabData->maxRankCount || InRankingData->_previousRank == 0)
		{
			_switcherValue->SetActiveWidgetIndex(1);
		}
		else
		{
			_switcherValue->SetActiveWidgetIndex(0);
			SetValue(FText::AsNumber(InRankingData->_previousRank));
		}
	}
}

void UGsUIRankingPersonalListEntry::SetData(FGsRankingMyData* InMyRanking)
{
	Super::SetData(InMyRanking);

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	const FGsSchemaRankingTypeObjective* subTabData = rankingManager->GetCurrentSubTabData();
	if (nullptr == subTabData)
		return;

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	// 캐릭터 이름 및 클릭 여부 설정
	_userName = userData->mUserName;
	_btnUserName->SetVisibility(ESlateVisibility::Collapsed);
	_textBlockName->SetText(FText::FromString(userData->mUserName));

	// 기사단(서버)이름 및 아이콘 설정
	EGsRankingDataType rankingDataType = rankingManager->GetRankingDataType(subTabData->rankingType);
	switch (rankingDataType)
	{
	case EGsRankingDataType::Local:
	{
		const FGsGuildDataDetail* guildData = GGuild()->GetGuildDetailData(true);
		if (true == guildData->IsExistGuild())
		{
			ShowMainText(FText::FromString(guildData->_guildName));
			SetEmblem(_iconEmblem, guildData->_emblemId);
		}
		else
		{
			HideMainText();
		}
		break;
	}
	case EGsRankingDataType::World:
	{
		_iconEmblem->SetVisibility(ESlateVisibility::Collapsed);
		ShowMainText(FText::FromString(GNet()->GetSelectPlanetWorldName()));
		break;
	}
	}

	// 표시한 랭킹에 배지 정보가 존재하는가?
	const FSoftObjectPath* tempBadge = FGsRankingHelper::GetBadgeIconPath(1, subTabData->rankingType);
	ESlateVisibility visibility = (nullptr != tempBadge && tempBadge->IsValid()) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	_spacerBadge->SetVisibility(visibility);
	_boxBadge->SetVisibility(visibility);

	if (nullptr == InMyRanking)
	{
		_switcherValue->SetActiveWidgetIndex(1);
		_switcherBadge->SetActiveWidgetIndex(1);
		return;
	}

	// 내 랭킹에 배지 정보가 존재하는가?
	if (InMyRanking->_hasHighRanking)
	{
		const FSoftObjectPath* badgeIconPath = FGsRankingHelper::GetBadgeIconPath(InMyRanking->_highRanking, subTabData->rankingType);
		if (nullptr == badgeIconPath)
		{
			_switcherBadge->SetActiveWidgetIndex(1);
		}
		else
		{
			_switcherBadge->SetActiveWidgetIndex(1);
			TWeakObjectPtr<UImage> weakThis(_iconBadge);
			TWeakObjectPtr<UWidgetSwitcher> weakSwitcher(_switcherBadge);
			FGsUIHelper::AsyncLoadSpriteFromPath(*badgeIconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, weakSwitcher](UObject* inRes)
				{
					if (UImage* strongThis = weakThis.Get())
					{
						if (UPaperSprite* badgeTexture = Cast<UPaperSprite>(inRes))
						{
							strongThis->SetBrushFromAtlasInterface(badgeTexture);
						}
					}

					if (UWidgetSwitcher* strongSwitcher = weakSwitcher.Get())
					{
						strongSwitcher->SetActiveWidgetIndex(0);
					}
				}));
		}
	}
	else
	{
		_switcherBadge->SetActiveWidgetIndex(1);
	}

	// 이전 순위 or 값 설정
	if (rankingManager->IsLevelDataType(subTabData->rankingType))
	{
		if (false == InMyRanking->_hasHighRanking || (int64)InMyRanking->_previousRank > (int64)subTabData->maxRankCount || InMyRanking->_previousRank == 0)
		{
			_switcherValue->SetActiveWidgetIndex(1);
		}
		else
		{
			_switcherValue->SetActiveWidgetIndex(0);
			SetValue(FText::AsNumber(InMyRanking->_previousRank));
		}
	}
	else
	{
		_switcherValue->SetActiveWidgetIndex(0);
		SetValue(FText::AsNumber(InMyRanking->_param1));
	}
}

void UGsUIRankingPersonalListEntry::SetLocalData(const FGsRankingData* InRankingData)
{
	const FGsRankingDetailPersonal* rankingDetailData = static_cast<FGsRankingDetailPersonal*>(InRankingData->_detailData.Get());
	if (nullptr == rankingDetailData)
		return;

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	// 캐릭터 이름 및 클릭 여부 설정
	_userName = rankingDetailData->_userName;
	_textBlockName->SetText(FText::FromString(_userName));
	_btnUserName->SetVisibility((userData->mHomeWorldId == rankingDetailData->_worldId) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// 기사단 설정
	if (INVALID_GUILD_EMBLEM_ID != rankingDetailData->_guildEmblemId)
	{
		ShowMainText(FText::FromString(rankingDetailData->_guildName));
		SetEmblem(_iconEmblem, rankingDetailData->_guildEmblemId);
	}
	else
	{
		HideMainText();
	}

	// 값 설정
	_switcherValue->SetActiveWidgetIndex(0);
	SetValue(FText::AsNumber(rankingDetailData->_param1));
}

void UGsUIRankingPersonalListEntry::SetWorldData(const FGsRankingData* InRankingData)
{
	const FGsRankingDetailWorld* rankingDetailData = static_cast<FGsRankingDetailWorld*>(InRankingData->_detailData.Get());
	if (nullptr == rankingDetailData)
		return;

	// 캐릭터 이름 및 클릭 여부 설정
	_userName = rankingDetailData->_userName;
	_textBlockName->SetText(FText::FromString(_userName));
	_btnUserName->SetVisibility(ESlateVisibility::Collapsed);

	// 서버 이름 설정
	FString serverName;
	GNet()->TryGetServerName(rankingDetailData->_worldId, serverName);

	ShowMainText(FText::FromString(serverName));
	_iconEmblem->SetVisibility(ESlateVisibility::Collapsed);

	// 값 설정
	_switcherValue->SetActiveWidgetIndex(0);
	SetValue(FText::AsNumber(rankingDetailData->_param1));
}

void UGsUIRankingPersonalListEntry::ShowMainText(const FText& InText)
{
	_switcherGuild->SetActiveWidgetIndex(0);
	_textBlockGuildOrServer->SetText(InText);
}

void UGsUIRankingPersonalListEntry::HideMainText()
{
	_switcherGuild->SetActiveWidgetIndex(1);
}

void UGsUIRankingPersonalListEntry::SetLevel(Level InLevel, Exp InExp)
{
	// 이전 구현상 InExp는 누적값이 아닌, 현재 레벨 Exp 값으로 그에 맞게 처리함
	float percentExp = FGsRewardHelper::GetExpRate(InLevel, InExp, false);
	SetLevel(InLevel, percentExp);
}

void UGsUIRankingPersonalListEntry::SetLevel(Level InLevel, float InPercentExp)
{
	_textBlockValue->SetText(FText::AsNumber(InLevel));
	_textBlockPercent->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FNumberFormattingOptions option;
	option.SetMaximumFractionalDigits(0);
	option.SetMinimumFractionalDigits(1);

	FText findText;
	FText::FindText(TEXT("RankingCommonUIText"), TEXT("RankingEXP"), findText);
	FText expText = FText::Format(findText, FText::AsPercent(InPercentExp, &option));
	_textBlockPercent->SetText(expText);
}

void UGsUIRankingPersonalListEntry::SetValue(const FText& InText)
{
	_textBlockValue->SetText(InText);
	_textBlockPercent->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIRankingPersonalListEntry::OnClickUserName()
{
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		if (userData->mUserName.Compare(_userName) == 0)
		{
			return;
		}
	}

	OnClickListItem(this);

	TWeakObjectPtr<UGsUIPopupChatUserInfo> contextPopup = Cast<UGsUIPopupChatUserInfo>(GUI()->OpenAndGetWidget(TEXT("PopupChatUserInfo")));
	if (contextPopup.IsValid())
	{
		// 2023/9/13 PKT - 이건??
		UGsUIPopupChatUserInfo::PopupInitData initData{ 0, _userName, TEXT(""), true, EGsUserContentsOwner::RANKING };
		contextPopup->SetData(&initData, true);
		contextPopup->SetPosition(_btnUserName);
	}
}