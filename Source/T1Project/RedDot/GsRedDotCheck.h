// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

/*
 * 레드닷 체크 기본 
 */
class T1PROJECT_API IGsRedDotCheck
{
protected:
	bool _bIsRedDot = false; // 결과 캐싱해놓음

public:
	IGsRedDotCheck() = default;
	virtual ~IGsRedDotCheck() = default;

public:
	// CheckRedDot을 통해 _bCachedRedDot 값을 채울 것
	virtual bool CheckRedDot(bool bInUpdate = true) = 0;
	virtual int32 GetRedDotCount() const { return (_bIsRedDot) ? 1 : 0; }

public:
	bool IsRedDot() const { return _bIsRedDot; }
	void SetRedDot(bool bIsRedDot) { _bIsRedDot = bIsRedDot; }
};
