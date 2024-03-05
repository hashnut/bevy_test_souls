// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBattlePassLevelHelper.h"

void UGsUIBattlePassLevelHelper::InitScrollPanel(UScrollBox* scrollBox)
{
	_cachedWidgets.Reset();

	_scrollBox = scrollBox;

	TArray<UWidget*> children = _scrollBox->GetAllChildren();
	for (auto child : children)
	{
		if (auto widget = Cast<UUserWidget>(child))
			_cachedWidgets.Add(widget);
	}
	_scrollBox->ClearChildren();
}

void UGsUIBattlePassLevelHelper::ClearScrollPanel()
{
	_cachedWidgets.Reset();
	_scrollBox->ClearChildren();
}

int32 UGsUIBattlePassLevelHelper::AddScrollWidget(UUserWidget* widget)
{
	if (_scrollBox)
	{
		_cachedWidgets.Remove(widget);
		_scrollBox->AddChild(widget);
		return _scrollBox->GetChildIndex(widget);
		
	}
	return INDEX_NONE;
}

//int32 UGsUIBattlePassLevelHelper::InsertScrollWidget(int32 insertIndex, UUserWidget* widget)
//{
//	if (_scrollBox)
//	{
//		_cachedWidgets.Remove(widget);
//		_scrollBox->InsertChildAt(insertIndex, widget);
//		return _scrollBox->GetChildIndex(widget);
//
//	}
//	return INDEX_NONE;
//}

TArray<UUserWidget*> UGsUIBattlePassLevelHelper::CashedAddScrollWidget(TSubclassOf<UUserWidget> InEntryClass, int count /*= 1*/, bool clear /*= true*/)
{
	TArray<UUserWidget*> cachedWidgets;

	if (clear)
	{
		GetAllScrollWidgetsByUClass(cachedWidgets, InEntryClass);
		_cachedWidgets.Append(cachedWidgets);
		_scrollBox->ClearChildren();

		cachedWidgets.Reset();
	}

	for (int index = 0; index < _cachedWidgets.Num();)
	{
		if (cachedWidgets.Num() < count && _cachedWidgets[index]->IsA(InEntryClass))
		{
			cachedWidgets.Add(_cachedWidgets[index]);
			_cachedWidgets.RemoveAt(index);
			continue;
		}
		index++;
	}

	while (count > cachedWidgets.Num())
	{
		auto userWidget = CreateWidget(_scrollBox, InEntryClass);
		cachedWidgets.Add(userWidget);
	}

	for (auto widget : cachedWidgets)
	{
		AddScrollWidget(widget);
	}

	return cachedWidgets;
}

void UGsUIBattlePassLevelHelper::RemoveScrollWidget(UUserWidget* widget)
{
	_scrollBox->RemoveChild(widget);
	_cachedWidgets.Add(widget);
}

void UGsUIBattlePassLevelHelper::RemoveScrollWidgetByIndex(int index)
{
	if (auto widget = Cast<UUserWidget>(_scrollBox->GetChildAt(index)))
	{
		RemoveScrollWidget(widget);
	}
}

void UGsUIBattlePassLevelHelper::ClearAllScrollWidgets()
{
	TArray<UUserWidget*> cachedWidgets;
	GetAllScrollWidgets(cachedWidgets);

	_cachedWidgets.Append(cachedWidgets);
	_scrollBox->ClearChildren();
}

UUserWidget* UGsUIBattlePassLevelHelper::GetScrollWidget(int index)
{
	return Cast<UUserWidget>(_scrollBox->GetChildAt(index));
}

void UGsUIBattlePassLevelHelper::GetAllScrollWidgets(OUT TArray<UUserWidget*>& widgets)
{
	TArray<UWidget*> arr = _scrollBox->GetAllChildren();
	Algo::TransformIf(arr, widgets, [](auto e) { return Cast<UUserWidget>(e) != nullptr; }, [](auto e) { return Cast<UUserWidget>(e); });
}
