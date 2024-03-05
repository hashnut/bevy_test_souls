// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildAuctionData.h"

#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Market/GsSchemaMarketCategoryData.h"

#include "GameFlow/GameContents/GsContentsMode.h"

#include "Guild/GsGuildHelper.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsTimeSyncUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildStorageComboBox.h"

#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/GsMessageHolder.h"

#include "Net/GsNetSendServiceGuild.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsGuildMessageParam.h"

#include "Currency/GsCurrencyHelper.h"


void FGsGuildAuctionHelper::GetRootText(const GuildStoreItemData& InData, OUT FText& OutRootText)
{	
	if (INVALID_CREATURE_ID != InData.mBossId)
	{
		if (const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(InData.mBossId))
		{
			bool bUseBossId = true;
			if (INVALID_MAP_ID != InData.mMapId)
			{	
				if (const FGsSchemaMapData* mapData =
					UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(InData.mMapId))
				{
					if (MapType::GUILD_DUNGEON == mapData->mapType)
					{
						// 기사단 던전은 NPC 이름 사용 안함
						bUseBossId = false;
					}
					else if (MapType::AGIT_DUNGEON_RAID == mapData->mapType)
					{					
						// TEXT: {0} 원정
						FText textFormat;
						if (FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Info_Raid_Acquisition"), textFormat))
						{
							OutRootText = FText::Format(textFormat, npcData->nameText);

							return;
						}
					}
				}
			}

			if (bUseBossId)
			{
				if (OutRootText.IsEmpty())
				{
					OutRootText = npcData->nameText;

					return;
				}
			}
		}
	}

	// Map Id 사용 - 기사단 던전
	if (INVALID_MAP_ID != InData.mMapId)
	{
		if (const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(InData.mMapId))
		{
			OutRootText = mapData->MapStringId;
		}
	}
	else
	{
		// 주의: 현재는 여기 들어오면 안된다고 함. mapId를 쓰는게 기사단 던전 밖에 없어서 가능한 로직
		// TEXT: 기사단 던전 클리어 보상
		FText::FindText(TEXT("GuildStorageAcquireText"), TEXT("GuildStorageAcquire_DUNJEON"), OutRootText);
	}
}

FGsGuildDivisionItem::FGsGuildDivisionItem(const GuildStoreItemData& InServerData)
{
	_serverData = InServerData;

	_distributedPrice = 0;
	_acquisitionUserDBId = INVALID_USER_DB_ID;
	_participantList.Empty();
	_resultUserList.Empty();

	if (const FGsSchemaItemCommon* itemCommon = FGsItem::GetTableDataByID(_serverData.mItemDataEquip.mTemplateId))
	{
		_marketCategory = SharedFunc::GetMarketCategory(itemCommon->categorySub);
	}
}

void FGsGuildDivisionItem::GetRootText(OUT FText& OutRootText) const
{
	FGsGuildAuctionHelper::GetRootText(_serverData, OutRootText);
}

bool FGsGuildDivisionItem::IsGuildDungeonItem() const
{
	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(_serverData.mMapId);
	if (mapData)
	{
		if (MapType::GUILD_DUNGEON == mapData->mapType)
		{
			return true;
		}		
	}

	return false;
}

FString FGsGuildDivisionItem::GetAcquisitionUserName() const
{
	FString outString;
	if (FGsGuildManager* guildManager = GGuild())
	{
		if (FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData())
		{
			outString = auctionData->GetUserName(_acquisitionUserDBId);
		}
	}

	if (outString.IsEmpty())
	{
		FText findText;
		const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(_serverData.mMapId);
		if (mapData)
		{
			if (MapType::GUILD_DUNGEON == mapData->mapType)
			{
				// TEXT: 기사단 던전
				FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Info_GuildDungeon"), findText);
			}
			else if (MapType::AGIT_DUNGEON_RAID == mapData->mapType)
			{
				// TEXT: 보스 원정
				FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Info_GuildRaidDungeon"), findText);
			}
		}

		outString = (false == findText.IsEmpty()) ? findText.ToString() : TEXT("-");
	}

	return outString;
}

bool FGsGuildDivisionItem::CanSelect() const
{
	// 분배 전 && 만료 시각을 넘지 않음
	if (GetDivisionState() == GuildDivisionState::STORAGE &&
		GetExpiredDateTime() >= FGsTimeSyncUtil::GetServerNowSecond())
	{
		return true;
	}
	
	return false;
}

void FGsGuildDivisionItem::UpdateDivisionItem(const GuildStoreItemData& InServerData)
{
	if (_serverData.mItemDataEquip.mItemDBId == InServerData.mItemDataEquip.mItemDBId)
	{
		_serverData = InServerData;
	}
}

void FGsGuildDivisionItem::SetWinningBidderUser(const FString& InUserName)
{
	_bidderUserName = InUserName;

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	_isMyItem = InUserName.Equals(userData->mUserName);
}

void FGsGuildDivisionItem::SetParticipantList(const TArray<UserDBId>& InUserList)
{
	_participantList.Empty();
	_participantList = InUserList;
}

void FGsGuildDivisionItem::SetResultList(const TArray<TPair<GuildMemberGradeType, UserDBId>>& InUserList)
{
	_resultUserList.Empty();
	_resultUserList = InUserList;
}

FGsGuildAuctionItem::FGsGuildAuctionItem(const GuildStoreItemData& InServerData)
{
	_serverData = InServerData;

	_expectedResultPrice = 0;
	_resultUserList.Empty();

	if (const FGsSchemaItemCommon* itemCommon = FGsItem::GetTableDataByID(_serverData.mItemDataEquip.mTemplateId))
	{
		_marketCategory = SharedFunc::GetMarketCategory(itemCommon->categorySub);
	}
}

void FGsGuildAuctionItem::GetRootText(OUT FText& OutRootText) const
{
	FGsGuildAuctionHelper::GetRootText(_serverData, OutRootText);
}

void FGsGuildAuctionItem::UpdateAuctionItem(const GuildStoreItemData& InServerData)
{
	if (GetAuctionId() == InServerData.mGuildAuctionId)
	{
		bool isAvailable = _serverData.mAvailable;
		GuildStoreAvailableResultType resultType = _serverData.mGuildStoreAvailableResultType;
		_serverData = InServerData;
		_serverData.mAvailable = isAvailable;
		_serverData.mGuildStoreAvailableResultType = resultType;
	}
}

void FGsGuildAuctionItem::SetResultList(const TArray<TPair<GuildMemberGradeType, UserDBId>>& InUserList)
{
	_resultUserList = InUserList;
}

bool FGsGuildAuctionItem::CheckBid(Currency InBidPrice) const
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return false;

	// 경매 탭 잠금 확인
	if (false == unlockManager->IsContentsUnlock(EGsUnlockContentsType::GUILD_AUCTION_TAB, true))
		return false;

	// 입찰 가능한 아이템인지 판단 (내가 입찰 가능한 대상에 포함되었는가?)
	if (false == CanBid())
	{
		FText errorText;
		switch (_serverData.mGuildStoreAvailableResultType)
		{
		case GuildStoreAvailableResultType::STANDBY: // 가입 대기 시간
			// TEXT: 기사단 가입 대기시간이 지나지 않아 경매에 참여할 수 없습니다. (7일 경과 후 참여 가능)
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Restriction1_Standby"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			break;
		case GuildStoreAvailableResultType::AUTHORITY: // 기사단원 등급
			// TEXT: 기사단원 등급이 낮아 경매에 참여할 수 없습니다. (2급 이상만 참여 가능)
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Restriction2_Authority"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			break;
		case GuildStoreAvailableResultType::APPOINTMENT: // 지정 경매 대상
			// TEXT: 해당 지정 경매 품목의 입찰 대상이 아닙니다.
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Restriction3_Appointment"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			break;
		case GuildStoreAvailableResultType::ACQUISITION: // 참여자 경매 대상
			// TEXT: 해당 참여자 경매 품목의 입찰 대상이 아닙니다.
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Restriction4_Acquisition"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			break;
		default:
			break;
		}
		return false;
	}

	if (_bidderUserName.IsEmpty())
	{
		// 입찰한 유저가 없으면 현재 입찰금 이상이면 시작 가능하다.
		if (InBidPrice < GetBidPrice())
		{
			// TEXT: 경매 시작 금액은 최소 {0} 이상이어야 합니다.
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Appointment_PopUp_BeginPriceLack"), errorText);
			errorText = FText::Format(errorText, GetBidPrice());
			FGsUIHelper::TrayMessageTicker(errorText);
			return false;
		}
	}
	else
	{
		// 현재 입찰금보다 커야 입찰이 가능하다.
		if (InBidPrice <= GetBidPrice())
		{
			// TEXT: 입찰금이 부족합니다.
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Bid_Shortage_Notify"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			return false;
		}
	}

	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
	if (GetExpiredDateTime() < serverNowSec)
		return false;

	// 다이아 재화 확인
	if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::DIA, InBidPrice, true))
		return false;

	return true;
}

