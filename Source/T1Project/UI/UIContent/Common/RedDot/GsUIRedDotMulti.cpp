// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRedDotMulti.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"


void UGsUIRedDotMulti::SetRedDot(bool bIsRedDot, EGsIconRedDotType InType, int32 InNumber)
{
	if (false == bIsRedDot)
	{
		if (_activeList.Contains(InType))
		{
			_activeList.Remove(InType);
		}

		CheckPriority();
		return;
	}

	if (false == _activeList.Contains(InType))
	{
		_activeList.Emplace(InType);
	}

	if (EGsIconRedDotType::NUMBER == InType)
	{
		// 100 넘으면 99+ 표기
		FText textNumber;
		FGsUIStringHelper::GetAmountText(InNumber, 100, textNumber);
		_textBlockNumber->SetText(textNumber);
	}

	CheckPriority();
}

void UGsUIRedDotMulti::CheckPriority()
{
	if (0 == _activeList.Num())
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);

	int32 index = -1;	
	for (int32 i = 0; i < _activeList.Num(); ++i)
	{
		if (-1 == index)
		{
			index = i;
		}
		else
		{
			// 강화 아이콘은 우선순위 밀림
			if (EGsIconRedDotType::REINFORCE != _activeList[i])
			{
				index = i;
			}
		}
	}

	_switcher->SetActiveWidgetIndex(static_cast<uint8>(_activeList[index]));
}
