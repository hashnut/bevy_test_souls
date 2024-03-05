
#include "GsUIPageDungeonGroupBase.h"
#include "Dungeon/GsDungeonGroupBase.h"
#include "../Dungeon/GsDungeonUICaptureData.h"
#include "T1Project.h"



void UGsUIPageDungeonGroupBase::Enter(const FGsDungeonGroupBase* InData, const FGsDungeonUICaptureData& InCaptureData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_data = InData;
}

const FGsDungeonGroupBase* UGsUIPageDungeonGroupBase::GetData() const
{
	return _data;
}