void FGsGuildAuctionData::Initialize()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;
	
	if (guildManager->IsGuild())
	{
		const FGsGuildRedDotCheckData& info = guildManager->GetRedDotCheckData();
		if (info._divisionCheckTime == 0)
		{
			guildManager->GetRedDotCheckData().UpdateDivisionCheckTime();
		}
		if (info._auctionCheckTime == 0)
		{
			guildManager->GetRedDotCheckData().UpdateAuctionCheckTime();
		}
		if (info._calculateCheckTime == 0)
		{
			guildManager->GetRedDotCheckData().UpdateCalculateCheckTime();
		}
		SetPrevCheckTimeDivision(info._divisionCheckTime);
		SetPrevCheckTimeAuction(info._auctionCheckTime);
		SetPrevCheckTimeCalculate(info._calculateCheckTime);
	}

	_categoryDataList.Empty();
	const UGsTableMarketCategoryData* table = Cast<const UGsTableMarketCategoryData>(FGsSchemaMarketCategoryData::GetStaticTable());
	if (table)
	{
		TArray<const FGsSchemaMarketCategoryData*> allRows;
		if (table->GetAllRows(allRows))
		{
			for (int32 i = 0; i < allRows.Num(); ++i)
			{
				if (EGsMarketUICategory::BOOKMARK == allRows[i]->category)
					continue;

				_categoryDataList.Emplace(allRows[i]);
			}
		}
	}
}

