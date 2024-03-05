// Fill out your copyright notice in the Description page of Project Settings.


#include "GsScreenFilterLocalPlayer.h"
#include "../../Management/ScopeGlobal/GsUIManager.h"
#include "DataSchema/GameObject/Common/GsSchemaScreenFilterInfo.h"


void FGsScreenFilterLocalPlayer::Initialize(class UGsGameObjectBase* Owner)
{
	_owner = Owner;
}

void FGsScreenFilterLocalPlayer::Finalize()
{
	_owner = nullptr;

	if (nullptr != _currentActive)
	{
		FName uiName = _currentActive->Data->uiName;
		if (GUI()->IsActiveWidget(uiName))
		{
			GUI()->CloseByKeyName(uiName);
		}
	}

	_activeFilters.Empty();
}

void FGsScreenFilterLocalPlayer::ActiveScreenFilter(EGsScreenFilterType ScreenFilterType, 
	const FGsSchemaScreenFilterInfo& ScreenFilterInfo)
{
	// 활성화된 필터에서 찾아서 
	FGsFilterRef* activeFilter = _activeFilters.FindByPredicate([ScreenFilterType](const FGsFilterRef& elem)->bool
		{
			return elem.Type == ScreenFilterType;
		}
	);

	if (nullptr != activeFilter)
	{
		//RefCount를 증가
		activeFilter->RefCount++;
	}
	else
	{
		// 신규 추가
		FGsFilterRef one;
		one.Type = ScreenFilterType;
		one.Data = &ScreenFilterInfo;
		one.RefCount++;
		_activeFilters.Add(one);
	}

	// 소팅을 통해 우선순위 정렬
	_activeFilters.Sort([](const FGsFilterRef& elemA, const FGsFilterRef& elemB)
		{
			return elemA.Type > elemB.Type;
		}
	);

	FGsFilterRef* NewFilter = &_activeFilters[0];

	// 활성화된 필터와 현재 필터관계정리
	if (nullptr != _currentActive)
	{
		// 다르다면 필터교체
		if (_currentActive->Type != NewFilter->Type)
		{
			// 이전것 삭제
			GUI()->CloseByKeyName(_currentActive->Data->uiName);
			_currentActive = NewFilter;
			FName uiName = _currentActive->Data->uiName;
			
			// 필터 교체
			check(GUI()->IsActiveWidget(uiName));
			GUI()->Open(uiName);
		}
	}
	else
	{
		_currentActive = NewFilter;
		FName uiName = _currentActive->Data->uiName;

		// 필터 교체
		check(!GUI()->IsActiveWidget(uiName));
		GUI()->Open(uiName);
	}
	//ShowScreenFilter(ScreenFilterInfo, (int)ScreenFilterType);
}

void FGsScreenFilterLocalPlayer::DeactiveScreenFilter(EGsScreenFilterType ScreenFilterType)
{
	FGsFilterRef* activeFilter = _activeFilters.FindByPredicate([ScreenFilterType](const FGsFilterRef& elem)->bool
		{
			return elem.Type == ScreenFilterType;
		}
	);

	if (nullptr != activeFilter)
	{
		//RefCount를 증가
		activeFilter->RefCount--;

		//완전제거일때
		if (0 >= activeFilter->RefCount)
		{			
			//혹시 화면에 출력중이라면
			if (_currentActive->Type == ScreenFilterType)
			{
				if (activeFilter == _currentActive)
				{
					GUI()->CloseByKeyName(activeFilter->Data->uiName);
					_currentActive = nullptr;
				}
			}
			FGsFilterRef refFilter = *activeFilter;
			_activeFilters.Remove(refFilter);
		}
	}
	else
	{
		GSLOG(Error, TEXT("DeactiveScreenFilter call But Can't Find Instance [%d].. Check RefCount"), ScreenFilterType);
	}

	// 화면에 출력중이던것이 삭제되었다면 잔여 필터중 우선순위 높은것을 출력한다.
	if (nullptr == _currentActive)
	{
		if (0 != _activeFilters.Num())
		{
			// 소팅을 통해 우선순위 정렬
			_activeFilters.Sort([](const FGsFilterRef& elemA, const FGsFilterRef& elemB)
				{
					return elemA.Type > elemB.Type;
				}
			);

			FGsFilterRef* NewFilter = &_activeFilters[0];
			_currentActive = NewFilter;
			FName uiName = _currentActive->Data->uiName;

			// 필터 교체
			check(GUI()->IsActiveWidget(uiName));
			GUI()->Open(uiName);
		}
	}

#if WITH_EDITOR
	if (nullptr != _currentActive)
	{
		check(_currentActive->RefCount);
	}	
#endif

}

