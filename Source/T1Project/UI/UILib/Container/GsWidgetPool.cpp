// Fill out your copyright notice in the Description page of Project Settings.


#include "GsWidgetPool.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"


void UGsWidgetPool::Initialize(TSubclassOf<class UUserWidget> InWidgetClass, int32 InWarmUpCount, TFunction<void(UUserWidget*)> InReleaseFunc)
{
	_widgetClass = InWidgetClass;
	_releaseFunc = InReleaseFunc;
	_lastCleanTime = 0.f;

	if (0 < InWarmUpCount)
	{
		WarmUp(InWarmUpCount);
	}
}

int32 UGsWidgetPool::GetTotalRunningCount()
{
	return _inactiveArray.Num() + _activeArray.Num();
}

UUserWidget* UGsWidgetPool::CreateOrGetInstance()
{
	if (false == IsValidLowLevel())
	{
		return nullptr;
	}

	UUserWidget* widgetInstance = nullptr;

	if (0 == _inactiveArray.Num())
	{
		widgetInstance = CreateWidgetInternal();
	}
	else
	{
		FGsWidgetPoolElement element = _inactiveArray.Pop();
		widgetInstance = element._widget;
	}

	if (widgetInstance)
	{
		_activeArray.Add(widgetInstance);

		// 로딩 과정에서 MyWidget이 날라가는 경우가 있음. 
		// TakeWidget 을 통해 없을 경우, 다시 생성하도록 처리.
		widgetInstance->TakeWidget();
	}

	return widgetInstance;
}

void UGsWidgetPool::Release(UUserWidget* InWidget)
{
	int32 index = _activeArray.Find(InWidget);
	if (INDEX_NONE != index)
	{
		_inactiveArray.Emplace(FGsWidgetPoolElement(InWidget, GetCurrTime()));
		_activeArray.RemoveAt(index);
	}

	if (nullptr != _releaseFunc)
	{
		_releaseFunc(InWidget);
	}
}

void UGsWidgetPool::ReleaseAll()
{
	float currTime = GetCurrTime();

	for (UUserWidget* widget : _activeArray)
	{
		//if (widget->IsInViewport())
		{
			widget->RemoveFromParent();
		}

		_inactiveArray.Emplace(FGsWidgetPoolElement(widget, currTime));

		if (nullptr != _releaseFunc)
		{
			_releaseFunc(widget);
		}
	}

	_activeArray.Empty();
}

void UGsWidgetPool::WarmUp(int32 InWarmupCount)
{
	float currTime = GetCurrTime();

	// 현재 있는 개수에서 모자란 만큼만 생성
	int32 count = InWarmupCount - _inactiveArray.Num();
	if (0 < count)
	{
		for (int32 i = 0; i < count; ++i)
		{
			if (UUserWidget* widgetInstance = CreateWidgetInternal())
			{
				_inactiveArray.Emplace(FGsWidgetPoolElement(widgetInstance, currTime));
			}
		}
	}
}

void UGsWidgetPool::SetReleaseFunc(TFunction<void(UUserWidget*)> InReleaseFunc)
{
	_releaseFunc = InReleaseFunc;
}

void UGsWidgetPool::KillUnusedWidgets(float InKillTime)
{
	int32 i = 0;

	while (i < _inactiveArray.Num())
	{
		if (_inactiveArray[i]._lastUsedTime < InKillTime)
		{
			// 오랫동안 사용하지 않은 항목 제거
			_inactiveArray[i]._widget = nullptr;
			_inactiveArray.RemoveAtSwap(i, 1, false);
		}
		else
		{
			++i;
		}
	}
	_inactiveArray.Shrink();

	float currTime = GetCurrTime();
	_lastCleanTime = (0 < currTime) ? currTime : 0.f;
}

UUserWidget* UGsWidgetPool::CreateWidgetInternal()
{
	if (false == IsValidLowLevel())
	{
		return nullptr;
	}

	if (nullptr == _widgetClass ||
		false == _widgetClass->IsValidLowLevel())
	{
		return nullptr;
	}

	// 월드로 넣어도 내부적으로 Instance가 있으면 Outer로 설정하도록 되어있음
	UWorld* world = GetWorld();
	if (nullptr == world)
	{
		return nullptr;
	}

	return CreateWidget(world, _widgetClass);
}

void UGsWidgetPool::Empty()
{
	for (UUserWidget* widget : _activeArray)
	{
		widget->RemoveFromParent();
	}
	_activeArray.Empty();

	_inactiveArray.Empty();
	_lastCleanTime = 0.f;
}

float UGsWidgetPool::GetCurrTime() const
{	
	if (UWorld* world = GetWorld())
	{
		return world->GetTimeSeconds();
	}

	return 0.f;
}