void FGsGuildAuctionData::ResetData()
{
	// 획득 설정 & 분배 & 경매 진입 시 받아오는 정보
	// 기사단 Window UI를 들어가고 나갈 때 초기화
	_divisionItemList.Empty();
	_auctionItemList.Empty();
	_calculateItemList.Empty();
	_checkedItemList.Empty();
	_userNameMap.Empty();

	_divisionListCount = 0;
	_auctionListCount = 0;
	_calculateListCount = 0;

	_selectedAuctionId = INVALID_GUILD_AUCTION_ID;

	_remainPayback = 0;

	_scrollOffset = 0.f;

	ClearFilter();
}

void FGsGuildAuctionData::ResetDivisionList()
{
	_divisionItemList.Empty();
	_checkedItemList.Empty();
	_divisionListCount = 0;
}

void FGsGuildAuctionData::ResetAuctionList()
{
	_auctionItemList.Empty();
	_auctionListCount = 0;
	_selectedAuctionId = INVALID_GUILD_AUCTION_ID;
}

void FGsGuildAuctionData::ResetCalculateList()
{
	_calculateItemList.Empty();
	_calculateListCount = 0;
}

void FGsGuildAuctionData::SendReqDivisionList(ItemDBId InLastItemDBId, bool InIsScrolled)
{
	FGsNetSendServiceGuild::SendReqGuildDivisionList(InLastItemDBId, InIsScrolled);

	//FGsGuildManager* guildManager = GGuild();
	//if (nullptr == guildManager)
	//	return;

	//bool isEmpty = _divisionItemList.Num() <= 0;
	//bool isRedDot = guildManager->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Division);

	//_divisionListCount = isRedDot ? 0 : _divisionListCount;

	//if (isEmpty || isRedDot)
	//{
	//	FGsNetSendServiceGuild::SendReqGuildDivisionList();
	//}
}

void FGsGuildAuctionData::SendReqAuctionList(GuildAuctionId InLastAuctionId, bool InIsScrolled)
{
	FGsNetSendServiceGuild::SendReqGuildAuctionList(InLastAuctionId, InIsScrolled);
}

void FGsGuildAuctionData::SendReqCalculateList(GuildAuctionId InLastAuctionId, bool InIsScrolled)
{
	FGsNetSendServiceGuild::SendReqGuildCalculateList(InLastAuctionId, InIsScrolled);
}

const FGsGuildDivisionItem* FGsGuildAuctionData::GetDivisionItem(ItemDBId InItemDBId)
{
	for (int32 i = 0; i < _divisionItemList.Num(); ++i)
	{
		if (_divisionItemList[i].GetItemData().mItemDBId == InItemDBId)
		{
			return &_divisionItemList[i];
		}
	}
	return nullptr;
}

const FGsGuildAuctionItem* FGsGuildAuctionData::GetAuctionItem(GuildAuctionId InAuctionId)
{
	for (int32 i = 0; i < _auctionItemList.Num(); ++i)
	{
		if (_auctionItemList[i].GetAuctionId() == InAuctionId)
		{
			return &_auctionItemList[i];
		}
	}
	return nullptr;
}

const FGsGuildDivisionItem* FGsGuildAuctionData::GetCalculateItem(ItemDBId InItemDBId)
{
	for (int32 i = 0; i < _calculateItemList.Num(); ++i)
	{
		if (_calculateItemList[i].GetItemData().mItemDBId == InItemDBId)
		{
			return &_calculateItemList[i];
		}
	}
	return nullptr;
}

void FGsGuildAuctionData::AddOrUpdateDivisionItem(const GuildStoreItemData& InData)
{
	bool isExist = false;
	for (int32 i = 0; i < _divisionItemList.Num(); ++i)
	{
		if (_divisionItemList[i].GetItemData().mItemDBId == InData.mItemDataEquip.mItemDBId)
		{
			_divisionItemList[i].UpdateDivisionItem(InData);
			isExist = true;
			break;
		}
	}

	if (false == isExist)
	{
		_divisionItemList.Emplace(InData);
	}
}

void FGsGuildAuctionData::AddDivisionItemList(const TArray<GuildStoreItemData>& InDataList)
{
	FGsGameFlowManager* modeManager = GMode();
	if (nullptr == modeManager)
		return;

	int32 contentStateIndex = modeManager->GetCurrentContentStateIndex();
	if (0 > contentStateIndex)
		return;

	if (static_cast<int32>(FGsContentsMode::InGame::ContentsHud) == contentStateIndex)
		return;

	//UGsUIManager* uiManager = GUI();
	//if (nullptr == uiManager)
	//	return;

	//// 기사단 메인 Window가 열려 있지 않으면 필요없는 정보이다.
	//if (false == uiManager->IsActiveWidget(TEXT("WindowGuildMain")))
	//	return;

	for (const GuildStoreItemData& serverData : InDataList)
	{
		
		AddOrUpdateDivisionItem(serverData);
	}
}

void FGsGuildAuctionData::UpdateDivisionItemList(const TArray<GuildStoreItemData>& InDataList)
{
	FGsGameFlowManager* modeManager = GMode();
	if (nullptr == modeManager)
		return;

	int32 contentStateIndex = modeManager->GetCurrentContentStateIndex();
	if (0 > contentStateIndex)
		return;

	if (static_cast<int32>(FGsContentsMode::InGame::ContentsHud) == contentStateIndex)
		return;

	//UGsUIManager* uiManager = GUI();
	//if (nullptr == uiManager)
	//	return;

	//// 기사단 메인 Window가 열려 있지 않으면 필요없는 정보이다.
	//if (false == uiManager->IsActiveWidget(TEXT("WindowGuildMain")))
	//	return;

	for (const GuildStoreItemData& serverData : InDataList)
	{
		UpdateDivisionItem(serverData);
	}
}

