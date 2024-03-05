#include "GsUIChatItem.h"
#include "RichTextBlock.h"
#include "WidgetSwitcher.h"
#include "PanelWidget.h"
#include "Ranking/GsRankingHelper.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"

void UGsUIChatItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIChatItem::SetChatData(TWeakPtr<FGsChatItem> item)
{
	if(auto data = item.Pin())
	{
		auto markupPrefix = data->GetMarkupName();
		GuildEmblemId emblemId = data->_type == EGsExChatType::GROUP ? data->GetGuildEmblemId() : INVALID_GUILD_EMBLEM_ID;

		AddChatText(markupPrefix + data->_markupText);

		_selectInvasionMark->SetActiveWidgetIndex(data->_isMyWorldPlayer ? 1 : 0);
		_invasionInfo->SetVisibility(data->_type != EGsExChatType::INVADE ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

		_guildMark->SetVisibility(emblemId != INVALID_GUILD_EMBLEM_ID ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		_badgeInfo->SetVisibility(ESlateVisibility::Collapsed);

		if (data->GetAccountGrade() == AccountGrade::GM)
		{
			if (const FSoftObjectPath* path = FGsUIHelper::GetBadgeIconPath(AccountGrade::GM))
			{
				if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(*path))
				{
					_badgeInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					_badge.SetResourceObject(loadObject);
				}
			}
			
		}
		else
		{
			if (data->GetAccountGrade() == AccountGrade::NORMAL)
			{
				if (const FSoftObjectPath* path = FGsRankingHelper::GetBadgeIconPath(data->GetUserRank()))
				{
					if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(*path))
					{
						_badgeInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						_badge.SetResourceObject(loadObject);
					}
				}
			}
		}

		if (emblemId != INVALID_GUILD_EMBLEM_ID)
		{
			if (const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(emblemId))
			{
				if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(emblemData->iconPathBig))
				{
					_emblem.SetResourceObject(loadObject);
				}
			}
		}
	}
}

void UGsUIChatItem::AddChatText(FString chatText)
{
	FText viewText = FText::FromString(chatText);
	_chatText->SetText(viewText);

	_invasionInfo->SetVisibility(ESlateVisibility::Collapsed);
	_badgeInfo->SetVisibility(ESlateVisibility::Collapsed);
	_guildMark->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIChatItem::ClearChatText()
{
	//_textBlock->Set
}