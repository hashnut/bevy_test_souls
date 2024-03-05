// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRankingListEntryBase.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"

#include "DataSchema/Ranking/GsSchemaRankingTypeObjective.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"

#include "Guild/GsGuildHelper.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "UI/UIContent/Window/Ranking/GsRankingListItem.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "PaperSprite.h"


void UGsUIRankingListEntryBase::NativeConstruct()
{
	Super::NativeConstruct();

	_textBlockRanking->SetText(FText::GetEmpty());
	_textBlockVariance->SetText(FText::GetEmpty());
}

void UGsUIRankingListEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UGsRankingListItem* listItem = Cast<UGsRankingListItem>(ListItemObject);
	if (nullptr == listItem)
		return;

	const FGsRankingData* rankingData = listItem->_data;
	if (nullptr == rankingData)
		return;

	SetData(rankingData);
}

void UGsUIRankingListEntryBase::SetData(const FGsRankingData* InRankingData)
{
	_switcherBackground->SetActiveWidgetIndex(0);

	SetRanking(InRankingData->_ranking);
	int64 variance = (0 == InRankingData->_previousRank) ? 0 : (int64)InRankingData->_previousRank - (int64)InRankingData->_ranking;
	SetRankingVariance(variance);
}

void UGsUIRankingListEntryBase::SetData(FGsRankingMyData* InMyRanking)
{
	_switcherBackground->SetActiveWidgetIndex(1);

	if (nullptr == InMyRanking)
	{
		SetRanking(0);
		SetRankingVariance(0);
		return;
	}

	if (InMyRanking->_hasHighRanking)
	{
		SetRanking(InMyRanking->_highRanking);
		int64 variance = (0 == InMyRanking->_previousRank) ? 0 : (int64)InMyRanking->_previousRank - (int64)InMyRanking->_highRanking;
		SetRankingVariance(variance);
	}
	else
	{
		float rankingPercent = InMyRanking->_ranking;
		if (0 > rankingPercent || 100 < rankingPercent)
		{
			rankingPercent = 0;
		}
		SetRankingPercent(rankingPercent);
		SetRankingVariance(0);
	}
}

void UGsUIRankingListEntryBase::SetEmblem(UImage* InIconEmblem, GuildEmblemId InEmblemId)
{
	InIconEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	const FGsSchemaGuildEmblem* emblemData = FGsGuildHelper::GetGuildEmblem(InEmblemId);
	if (nullptr == emblemData)
	{
		GSLOG(Error, TEXT("Ranking : Can't Find Guild Emblem [%d]"), InEmblemId);
		return;
	}
	if (UPaperSprite* sprite = FGsUIHelper::LoadSpriteFromPath(emblemData->iconPathSmall))
	{
		InIconEmblem->SetBrushFromAtlasInterface(sprite);
	}
}

void UGsUIRankingListEntryBase::SetRanking(uint16 InRanking)
{
	switch (InRanking)
	{
	case 0: // 랭킹 존재 X
		_rankingGrade = EGsRankingGrade::NONE;
		break;
	case 1: // 랭킹 1등
		_rankingGrade = EGsRankingGrade::First;
		break;
	case 2: // 랭킹 2등
		_rankingGrade = EGsRankingGrade::Second;
		break;
	case 3: // 랭킹 3등
		_rankingGrade = EGsRankingGrade::Third;
		break;
	default: // 그외
		_rankingGrade = EGsRankingGrade::Other;
		break;
	}

	// 랭킹 이미지 설정
	_switcherRanking->SetActiveWidgetIndex((int32)_rankingGrade);

	// 랭킹 텍스트 설정
	_textBlockRanking->SetText(FText::AsNumber(InRanking));
}

void UGsUIRankingListEntryBase::SetRankingPercent(float InRanking)
{
	if (0 == InRanking)
	{
		_switcherRanking->SetActiveWidgetIndex((int32)EGsRankingGrade::NONE);
		return;
	}

	// 등 수 외 표시 : 상위 --.--%
	FText rankText;
	FText::FindText(TEXT("RankingCommonUIText"), TEXT("RankingCount"), rankText);

	FNumberFormattingOptions option;
	option.SetMaximumFractionalDigits(2);
	option.SetMinimumFractionalDigits(0);

	// 랭킹 이미지 설정
	_switcherRanking->SetActiveWidgetIndex((int32)EGsRankingGrade::Other);

	// 랭킹 텍스트 설정
	_textBlockRanking->SetText(FText::Format(rankText, FText::AsNumber(InRanking, &option)));
}

void UGsUIRankingListEntryBase::SetRankingVariance(int64 InVariance)
{
	// 랭킹 변동 설정
	// 랭킹 유지
	if (0 == InVariance)
	{
		_switcherVariance->SetActiveWidgetIndex(1);
	}
	else
	{
		_switcherVariance->SetActiveWidgetIndex(0);

		// 랭킹 감소
		if (0 > InVariance)
		{
			_switcherVarianceArrow->SetActiveWidgetIndex(0);
			_textBlockVariance->SetText(FText::AsNumber(-InVariance));
			_textBlockVariance->SetColorAndOpacity(_colorDown.GetRow()->color);
		}
		// 랭킹 증가
		else
		{
			_switcherVarianceArrow->SetActiveWidgetIndex(1);
			_textBlockVariance->SetText(FText::AsNumber(InVariance));
			_textBlockVariance->SetColorAndOpacity(_colorUP.GetRow()->color);
		}
	}
}
