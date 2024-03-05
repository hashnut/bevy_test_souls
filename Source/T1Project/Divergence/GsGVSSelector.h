// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FGsGVSPlatform;
struct FGsGVSInfo;
/**
 * 
 */
class FGsGVSSelector
{
	// 멤버 변수
private:
	// gvs 선택 여부
	bool _hasSelect = false;
	// 클라이언트 업데이트 필요성(마켓)
	bool _isNeedClientUpdate = false;
	// 현재 클라이언트 버전
	FString _clientVersion;
	// 선택된 인포
	FGsGVSInfo* _selectedInfo;

	// 생성자, 소멸자
public:
	FGsGVSSelector();
	virtual ~FGsGVSSelector();

	// 로직 함수
public:	
	// 클라이언트 버전 로드
	void LoadClientVersion();
	void DoSelect(FGsGVSPlatform* In_platform);

	// get, set
public:
	// 선택 되었는가?
	bool HasSelected()
	{
		return _hasSelect;
	}
	bool IsNeedClientUpdate()
	{
		return _isNeedClientUpdate;
	}

	FGsGVSInfo* GetSelectedGvsInfo()
	{
		return _selectedInfo;
	}

};
