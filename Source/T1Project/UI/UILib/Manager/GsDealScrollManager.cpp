// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDealScrollManager.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UIContent/DealScroll/GsUIDealScrollBase.h"
#include "DataSchema/DealScroll/GsSchemaDealScrollData.h"
#include "UI/UILib/Container/GsWidgetPool.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "Data/GsDataContainManager.h"
#include "T1Project.h"
#include "Management/ScopeGlobal/GsPathManager.h"


void UGsDealScrollManager::Initialize()
{
	_onReleaseCompleteFunc = [](UUserWidget* InWidget)
	{
		UGsUIDealScrollBase* widget = Cast<UGsUIDealScrollBase>(InWidget);
		if (widget)
		{
			widget->OnReleaseComplete();
		}
	};

	// 0.2초
	_speedCheckTick = ETimespan::TicksPerSecond / 5;
}

void UGsDealScrollManager::Finalize()
{
	_prevWidgetMap.Empty();
}

void UGsDealScrollManager::Reset()
{
	_prevWidgetMap.Empty();
}

const FGsSchemaDealScrollData* UGsDealScrollManager::GetTableRow(const FName& InKey) const
{
	const UGsTable* table = FGsSchemaDealScrollData::GetStaticTable();
	if (table)
	{
		const FGsSchemaDealScrollData* Row = nullptr;
		table->FindRow<FGsSchemaDealScrollData>(InKey, Row);
		return Row;
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[DealScroll] Fail to find row. key: %s"), *InKey.ToString());
#endif //WITH_EDITOR

	return nullptr;
}

UGsUIDealScrollBase* UGsDealScrollManager::AddWidget(const FName& InKey, USceneComponent* InTarget,
	const FString& InValue, FVector InDir, int64 InGameId)
{
	const FGsSchemaDealScrollData* data = GetTableRow(InKey);
	if (nullptr == data)
	{
		return nullptr;
	}

	UClass* widgetClass = nullptr;
	if (UGsPathManager* pathMgr = GPath())
	{
		widgetClass = pathMgr->GetClassByObjectPath(data->WidgetPath);
	}

	if (nullptr == widgetClass)
	{
		return nullptr;
	}

	UGsUIDealScrollBase* widget = Claim(widgetClass);
	if (widget)
	{
		// 해당 타겟에 이전에 재생한 위젯과의 텀이 짧을 경우, 이전 위젯의 애니메이션 재생 속도를 올린다.
		int64 startTick = FDateTime::UtcNow().GetTicks();
		if (0 < InGameId)
		{
			TWeakObjectPtr<UGsUIDealScrollBase>* prevWidgetPtr = _prevWidgetMap.Find(InGameId);
			if (nullptr == prevWidgetPtr)
			{
				_prevWidgetMap.Emplace(InGameId, widget);
			}
			else
			{
				TWeakObjectPtr<UGsUIDealScrollBase> prevWidget = *prevWidgetPtr;
				if (prevWidget.IsValid())
				{
					int64 prevTick = prevWidget->GetStartTick();

					// 0.2초(_speedCheckTick) 내로 들어왔을 경우, 이전 위젯 애니메이션의 스피드를 올린다. 
					if (_speedCheckTick > (startTick - prevTick))
					{
						prevWidget->SetCurrentPlaybackSpeed(2.0f);
					}

					_prevWidgetMap[InGameId] = widget;
				}
			}
		}

		widget->AddToViewport();

		FVector location = (nullptr != InTarget) ? InTarget->GetSocketLocation(data->SocketName) : FVector::ZeroVector;
		widget->SetData(data, location, InValue, InDir, InGameId, startTick);
	}

	return widget;
}

void UGsDealScrollManager::ReleaseWidget(UGsUIDealScrollBase* InWidget)
{
	if (InWidget)
	{
		if (_poolManager.IsValid())
		{
			int64 gameId = InWidget->GetTargetGameId();

			// 저장된 이전 위젯이 있고
			TWeakObjectPtr<UGsUIDealScrollBase>* prevWidgetPtr = _prevWidgetMap.Find(gameId);
			if (prevWidgetPtr)
			{
				TWeakObjectPtr<UGsUIDealScrollBase> prevWidget = *prevWidgetPtr;
				if (prevWidget.IsValid())
				{
					// 현재 반납되는 위젯이 저장된 이전 위젯과 같으면 지움
					if (InWidget->GetStartTick() == prevWidget->GetStartTick())
					{
						_prevWidgetMap.Remove(gameId);
					}
				}
				else
				{
					// 위젯이 Invalid 이면 지움
					_prevWidgetMap.Remove(gameId);
				}
			}

			InWidget->RemoveFromParent();

			_poolManager->ReleaseWidget(InWidget->GetClass(), InWidget);
		}
	}
}

UGsUIDealScrollBase* UGsDealScrollManager::Claim(UClass* InClass)
{
	if (false == _poolManager.IsValid())
	{
		_poolManager = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
		if (false == _poolManager.IsValid())
		{
			return nullptr;
		}
	}

	UGsWidgetPool* pool = _poolManager->FindPool(InClass);
	if (nullptr == pool)
	{
		pool = _poolManager->CreatePool(InClass, _onReleaseCompleteFunc);
		if (nullptr == pool)
		{
			return nullptr;
		}
	}

	return pool->Claim<UGsUIDealScrollBase>();
}
