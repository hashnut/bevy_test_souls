// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "RedDot/GsRedDotCheck.h"
#include "Unlock/GsUnlockDefine.h"

//----------------------------------------------------------------------------------------------------------------------
/*
 * 레드닷 편의용 클래스
 */
class T1PROJECT_API FGsRedDotCheckFunc : public IGsRedDotCheck
{
protected:
	TFunction<bool()> _checkFunc;

public:
	FGsRedDotCheckFunc() = default;
	explicit FGsRedDotCheckFunc(TFunction<bool()> InFunc)
		: _checkFunc(InFunc)
	{
		SetRedDot(false);
	}
	virtual ~FGsRedDotCheckFunc() = default;

public:
	virtual bool CheckRedDot(bool bInUpdate = true) override;

public:
	void SetCheckFunction(TFunction<bool()> InFunc) { _checkFunc = InFunc; }
};

//----------------------------------------------------------------------------------------------------------------------
/*
 * 레드닷 편의용 클래스. 레드닷 숫자를 사용할 경우
 */
class T1PROJECT_API FGsRedDotCheckFuncNumber : public IGsRedDotCheck
{
protected:
	int32 _cachedCount = 0;
	TFunction<int32()> _checkFunc;

public:
	FGsRedDotCheckFuncNumber() = default;
	explicit FGsRedDotCheckFuncNumber(TFunction<int32()> InFunc)
		: _checkFunc(InFunc)
	{
		SetRedDot(false);
	}
	virtual ~FGsRedDotCheckFuncNumber() = default;

public:
	virtual bool CheckRedDot(bool bInUpdate = true) override;
	virtual int32 GetRedDotCount() const override { return _cachedCount; }

public:
	void SetCheckFunction(TFunction<int32()> InFunc) { _checkFunc = InFunc; }
};

//----------------------------------------------------------------------------------------------------------------------
/*
 * 레드닷 편의용 클래스. 하위 항목을 체크할 경우
 */
class T1PROJECT_API FGsRedDotCheckSubCheck : public IGsRedDotCheck
{
protected:
	TArray<IGsRedDotCheck*> _subCheckList; // 포인터만 채울 것
	int32 _cachedCount = 0;

public:
	FGsRedDotCheckSubCheck() = default;
	virtual ~FGsRedDotCheckSubCheck() = default;

public:
	virtual bool CheckRedDot(bool bInUpdate = true);
	virtual int32 GetRedDotCount() const override { return _cachedCount; }

public:
	void Clear();
	void AddTarget(IGsRedDotCheck* InTarget);
};

//----------------------------------------------------------------------------------------------------------------------
/*
 * 레드닷 퀵메뉴 클래스. 하위 항목 및 언락 체크 
 */
class T1PROJECT_API FGsRedDotCheckQuickMenu : public IGsRedDotCheck
{
protected:
	TArray<TPair<IGsRedDotCheck*, EGsUnlockContentsType>> _subCheckList;
	int32 _cachedCount = 0;

public:
	FGsRedDotCheckQuickMenu() = default;
	virtual ~FGsRedDotCheckQuickMenu() = default;

public:
	virtual bool CheckRedDot(bool bInUpdate = true);
	virtual int32 GetRedDotCount() const override { return _cachedCount; }

public:
	void Clear();
	void AddTarget(IGsRedDotCheck* InTarget, EGsUnlockContentsType InContentsType);
};
