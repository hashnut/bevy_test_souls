// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGVSPlatform.h"

#include "Kismet/GameplayStatics.h"
#include "T1Project.h"

#define LOG_DIVERGENCE

FGsGVSPlatform::FGsGVSPlatform()
{
	_mapTypeStr.Add(EGsServiceType::live, TEXT("live"));
	_mapTypeStr.Add(EGsServiceType::submission, TEXT("submission"));
	_mapTypeStr.Add(EGsServiceType::qa, TEXT("qa"));
	_mapTypeStr.Add(EGsServiceType::dev, TEXT("dev"));
	_mapTypeStr.Add(EGsServiceType::Test01, TEXT("Test01"));
	_mapTypeStr.Add(EGsServiceType::Test02, TEXT("Test02"));
	_mapTypeStr.Add(EGsServiceType::MAX, TEXT("MAX"));
}

FGsGVSPlatform::~FGsGVSPlatform()
{
	// 맵 구성원 제거
	_mapTypeStr.Empty();
}


void FGsGVSPlatform::SetInfo(TArray<FGsGVSInfo>& In_arrInfo)
{
	// 초기화
	// struct 메모리 초기화는 언제
	_mapTypeGVS.Reset();

	// 모든 gvs에서 
	for (auto& iter:In_arrInfo)
	{
		// 현재 플랫폼과 같다면
		if (iter.IsCurrentPlatform() == true)
		{
			for (auto& typeIter: _mapTypeStr)
			{
				// 서비스 타입이 같으면
				if (iter.IsSameServiceType(typeIter.Value) == true)
				{
					// 추가
					_mapTypeGVS.Add(typeIter.Key, iter);

//#ifdef LOG_DIVERGENCE
					GSLOG( Log,
						TEXT("typeStr: %s, platform: %s, cdn_suburl: %s, chunk_num: %s" ), 
						*typeIter.Value,
						*iter.platform, 
						*iter.cdn_suburl,
						*iter.chunk_num);
//#endif
				}
				
			}
		}
	}
}
// 기준이 되는 라이브 gvs 정보가 존재하는지 확인 
bool FGsGVSPlatform::VerifyliveGvs()
{
	// 키값이 있는지?
	bool isExist = _mapTypeGVS.Contains(EGsServiceType::live);

	if (isExist == false)
	{
		return false;
	}
	// 값이 nullptr이 아닌지
	return (_mapTypeGVS.Find(EGsServiceType::live) != nullptr);
}

// 접근자
FGsGVSInfo* FGsGVSPlatform::GetTypeGVS(EGsServiceType In_type)
{
	return _mapTypeGVS.Find(In_type);
}

// 라이브 버전과 동일한지 여부 검사
bool FGsGVSPlatform::EqualLiveVersion(const FString& In_versionText)
{
	//if (false == VerifyliveGvs())
	//{
	//	return false;
	//}

	FGsGVSInfo* findInfo = _mapTypeGVS.Find(EGsServiceType::live);
	if (nullptr == findInfo)
	{
		return false;
	}
	return findInfo->IsIncludeVersion(In_versionText);
}

// 클라이언트 버전이 라이브버전보다 적은지 검사
bool FGsGVSPlatform::ClientVersionLessLiveVersion(const FString& In_versionText)
{
	float clientVer = FCString::Atof(*In_versionText);

	FGsGVSInfo* info = GetTypeGVS(EGsServiceType::live);

	if (nullptr == info)
	{
		return false;
	}
	return !info->IsLessInVersion(clientVer);
}
// 일치하는 버전을 찾아서 넘겨준다
FGsGVSInfo* FGsGVSPlatform::GetMatchedGVS(const FString& In_version)
{
	for (auto& iter: _mapTypeGVS)
	{
		if (iter.Value.IsIncludeVersion(In_version))
		{
			return &iter.Value;
		}
	}
	return nullptr;
}