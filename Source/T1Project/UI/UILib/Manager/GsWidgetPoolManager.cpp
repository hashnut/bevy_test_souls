// Fill out your copyright notice in the Description page of Project Settings.


#include "GsWidgetPoolManager.h"
#include "UI/UILib/Container/GsWidgetPool.h"
#include "Blueprint/UserWidget.h"
#include "T1Project.h"

static float GGsWidgetPoolKillUnusedTime = 180.f; // 이 시간 이상 사용 안한 항목은 삭제
static float GGsWidgetPoolCleanTime = 30.f; // 검사 타이밍 체크를 위한 시간


void UGsWidgetPoolManager::Initialize()
{

}

void UGsWidgetPoolManager::Finalize()
{
	RemoveAll();
}

void UGsWidgetPoolManager::RemoveAll()
{
	for (UGsWidgetPool* iter : PoolList)
	{
		iter->Empty();
	}

	PoolMap.Empty();
	PoolList.Empty();
}

void UGsWidgetPoolManager::ReleaseAll()
{
	float currTime = GetCurrTime();

	for (UGsWidgetPool* iter : PoolList)
	{
		iter->ReleaseAll();
		iter->KillUnusedWidgets(currTime - GGsWidgetPoolKillUnusedTime);
	}
}

UGsWidgetPool* UGsWidgetPoolManager::CreatePool(TSubclassOf<UUserWidget> InWidgetClass, 
	TFunction<void(UUserWidget*)> InReleaseFunc, int32 InWarmUpCount)
{
	if (false == IsValidLowLevel())
	{
		return nullptr;
	}

	// 중복체크
	FName key = FName(*InWidgetClass.Get()->GetPathName());
	if (PoolMap.Contains(key))
	{
		return nullptr;
	}

	// Pool 개수 제한 체크
	if (LimitPoolCount <= PoolList.Num())
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("[WidgetPool] Count over limit. class: %s"), *InWidgetClass.Get()->GetPathName());
#endif //WITH_EDITOR
		return nullptr;
	}

	// 생성, 초기화
	UGsWidgetPool* pool = NewObject<UGsWidgetPool>(this);
	pool->Initialize(InWidgetClass, InWarmUpCount, InReleaseFunc);

	// 저장
	PoolList.Add(pool);
	PoolMap.Emplace(key, pool);

	return pool;
}

UGsWidgetPool* UGsWidgetPoolManager::FindPool(TSubclassOf<UUserWidget> InWidgetClass)
{
	if (false == IsValidLowLevel())
	{
		return nullptr;
	}

	FName key = FName(*InWidgetClass.Get()->GetPathName());
	
	// 검색
	if (TWeakObjectPtr<UGsWidgetPool>* result = PoolMap.Find(key))
	{
		if (result->IsValid())
		{
			return result->Get();
		}
		else
		{
			// PoolList와 쌍으로 움직이며, 풀 객체를 끄기 전엔 따로 지우지 않는데 들어오는건 이상하다.
			// 혹시 몰라 에러 로그를 남겨둠
			GSLOG(Error, TEXT("[UIPool] Invalid pool. %s"), *key.ToString());
		}
	}

	return nullptr;
}

void UGsWidgetPoolManager::ReleaseWidget(TSubclassOf<UUserWidget> InWidgetClass, UUserWidget* InWidget)
{
	UGsWidgetPool* widgetPool = FindPool(InWidgetClass);
	if (widgetPool)
	{
		widgetPool->Release(InWidget);

		float currTime = GetCurrTime();
		if (currTime - widgetPool->GetLastCleanTime() > GGsWidgetPoolCleanTime)
		{
			widgetPool->KillUnusedWidgets(currTime - GGsWidgetPoolKillUnusedTime);
		}
	}
}

void UGsWidgetPoolManager::PrintLog()
{
	GSLOG(Warning, TEXT("[UIPool]***************************************************************************"));
	for (TPair<FName, TWeakObjectPtr<UGsWidgetPool>>& iter : PoolMap)
	{
		GSLOG(Warning, TEXT("[UIPool] %s : %d"), *iter.Key.ToString(), iter.Value->GetTotalRunningCount());
	}
	GSLOG(Warning, TEXT("[UIPool]***************************************************************************"));
}

float UGsWidgetPoolManager::GetCurrTime() const
{
	// 월드 타임을 쓰면 레벨 이동 시 시간이 리셋되어 시간을 정확하게 사용할 수는 없다.
	// 그정도 불확실성은 그냥 감수하고 사용
	if (UWorld* world = GetWorld())
	{
		return world->GetTimeSeconds();
	}

	return 0.f;
}
