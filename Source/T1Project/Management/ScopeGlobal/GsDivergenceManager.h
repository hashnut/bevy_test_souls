// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsSingleton.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "GsDivergenceManager.generated.h"


/*
* 0. 기능은 최대한 한군데에 작업하기, Manager특성상 다른군데서 활용할 코드도 드믈고 Unreal모듈을 또 랩핑하는건 이중으로 하는 너낌, 코드 분석, 디버깅하기 용이, 나름 Unreal스타일
*    무엇보다 프로세스 흐름, 분석, 디버깅에 용이
* 1. Divergence 파일 CDN에서 다운
* 2. Parsing 후 데이터 Save
* 3. 원하는 정보 Get지원
*/

class IHttpRequest;

USTRUCT()
struct FDivergenceInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString ResetDataVersion;
};

USTRUCT()
struct FDivergence
{
	GENERATED_BODY()

	UPROPERTY()
	FString Platform;

	UPROPERTY()
	FString ClientVersion;

	// Main PAK 패치버전(이름)이다. 이미 받은게 있어서 같은 Cloud공간에서 패키징한거라면 변경된걸 알아서 받는다.
	UPROPERTY()
	FString PatchVersion;

	// Base에서 Chunk0 외에 변경된 리소스 버전만 있는 pak, _P가 붙는다 자동으로 반드시 기준 Base(PatchVersion) 버전이 있어야 된다. 패키징시 Base기준으로 만들기에
	UPROPERTY()
	FString DLCVersion;

	// CDN root주소 여기 Base에 Service, Platfrom이 붙어서 최종 CDN Cloud 경로를 완성한다.
	UPROPERTY()
	FString CdnUrl;

	UPROPERTY()
	FString Service;

	// Service 가 가리키는 CDN 폴더명을 아래로 강제 지정할때 사용(없을때는 기존처럼 Service)
	UPROPERTY()
	FString ServiceDirOverride;

	UPROPERTY()
	FString RegionServerId;

	UPROPERTY()
	TArray<FString> AuthAgentList;

	UPROPERTY()
	TArray<FString> AuthAgentName;

	UPROPERTY()
	FString AuthAgentPort;

	UPROPERTY()
	FString ResetData;

	UPROPERTY()
	FString Patching;

	UPROPERTY()
	FString UseIAP;
};

UCLASS()
class T1PROJECT_API UGsDivergenceManager :
	public UObject,
	public TGsSingleton<UGsDivergenceManager>,
	public IGsManager
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;
	virtual void Finalize() override;
	
	const FString& GetPlatform() { return SelectedDivergence.Platform;	}
	const FString& GetClientVersion() { return SelectedDivergence.ClientVersion; }
	float GetClientVersionFloat() { return ClientVersionFloat; }
	const FString& GetPatchVersion() { return SelectedDivergence.PatchVersion; }
	const FString& GetDLCVersion() { return SelectedDivergence.DLCVersion; }
	const FString& GetCdnUrl() { return SelectedDivergence.CdnUrl; }
	const FString& GetService() { return SelectedDivergence.Service; }
	const FString& GetServiceDirOverride() { return SelectedDivergence.ServiceDirOverride; }
	const TArray<FString>& GetAuthAgentList() { return SelectedDivergence.AuthAgentList; }
	const FString& GetRegionServerId() { return SelectedDivergence.RegionServerId; }
	const TArray<FString>& GetAuthAgentNameList() { return SelectedDivergence.AuthAgentName; }
	const FString& GetAuthAgentPort() { return SelectedDivergence.AuthAgentPort; }	
	const FString& GetUseIAP() { return SelectedDivergence.UseIAP; }
	bool IsResetData();
	bool PatchingProcessOn();

	const FString& GetLocalClientVersion() { return SelectedDivergence.ClientVersion; }
	
	void TryLoadDivergenceFile(int TryNumber = 0);

public:
	const bool IsUseIAP();
	FString GetProjectVersion();

private:
	bool ParsingAndSelectDivergence(const FString& DivergenceString);
	bool WriteStringAsUtf8TextFile(const FString& FileText, const FString& FilePath);
	bool IsFindDivergenceInfoFile();
	void SaveDivergenceInfoFile();
	bool ParsingAndLoadDivergenceInfoFile();
	void LoadLocalSelectDivergenceFile();

private:
	// divergence download request
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> DivergenceRequest;

	// result error text
	FText LastError;

	// Selected Divergence
	FDivergence SelectedDivergence; // url에서 리스트에서 버전에 맞게 셋팅된 파일
	FDivergenceInfo DivergenceInfo;

	float ClientVersionFloat = 1.f;
};

using GsDivergenceManagerSingle = TGsSingleton<UGsDivergenceManager>;
#define GDivergence() GsDivergenceManagerSingle::Instance()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////