void FGsGuildAuctionData::AddOrUpdateAuctionItem(const GuildStoreItemData& InData)
{
	bool isExist = false;
	for (int32 i = 0; i < _auctionItemList.Num(); ++i)
	{
		if (_auctionItemList[i].GetItemData().mItemDBId == InData.mItemDataEquip.mItemDBId)
		{
			_auctionItemList[i].UpdateAuctionItem(InData);
			isExist = true;
			break;
		}
	}

	if (false == isExist)
	{
		FGsGuildAuctionItem auctionItem(InData);
		_auctionItemList.Emplace(auctionItem);
	}
}

void FGsGuildAuctionData::UpdateAuctionItem(const GuildStoreItemData& InData)
{
	for (FGsGuildAuctionItem& auctionItem : _auctionItemList)
	{
		if (auctionItem.GetAuctionId() == InData.mGuildAuctionId)
		{
			auctionItem.UpdateAuctionItem(InData);
		}
	}
}

void FGsGuildAuctionData::AddAuctionItemList(const TArray<GuildStoreItemData>& InDataList)
{
	FGsGameFlowManager* modeManager = GMode();
	if (nullptr == modeManager)
		return;

	int32 contentStateIndex = modeManager->GetCurrentContentStateIndex();
	if (0 > contentStateIndex)
		return;

	if (static_cast<int32>(FGsContentsMode::InGame::ContentsHud) == contentStateIndex)
		return;

	//UGsUIManager* uiManager = GUI();
	//if (nullptr == uiManager)
	//	return;

	//// 기사단 메인 Window가 열려 있지 않으면 필요없는 정보이다.
	//if (false == uiManager->IsActiveWidget(TEXT("WindowGuildMain")))
	//	return;

	for (const GuildStoreItemData& serverData : InDataList)
	{
		AddOrUpdateAuctionItem(serverData);
	}
}

void FGsGuildAuctionData::UpdateAuctionItemList(const TArray<GuildStoreItemData>& InDataList)
{
	FGsGameFlowManager* modeManager = GMode();
	if (nullptr == modeManager)
		return;

	int32 contentStateIndex = modeManager->GetCurrentContentStateIndex();
	if (0 > contentStateIndex)
		return;

	if (static_cast<int32>(FGsContentsMode::InGame::ContentsHud) == contentStateIndex)
		return;

	//UGsUIManager* uiManager = GUI();
	//if (nullptr == uiManager)
	//	return;

	//// 기사단 메인 Window가 열려 있지 않으면 필요없는 정보이다.
	//if (false == uiManager->IsActiveWidget(TEXT("WindowGuildMain")))
	//	return;

	for (const GuildStoreItemData& serverData : InDataList)
	{
		UpdateAuctionItem(serverData);
	}
}

void FGsGuildAuctionData::AddCalculateItemList(const TArray<GuildStoreItemData>& InDataList)
{
	FGsGameFlowManager* modeManager = GMode();
	if (nullptr == modeManager)
		return;

	int32 contentStateIndex = modeManager->GetCurrentContentStateIndex();
	if (0 > contentStateIndex)
		return;

	if (static_cast<int32>(FGsContentsMode::InGame::ContentsHud) == contentStateIndex)
		return;

	//UGsUIManager* uiManager = GUI();
	//if (nullptr == uiManager)
	//	return;

	//// 기사단 메인 Window가 열려 있지 않으면 필요없는 정보이다.
	//if (false == uiManager->IsActiveWidget(TEXT("WindowGuildMain")))
	//	return;

	for (const GuildStoreItemData& serverData : InDataList)
	{
		bool isExist = false;
		for (int32 i = 0; i < _calculateItemList.Num(); ++i)
		{
			if (_calculateItemList[i].GetAuctionDoneId() == serverData.mGuildAuctionDoneId)
			{
				isExist = true;
				break;
			}
		}

		if (false == isExist)
		{
			_calculateItemList.Emplace(serverData);
		}
	}
}

void FGsGuildAuctionData::AddUserName(UserDBId InUserDBId, const FString& InUserName)
{
	if (false == _userNameMap.Contains(InUserDBId))
	{
		_userNameMap.Emplace(InUserDBId, InUserName);
	}
}

FString FGsGuildAuctionData::GetUserName(UserDBId InUserDBId)
{
	if (_userNameMap.Contains(InUserDBId))
	{
		return _userNameMap[InUserDBId];
	}
	return FString();
}

void FGsGuildAuctionData::UpdateDetailCaptureInfo(ItemDBId InItemDBId, UserDBId InProcurerUserDBId, const FString& InBidderUserName)
{
	for (FGsGuildDivisionItem& divisionItem : _divisionItemList)
	{
		if (divisionItem.GetItemData().mItemDBId == InItemDBId)
		{
			divisionItem.SetAcquisitionUser(InProcurerUserDBId);
			divisionItem.SetWinningBidderUser(InBidderUserName);
			break;
		}
	}

	for (FGsGuildDivisionItem& calculateItem : _calculateItemList)
	{
		if (calculateItem.GetItemData().mItemDBId == InItemDBId)
		{
			calculateItem.SetAcquisitionUser(InProcurerUserDBId);
			calculateItem.SetWinningBidderUser(InBidderUserName);
			break;
		}
	}
}

