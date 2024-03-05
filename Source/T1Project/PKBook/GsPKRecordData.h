
#pragma once
#include "GsPKBookData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"



/**
 * 2022/09/19 PKT - PK Book 관련 Record Data
 * 
 */
class FGsPKRecordData
{
protected:
	// 2022/09/19 PKT - record 고유 식별 ID
	PKRecordId _recordId = INVALID_PK_RECORD_ID;

	// 2022/09/19 PKT - PK 발생 시간
	time_t _occurrenceTime = 0;

	// 2022/09/19 PKT - PK Target
	FGsPKRecordingContributorData _mainContributorData;

	// 2022/09/19 PKT - PK 발생 지역
	MapId _occurrenceMapId = INVALID_MAP_ID;

	TUniquePtr<FGsPKDetailRecordData> _detailData;

protected:
	void CreateDetailData(const TArray<FGsPKRecordingContributorData>& InContributorDataSet, Exp InLostExp
		, float InLostExpPercent, int32 InRemaindCount);

public:
	FGsPKRecordData() = default;
	explicit FGsPKRecordData(const FGsPKRecordData& InData);
	explicit FGsPKRecordData(
		PKRecordId InRecordId, const time_t& InOccurrenceTime, const FGsPKRecordingContributorData& InMainContributorData, MapId InMapId);

	~FGsPKRecordData();

	/**
	 * Set Data
	*/
	void SetDetailData(const FGsPKDetailRecordData& InDetailData);
	void SetDetailData(const TArray<FGsPKRecordingContributorData>& InContributorDataSet, Exp InLostExp
		, float InLostExpPercent, int32 InRemaindCount);
	void SetActionRemaindCount(int32 RemaindCount);

	///**
	// * Get Data
	// */

	PKRecordId GetId() const;

	PKBookResult GetReason() const;

	const time_t& GetOccurrenceTime() const;

	Exp GetLostExp() const;

	float GetLostExpPercent() const;

	const FGsPKRecordingContributorData& GetMainContributorData() const;

	MapId GetOccurrenceMapId() const;

	bool IsDetailDataCaching() const;

	const FGsPKDetailRecordData* GetDetailData() const;

	int32 GetActionRemaindCount() const;
};