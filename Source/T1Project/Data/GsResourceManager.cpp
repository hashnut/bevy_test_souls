// Fill out your copyright notice in the Description page of Project Settings.


#include "GsResourceManager.h"
#include "T1Project.h"

void UGsResourceManager::Initialize()
{
}

void UGsResourceManager::Finalize()
{	
	CleanUp();
}

void UGsResourceManager::Update(float inTick)
{
	TArray<FAsyncLoadInfo*> remove;
	for (int i = 0; i < TryAsyncLoad.Num(); ++i)
	{
		if (TryAsyncLoad[i]->CallLoadComplete)
		{
			if (TryAsyncLoad[i]->ArrayLoadedDelegate.Num() != 0)
			{
				TryAsyncLoad[i]->LoadComplete();
			}
			remove.Add(TryAsyncLoad[i]);
		}
	}
	for (int j = 0; j < remove.Num(); ++j)
	{
		TryAsyncLoad.Remove(remove[j]);
		if (remove[j]->AsyncLoadHandle.IsValid())
		{
			remove[j]->AsyncLoadHandle->ReleaseHandle();
			remove[j]->AsyncLoadHandle.Reset();
			remove[j]->Path.Reset();
		}
		PoolLoadInfo.Release(remove[j]);
	}
}

void UGsResourceManager::CancelAll()
{
	if (!ensure(IsInGameThread()))
	{	// 2023/6/1 PKT - 게임 스레드 검사 추가
		return;
	}

	/**
	 * 2023/6/1 PKT - 로딩 스레드로 전환 하기 이전 요청한 비동기 로딩은 모두 취소
	 */	
	for (int32 idx = 0; idx < TryAsyncLoad.Num(); ++idx)
	{

		if (TryAsyncLoad[idx]->AsyncLoadHandle.IsValid())
		{
			TryAsyncLoad[idx]->AsyncLoadHandle->CancelHandle();
			TryAsyncLoad[idx]->AsyncLoadHandle->ReleaseHandle();
			TryAsyncLoad[idx]->AsyncLoadHandle.Reset();
			TryAsyncLoad[idx]->ArrayLoadedDelegate.Empty();
			TryAsyncLoad[idx]->Path.Reset();
			TryAsyncLoad[idx]->CallLoadComplete = false;
		}
		PoolLoadInfo.Release(TryAsyncLoad[idx]);
	}
	TryAsyncLoad.Empty();
}

void UGsResourceManager::CleanUp()
{
	for (int i = 0; i < TryAsyncLoad.Num(); ++i)
	{
		if (TryAsyncLoad[i]->AsyncLoadHandle.IsValid())
		{
			TryAsyncLoad[i]->AsyncLoadHandle->ReleaseHandle();
			TryAsyncLoad[i]->AsyncLoadHandle.Reset();
			TryAsyncLoad[i]->Path.Reset();
		}
		PoolLoadInfo.Release(TryAsyncLoad[i]);
	}
	TryAsyncLoad.Empty();
	PoolLoadInfo.RemovePool();
}

void  UGsResourceManager::LoadAsync(const FSoftObjectPath& Path, FStreamableDelegateParam LoadedDelegate, bool IsUClass)
{
	if (!ensure(IsInGameThread()))
	{	// 2023/6/1 PKT - 게임 스레드 검사 추가
		return;
	}

	FSoftObjectPath path = IsUClass ? Path.ToString() + TEXT("_C") : Path.ToString();
	
	FAsyncLoadInfo** findOne = TryAsyncLoad.FindByPredicate([&path](const FAsyncLoadInfo* rhs)->bool {
		return rhs->IsSameType(path);
		});

	if (nullptr == findOne )
	{
		FAsyncLoadInfo* info = PoolLoadInfo.Claim();
		info->Path = path;
		info->CallLoadComplete = false;
		info->ArrayLoadedDelegate.Add(LoadedDelegate);
		info->AsyncLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(path, FStreamableDelegate::CreateRaw(info, &FAsyncLoadInfo::LoadComplete));
		TryAsyncLoad.Add(info);
	}
	else
	{
		(*findOne)->ArrayLoadedDelegate.Add(LoadedDelegate);
	}
}


