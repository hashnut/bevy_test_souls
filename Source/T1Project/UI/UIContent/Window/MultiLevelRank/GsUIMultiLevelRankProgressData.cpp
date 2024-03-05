// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMultiLevelRankProgressData.h"
#include "MultiLevelRank/GsMultiLevelRankDefine.h"
#include "MultiLevelRank/GsMultiLevelRankData.h"

#include "Management/ScopeGame/GsMultiLevelRankManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"

#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "UObject/WeakObjectPtrTemplates.h"


void UGsUIMultiLevelRankProgressData::BeginDestroy()
{
	_rankData = nullptr;

	Super::BeginDestroy();
}

void UGsUIMultiLevelRankProgressData::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIMultiLevelRankProgressData::Initialized(const FGsMultiLevelRankData* InRank)
{
	if (nullptr == InRank)
	{
		GSLOG(Error, TEXT("UGsUIMultiLevelRankProgressData::Initialized : Invaild InRank Ptr"));
		return;
	}

	// 초기화
	_rankData = InRank;

	/************************************************************************/
	/* Set Progress Data                                                    */
	/************************************************************************/
	// 각 구간 마다의 exp 시작과 끝을 설정해야 한다.
	auto CalcPoint = [=](const FGsMultiLevelRankData* InBeineRank
						, const FGsMultiLevelRankData* InEndRank
						, const FGsMultiLevelRankData* InDeltaRank
						, SectionPointIndex InIndex
						, float InMaxRate)
	{
		uint64 begin = (InBeineRank) ? InBeineRank->_rankSchema->exp : 0;
		uint64 end = (InEndRank) ? InEndRank->_rankSchema->exp : 0;

		SectionPoint newPoint;
		if (SectionPointIndex::Front == InIndex)
		{
			newPoint._beginePoint = (begin > 0) ? begin + ((end - begin) * 0.5f) : 0;
			newPoint._endPoint = end;
			newPoint._maxRateValue = (nullptr != InDeltaRank) ? 1.0f : 2.0f;
		}
		else if (SectionPointIndex::Back == InIndex)
		{
			newPoint._beginePoint = begin;
			newPoint._endPoint = begin + ((end - begin) * 0.5f);
			// 2021/06/22 PKT - 가장 마지막 라인은 Percent 값이 0이여야 한다.
			newPoint._maxRateValue = (nullptr != InEndRank) ? 1.0f : 0.f;
		}
	
		return newPoint;
	};

	int32 CurrentIndex = GSMultiLevelRank()->GetIndexByRankData(_rankData);
	const FGsMultiLevelRankData* PrevRank = GSMultiLevelRank()->GetRankByIndex(CurrentIndex - 1);
	const FGsMultiLevelRankData* NextRank = GSMultiLevelRank()->GetRankByIndex(CurrentIndex + 1);	
	float maxRate = 1.0f;

	_point[SectionPointIndex::Front] = CalcPoint(PrevRank, _rankData, NextRank, SectionPointIndex::Front, maxRate);
	_point[SectionPointIndex::Back] = CalcPoint(_rankData, NextRank, PrevRank, SectionPointIndex::Back, maxRate);

	ProgressBarSwitcherIndex Index = ProgressBarSwitcherIndex::Middle;	
	if (nullptr == NextRank)
		Index = ProgressBarSwitcherIndex::End;

	_progressBarSwitcher->SetActiveWidgetIndex(int32(Index));
}

void UGsUIMultiLevelRankProgressData::InvalidateExp(uint64 InCurrentExp)
{
	bool isShowExpPercent = false;

	int32 CurrentIndex = GSMultiLevelRank()->GetIndexByRankData(_rankData);	
	const FGsMultiLevelRankData* NextRank = GSMultiLevelRank()->GetRankByIndex(CurrentIndex + 1);
	float maxRate = 1.0f;

	do 
	{
		if (nullptr == NextRank)
		{
			break;
		}

		const uint64 begineExp = _rankData->_rankSchema->exp;
		const uint64 endExp = NextRank->_rankSchema->exp;

		if (InCurrentExp <= begineExp)
		{
			break;
		}

		if (InCurrentExp >= endExp)
		{
			_switcherSlotPoint->SetActiveWidgetIndex(1);
			break;
		}

		_switcherSlotPoint->SetActiveWidgetIndex(0);
		double tenthousandths = double(InCurrentExp - begineExp) / (endExp - begineExp) * 10000.0;
		if (0 >= tenthousandths || tenthousandths >= 10000.00)
		{
			break;
		}
		
		isShowExpPercent = true;

		const FString textPecent = FString::Printf(TEXT("%02.2f%%"), tenthousandths * 0.01);
		_textExpPercent->SetText(FText::FromString(textPecent));

	} while (0);

	_textExpPercent->SetVisibility(isShowExpPercent ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIMultiLevelRankProgressData::InvalidateExpProgressBar()
{
	if (nullptr == _rankData)
		return;

	auto CalcRate = [=](const uint64 InCurrnetExp, const SectionPoint& InNewPoint)
	{
		if (InCurrnetExp < InNewPoint._beginePoint)
			return 0.0f;
		else if (InCurrnetExp > InNewPoint._endPoint)
			return InNewPoint._maxRateValue;

		int64 begin = InNewPoint._beginePoint;
		int64 end = InNewPoint._endPoint;
		int64 current = InCurrnetExp;

		float rate = static_cast<float>(current - begin) / (end - begin);
		rate *= InNewPoint._maxRateValue;
		return FMath::Clamp(rate, 0.f, 2.0f);
	};

	TWeakObjectPtr<UProgressBar> progressBar = Cast<UProgressBar>(_progressBarSwitcher->GetActiveWidget());
	if (progressBar.IsValid())
	{
		float percent = 0.f;
		do
		{
			UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
			const FGsGameObjectDataLocalPlayer* LocalPlayer = (local) ? local->GetCastData<FGsGameObjectDataLocalPlayer>() : nullptr;
			if (nullptr == LocalPlayer)
			{
				break;
			}

			uint64 CurrentExp = LocalPlayer->GetUserExp();
			// uint64 CurrentExp = 유저의 최고 누적 경험치 필요

			percent += CalcRate(CurrentExp, _point[static_cast<int32>(SectionPointIndex::Front)]) * 0.5f;
			percent += CalcRate(CurrentExp, _point[static_cast<int32>(SectionPointIndex::Back)]) * 0.5f;

			InvalidateExp(CurrentExp);

		} while (0);

		progressBar->SetPercent(percent);
	}
}

const FGsMultiLevelRankData* UGsUIMultiLevelRankProgressData::GetData() const
{
	return _rankData;
}