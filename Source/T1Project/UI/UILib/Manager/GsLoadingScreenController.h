// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageUI.h"
#include "GsLoadingScreenController.generated.h"

class UUserWidget;
class UGsUILoadingScreen;
struct FGsSchemaLoadingScreenGroup;
struct FGsSchemaLoadingScreen;
struct FGsSchemaLoadingScreenPlayTip;

/**
 * 로딩화면 관련 로직 및 리소스 관리
 */
UCLASS()
class T1PROJECT_API UGsLoadingScreenController : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	UGsUILoadingScreen* _loadingScreenForModule; // 모듈에 넘기기용
	UPROPERTY(Transient)
	UGsUILoadingScreen* _loadingScreenUI; // 모듈전후에 일반 UI를 켜서 공백을 감추기 위함

private:
	bool _bIsAfterDownload = false;
	int32 _defaultGroupId = 0;
	int32 _invadeGroupId = 0;

public:
	void Initialize();
	void Finalize();
	void InitializeAfterResourceDownload();

	void OnPrevLoadMap();
	void OnPostLoadMap();

private:
	void PreCreateLoadingWidget();	

public:
	void ShowLoading(int32 InMapId, bool bIsInvadeWorld, int32 InPlayerLevel);
	void HideLoading();

	UGsUILoadingScreen* GetLoadingWidgetForModule() const { return _loadingScreenForModule; }
	bool IsVisible() const;

private:
	const FGsSchemaLoadingScreenGroup* GetLoadingGroup(int32 InMapId, bool bIsInvadeWorld);
	void GetTipIdList(int32 InPlayerLevel, OUT TArray<const FGsSchemaLoadingScreenPlayTip*>& OutList);
};
