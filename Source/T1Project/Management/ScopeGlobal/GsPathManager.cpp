// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPathManager.h"
#include "GsTableManager.h"
#include "DataSchema/GsSchemaPathData.h"
#include "T1Project.h"


UClass* UGsPathManager::GetClassStatic(const FSoftObjectPath& InPath)
{
	if (InPath.IsValid())
	{
		FString strPath = InPath.ToString() + TEXT("_C");
		if (UObject* objClass = StaticLoadObject(UClass::StaticClass(), NULL, *strPath, NULL, LOAD_NoWarn, NULL))
		{
			return Cast<UClass>(objClass);
		}
	}

	return nullptr;
}

void UGsPathManager::Initialize()
{
}

void UGsPathManager::Finalize()
{
	//ClassMap.Empty();
	//ClassList.Empty();
}

const UGsTablePathData* UGsPathManager::GetTable(EGsPathType InType)
{
	FString strTableName;
	switch (InType)
	{
	case EGsPathType::UI:
		strTableName = TEXT("DTGsPathDataUI");
		break;
	case EGsPathType::GameObject:
		strTableName = TEXT("DTGsPathDataGameObject");
		break;
	case EGsPathType::Asset:
		strTableName = TEXT("DTGsPathDataAsset");
		break;
	
	// ... Ãß°¡
	}

	const UGsTableManager* tblManager = &(UGsTableManager::GetInstance());
	if (tblManager == nullptr)
	{
		return nullptr;
	}
	const UGsTable* table = tblManager->GetTable(FGsSchemaPathData::StaticStruct(), strTableName);
	if (table)
	{
		return Cast<UGsTablePathData>(table);
	}

	GSLOG(Error, TEXT("[Path] Fail to get table. pathType: %d"), InType);
	return nullptr;
}

FString UGsPathManager::GetPath(EGsPathType InType, const FName& InKey)
{
	return GetObjectPath(InType, InKey).ToString();
}

FSoftObjectPath UGsPathManager::GetObjectPath(EGsPathType InType, const FName& InKey)
{
	if (const UGsTablePathData* table = GetTable(InType))
	{
		const FGsSchemaPathData* row = nullptr;
		if (table->FindRow(InKey, row))
		{
			return row->path;
		}
	}

	GSLOG(Error, TEXT("[Path] Path is empty. EGsPathType: %d, Key: %s"), InType, *InKey.ToString());

	return FSoftObjectPath();
}

FString UGsPathManager::GetClassPath(EGsPathType InType, const FName& InKey)
{
	FString path = GetPath(InType, InKey);
	if (false == path.IsEmpty())
	{
		return path.Append(TEXT("_C"));
	}

	return path;
}

UClass* UGsPathManager::GetClass(EGsPathType InType, const FName& InKey)
{
	return GetClassStatic(GetObjectPath(InType, InKey));
}

UClass* UGsPathManager::GetClassByObjectPath(const FSoftObjectPath& InPath)
{
	return GetClassStatic(InPath);

	/*
	UClass* targetClass = ClassMap.FindRef(classObjPath.GetAssetPathName());
	if (nullptr == targetClass)
	{
		TSoftClassPtr<UObject> classPtr = TSoftClassPtr<UObject>(classObjPath);
		targetClass = classPtr.LoadSynchronous();
		ClassList.AddUnique(targetClass);
		ClassMap.Add(classObjPath.GetAssetPathName(), targetClass);
	}
	return targetClass;
	*/
}