void FGsGuildAuctionData::UpdateDetail(ItemDBId InItemDBId, const TArray<UserDBId>& InParticipantList, const TArray<TPair<GuildMemberGradeType, UserDBId>>& InResultList, Currency InCalculatedPrice)
{
	for (FGsGuildDivisionItem& divisionItem : _divisionItemList)
	{
		if (divisionItem.GetItemData().mItemDBId == InItemDBId)
		{
			divisionItem.SetParticipantList(InParticipantList);
			divisionItem.SetResultList(InResultList);
			divisionItem.SetDistributedPrice(InCalculatedPrice);
			break;
		}
	}

	for (FGsGuildAuctionItem& auctionItem : _auctionItemList)
	{
		if (auctionItem.GetItemData().mItemDBId == InItemDBId)
		{
			auctionItem.SetResultList(InResultList);
			auctionItem.SetExpectedResultPrice(InCalculatedPrice);
			break;
		}
	}

	for (FGsGuildDivisionItem& calculateItem : _calculateItemList)
	{
		if (calculateItem.GetItemData().mItemDBId == InItemDBId)
		{
			calculateItem.SetParticipantList(InParticipantList);
			calculateItem.SetResultList(InResultList);
			calculateItem.SetDistributedPrice(InCalculatedPrice);
			break;
		}
	}
}

void FGsGuildAuctionData::SetAuctionBidder(GuildAuctionId InAuctionId, const FString& InUserName)
{
	for (FGsGuildAuctionItem& auctionItem : _auctionItemList)
	{
		if (auctionItem.GetAuctionId() == InAuctionId)
		{
			auctionItem.SetBidderUserName(InUserName);
			break;
		}
	}
}

void FGsGuildAuctionData::SetDivisionBidder(ItemDBId InItemDBId, const FString& InUserName)
{
	for (FGsGuildDivisionItem& divisionItem : _divisionItemList)
	{
		if (divisionItem.GetItemDBId() == InItemDBId)
		{
			divisionItem.SetWinningBidderUser(InUserName);
		}
	}
}

void FGsGuildAuctionData::SetAcquisitionSettings(const GuildAcquisitionSettings& InAcquisitionSettings)
{
	_acquisitionSettings.mGuildDBId = InAcquisitionSettings.mGuildDBId;
	_acquisitionSettings.mAcquisition = InAcquisitionSettings.mAcquisition;
	for (int32 i = 0; i < MAX_GUILD_ACQUISITION_SETTING_COUNT; ++i)
	{
		_acquisitionSettings.mAcquisitionSetting[i] = InAcquisitionSettings.mAcquisitionSetting[i];
	}
}

GuildAcquisitionSettingId FGsGuildAuctionData::GetAcquisitionSetting(int32 InOptionId)
{
	return InOptionId < MAX_GUILD_ACQUISITION_SETTING_COUNT ? _acquisitionSettings.mAcquisitionSetting[InOptionId] : 0;
}

int32 FGsGuildAuctionData::GetMaxDivisionCount()
{
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
		return 0;

	if (false == guildConfig->guildAuctionList.IsValidIndex(0))
		return 0;

	const FGsSchemaGuildAuction* auctionConfig = &(guildConfig->guildAuctionList[0]);
	if (nullptr == auctionConfig)
		return 0;

	return auctionConfig->safeCapacity;
}

void FGsGuildAuctionData::UpdateDivisionItem(const GuildStoreItemData& InData)
{
	for (FGsGuildDivisionItem& divisionItem : _divisionItemList)
	{
		if (divisionItem.GetItemDBId() == InData.mItemDataEquip.mItemDBId)
		{
			divisionItem.UpdateDivisionItem(InData);
		}
	}
}

void FGsGuildAuctionData::AddCheckedItem(ItemDBId InAcquisitionItemDBId)
{
	// 지정 경매일 경우 개수 제한이 있음
	if (_currentDivisionType == GuildAuctionType::APPOINTMENT || _currentDivisionType == GuildAuctionType::DIRECT_DISTRIBUTION)
	{
		const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
		if (nullptr == guildConfig)
			return;

		if (false == guildConfig->guildAuctionList.IsValidIndex(0))
			return;

		const FGsSchemaGuildAuction* auctionConfig = &(guildConfig->guildAuctionList[0]);
		if (nullptr == auctionConfig)
			return;

		if (_checkedItemList.Num() >= auctionConfig->multipleAppointmentMax)
		{
			// 지정 분배 시 동시에 {0}개 아이템만 선택할 수 있습니다.
			FText findText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Division_Appointment_OverCount"), findText);
			FText errorText = FText::Format(findText, FText::AsNumber(auctionConfig->multipleAppointmentMax));
			FGsUIHelper::TrayMessageTicker(errorText);
			return;
		}
	}
	_checkedItemList.Emplace(InAcquisitionItemDBId);
}

void FGsGuildAuctionData::AddCheckedItemAll()
{
	for (int32 i = 0; i < _divisionItemList.Num(); ++i)
	{
		if (_divisionItemList[i].CanSelect())
		{
			AddCheckedItem(_divisionItemList[i].GetItemDBId());
		}
	}
}

