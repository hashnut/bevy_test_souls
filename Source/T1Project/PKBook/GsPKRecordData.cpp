
#include "GsPKRecordData.h"
#include "../T1Project.h"
#include "../UTIL/GsTimeSyncUtil.h"


FGsPKRecordData::FGsPKRecordData(const FGsPKRecordData& InData)
	: _recordId(InData.GetId()), _occurrenceTime(InData.GetOccurrenceTime())
	, _mainContributorData(InData.GetMainContributorData()), _occurrenceMapId(InData.GetOccurrenceMapId())
{
	_detailData.Reset(nullptr);

	if (InData.IsDetailDataCaching())
	{
		SetDetailData(*(InData.GetDetailData()));
	}	
}

FGsPKRecordData::FGsPKRecordData(
	PKRecordId InRecordId, const time_t& InOccurrenceTime, const FGsPKRecordingContributorData& InMainContributorData, MapId InMapId
)
: _recordId(InRecordId), _occurrenceTime(InOccurrenceTime), _mainContributorData(InMainContributorData), _occurrenceMapId(InMapId)
{
	_detailData.Reset(nullptr);
}

FGsPKRecordData::~FGsPKRecordData()
{
	_detailData.Reset(nullptr);
}

void FGsPKRecordData::CreateDetailData(
	const TArray<FGsPKRecordingContributorData>& InContributorDataSet, Exp InLostExp, float InLostExpPercent, int32 InRemaindCount
)
{
	_detailData.Reset(nullptr);
	_detailData = TUniquePtr<FGsPKDetailRecordData>(new FGsPKDetailRecordData());

	for (const auto& item : InContributorDataSet)
	{
		_detailData->_contributorSet.Emplace(item);
	}
	_detailData->_lostExp = InLostExp;
	_detailData->_lostExpPercent = InLostExpPercent;
	_detailData->_actionRemaindCount = InRemaindCount;
}

void FGsPKRecordData::SetDetailData(const FGsPKDetailRecordData& InDetailData)
{
	if (IsDetailDataCaching())
	{
		GSLOG(Error, TEXT("already Caching Detail Data"));
		return;
	}
	
	CreateDetailData(InDetailData._contributorSet, InDetailData._lostExp, InDetailData._lostExpPercent, InDetailData._actionRemaindCount);
}

void FGsPKRecordData::SetDetailData(const TArray<FGsPKRecordingContributorData>& InContributorDataSet, Exp InLostExp
	, float InLostExpPercent, int32 InRemaindCount)
{
	if (IsDetailDataCaching())
	{
		GSLOG(Error, TEXT("already Caching Detail Data"));
		return;
	}

	CreateDetailData(InContributorDataSet, InLostExp, InLostExpPercent, InRemaindCount);
}

void FGsPKRecordData::SetActionRemaindCount(int32 RemaindCount)
{
	if (false == IsDetailDataCaching())
	{
		GSLOG(Error, TEXT("false == IsDetailDataCaching() : [%lu]"), _recordId);
		return;
	}

	_detailData->_actionRemaindCount = RemaindCount;
}


PKRecordId FGsPKRecordData::GetId() const
{
	return _recordId;
}

PKBookResult FGsPKRecordData::GetReason() const
{
	return _mainContributorData._reason;
}

const time_t& FGsPKRecordData::GetOccurrenceTime() const
{
	return _occurrenceTime;
}

Exp FGsPKRecordData::GetLostExp() const
{
	return (IsDetailDataCaching()) ? _detailData->_lostExp : 0;
}

float FGsPKRecordData::GetLostExpPercent() const
{
	return (IsDetailDataCaching()) ? _detailData->_lostExpPercent : 0.f;
}

const FGsPKRecordingContributorData& FGsPKRecordData::GetMainContributorData() const
{
	return _mainContributorData;
}

MapId FGsPKRecordData::GetOccurrenceMapId() const
{
	return _occurrenceMapId;
}

bool FGsPKRecordData::IsDetailDataCaching() const
{
	return _detailData.IsValid();
}

const FGsPKDetailRecordData* FGsPKRecordData::GetDetailData() const
{
	return (false == _detailData.IsValid()) ? nullptr : _detailData.Get();
}

int32 FGsPKRecordData::GetActionRemaindCount() const
{
	return (false == _detailData.IsValid()) ? 0 : _detailData.Get()->_actionRemaindCount;
}