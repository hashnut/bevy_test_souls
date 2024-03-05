#include "GsManifestInfo.h"

#include "Math/NumericLimits.h"
#include "Kismet/GameplayStatics.h"
#include "T1Project.h"

// 로그 출력
//void FGsManifestInfo::PrintLog()
//{
//	GSLOG(Warning,
//		TEXT("filename: %s, uniqueFilename : %s, length: %s, URL: %s, hash: %s, hash256: %s"),
//		*filename, *uniqueFilename, *length, *URL, *hash, *hash256);
//}

//// 현재 플랫폼과 같은지?
//bool FGsGVFGsManifestInfoSInfo::IsCurrentPlatform()
//{
//	// 현재 기기의 플랫폼 이름 가져오기
//	FString devicePlatformName = TEXT("Android");// UGameplayStatics::GetPlatformName();
//
//	// 안드면
//	if (devicePlatformName.Compare(TEXT("Android")) == 0)
//	{
//		// 인포의 텍스트랑 비교
//		if (platform.Compare(TEXT("android")) == 0)
//		{
//			return true;
//		}
//	}
//	// 나머진 ios다
//	else
//	{
//		// 인포의 텍스트랑 비교
//		if (platform.Compare(TEXT("ios")) == 0)
//		{
//			return true;
//		}
//	}
//
//	return false;
//}
//
//// 서비스 타입이 동일한지?
//bool FGsManifestInfo::IsSameServiceType(const FString& In_str)
//{
//	// 같으면 true, 다르면 false
//	return (service_type.Compare(In_str) == 0) ? true : false;
//}
//
//// 버전 정보 파일을 가지고 있는지 검사
//bool FGsManifestInfo::IsIncludeVersion(const FString& In_versetText)
//{
//	for (auto& iter : version)
//	{
//		// 같은지?
//		if (iter.Compare(In_versetText) == 0)
//		{
//			return true;
//		}
//	}
//
//	// 해당 버전이 없다
//	return false;
//}
//
//// 문서정보에 의거 Version 비교시 version element는 어레이로 구성되기에
//// 그중 가장 높은 버전과 비교한다
//bool FGsManifestInfo::IsLessInVersion(float In_version)
//{
//	GSLOG(Log, TEXT("In_version %f"), In_version);
//
//	float maxVer = TNumericLimits<float>::Min();
//	for (auto& iter : version)
//	{
//		GSLOG(Log, TEXT("Target %s"), *iter);
//		float parsingVer = FCString::Atof(*iter);
//		if (maxVer < parsingVer)
//		{
//			maxVer = parsingVer;
//		}
//	}
//
//	return maxVer < In_version;
//}