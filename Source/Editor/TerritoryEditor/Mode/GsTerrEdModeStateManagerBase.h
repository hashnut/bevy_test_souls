
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor.h"

template<class T>
class FGsTerrEdModeStateManagerBase
{
public:
	TMap<FName, TSharedPtr<T>> _stateMap;
	TWeakPtr<T> _currentState;
	FName _currentStateName;

public:
	FGsTerrEdModeStateManagerBase();
	virtual ~FGsTerrEdModeStateManagerBase()
	{
		for (TPair<FName, TSharedPtr<T>> mode : _stateMap)
		{
			if (mode.Value.IsValid())
			{
				mode.Value.Reset();
			}
		}

		_stateMap.Empty();
	}
	virtual void AddState(FName inName, TSharedPtr<T> inState)
	{
		_stateMap.Emplace(inName, inState);
	}
	virtual void ChangeState(FName inStateName)
	{
		if (_currentState.IsValid())
		{
			_currentState.Pin()->Exit();
		}

		if (_stateMap.Contains(inStateName))
		{
			_currentState = _stateMap[inStateName];
			_currentStateName = inStateName;

#if TERRITORY_EDITOR_DEBUG
			UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdModeStateManagerBase::ChangeState - %s"), *_currentStateName.ToString());
#endif
		}

		if (_currentState.IsValid())
		{
			_currentState.Pin()->Enter();
		}
	}
	virtual void Tick(float inTIck)
	{
		if (_currentState.IsValid())
		{
			_currentState.Pin().Get()->Tick(inTIck);
		}
	}
	virtual bool IsState(FName inName) { return _currentStateName == inName; }
	virtual TWeakPtr<T> GetState(FName inName)
	{
		if (_stateMap.Contains(inName))
		{
			return _stateMap[inName];
		}

		return TWeakPtr<T>();
	}

	virtual T* GetCurrentState()
	{
		return _currentState.Pin().Get();
	}

	FName GetCurrentStateName(){return _currentStateName;}
};

template<class T>
FGsTerrEdModeStateManagerBase<T>::FGsTerrEdModeStateManagerBase()
{

}


