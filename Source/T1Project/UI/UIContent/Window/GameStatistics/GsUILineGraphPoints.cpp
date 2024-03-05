// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUILineGraphPoints.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "GameStatistics/GsLineGraphData.h"


void UGsUILineGraphPoints::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int32 count = _panelRootButtons->GetChildrenCount();
	for (int32 i = 0; i < count; ++i)
	{
		UWidget* widget = _panelRootButtons->GetChildAt(i);
		if (UGsButton* listEntry = Cast<UGsButton>(widget))
		{
			_btnList.Emplace(listEntry);
		}
	}
}

void UGsUILineGraphPoints::NativeDestruct()
{
	_data = nullptr;

	Super::NativeDestruct();
}

void UGsUILineGraphPoints::SetData(const FGsLineGraphData* InData)
{
	_data = InData;

	for (int i = 0; i < _btnList.Num(); ++i)
	{
		_btnList[i]->SetRenderTranslation(_data->_pointList[i]);
	}
}

void UGsUILineGraphPoints::OnClickPoint(int32 InIndex)
{
	if (_data &&
		_data->_valueList.IsValidIndex(InIndex))
	{
		// TEXT: {0}일: {1}
		FText textFormat;
		if (FText::FindText(TEXT("GameStatisticsText"), TEXT("NDayValue"), textFormat))
		{
			FText textTooltip;
			switch (_data->_valueType)
			{
			case EGsStatisticsValueType::RATE:
			{
				// 만분율을 퍼센트로 변환. value / 10000 * 100 = value * 0.01
				float percent = _data->_valueList[InIndex] * 0.01f;
				FString strPercent = FString::Printf(TEXT("%.2f%%"), percent);

				textTooltip = FText::Format(textFormat, InIndex + 1, FText::FromString(strPercent));
			}
			break;
			case EGsStatisticsValueType::TIME:
			{
				FTimespan time = FTimespan::FromSeconds(_data->_valueList[InIndex]);
				FString strTime;
				FGsTimeStringHelper::GetTimeString(time, strTime);

				textTooltip = FText::FromString(strTime);
			}
			break;
			default:
			{
				textTooltip = FText::Format(textFormat, InIndex + 1, _data->_valueList[InIndex]);
			}
			break;
			}

			_tooltip->SetDescText(textTooltip);
		}

		_tooltip->SetRenderTranslation(_data->_pointList[InIndex]);
		_tooltip->Open();

		return;
	}

	_tooltip->Close();
}
