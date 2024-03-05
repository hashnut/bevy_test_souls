// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "GsPathManager.generated.h"


UENUM(BlueprintType)
enum class EGsPathType : uint8
{	
	UI,			// UUserWidget 상속 에셋
	GameObject, // Actor 상속 혹은 GameObject에 관련된 에셋
	Asset,		// 그 외의 기타 에셋. 개수가 많아지면 종류별로 분리

	Max
};

/**
 * 게임 내 여러 경로를 관리. UDataTable에 저장된 경로를 통해 Path, UClass 등을 리턴.
 * FGsSchemaPathData 사용.
 */
UCLASS()
class T1PROJECT_API UGsPathManager : 
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

public:
	static UClass* GetClassStatic(const FSoftObjectPath& InPath);

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	FString GetPath(EGsPathType InType, const FName& InKey);
	FSoftObjectPath GetObjectPath(EGsPathType InType, const FName& InKey);
	FString GetClassPath(EGsPathType InType, const FName& InKey);
	UClass* GetClass(EGsPathType InType, const FName& InKey);
	
	// ObjectPath를 받아 _C를 붙여 ClassMap을 검색, 없으면 로드 후 저장한다.
	UClass* GetClassByObjectPath(const FSoftObjectPath& InPath);

	// 주의: Path/ClassPath를 얻을때만 사용. UClass 얻을 때는 GetClass 함수 쓸 것.
	const class UGsTablePathData* GetTable(EGsPathType InType);
};


#define GPath() UGsScopeHolder::GetGlobalManager<UGsPathManager>(UGsScopeGlobal::EManagerType::Path)