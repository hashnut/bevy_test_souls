#include "GsUIGuildSnsUrlListEntry.h"

#include "../Plugins/Experimental/CommonUI/Source/CommonUI/Public/CommonTextBlock.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "DataSchema/Guild/GsSchemaGuildSnsShortcutUrl.h"
#include "GsUIGuildSnsIcon.h"
#include "Management/GsScopeGlobal.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "T1Project.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

UGsUIGuildSnsUrlListItem* UGsUIGuildSnsUrlListItem::CreateInstance(const FString& InSnsUrl)
{
	const FGsGuildManager* GuildManager = GGuild();
	if (nullptr == GuildManager)
	{
		return nullptr;
	}

	const FGsSchemaGuildSnsShortcutUrl* SnsShortcutUrl = GuildManager->FindSnsLinkInfo(InSnsUrl);

	UGsUIGuildSnsUrlListItem* ListItem = NewObject<UGsUIGuildSnsUrlListItem>();

	ListItem->_index = -1;
	ListItem->_url.Empty();

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

void UGsUIGuildSnsUrlListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btn->OnClicked.AddUniqueDynamic(this, &UGsUIGuildSnsUrlListEntry::OnClick);
}

void UGsUIGuildSnsUrlListEntry::NativeOnListItemObjectSet(UObject* InListItemObject)
{
	Invalidate(InListItemObject);
}

void UGsUIGuildSnsUrlListEntry::Invalidate(const UObject* InListItemObject)
{
	const UGsUIGuildSnsUrlListItem* ListItem = Cast<UGsUIGuildSnsUrlListItem>(InListItemObject);
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	_icon->SetParameter({ ListItem->_iconIndex, !ListItem->_url.IsEmpty() });

	if (ListItem->_url.IsEmpty())
	{
		_widgetSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		_linkTxt->SetText(FText::FromString(ListItem->_url));
		_widgetSwitcher->SetActiveWidgetIndex(1);
	}

	_btn->SetIsEnabled(ListItem->_enableButton);
}

void UGsUIGuildSnsUrlListEntry::OnClick()
{
	const UGsUIGuildSnsUrlListItem* ListItem = GetListItem<UGsUIGuildSnsUrlListItem>();
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	ListItem->_onClick.ExecuteIfBound(ListItem->_index);
}
