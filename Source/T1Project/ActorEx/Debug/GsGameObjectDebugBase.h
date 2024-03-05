// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GsGameObjectDebugBase.generated.h"

// GameObject 디버그 정보 설정 구조체

// 최상위 기본 구조체
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsGameObjectDebugBase
{
	GENERATED_BODY()
};

// CreatureType
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsGameObjectDebugCreature
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectDebug, Meta = (Tooltip = "GameObject 상태 변환 콜스택 출력로그 설정"))
	bool _isFsmCallstackLog = false;

// #if WITH_EDITOR
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectDebug, Meta = (Tooltip = "GameObject 디버그 출력로그 갯수"))
// 	int _displayLogCount = 10;
//  	UPROPERTY(Transient, VisibleDefaultsOnly, Category = GameObjectDebug, Meta = (Tooltip = "GameObject 디버그 출력로그"))
//  	TArray<FString> _listLog;
// 
// 	void AddLog(const FString& log)
// 	{
//  		if (_listLog.Num() > _displayLogCount)
//  		{
//  			_listLog.RemoveAt(0);
//  		}
//  		_listLog.Emplace(log);
// 		UE_LOG(LogTemp, Warning, TEXT("[DebugGameObjectLog] [ %s ] %s"), *(FDateTime::Now().ToString()), *log);
// 	};
// #endif
};

// LocalPlayer
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsGameObjectDebugLocalPlayer
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectDebug, Meta=(Tooltip="타겟설정 옵션 : 설정 순서대로 우선순위 검색"))
	TArray<EGsAutoTargetSearchType> _autoTargetOption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectDebug, Meta=(Tooltip="타겟설정 옵션 : 설정 순서대로 우선순위 검색"))
	bool _isGlobalObjectsDisplay = false;
};
