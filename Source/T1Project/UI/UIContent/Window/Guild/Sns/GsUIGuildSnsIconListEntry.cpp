#include "GsUIGuildSnsIconListEntry.h"

#include "DataSchema/Guild/GsSchemaGuildSnsShortcutPlatform.h"
#include "DataSchema/Guild/GsSchemaGuildSnsShortcutUrl.h"
#include "GsUIGuildSnsIcon.h"
#include "Management/GsScopeGlobal.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "T1Project.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

UGsUIGuildSnsIconListItem* UGsUIGuildSnsIconListItem::CreateInstance(const FString& InSnsUrl)
{
	const FGsGuildManager* GuildManager = GGuild();
	if (nullptr == GuildManager)
	{
		return nullptr;
	}

	const FGsSchemaGuildSnsShortcutUrl* SnsShortcutUrl = GuildManager->FindSnsLinkInfo(InSnsUrl);

	UGsUIGuildSnsIconListItem* ListItem = NewObject<UGsUIGuildSnsIconListItem>();

	ListItem->_index = -1;
	ListItem->_hasUrl = false;

	if (nullptr == SnsShortcutUrl || nullptr == SnsShortcutUrl->platform.GetRow())
	{
		ListItem->_iconIndex = GUILD_SNS_ICON_UNKNOWN_INDEX;
	}
	else
	{
		const FGsSchemaGuildSnsShortcutPlatform* SnsShortcutPlatform = SnsShortcutUrl->platform.GetRow();
		ListItem->_iconIndex = SnsShortcutPlatform->iconIndex;
	}

	return ListItem;
}

void UGsUIGuildSnsIconListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btn->OnClicked.AddUniqueDynamic(this, &UGsUIGuildSnsIconListEntry::OnClickIcon);
}

void UGsUIGuildSnsIconListEntry::NativeOnListItemObjectSet(UObject* InListItemObject)
{
	Invalidate(InListItemObject);
}

void UGsUIGuildSnsIconListEntry::Invalidate(const UObject* InListItemObject)
{
	const UGsUIGuildSnsIconListItem* ListItem = Cast<UGsUIGuildSnsIconListItem>(InListItemObject);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	_icon->SetParameter({ ListItem->_iconIndex, ListItem->_hasUrl });
}

void UGsUIGuildSnsIconListEntry::OnClickIcon()
{
	const UGsUIGuildSnsIconListItem* ListItem = GetListItem<UGsUIGuildSnsIconListItem>();
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	ListItem->_onClickIcon.ExecuteIfBound(ListItem->_index);
}
