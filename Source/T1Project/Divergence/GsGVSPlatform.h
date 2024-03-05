// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsRelatedToPatchType.h"
#include "GsGVSInfo.h"

// 플랫폼 정보
class FGsGVSPlatform
{
	// 멤버 변수
private:
	// 타입별 스트링
	TMap<EGsServiceType, FString> _mapTypeStr;
	// 타입별 gvs 정보
	TMap<EGsServiceType, FGsGVSInfo> _mapTypeGVS;

	// 생성자, 소멸자
public:		
	FGsGVSPlatform();
	virtual ~FGsGVSPlatform();

	// 로직 함수
public:
	// gvs json 데이터 가공
	void SetInfo(TArray<FGsGVSInfo>& In_arrInfo);
	// 기준이 되는 라이브 gvs 정보가 존재하는지 확인 
	bool VerifyliveGvs();
	// 라이브 버전과 동일한지 여부 검사
	bool EqualLiveVersion(const FString& In_versionText);
	// 클라이언트 버전이 라이브버전보다 적은지 검사
	bool ClientVersionLessLiveVersion(const FString& In_versionText);

	// get, set
public:
	// 접근자
	FGsGVSInfo* GetTypeGVS(EGsServiceType In_type);
	// 일치하는 버전을 찾아서 넘겨준다
	FGsGVSInfo* GetMatchedGVS(const FString& In_version);
};
