// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Client/SharedEnums/SharedMarketEnum.h"


enum class EGsGuildAuctionFilterType : uint8
{
	DIVISION_PARTICIPANT,
	DIVISION_CATEGORY,

	AUCTION_CAN_BID,
	AUCTION_CATEGORY,

	CALCULATE_PARTICIPANT,
	CALCULATE_SUCCESS_BID,
	CALCULATE_CATEGORY,
};
//----------------------------------------------------------------------------------------------------------------------
struct FGsGuildAuctionHelper final
{
	static void GetRootText(const GuildStoreItemData& InData, OUT FText& OutRootText);
};
//----------------------------------------------------------------------------------------------------------------------
/**
 * 기사단 분배 목록 데이터
 */
struct T1PROJECT_API FGsGuildDivisionItem final
{
protected:
	GuildStoreItemData _serverData;

	Currency _distributedPrice = 0;	// 개별 정산금
	FString _bidderUserName;
	bool _isMyItem = false;
	UserDBId _acquisitionUserDBId = INVALID_USER_DB_ID; // 획득자
	TArray<UserDBId> _participantList;	// 참여자 목록
	TArray<TPair<GuildMemberGradeType, UserDBId>> _resultUserList; // 정산 대상 목록
	MarketCategory _marketCategory = MarketCategory::MARKET_CATEGORY_ALL;

public:
	FGsGuildDivisionItem() = default;
	FGsGuildDivisionItem(const GuildStoreItemData& InServerData);

	~FGsGuildDivisionItem() = default;

public:
	const GuildStoreItemData& GetDefaultData() const { return _serverData; }
	const ItemDataEquip& GetItemData() const { return _serverData.mItemDataEquip; }

	GuildAuctionId GetAuctionId() const { return _serverData.mGuildAuctionId; }
	GuildAuctionId GetAuctionDoneId() const { return _serverData.mGuildAuctionDoneId; }
	ItemDBId GetItemDBId() const { return _serverData.mItemDataEquip.mItemDBId; }
	Currency GetWinningBidPrice() const { return _serverData.mCurrency; }
	GuildDivisionState GetDivisionState() const { return _serverData.mDivisionState; }
	GuildAuctionType GetDivisionType() const { return _serverData.mGuildAuctionType; }
	GuildAuctionResultType GetResultType() const { return _serverData.mGuildAuctionResultType; }
	time_t GetAuctionDateTime() const { return _serverData.mAuctionDateTime; }
	time_t GetExpiredDateTime() const { return _serverData.mExpiredDateTime; }
	time_t GetAcquisitionDateTime() const { return _serverData.mAcquisitionDateTime; }
	int32 GetParticipantListCount() const { return _serverData.mContributorMemberCount; }
	bool IsInParticipantList() const { return _serverData.mJoin; }
	Currency GetDistributedPrice() const { return _distributedPrice; }
	const TArray<UserDBId>& GetParticipantList() const { return _participantList; }
	const TArray<TPair<GuildMemberGradeType, UserDBId>>& GetResultList() const { return _resultUserList; }
	
	void GetRootText(OUT FText& OutRootText) const; // 획득 정보
	bool IsGuildDungeonItem() const;
	FString GetWinningBidder() const { return _bidderUserName; }
	FString GetAcquisitionUserName() const;
	bool CanSelect() const; // 선택할 수 있는 항목인가?
	bool IsMyItem() const { return _isMyItem; }
	MarketCategory GetMarketCategory() const { return _marketCategory; }
	
	void UpdateDivisionItem(const GuildStoreItemData& InServerData);
	void SetDistributedPrice(Currency InDistributedPrice) { _distributedPrice = InDistributedPrice; }
	void SetWinningBidderUser(const FString& InUserName);
	void SetAcquisitionUser(UserDBId InUserDBId) { _acquisitionUserDBId = InUserDBId; }
	void SetParticipantList(const TArray<UserDBId>& InUserList);
	void SetResultList(const TArray<TPair<GuildMemberGradeType, UserDBId>>& InUserList);
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 기사단 경매 목록 데이터
 */
struct T1PROJECT_API FGsGuildAuctionItem final
{
protected:
	GuildStoreItemData _serverData;

	Currency _expectedResultPrice = 0;	// 예상 정산금
	MarketCategory _marketCategory = MarketCategory::MARKET_CATEGORY_ALL;

