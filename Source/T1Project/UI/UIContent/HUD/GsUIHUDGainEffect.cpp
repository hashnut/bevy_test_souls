// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDGainEffect.h"
#include "UI/UIContent/HUD/GainEffect/GsUIGainEffectSlot.h"
#include "Classes/Engine/GameViewportClient.h"
#include "Engine/Engine.h"


void UGsUIHUDGainEffect::BeginDestroy()
{
	_poolDelayData.RemovePool();

	Super::BeginDestroy();
}

void UGsUIHUDGainEffect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_textFormatOptionExp.SetUseGrouping(false);

	ResetSlots();
}

void UGsUIHUDGainEffect::NativeConstruct()
{
	Super::NativeConstruct();

	// 백그라운드로 내렸을 때 NativeTick이 돌지 않아서 쌓이는 현상 있음. 다른 틱을 사용하도록 교체
	if (GEngine->GameViewport)
	{
		_tickHandle = GEngine->GameViewport->OnTick().AddUObject(this, &UGsUIHUDGainEffect::UpdateTick);
	}
}

void UGsUIHUDGainEffect::NativeDestruct()
{
	if (GEngine->GameViewport)
	{
		GEngine->GameViewport->OnTick().Remove(_tickHandle);
	}

	_delayList.Empty();
	_removeDelayList.Empty();
	_poolDelayData.ReleaseAll();

	ResetSlots();

	Super::NativeDestruct();
}

void UGsUIHUDGainEffect::SetHUDMode(EGsUIHUDMode InMode)
{
	Super::SetHUDMode(InMode);

	if (IsHideMode(InMode))
	{
		ResetSlots();
	}
}

void UGsUIHUDGainEffect::UpdateTick(float InDeltaTime)
{
	// 생명주기 체크
	for (int32 i = 0; i < _activeList.Num(); ++i)
	{
		TWeakObjectPtr<UGsUIGainEffectSlot> slot = _activeList[i];
		if (slot.IsValid())
		{
			slot->UpdateTick(InDeltaTime);

			if (slot->IsEndLifeTime())
			{
				_removeList.Emplace(slot);
				_inactiveList.Emplace(slot);
			}
		}
	}

	for (TWeakObjectPtr<UGsUIGainEffectSlot> slot : _removeList)
	{
		_activeList.Remove(slot);
	}
	_removeList.Empty();

	// 지연 연출 체크
	for (int32 i = 0; i < _delayList.Num(); ++i)
	{
		if (FGsGainEffectDelayData* delayData = _delayList[i])
		{
			delayData->_remainSec -= InDeltaTime;

			if (0 >= delayData->_remainSec)
			{
				_removeDelayList.Emplace(delayData);
				SetValue(delayData->_value, false);
			}
		}
	}

	// 지연 연출 정보 지움
	for (FGsGainEffectDelayData* removeDelayData : _removeDelayList)
	{
		_poolDelayData.Release(removeDelayData);
		_delayList.Remove(removeDelayData);
	}
	_removeDelayList.Empty();
}

void UGsUIHUDGainEffect::ResetSlots()
{
	_activeList.Empty();
	_removeList.Empty();
	_delayList.Empty();

	_inactiveList.Emplace(_slot0);
	_inactiveList.Emplace(_slot1);
	_inactiveList.Emplace(_slot2);
	_inactiveList.Emplace(_slot3);
	_inactiveList.Emplace(_slot4);

	for (TWeakObjectPtr<UGsUIGainEffectSlot> slot : _inactiveList)
	{
		slot->Reset();
	}
}

void UGsUIHUDGainEffect::SetExp(uint64 InValue)
{
	// 주의: 선판정 동기화 이슈로 인해 자연스러워 보이기 위한 딜레이 값 등록
	if (0 == DELAY_HP_DISPLAY_TIME)
	{
		SetValue(InValue, false);
	}
	else
	{
		FGsGainEffectDelayData* delayData = _poolDelayData.Claim();
		delayData->_remainSec = DELAY_HP_DISPLAY_TIME;
		delayData->_value = InValue;

		_delayList.Emplace(delayData);
	}
}

void UGsUIHUDGainEffect::SetGold(uint64 InValue)
{
	// 주의: 선판정 동기화 이슈로 인해 자연스러워 보이기 위한 딜레이 값 등록
	if (0 == DELAY_HP_DISPLAY_TIME)
	{
		SetValue(InValue, false);
	}
	else
	{
		FGsGainEffectDelayData* delayData = _poolDelayData.Claim();
		delayData->_remainSec = DELAY_HP_DISPLAY_TIME;
		delayData->_value = InValue;

		_delayList.Emplace(delayData);
	}
}

void UGsUIHUDGainEffect::SetMonsterBookPoint(uint64 InValue)
{
	//// 지연 안하기 때문에 바로 수행
	//SetValue(InValue, true);

	// 주의: 선판정 동기화 이슈로 인해 자연스러워 보이기 위한 딜레이 값 등록
	if (0 == DELAY_HP_DISPLAY_TIME)
	{
		SetValue(InValue, false);
	}
	else
	{
		FGsGainEffectDelayData* delayData = _poolDelayData.Claim();
		delayData->_remainSec = DELAY_HP_DISPLAY_TIME;
		delayData->_value = InValue;

		_delayList.Emplace(delayData);
	}
}

void UGsUIHUDGainEffect::SetValue(uint64 InValue, bool bUseGrouping)
{
	TWeakObjectPtr<UGsUIGainEffectSlot> addSlot = nullptr;

	if (0 == _inactiveList.Num())
	{
		if (_activeList[0].IsValid())
		{
			addSlot = _activeList[0];
			addSlot->Reset();

			_activeList.RemoveAt(0);
		}
	}
	else
	{
		addSlot = _inactiveList.Pop();
	}

	// 이전 슬롯들 올리기
	int32 orderIndex = 0;
	for (int32 i = _activeList.Num() - 1; i >= 0; --i)
	{
		_activeList[i]->SetUpEffect(orderIndex * _upHeight, (orderIndex + 1) * _upHeight);
		++orderIndex;
	}

	// 현재슬롯을 마지막으로 넣기
	if (addSlot.IsValid())
	{
		if (bUseGrouping)
		{
			addSlot->SetTextValue(FText::AsNumber(InValue));
		}
		else
		{
			addSlot->SetTextValue(FText::AsNumber(InValue, &_textFormatOptionExp));
		}

		float randX = FMath::RandRange(0.f, _randomPosX);

		addSlot->SetStartEffect(_firstSlotScale, randX);
		_activeList.Push(addSlot);
	}
}
