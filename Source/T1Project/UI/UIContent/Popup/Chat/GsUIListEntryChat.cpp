#include "GsUIListEntryChat.h"
#include "GsUIListDataChat.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "RichTextBlock.h"
#include "WidgetSwitcher.h"
#include "PanelWidget.h"
#include "Ranking/GsRankingHelper.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"

void UGsUIListEntryChat::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnNickName->OnClicked.AddDynamic(this, &UGsUIListEntryChat::OnClickNickName);
}

void UGsUIListEntryChat::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	_data = nullptr;
	if (UGsUIListDataChat* item = Cast< UGsUIListDataChat>(ListItemObject))
	{
		_data = item;

		auto CheckChatMenu = [](EGsExChatType chatType, FString nick)->bool {
			auto typefault = chatType == EGsExChatType::EMOTICON || chatType == EGsExChatType::OPERATOR || chatType == EGsExChatType::SYSTEM;
			return !typefault && !nick.IsEmpty();
		};

		EGsExChatType chatType = item->GetChatType();
		GuildEmblemId emblemId = chatType == EGsExChatType::GROUP ? item->GetGuildEmblemId() : INVALID_GUILD_EMBLEM_ID;

		_btnNickName->SetIsEnabled(CheckChatMenu(chatType, item->GetNickName()));

		_nickName->SetText(FText::FromString(item->GetMarkupName()));
		_nickName->SetVisibility(ESlateVisibility::Hidden);
		_selectInvasionMark->SetActiveWidgetIndex(item->IsMyWorldPlayer() ? 1 : 0);
		_invasionInfo->SetVisibility(chatType != EGsExChatType::INVADE ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

		_guildMark->SetVisibility(emblemId != INVALID_GUILD_EMBLEM_ID ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		_badgeInfo->SetVisibility(ESlateVisibility::Collapsed);

		if (!item->IsGM())
		{
			if (const FSoftObjectPath* path = FGsRankingHelper::GetBadgeIconPath(item->GetChatRank()))
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
			if (const FSoftObjectPath* path = FGsUIHelper::GetBadgeIconPath(AccountGrade::GM))
			{
				if (UObject* loadObject = UAssetManager::GetStreamableManager().LoadSynchronous(*path))
				{
					_badgeInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					_badge.SetResourceObject(loadObject);
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

		FText viewText = FText::FromString(item->GetMarkupName() + item->GetMarkupText() + item->GetMarkupTime());
		_chatText->SetText(viewText);
		_passTime = static_cast<int>(item->GetCheckRemainTime() / CHAT_UPDATE_TIME);
	}
	else _passTime = 0.f;
	
	Super::NativeOnListItemObjectSet(ListItemObject);
	
}

void UGsUIListEntryChat::NativeOnItemSelectionChanged(bool bIsSelected)
{
	//IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

void UGsUIListEntryChat::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//_passTime -= InDeltaTime;
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);
	if (UGsUIListDataChat* data = Cast< UGsUIListDataChat>(GetListItem()))
	{
		auto passTime = static_cast<int>(data->GetCheckRemainTime() / CHAT_UPDATE_TIME);
		if (passTime != _passTime)
		{
			_passTime = passTime;
			FText viewText = FText::FromString(data->GetMarkupName() + data->GetMarkupText() + data->GetMarkupTime());
			_chatText->SetText(viewText);
		}
	}
}

void UGsUIListEntryChat::SetData(UObject* ListItemObject)
{
	NativeOnListItemObjectSet(ListItemObject);
}

void UGsUIListEntryChat::OnClickNickName()
{
	if (UGsUIListDataChat* data = Cast< UGsUIListDataChat>(GetListItem()))
	{
		if (_cbSelection)
		{
			_cbSelection(data);
		}
	}
}

//float UGsUIListEntryChat::ComputeDesiredSize(FText text) const
//{
//	auto MinDesiredWidth = _chatText->GetDesiredSize();
//	const FVector2D LocalShadowOffset = _chatText->ShadowOffset;
//	const float LocalOutlineSize = _chatText->Font.OutlineSettings.OutlineSize;
//
//	const FVector2D ComputedOutlineSize(LocalOutlineSize * 2, LocalOutlineSize);
//	if (FSlateRenderer* renderer = FSlateApplication::Get().GetRenderer())
//	{
//		const FVector2D TextSize = renderer->GetFontMeasureService()->Measure(text, _talkText->Font) + ComputedOutlineSize + LocalShadowOffset;
//		return TextSize.X;
//	}
//	return MinDesiredWidth.X;
//}