void FGsGuildAuctionData::RemoveCheckedItem(ItemDBId InAcquisitionItemDBId)
{
	_checkedItemList.Remove(InAcquisitionItemDBId);
}

void FGsGuildAuctionData::RemoveCheckedItemAll()
{
	_checkedItemList.Empty();
}

bool FGsGuildAuctionData::IsCheckedItem(ItemDBId InAcquisitionItemDBId)
{
	for (ItemDBId checkedItemDBId : _checkedItemList)
	{
		if (checkedItemDBId == InAcquisitionItemDBId)
			return true;
	}
	return false;
}

bool FGsGuildAuctionData::HasCheckedItem()
{
	if (_checkedItemList.Num() > 0)
		return true;

	return false;
}

bool FGsGuildAuctionData::IsCheckedAllItem()
{
	// 선택된 목록이 없다.
	if (_checkedItemList.Num() == 0)
		return false;

	int32 count = 0;
	for (const FGsGuildDivisionItem& acquisitionItem : _divisionItemList)
	{
		if (acquisitionItem.CanSelect())
		{
			++count;
		}
	}

	// 선택할 수 있는 모든 요소의 수 == 현재 선택된 요소의 수
	return count == _checkedItemList.Num();
}

TArray<ItemDBId> FGsGuildAuctionData::GetCheckedDivisionItemList()
{
	return _checkedItemList.Array();
}

int32 FGsGuildAuctionData::GetMaxBiddingTime()
{
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
		return 0;

	if (false == guildConfig->guildAuctionList.IsValidIndex(0))
		return 0;

	const FGsSchemaGuildAuction* auctionConfig = &(guildConfig->guildAuctionList[0]);
	if (nullptr == auctionConfig)
		return 0;

	return auctionConfig->auctionOngoing;
}

Currency FGsGuildAuctionData::GetMinAuctionPrice(ItemGrade InItemGrade)
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return 0;

	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
		return 0;

	if (false == guildConfig->guildAuctionList.IsValidIndex(0))
		return 0;

	const FGsSchemaGuildAuction* auctionConfig = &(guildConfig->guildAuctionList[0]);
	if (nullptr == auctionConfig)
		return 0;

	Currency gradeMinPrice = 0;
	for (int32 i = 0; i < auctionConfig->auctionRegistration.Num(); ++i)
	{
		if (auctionConfig->auctionRegistration[i].itemGrade == InItemGrade)
		{
			gradeMinPrice = auctionConfig->auctionRegistration[i].minPrice;
		}
	}

	Currency memberCountPrice = guildManager->GetMyGuildData()->GetMaxMemberNum();
	float commissionPrice = memberCountPrice * GetCommissionRate() / 10000.0f;
	memberCountPrice = memberCountPrice + FMath::CeilToInt(commissionPrice);
	return gradeMinPrice > memberCountPrice ? gradeMinPrice : memberCountPrice;
}

Currency FGsGuildAuctionData::GetMaxAuctionPrice()
{
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
		return 0;

	if (false == guildConfig->guildAuctionList.IsValidIndex(0))
		return 0;

	const FGsSchemaGuildAuction* auctionConfig = &(guildConfig->guildAuctionList[0]);
	if (nullptr == auctionConfig)
		return 0;

	return auctionConfig->maxPrice;
}

Currency FGsGuildAuctionData::GetMaxAuctionBeginPrice()
{
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
		return 0;

	if (false == guildConfig->guildAuctionList.IsValidIndex(0))
		return 0;

	const FGsSchemaGuildAuction* auctionConfig = &(guildConfig->guildAuctionList[0]);
	if (nullptr == auctionConfig)
		return 0;

	return auctionConfig->auctionBeginMaxPrice;
}

int32 FGsGuildAuctionData::GetMaxAuctionCount()
{
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
		return 0;

	if (false == guildConfig->guildAuctionList.IsValidIndex(0))
		return 0;

	const FGsSchemaGuildAuction* auctionConfig = &(guildConfig->guildAuctionList[0]);
	if (nullptr == auctionConfig)
		return 0;

	return auctionConfig->auctionCapacity;
}

int32 FGsGuildAuctionData::GetCommissionRate()
{
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
		return 0;

	if (false == guildConfig->guildAuctionList.IsValidIndex(0))
		return 0;

	const FGsSchemaGuildAuction* auctionConfig = &(guildConfig->guildAuctionList[0]);
	if (nullptr == auctionConfig)
		return 0;

	// 만분률 수수료
	return auctionConfig->auctionCommissionRate;
}

void FGsGuildAuctionData::GetCommissionRateText(OUT FText& OutCommissionText)
{
	FNumberFormattingOptions option;
	option.SetMaximumFractionalDigits(2);
	option.SetMinimumFractionalDigits(0);

	FText percentText = FText::FromString(TEXT("{0}%"));
	float commisionRate = (float)GetCommissionRate() / 100.0f;
	OutCommissionText = FText::Format(percentText, FText::AsNumber(commisionRate, &option));
}