	FString _bidderUserName;
	TArray<TPair<GuildMemberGradeType, UserDBId>> _resultUserList; // 정산 대상 목록

public:
	FGsGuildAuctionItem() = default;
	FGsGuildAuctionItem(const GuildStoreItemData& InServerData);

	~FGsGuildAuctionItem() = default;

public:
	const GuildStoreItemData& GetDefaultData() const { return _serverData; }
	const ItemDataEquip& GetItemData() const { return _serverData.mItemDataEquip; }

	GuildAuctionId GetAuctionId() const { return _serverData.mGuildAuctionId; }
	Currency GetBidPrice() const { return _serverData.mCurrency; }
	GuildDivisionState GetAuctionState() const { return _serverData.mDivisionState; }
	GuildAuctionType GetBiddingType() const { return _serverData.mGuildAuctionType; }
	GuildAuctionResultType GetResultType() const { return _serverData.mGuildAuctionResultType; }
	time_t GetAuctionDateTime() const { return _serverData.mAuctionDateTime; }
	time_t GetExpiredDateTime() const { return _serverData.mExpiredDateTime; }
	time_t GetAcquisitionDateTime() const { return _serverData.mAcquisitionDateTime; }
	bool CanBid() const { return _serverData.mAvailable; }
	Currency GetExpectedResultPrice() const { return _expectedResultPrice; }
	const TArray<TPair<GuildMemberGradeType, UserDBId>>& GetResultList() const { return _resultUserList; }

	void GetRootText(OUT FText& OutRootText) const; // 획득 경로
	FString GetBidderName() const { return _bidderUserName; }
	MarketCategory GetMarketCategory() const { return _marketCategory; }
	
	void UpdateAuctionItem(const GuildStoreItemData& InServerData);
	void SetExpectedResultPrice(Currency InPrice) { _expectedResultPrice = InPrice; }
	void SetBidderUserName(const FString& InUserName) { _bidderUserName = InUserName; }
	void SetResultList(const TArray<TPair<GuildMemberGradeType, UserDBId>>& InUserList);

	bool CheckBid(Currency InBidPrice) const;
};

struct FGsSchemaMarketCategoryData;
//----------------------------------------------------------------------------------------------------------------------
/**
 * 기사단 경매 데이터
 */
class T1PROJECT_API FGsGuildAuctionData final
{
private:
	GuildAcquisitionSettings _acquisitionSettings;

	TArray<FGsGuildDivisionItem> _divisionItemList; // 분배 페이지 획득 내역 정보
	TArray<FGsGuildAuctionItem> _auctionItemList; // 경매 페이지 경매 내역 정보
	TArray<FGsGuildDivisionItem> _calculateItemList; // 정산 페이지 내역 정보
	TMap<UserDBId, FString> _userNameMap; // 분배 & 경매 페이지 유저 정보

	int32 _divisionListCount = 0; // 분배 목록 내역 개수
	int32 _auctionListCount = 0; // 경매 목록 내역 개수
	int32 _calculateListCount = 0; // 정산 목록 내역 개수

	TSet<ItemDBId> _checkedItemList;
	GuildAuctionType _currentDivisionType = GuildAuctionType::NONE;
	GuildAuctionResultType _currentResultType = GuildAuctionResultType::NORMAL;

	Currency _currentBidPrice = 0;
	Currency _remainPayback = 0;

	GuildAuctionId _selectedAuctionId = INVALID_GUILD_AUCTION_ID;

	time_t _lastAcquisitionUpdateTime = 0; // 획득 설정이 변경된 마지막 시각
	time_t _lastDivisionUpdateTime = 0; // 분배 정보가 변경된 마지막 시각
	time_t _lastAuctionUpdateTime = 0; // 경매 정보가 변경된 마지막 시각
	time_t _lastCalculateUpdateTime = 0; // 정산 정보가 변경된 마지막 시각

	time_t _prevDivisionCheckTime = 0; // 분배 정보를 이전에 확인했던 시각
	time_t _prevAuctionCheckTime = 0; // 경매 정보를 이전에 확인했던 시각
	time_t _prevCalculateCheckTime = 0; // 정산 정보를 이전에 확인했던 시각

	float _scrollOffset = 0.f;

