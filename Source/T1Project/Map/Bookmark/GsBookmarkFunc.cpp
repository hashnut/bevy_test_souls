#include "GsBookmarkFunc.h"

#include "UTIL/GsText.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "Map/GsMapFunc.h"

#include "Shared/Client/SharedEnums/SharedBookmarkEnum.h"

// 위치 즐겨찾기 글자 크기 체크
bool FGsBookmarkFunc::CheckBookmarkNameLength(const FString& In_name)
{
	int convertLen = FGsTextUtil::GetStringLengthRegex(In_name);

	// 최대 자수 넘음
	if (convertLen > GData()->GetGlobalData()->_bookmarkNameLenMax)
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorCreateNameMax"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}
	// 최소 자수 보다 적거나 같음
	else if (convertLen < GData()->GetGlobalData()->_bookmarkNameLenMin)
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorCreateNameMin"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	return true;
}


// 북마크 스크롤 아이템 갯수 구하기
int FGsBookmarkFunc::GetBookmarkScrollItemCount()
{
	TArray<TWeakPtr<FGsItem>> findItems =
		GItem()->FindItem(ItemType::CONSUME, ItemCategorySub::WARP_TO_BOOKMARK_SCROLL);

	int totalCount = 0;
	if (findItems.Num() != 0)
	{
		for (const TWeakPtr<FGsItem> iter : findItems)
		{
			if (false == iter.IsValid())
			{
				continue;
			}
			totalCount += iter.Pin()->GetAmount();
		}
	}

	return totalCount;
}

// 즐겨찾기 아이템 있는지(없으면 티커 출력)
bool FGsBookmarkFunc::IsBookmarkScrollExist(uint64& Out_itemDBId, OUT ItemId& OutItemId)
{
	TArray<TWeakPtr<FGsItem>> findItems =
		GItem()->FindItem(ItemType::CONSUME, ItemCategorySub::WARP_TO_BOOKMARK_SCROLL);

	if (findItems.Num() == 0 ||
		findItems[0] == nullptr)
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorTeleportLackItem"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	if (findItems[0].IsValid())
	{
		TSharedPtr<FGsItem> Item = findItems[0].Pin();
		Out_itemDBId = Item->GetDBID();
		OutItemId = Item->GetTID();
	}

	return true;
}

// 공유 응답 패킷 result 티커 출력
bool FGsBookmarkFunc::ShowTickerBookmarkShareError(PD::Result In_result)
{
	switch (In_result)
	{
		// 파티장만 공유가 가능합니다
	case PD::Result::PARTY_ERROR_NOT_LEADER:
	case PD::Result::PARTY_ERROR_NOT_JOINED:
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorSharingPartyMaster"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return true;
	}
	
	case PD::Result::BOOKMARK_ERROR_NO_SHARE_RIGHT:
	{
		FText findText;
		if (GSBookmark()->GetCurrentShareType() == BookmarkShareType::GUILD)
		{
			// 기사단 부단장 이상만 공유가 가능합니다
			FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorSharingGuildMaster"), findText);
		}
		else
		{
			// 파티장만 공유가 가능합니다
			FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorSharingPartyMaster"), findText);
		}
		FGsUIHelper::TrayMessageTicker(findText);
		return true;
	}
	// 나를 제외한 기사단원이 1명 이상 존재해야 합니다.
	case PD::Result::BOOKMARK_ERROR_NO_GUILD_MEMBERS:
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorSharingPersonGuild"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return true;
	}
	// 나를 제외한 기사단원이 1명 이상 존재해야 합니다.
	case PD::Result::BOOKMARK_ERROR_NO_PARTY_MEMBERS:
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorSharingPersonParty"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return true;
	}
	// 공유는 60초에 한 번만 가능합니다.
	case PD::Result::BOOKMARK_ERROR_NOT_SHARE_TIME:
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorSharingCoolTime"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return true;
	}
	}

	return false;
}

// 공유 이동 응답 패킷 result 티커 출력
bool FGsBookmarkFunc::ShowTickerBookmarkShareWarpError(PD::Result In_result)
{
	switch (In_result)
	{
		// 해당 채널에 유저가 많아서 이동할 수 없습니다.
	case PD::Result::SELECT_CHANNEL_ERROR_CONGESTION_FULL:
	{
		FText findText;
		FText::FindText(TEXT("UICommonText"), TEXT("ChannelChangeDeny"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return true;
	}
	}

	return false;
}

// 북마크 공유 성공 티커 출력
void FGsBookmarkFunc::ShowTickerBookmarkShareSuccess()
{
	FText findText;
	FText::FindText(TEXT("UserTeleportText"), TEXT("NoticeShareCompleted"), findText);
	FGsUIHelper::TrayMessageTicker(findText);
}