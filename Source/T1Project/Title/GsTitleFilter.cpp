
#include "GsTitleFilter.h"
#include "GsTitleData.h"
#include "T1Project/T1Project.h"

// -----------------------------------------------------------------------------------------------------------------------------------------
// 2022/01/17 PKT - View Type
// 
void FGsTitleViewTypeFilter::SetFilter(ETitleViewListType InType)
{
	switch (InType)
	{
	case ETitleViewListType::eAll:
		_filter = [this](const FGsTitleData* InData, OUT TArray<const FGsTitleData*>& OutList)
		{
			OutList.Emplace(InData);
		};
		break;
	case ETitleViewListType::eAcquire:
		_filter = [this](const FGsTitleData* InData, OUT TArray<const FGsTitleData*>& OutList)
		{
			if (InData->IsAcquire())
			{
				OutList.Emplace(InData);
			}
		};
		break;
	case ETitleViewListType::eDotHave:
		_filter = [this](const FGsTitleData* InData, OUT TArray<const FGsTitleData*>& OutList)
		{
			if (false == InData->IsAcquire())
			{
				OutList.Emplace(InData);
			}
		};
		break;
	default:
		_filter = nullptr;
		break;
	}
}

void FGsTitleViewTypeFilter::Execute(const TArray<const FGsTitleData *>& InList, OUT TArray<const FGsTitleData *>& OutList)
{
	if (nullptr == _filter)
	{
		GSLOG(Error, TEXT("nullptr == _filter"));
		return;
	}

	for (const FGsTitleData* data : InList)
	{
		if (data)
		{
			_filter(data, OutList);
		}			
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------



// -----------------------------------------------------------------------------------------------------------------------------------------
// 2022/01/17 PKT - Stat List
// 
void FGsTitleStatFilter::SetFilter(const TArray<StatType>& InData)
{
	_filterStats = InData;
}

void FGsTitleStatFilter::Execute(const TArray<const FGsTitleData*>& InList, OUT TArray<const FGsTitleData*>& OutList)
{
	if (0 >= _filterStats.Num())
	{	
		OutList.Empty();
		return;
	}

	for (const FGsTitleData* data : InList)
	{
		if (data)
		{
			const int32 maxCount = data->GetStatCount();
			for (int32 idx = 0; idx < maxCount; ++idx)
			{
				if (true == _filterStats.Contains(data->GetStatType(idx)))
				{
					OutList.Emplace(data);
					break;
				}
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------