	TSet<EGsGuildAuctionFilterType> _filterSet;
	TMap<EGsGuildAuctionFilterType, TPair<int32, int32>> _filterCategoryMap;
	TArray<const FGsSchemaMarketCategoryData*> _categoryDataList;
	
public:
	void Initialize();

	void ResetData();
	void ResetDivisionList();
	void ResetAuctionList();
	void ResetCalculateList();

	void SendReqDivisionList(ItemDBId InLastItemDBId = INVALID_ITEM_DB_ID, bool InIsScrolled = false);
	void SendReqAuctionList(GuildAuctionId InLastAuctionId = INVALID_GUILD_AUCTION_ID, bool InIsScrolled = false);
	void SendReqCalculateList(GuildAuctionId InLastAuctionId = INVALID_GUILD_AUCTION_ID, bool InIsScrolled = false);

	void SetDivisionListCount(int32 InCount) { _divisionListCount = InCount; }
	void SetAuctionListCount(int32 InCount) { _auctionListCount = InCount; }
	void SetCalculateListCount(int32 InCount) { _calculateListCount = InCount; }

	int32 GetDivisionListCount() { return _divisionListCount; }
	int32 GetAuctionListCount() { return _auctionListCount; }
	int32 GetCalculateListCount() { return _calculateListCount; }

	const TArray<FGsGuildDivisionItem>& GetDivisionList() { return _divisionItemList; }
	const TArray<FGsGuildAuctionItem>& GetAuctionList() { return _auctionItemList; }
	const TArray<FGsGuildDivisionItem>& GetCalculateList() { return _calculateItemList; }

	const FGsGuildDivisionItem* GetDivisionItem(ItemDBId InItemDBId);
	const FGsGuildAuctionItem* GetAuctionItem(GuildAuctionId InAuctionId);
	const FGsGuildDivisionItem* GetCalculateItem(ItemDBId InItemDBId);

	void AddOrUpdateDivisionItem(const GuildStoreItemData& InData);
	void AddDivisionItemList(const TArray<GuildStoreItemData>& InDataList);
	void UpdateDivisionItemList(const TArray<GuildStoreItemData>& InDataList);
	void AddOrUpdateAuctionItem(const GuildStoreItemData& InData);
	void UpdateAuctionItem(const GuildStoreItemData& InData);
	void AddAuctionItemList(const TArray<GuildStoreItemData>& InDataList);
	void UpdateAuctionItemList(const TArray<GuildStoreItemData>& InDataList);
	void AddCalculateItemList(const TArray<GuildStoreItemData>& InDataList);

	void AddUserName(UserDBId InUserDBId, const FString& InUserName);
	FString GetUserName(UserDBId InUserDBId);

	void UpdateDetailCaptureInfo(ItemDBId InItemDBId, UserDBId InProcurerUserDBId, const FString& InBidderUserName);
	void UpdateDetail(ItemDBId InItemDBId, const TArray<UserDBId>& InParticipantList, const TArray<TPair<GuildMemberGradeType, UserDBId>>& InResultList, Currency InCalculatedPrice);

	void SetAuctionBidder(GuildAuctionId InAuctionId, const FString& InUserName);
	void SetDivisionBidder(ItemDBId InItemDBId, const FString& InUserName);

public:
	/* 획득 설정 */
	bool IsAcquisitionOn() { return _acquisitionSettings.mAcquisition; }
	void SetAcquisitionSettings(const GuildAcquisitionSettings& InAcquisitionSettings);
	GuildAcquisitionSettingId GetAcquisitionSetting(int32 InOptionId);

	/* 분배 */
	int32 GetMaxDivisionCount(); // 분배 페이지 목록 최대 노출 수

	void SetCurrentDivisionType(GuildAuctionType InDivisionType) { _currentDivisionType = InDivisionType; }
	void SetCurrentResultType(GuildAuctionResultType InResultType) { _currentResultType = InResultType; }
	GuildAuctionType GetCurrentDivisionType() { return _currentDivisionType; }
	GuildAuctionResultType GetCurrentResultType() { return _currentResultType; }

	void UpdateDivisionItem(const GuildStoreItemData& InData);
	