int32 FGsGuildAuctionData::GetAutoAddedBid()
{
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
		return 0;

	if (false == guildConfig->guildAuctionList.IsValidIndex(0))
		return 0;

	const FGsSchemaGuildAuction* auctionConfig = &(guildConfig->guildAuctionList[0]);
	if (nullptr == auctionConfig)
		return 0;

	// 패널 선택 시 현재 입찰금에서 더해지는 수치
	return auctionConfig->auctionAutoAddedBid;
}

int32 FGsGuildAuctionData::GetAuctionStandby()
{
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
		return 0;

	if (false == guildConfig->guildAuctionList.IsValidIndex(0))
		return 0;

	const FGsSchemaGuildAuction* auctionConfig = &(guildConfig->guildAuctionList[0]);
	if (nullptr == auctionConfig)
		return 0;

	return auctionConfig->auctionStandby * 60; // 분 -> 초 기준으로 변경
}

void FGsGuildAuctionData::SetLastUpdateTimeAcquisition(time_t InTime)
{
	_lastAcquisitionUpdateTime = InTime;
}

void FGsGuildAuctionData::SetLastUpdateTimeDivision(time_t InTime)
{
	_lastDivisionUpdateTime = InTime;
}

void FGsGuildAuctionData::SetLastUpdateTimeAuction(time_t InTime)
{
	_lastAuctionUpdateTime = InTime;
}

void FGsGuildAuctionData::SetLastUpdateTimeCalculate(time_t InTime)
{
	_lastCalculateUpdateTime = InTime;
}

void FGsGuildAuctionData::SetPrevCheckTimeDivision(time_t InTime)
{
	_prevDivisionCheckTime = InTime;
}

void FGsGuildAuctionData::SetPrevCheckTimeAuction(time_t InTime)
{
	_prevAuctionCheckTime = InTime;
}

void FGsGuildAuctionData::SetPrevCheckTimeCalculate(time_t InTime)
{
	_prevCalculateCheckTime = InTime;
}

void FGsGuildAuctionData::AddFilter(EGsGuildAuctionFilterType InType)
{
	if (false == _filterSet.Contains(InType))
	{
		_filterSet.Emplace(InType);
	}
}

void FGsGuildAuctionData::AddOrUpdateFilterCategory(EGsGuildAuctionFilterType InType, int32 InMainIndex, int32 InSubIndex)
{
	if (_filterCategoryMap.Contains(InType))
	{
		_filterCategoryMap[InType] = TPair<int32, int32>(InMainIndex, InSubIndex);
	}
	else
	{
		_filterCategoryMap.Emplace(InType, TPair<int32, int32>(InMainIndex, InSubIndex));
	}
}

void FGsGuildAuctionData::RemoveFilter(EGsGuildAuctionFilterType InType)
{
	_filterSet.Remove(InType);
	_filterCategoryMap.Remove(InType);
}

void FGsGuildAuctionData::ClearFilter()
{
	_filterSet.Empty();
	_filterCategoryMap.Empty();
}

bool FGsGuildAuctionData::GetFilter(EGsGuildAuctionFilterType InType)
{
	return _filterSet.Contains(InType);
}

TPair<int32, int32> FGsGuildAuctionData::GetFilterCategory(EGsGuildAuctionFilterType InType)
{
	if (_filterCategoryMap.Contains(InType))
	{
		return _filterCategoryMap[InType];
	}

	return TPair<int32, int32>(0, 0);
}

TArray<MarketCategory> FGsGuildAuctionData::GetFilterCategoryList(EGsGuildAuctionFilterType InType)
{
	TArray<MarketCategory> filterCategoryList;

	TPair<int32, int32> filterCategoryData = GetFilterCategory(InType);
	if (_categoryDataList.IsValidIndex(filterCategoryData.Key))
	{
		if (const FGsSchemaMarketCategoryData* categoryData = _categoryDataList[filterCategoryData.Key])
		{
			if (categoryData->subCategoryList.IsValidIndex(filterCategoryData.Value))
			{
				const FGsSchemaMarketSubCategoryData& subCategoryData = categoryData->subCategoryList[filterCategoryData.Value];
				if (subCategoryData.isAll)
				{
					for (const FGsSchemaMarketSubCategoryData& realSubCategoryData : categoryData->subCategoryList)
					{
						if (realSubCategoryData.isAll)
							continue;

						filterCategoryList.Emplace(realSubCategoryData.marketCategory);
					}
				}
				else
				{
					filterCategoryList.Emplace(subCategoryData.marketCategory);
				}
			}
		}
	}

	return filterCategoryList;
}

