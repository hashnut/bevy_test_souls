// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/App.h"
#include "Engine/AssetManager.h"
#include "Management/GsScopeHolder.h"
#include "Classes/GsManager.h"
#include "Classes/GsDelegate.h"
#include "T1Project.h"
#include "Classes/GsPool.h"
#include "Engine/StreamableManager.h"
#include "GsResourceManager.generated.h"




UCLASS()
class T1PROJECT_API UGsResourceManager :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

protected:

	struct FAsyncLoadInfo
	{
		FAsyncLoadInfo() = default;
		~FAsyncLoadInfo()
		{
			// 제거 될때 FStreamableHandle 정지
			if (AsyncLoadHandle.IsValid())
			{
				AsyncLoadHandle->CancelHandle();
				AsyncLoadHandle->ReleaseHandle();
				AsyncLoadHandle.Reset();
			}
			ArrayLoadedDelegate.Empty();
			Path.Reset();
			CallLoadComplete = false;
		}

		bool								CallLoadComplete = false;
		FSoftObjectPath						Path;
		TArray<FStreamableDelegateParam>	ArrayLoadedDelegate;
		TSharedPtr<FStreamableHandle>		AsyncLoadHandle;

		bool IsSameType(const FSoftObjectPath& inPath) const
		{
			if (inPath == Path)
				return true;
			return false;
		}


		void LoadComplete()
		{
			if (IsInGameThread())
				CallLoadComplete = true;
			if (false == AsyncLoadHandle.IsValid())
			{
				return;
			}
			
			/**
			 * 요청한 쪽에서 이미 소멸되었지만 비동기로 인해 로딩이 완료 되었다고 호출되는 시점이 있음. nullptr 검사함.
			 */
			UObject* LoadObject = AsyncLoadHandle->GetLoadedAsset();
			if (nullptr != LoadObject)
			{
				for (int i = 0; i < ArrayLoadedDelegate.Num(); ++i)
				{
					ArrayLoadedDelegate[i].ExecuteIfBound(LoadObject);
				}
			}

			if(0 != ArrayLoadedDelegate.Num())
				ArrayLoadedDelegate.Empty();
		}
	};

private:
	// 로드 관련된 목록
	TArray<FAsyncLoadInfo*>					TryAsyncLoad;
	TGsPoolDefault<FAsyncLoadInfo>			PoolLoadInfo;
public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	virtual void Update(float inTick) override;

	void CancelAll();

private:
	void CleanUp();

public:
	void LoadAsync(const FSoftObjectPath& Path, FStreamableDelegateParam LoadedDelegate, bool IsUClass = false);

	template< typename T >
	T* LoadSync(const FSoftObjectPath& Path, bool IsUClass = false)
	{
		FSoftObjectPath path = IsUClass ? Path.ToString() + TEXT("_C") : Path.ToString();		
		return Cast<T>(UAssetManager::GetStreamableManager().LoadSynchronous(path.ToString()));
	}
	
};

#define GResource() UGsScopeHolder::GetGlobalManager<UGsResourceManager>(UGsScopeGlobal::EManagerType::Resource)