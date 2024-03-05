#include "GsUIBookmarkItem.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupBookmark.h"
#include "UI/UIContent/Popup/GsUIPopupWarp.h"
#include "UI/UIContent/Popup/GsUIPopupBookmark.h"

#include "Map/Bookmark/GsBookmarkDefine.h"
#include "Map/Bookmark/GsBookmarkFunc.h"
#include "Map/GsMapFunc.h"
#include "Map/WorldMap/GsWorldMapFunc.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Components/Image.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"

#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Client/SharedEnums/SharedBookmarkEnum.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Bookmark/GsBookmarkHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "Net/GsNetSendServiceWorld.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "T1Project.h"

void UGsUIBookmarkItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnShare->OnClicked.AddDynamic(this, &UGsUIBookmarkItem::OnClickShare);
	_btnMove->OnClicked.AddDynamic(this, &UGsUIBookmarkItem::OnClickMove);	
	_btnItem->OnClicked.AddDynamic(this, &UGsUIBookmarkItem::OnClickItem);
	_btnAutoMove->OnClicked.AddDynamic(this, &UGsUIBookmarkItem::OnClickAutoMove);

	_btnShareParty->OnClicked.AddDynamic(this, &UGsUIBookmarkItem::OnClickShareParty);
	_btnShareGuild->OnClicked.AddDynamic(this, &UGsUIBookmarkItem::OnClickShareGuild);
	_btnShareMenuClose->OnClicked.AddDynamic(this, &UGsUIBookmarkItem::OnClickShareMenuClose);
}


void UGsUIBookmarkItem::OnClickShare()
{
	// 쿨타임 체크
	if (GSBookmark()->GetIsShareCoolTime() == true)
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorSharingCoolTime"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	SetShareMenuIndex(_itemIndex);
}
void UGsUIBookmarkItem::OnClickMove()
{
	// 개인 이벤트 채널에서의 이동을 막음
	if (GLevel()->IsInstanceSingleMap())
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorUseInvalidArea"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	// local이  상태이상이면 못보냄
	if (true == FGsMapFunc::IsLocalWarpImpossible())
	{
		return;
	}

	int selectIndex = _indexInGroup;

	TSharedRef<FGsCostPackage> CostPackage(MakeShared<FGsCostPackage>());
	CostPackage->AddCost(ItemCategorySub::WARP_TO_BOOKMARK_SCROLL, 1);

	GSWorldMap()->ReserveWarpBookmarkItem(
		_textName.ToString(),
		MoveTemp(CostPackage),
		_indexInGroup);

}

// 데이터 세팅
void UGsUIBookmarkItem::SetData(bool In_isShowSharedMenu, 
	int In_itemIndex,
	uint64 In_bookmarkId, FString In_name)
{
	_bookmarkId = In_bookmarkId;
	_textName = FText::FromString(In_name);
	_itemIndex = In_itemIndex;

	// 0. 일반 메뉴
	// 1. 공유 메뉴
	int switchIndex = (In_isShowSharedMenu == true) ? 1 : 0;
	_switcherType->SetActiveWidgetIndex(switchIndex);
}

void UGsUIBookmarkItem::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	if (_imgSelected == nullptr)
	{
		return;
	}

	_imgSelected->SetVisibility((bInIsSelected)? ESlateVisibility::SelfHitTestInvisible: ESlateVisibility::Hidden);
}

bool UGsUIBookmarkItem::GetIsSelected() const
{
	return _bIsSelected;
}
// 아이템 클릭
void UGsUIBookmarkItem::OnClickItem()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		// 이미 선택이 되어있으면
		if (_bIsSelected == true)
		{
			// 해제(-1이 선택되게 처리)
			_toggleCallback(-1, true);
		}
		else
		{
			_toggleCallback(_indexInGroup, !_bIsSelected);
		}
	}
}

// 파티 공유 클릭
void UGsUIBookmarkItem::OnClickShareParty()
{
	CloseShareMenu();

	// 파티장인지 체크후 아니면 노티
	if (false == GSParty()->GetIsInParty() || 
		false == GSParty()->AmILeader())
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorSharingPartyMaster"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	// 나 외 1명 이상 있어야 함
	if (2 > GSParty()->GetPartyMemberCount())
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorSharingPersonParty"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	GSBookmark()->SetCurrentShareType(BookmarkShareType::PARTY);
	// 서버 요청
	FGsNetSendServiceWorld::SendBookmarkShare(BookmarkShareType::PARTY, _bookmarkId);
}
// 길드 공유 클릭
void UGsUIBookmarkItem::OnClickShareGuild()
{
	CloseShareMenu();
	// 길드 간부가 아닌지 체크후 아니면 노티
	bool isShareGuildPossible = false;
	if (GGuild()->IsGuild() == true)
	{
		GuildMemberGradeType type = GGuild()->GetMemeberGrade();
		// 부단장 이상만 됨
		if ((int)type >= (int)GuildMemberGradeType::SUBCAPTAIN)
		{
			isShareGuildPossible = true;
		}
	}
	if (isShareGuildPossible == false)
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorSharingGuildMaster"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	// 나외 1명 이상 있어야함(요건 현재 길드 인원수 정보를 특정 상태에서만 요청해서 받으므로
	// 클라에서 체크할수 없음)->서버 에러값으로 출력해야함
	
	GSBookmark()->SetCurrentShareType(BookmarkShareType::GUILD);
	// 서버 요청
	FGsNetSendServiceWorld::SendBookmarkShare(BookmarkShareType::GUILD, _bookmarkId);
}
// 공유 메뉴 닫기 클릭
void UGsUIBookmarkItem::OnClickShareMenuClose()
{
	CloseShareMenu();
}

// 공유 메뉴 닫기
void UGsUIBookmarkItem::CloseShareMenu()
{
	SetShareMenuIndex(-1);
}
// 공유 메뉴 인덱스 세팅
void UGsUIBookmarkItem::SetShareMenuIndex(int In_index)
{
	TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->GetWidgetByKey(TEXT("PopupBookmark"));

	if (wigdet.IsValid() == false)
	{
		return;
	}
	TWeakObjectPtr<UGsUIPopupBookmark> popupBookmark = Cast<UGsUIPopupBookmark>(wigdet);
	popupBookmark->SetShareMenuItemIndex(In_index);
}

// 자동 이동 클릭
void UGsUIBookmarkItem::OnClickAutoMove()
{
	FGsWorldMapFunc::ClickAutoMoveBookMarkPopup( _indexInGroup);
}