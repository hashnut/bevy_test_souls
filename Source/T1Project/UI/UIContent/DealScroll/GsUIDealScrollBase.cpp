// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIDealScrollBase.h"
#include "Components/TextBlock.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UILib/Manager/GsDealScrollManager.h"
#include "DataSchema/DealScroll/GsSchemaDealScrollData.h"
#include "Public/TimerManager.h"


void UGsUIDealScrollBase::BeginDestroy()
{
	// 정상 상황이라면 파괴 전에 해제가 되어있어야 함. 혹시모를 상황을 대비.
	if (_timerHandle.IsValid())
	{
		if (nullptr != GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
		}
	}

	Super::BeginDestroy();
}

void UGsUIDealScrollBase::NativeDestruct()
{
	if (_timerHandle.IsValid())
	{
		if (nullptr != GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
		}
	}

	Super::NativeDestruct();
}

void UGsUIDealScrollBase::CloseInternal()
{
	Release();
	// 주의: Pop로직을 타지 않을 것이므로 Super::Close 호출하지 말 것.
}

void UGsUIDealScrollBase::Release()
{
	// 풀에 반납
	if (UGsUIManager* uimgr = GUI())
	{
		UGsDealScrollManager* dealScrollManager = uimgr->GetDealScrollManager();
		if (nullptr != dealScrollManager)
		{
			dealScrollManager->ReleaseWidget(this);
		}
	}
}

void UGsUIDealScrollBase::OnReleaseComplete()
{
	if (nullptr != GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
	}

	_randomScreenLocation = FVector2D::ZeroVector;
	_dirOffset = FVector::ZeroVector;
}

void UGsUIDealScrollBase::StartAnimation(const FGsSchemaDealScrollData& InData)
{
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
		world->GetTimerManager().SetTimer(_timerHandle, this,
			&UGsUIDealScrollBase::OnEndDelayTime, InData.LifeTime, false);
	}
}

void UGsUIDealScrollBase::SetData(const FGsSchemaDealScrollData* InData, FVector InLocation, const FString& InValue, 
	const FVector& InDir, int64 InGameId, int64 InStartTick)
{
	_targetLocation = InLocation;
	_targetGameId = InGameId;
	_startTick = InStartTick;

	// 텍스트 세팅
	if (false == InData->DescText.IsEmpty())
	{
		SetValueText(InData->DescText);
	}
	else
	{
		SetValueString(InValue, true);
	}

	// 위치값 관련 세팅
	if (0.0f < InData->DirOffset)
	{
		_dirOffset = InDir;
		_dirOffset.Z = 0.0f;
		_dirOffset.Normalize();
		_dirOffset = _dirOffset * InData->DirOffset;
	}
	else
	{
		_dirOffset = FVector::ZeroVector;
	}

	// 랜덤에 의해 위치가 더 내려가게 하지않음
	FVector2D rand = V2DRand();
	rand.Y = (0.0f > rand.Y) ? rand.Y : 0.0f;
	_randomScreenLocation = rand * InData->LocationRandomOffset;
	
	// 스크린 좌표계에서 위치 조정값
	//_screenOffset = InData->screenOffset;

	SetTransformInter(InData);

	// 폰트, 애니메이션, 라이프타임 등 세팅
	StartAnimation(*InData);
}

void UGsUIDealScrollBase::OnEndDelayTime()
{
	Release();
}

void UGsUIDealScrollBase::SetTransformInter(const struct FGsSchemaDealScrollData* InData)
{
	FVector worldLocation = _targetLocation + InData->RelativeLocation + _dirOffset;

	FVector2D screenLocation = FVector2D::ZeroVector;
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (nullptr != playerController)
	{
		playerController->ProjectWorldLocationToScreen(worldLocation, screenLocation);
	}

	screenLocation += _randomScreenLocation;
	screenLocation += InData->screenOffset;

	SetPositionInViewport(screenLocation);
}

FVector2D UGsUIDealScrollBase::V2DRand()
{
	// FVector::VRand()에서 Z값만 제거
	FVector2D Result;
	float L;

	do
	{
		Result.X = FGenericPlatformMath::FRand() * 2.0f - 1.0f;
		Result.Y = FGenericPlatformMath::FRand() * 2.0f - 1.0f;
		L = Result.SizeSquared();
	} while (L > 1.0f || L < KINDA_SMALL_NUMBER);

	return Result * (1.0f / FGenericPlatformMath::Sqrt(L));
}
