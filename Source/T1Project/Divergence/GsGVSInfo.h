#pragma once

#include "CoreMinimal.h"
#include "GsGVSInfo.generated.h"

// gvs 기본 정보
// parse(FJsonObjectConverter::JsonArrayStringToUStruct)하기 위해서 
// 멤버 이름이 json 파일과 이름이 같아야 함
USTRUCT()
struct FGsGVSInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString platform;	

	UPROPERTY()
	TArray<FString> version;

	UPROPERTY()
	TArray<FString> patch_version;

	UPROPERTY()
	FString chunk_num;

	UPROPERTY()
	FString cdn_suburl;

	UPROPERTY()
	FString service_type;

	UPROPERTY()
	FString server_id;

	UPROPERTY()
	TArray<FString> auth_agent;

	UPROPERTY()
	FString auth_agent_port;	

public:
	// 로그 출력
	void PrintLog();
	// 현재 플랫폼과 같은지?
	bool IsCurrentPlatform();
	// 서비스 타입이 동일한지?
	bool IsSameServiceType(const FString& In_str);
	// 버전 정보 파일을 가지고 있는지 검사
	bool IsIncludeVersion(const FString& In_versetText);
	// 문서정보에 의거 Version 비교시 version element는 어레이로 구성되기에
	// 그중 가장 높은 버전과 비교한다
	bool IsLessInVersion(float In_version);
};