	/* 분배 목록 선택 정보 */
	void AddCheckedItem(ItemDBId InAcquisitionItemDBId);
	void AddCheckedItemAll();
	void RemoveCheckedItem(ItemDBId InAcquisitionItemDBId);
	void RemoveCheckedItemAll();
	bool IsCheckedItem(ItemDBId InAcquisitionItemDBId);
	bool HasCheckedItem();
	bool IsCheckedAllItem();
	TArray<ItemDBId> GetCheckedDivisionItemList();

	/* 경매 */
	int32 GetMaxBiddingTime(); // 최대 입찰 가능 시간
	Currency GetMinAuctionPrice(ItemGrade InItemGrade); // 경매 등록 가능한 최소 금액
	Currency GetMaxAuctionPrice(); // 경매 입찰 가능한 최대 금액
	Currency GetMaxAuctionBeginPrice(); // 경매 시작 금액의 상한선
	int32 GetMaxAuctionCount(); // 경매 목록 최대 용량
	int32 GetCommissionRate(); // 경매 수수료 만분률 ex) 300 -> 3%
	void GetCommissionRateText(OUT FText& OutCommissionText); // 경매 수수료 텍스트 "N%"
	int32 GetAutoAddedBid(); // 패널 선택 시 현재 입찰금에서 더해지는 수치
	int32 GetAuctionStandby(); // 기사단 가입 후 경매 참여 대기 기간

	void SetSelectedAuctionId(GuildAuctionId InAuctionId) { _selectedAuctionId = InAuctionId; }
	GuildAuctionId GetSelectedAuctionId() { return _selectedAuctionId; }

	void SetNumpadBidPrice(Currency InPrice) { _currentBidPrice = InPrice; }
	Currency GetNumpadBidPrice() { return _currentBidPrice; }
	
	/* 회수금 */
	Currency GetRemainPayback() { return _remainPayback; }
	void SetRemainPayback(Currency InRemainPayback) { _remainPayback = InRemainPayback; }

	/* 레드닷 필요 정보 */
	time_t GetLastUpdateTimeAcquisition() const { return _lastAcquisitionUpdateTime; }
	time_t GetLastUpdateTimeDivision() const { return _lastDivisionUpdateTime; }
	time_t GetLastUpdateTimeAuction() const { return _lastAuctionUpdateTime; }
	time_t GetLastUpdateTimeCalculate() const { return _lastCalculateUpdateTime; }
	time_t GetPrevCheckTimeDivision() const { return _prevDivisionCheckTime; }
	time_t GetPrevCheckTimeAuction() const { return _prevAuctionCheckTime; }
	time_t GetPrevCheckTimeCalculate() const { return _prevCalculateCheckTime; }

	void SetLastUpdateTimeAcquisition(time_t InTime);
	void SetLastUpdateTimeDivision(time_t InTime);
	void SetLastUpdateTimeAuction(time_t InTime);
	void SetLastUpdateTimeCalculate(time_t InTime);
	void SetPrevCheckTimeDivision(time_t InTime);
	void SetPrevCheckTimeAuction(time_t InTime);
	void SetPrevCheckTimeCalculate(time_t InTime);

	/* 스크롤 정보 */
	void SetScrollOffset(float InOffset) { _scrollOffset = InOffset; }
	float GetScrollOffset() { return _scrollOffset; }

	/* 필터 정보 */
	void AddFilter(EGsGuildAuctionFilterType InType);
	void AddOrUpdateFilterCategory(EGsGuildAuctionFilterType InType, int32 InMainIndex, int32 InSubIndex = 0);
	void RemoveFilter(EGsGuildAuctionFilterType InType);
	void ClearFilter();
	bool GetFilter(EGsGuildAuctionFilterType InType);
	TPair<int32, int32> GetFilterCategory(EGsGuildAuctionFilterType InType);
	TArray<MarketCategory> GetFilterCategoryList(EGsGuildAuctionFilterType InType);

	bool CheckPacketError(int64 InPacketResult);

public:
	static void MakeComboBoxParticipant(class UGsUIGuildStorageComboBox* InComboBox);
	void MakeComboBoxCategory(class UGsUIGuildStorageComboBox* InComboBox);
	static void MakeComboBoxBid(class UGsUIGuildStorageComboBox* InComboBox);
	static void MakeComboBoxSuccessBid(class UGsUIGuildStorageComboBox* InComboBox);

};