bool FGsGuildAuctionData::CheckPacketError(int64 InPacketResult)
{
	if (PACKET_RESULT_SUCCESS == InPacketResult)
		return false;

	FText findText;
	switch (InPacketResult)
	{
	case PD::Result::GUILD_ERROR_GRADE:
		// TEXT: 권한이 없습니다.
		FText::FindText(TEXT("GuildText"), TEXT("Alert_NoAuthority"), findText);
		break;
	case PD::Result::GUILD_AUCTION_ERROR_INVALID_ID:
		// TEXT: 경매 내역이 존재하지 않습니다. - 입찰 실패
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_List_Empty_Notify"), findText);
		break;
	case PD::Result::GUILD_AUCTION_ERROR_EXPIRED_DATE_TIME:
		break;
	case PD::Result::GUILD_AUCTION_ERROR_DIVISION_STATE:
		break;
	case PD::Result::GUILD_AUCTION_ERROR_ENOUGH_CURRENCY:
		// TEXT: 입찰금이 부족합니다.
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Bid_Shortage_Notify"), findText);
		break;
	case PD::Result::GUILD_AUCTION_ERROR_APPOINTMENT:
		break;
	case PD::Result::GUILD_AUCTION_ERROR_PARTICIPANTS:
		break;
	case PD::Result::GUILD_AUCTION_ERROR_DB:
		break;
	case PD::Result::GUILD_AUCTION_ERROR_BUY_STATE:
		break;
	case PD::Result::GUILD_AUCTION_ERROR_RETURN_CURRENCY:
		break;
	case PD::Result::GUILD_AUCTION_ERROR_DUPLICATION_REGISTER:
		break;
	case PD::Result::GUILD_AUCTION_ERROR_FULL_REGISTRATION:
		// TEXT: 경매 동시 진행 최대치를 초과했습니다. - 경매 시작
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Over_Limit_Notify"), findText);
		break;
	case PD::Result::GUILD_AUCTION_ERROR_HIGHER_BID:
		// TEXT: 기사단 경매는 상위입찰만 가능합니다.
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Bidding_Failure_Notify"), findText);
		break;
	case PD::Result::GUILD_AUCTION_ERROR_EMPTY_ITEM:
		// TEXT: 경매 가능한 아이템이 없습니다. - 경매 시작
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Empty_Notify"), findText);
		break;
	case PD::Result::GUILD_AUCTION_ERROR_JOINED_TIME:
		// TEXT: 기사단 가입 시점으로부터 일주일간 경매를 이용할 수 없습니다.
		FText::FindText(TEXT("NetText"), TEXT("GUILD_AUCTION_ERROR_JOINED_TIME"), findText);
		break;
	}
	
	//// TEXT: 기사단 분배에 대한 권한을 가지고 있지 않습니다. - 경매 시작
	//FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Authority_Notify"), findText);
	//// TEXT: 해당 유저를 기사단원 목록에서 찾을 수 없습니다.
	//FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Nobody_Notify"), findText);
	
	if (findText.IsEmpty())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(InPacketResult));
	}
	else
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
	
	return true;
}

void FGsGuildAuctionData::MakeComboBoxParticipant(class UGsUIGuildStorageComboBox* InComboBox)
{
	FText textFilterAll;
	FText textFilterParticipant;
	FText baseText;
	FText::FindText(TEXT("GuildText"), TEXT("UI_ItemCollection_SubCategory_Name_All"), textFilterAll); // 전체
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_FilterButton_Acquisition"), textFilterParticipant); // 참여
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Participant_PopUp_Participant"), baseText);

	InComboBox->StartAddMenu();
	InComboBox->SetBaseText(baseText);
	InComboBox->AddMenu(textFilterAll);
	InComboBox->AddMenu(textFilterParticipant);
	InComboBox->FinishAddMenu();
	InComboBox->SetSelectedIndex(0);
}

void FGsGuildAuctionData::MakeComboBoxCategory(class UGsUIGuildStorageComboBox* InComboBox)
{
	FText baseText;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Acquisition_ItemName2"), baseText);

	InComboBox->StartAddMenu();
	InComboBox->SetBaseText(baseText);

	for (const FGsSchemaMarketCategoryData* categoryData : _categoryDataList)
	{
		TArray<FText> subMenuTextList;
		for (const FGsSchemaMarketSubCategoryData& realSubCategoryData : categoryData->subCategoryList)
		{
			subMenuTextList.Emplace(realSubCategoryData.marketCategoryText);
		}

		InComboBox->AddMenuWithSubMenu(categoryData->categoryText, subMenuTextList);
	}

	InComboBox->FinishAddMenu();
	InComboBox->SetSelectedIndex(0);
}

void FGsGuildAuctionData::MakeComboBoxBid(class UGsUIGuildStorageComboBox* InComboBox)
{
	FText textFilterAll;
	FText textFilterCanBid;
	FText baseText;
	FText::FindText(TEXT("GuildText"), TEXT("UI_ItemCollection_SubCategory_Name_All"), textFilterAll); // 전체
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_FilterButton_Available"), textFilterCanBid); // 가능
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Participant_PopUp_Participant"), baseText);
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Bid2"), baseText);

	InComboBox->StartAddMenu();
	InComboBox->SetBaseText(baseText);
	InComboBox->AddMenu(textFilterAll);
	InComboBox->AddMenu(textFilterCanBid);
	InComboBox->FinishAddMenu();
	InComboBox->SetSelectedIndex(0);
}

void FGsGuildAuctionData::MakeComboBoxSuccessBid(class UGsUIGuildStorageComboBox* InComboBox)
{
	FText textFilterAll;
	FText textFilterSuccess;
	FText baseText;
	FText::FindText(TEXT("GuildText"), TEXT("UI_ItemCollection_SubCategory_Name_All"), textFilterAll); // 전체
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_FilterButton_Success"), textFilterSuccess); // 성공
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Calculate_SuccessfulBid2"), baseText);

	InComboBox->StartAddMenu();
	InComboBox->SetBaseText(baseText);
	InComboBox->AddMenu(textFilterAll);
	InComboBox->AddMenu(textFilterSuccess);
	InComboBox->FinishAddMenu();
	InComboBox->SetSelectedIndex(0);
}
