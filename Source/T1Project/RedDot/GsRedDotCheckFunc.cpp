// Fill out your copyright notice in the Description page of Project Settings.

#include "GsRedDotCheckFunc.h"
#include "Management/ScopeGame/GsUnlockManager.h"

//----------------------------------------------------------------------------------------------------------------------
bool FGsRedDotCheckFunc::CheckRedDot(bool bInUpdate)
{
	if (_checkFunc)
	{
		_bIsRedDot = _checkFunc();
	}

	return _bIsRedDot;
}
//----------------------------------------------------------------------------------------------------------------------
bool FGsRedDotCheckFuncNumber::CheckRedDot(bool bInUpdate)
{
	if (_checkFunc)
	{
		_cachedCount = _checkFunc();
	}

	SetRedDot((0 < _cachedCount) ? true : false);

	return IsRedDot();
}
//----------------------------------------------------------------------------------------------------------------------
bool FGsRedDotCheckSubCheck::CheckRedDot(bool bInUpdate)
{
	int32 count = 0;

	if (bInUpdate)
	{
		for (IGsRedDotCheck* checker : _subCheckList)
		{
			if (checker->CheckRedDot(bInUpdate))
			{
				++count;
			}
		}
	}
	else
	{
		for (IGsRedDotCheck* checker : _subCheckList)
		{
			if (checker->IsRedDot())
			{
				++count;
			}
		}
	}

	_cachedCount = count;

	SetRedDot((0 < _cachedCount) ? true : false);

	return IsRedDot();
}

void FGsRedDotCheckSubCheck::Clear()
{
	_subCheckList.Empty();
	_cachedCount = 0;
}

void FGsRedDotCheckSubCheck::AddTarget(IGsRedDotCheck* InTarget)
{
	_subCheckList.Emplace(InTarget);
}

//----------------------------------------------------------------------------------------------------------------------
bool FGsRedDotCheckQuickMenu::CheckRedDot(bool bInUpdate /*= true*/)
{
	int32 count = 0;

	if (bInUpdate)
	{
		for (TPair<IGsRedDotCheck*, EGsUnlockContentsType> checker : _subCheckList)
		{
			if (false == GSUnlock()->IsContentsUnlock(checker.Value))
			{
				continue;
			}

			if (GSUnlock()->IsNewContents(checker.Value))
			{
				++count;
				continue;
			}

			if (checker.Key && checker.Key->CheckRedDot(bInUpdate))
			{
				++count;
			}
		}
	}
	else
	{
		for (TPair<IGsRedDotCheck*, EGsUnlockContentsType> checker : _subCheckList)
		{
			if (false == GSUnlock()->IsContentsUnlock(checker.Value))
			{
				continue;
			}

			if (GSUnlock()->IsNewContents(checker.Value))
			{
				++count;
				continue;
			}
			
			if (checker.Key && checker.Key->IsRedDot())
			{
				++count;
			}
		}
	}

	_cachedCount = count;

	SetRedDot((0 < _cachedCount) ? true : false);

	return IsRedDot();
}

void FGsRedDotCheckQuickMenu::Clear()
{
	_subCheckList.Empty();
	_cachedCount = 0;
}

void FGsRedDotCheckQuickMenu::AddTarget(IGsRedDotCheck* InTarget, EGsUnlockContentsType InContentsType)
{
	_subCheckList.Emplace(InTarget, InContentsType);
}
