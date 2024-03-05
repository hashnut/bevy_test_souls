// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconBattlePassExp.h"
#include "BattlePass/GsBattlePassMissionData.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Image.h"

void UGsUIIconBattlePassExp::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == _slotButton->_onFinishedLongPressGauge.IsBoundToObject(this))
	{
		_longPressDelegate = _slotButton->_onFinishedLongPressGauge.AddUObject(this, &UGsUIIconBattlePassExp::OnFinishedLongPress);
	}
	_slotButton->OnChangeLongPress(true);
}

void UGsUIIconBattlePassExp::NativeDestruct()
{
	if (true == _slotButton->_onFinishedLongPressGauge.IsBoundToObject(this) && true == _longPressDelegate.IsValid())
	{
		_slotButton->_onFinishedLongPressGauge.Remove(_longPressDelegate);
		_longPressDelegate.Reset();
	}

	Super::NativeDestruct();
}

void UGsUIIconBattlePassExp::OnFinishedLongPress()
{
	if (OnLongPressEvent.IsBound())
	{
		OnLongPressEvent.Broadcast(_data);
	}
}

void UGsUIIconBattlePassExp::SetData(TWeakPtr<IconData> data)
{
	_data = data;
}

void UGsUIIconBattlePassExp::RefreshUI()
{
	if (auto data = _data.Pin())
	{
		if (data->_type == IconData::DataType::BattlePassExp)
		{
			if (auto expData = StaticCastSharedPtr<IconBattlePassExpData>(data))
			{
				if (auto missionData = expData->_data.Pin())
				{
					auto exp = missionData->GetPassMissionRewardExp();
					_countText = exp > 0 ? FText::AsNumber(exp) : FText::GetEmpty();
					//_countText = FText::AsNumber(missionData->GetCompletePoint());
					SetIconImage(missionData->GetMissionRewardIcon());
				}
			}
		}
	}

}
