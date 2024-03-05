// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBookmarkStatListViewEntry.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"
#include "GameObject/Stat/GsStatHelper.h"

#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"


void UGsBookmarkStatListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_bookmarkToggleButton->OnClicked.AddDynamic(this, &UGsBookmarkStatListViewEntry::OnClickBookmark);
}

void UGsBookmarkStatListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(TEXT("StatText"), TEXT("UI_Stat_Bookmark_Excess"), _maxBookmarkStatOverflowText);

	BindMessage();
}

void UGsBookmarkStatListViewEntry::NativeDestruct()
{
	UnbindMessage();

	Super::NativeDestruct();
}

void UGsBookmarkStatListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	if (nullptr == inListItemObject)
	{
		return;
	}

	UGsBookmarkStatListViewData* data = Cast<UGsBookmarkStatListViewData>(inListItemObject);
	if (nullptr == data)
	{
		return;
	}

	_isLine = data->_isLine;
	_statType = data->_statType;
	_bookmarkStatControl = FGsPlayerStatHandler::GetInstance()->GetBookmarkStatController();

	InvalidWidget();
}

void UGsBookmarkStatListViewEntry::InvalidWidget()
{
	if (_isLine)
	{
		_lineWidgetSwitcher->SetActiveWidgetIndex(1);
	}
	else
	{
		_lineWidgetSwitcher->SetActiveWidgetIndex(0);

		InvalidFavorit();
		InvalidStatName();
		InvalidStatValue();
	}
}

void UGsBookmarkStatListViewEntry::InvalidFavorit()
{
	bool isBookmark = _bookmarkStatControl->IsBookmarkStat(_statType);
	_bookmarkToggleButton->SetIsSelectedNoEvent(isBookmark);
}

void UGsBookmarkStatListViewEntry::InvalidStatName()
{
	FText statNameText;
	FGsStatHelper::GetDisPlayStatName(_statType, statNameText);	
	_statNameText->SetText(statNameText);
}

void UGsBookmarkStatListViewEntry::InvalidStatValue()
{
	FText statValueText;
	FGsPlayerStatHandler::GetInstance()->TryGetPlayerStatText(_statType, statValueText);
	_statValueText->SetText(statValueText);
}

void UGsBookmarkStatListViewEntry::OnInvalidWidget()
{
	InvalidWidget();
}

void UGsBookmarkStatListViewEntry::OnPlayerStatUpdate(const struct IGsMessageParam* inParam)
{
	const FGsGameObjectMessageParamPlayerStat* param = inParam->Cast<const FGsGameObjectMessageParamPlayerStat>();
	if (nullptr == param)
		return;

	for (const TPair<StatType, int32>& stat : param->StatMap)
	{
		if (stat.Key == _statType)
		{
			InvalidWidget();
			return;
		}
	}
}

void UGsBookmarkStatListViewEntry::OnClickBookmark()
{
	bool isBookmarkStat = _bookmarkStatControl->IsBookmarkStat(_statType);
	if (isBookmarkStat)
	{
		_bookmarkStatControl->DeleteStatBookmark(_statType);
	}
	else
	{
		if (false == _bookmarkStatControl->AddStatBookmark(_statType))
		{
			//ticker
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, _maxBookmarkStatOverflowText);
		}		
	}
}

void UGsBookmarkStatListViewEntry::BindMessage()
{
	//bind message
	_hudMessageHandlerArray.Empty();
	_hudMessageHandlerArray.Emplace(GMessage()->GetContentsHud().AddUObject(MessageContentHud::INVALIDATE_STAT
		, this
		, &UGsBookmarkStatListViewEntry::OnInvalidWidget));

	_listEventDelegate.Empty();
	_listEventDelegate.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::PLAYER_STAT_UPDATE,
		this, &UGsBookmarkStatListViewEntry::OnPlayerStatUpdate));
}

void UGsBookmarkStatListViewEntry::UnbindMessage()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (messageHolder)
	{
		for (const MsgHudHandle& pair : _hudMessageHandlerArray)
		{
			messageHolder->GetContentsHud().Remove(pair);
		}
		_hudMessageHandlerArray.Empty();

		for (MsgGameObjHandle& elemItem : _listEventDelegate)
		{
			messageHolder->GetGameObject().Remove(elemItem);
		}
		_listEventDelegate.Empty();
	}
}