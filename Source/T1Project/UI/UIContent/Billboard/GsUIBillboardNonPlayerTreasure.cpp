// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Billboard/GsUIBillboardNonPlayerTreasure.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

void UGsUIBillboardNonPlayerTreasure::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (_endLifeTime > 0)
	{
		UpdateLifeTime();
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIBillboardNonPlayerTreasure::InvalidateAllInternal()
{
	Super::InvalidateAllInternal();

	if (_endLifeTime > 0)
	{
		FDateTime endTime(_endLifeTime);
		SetRemainLifeTime(endTime.GetTicks());
	}
}

void UGsUIBillboardNonPlayerTreasure::Reset()
{
	if (_isInitialize) 
	{
		return;
	}

	Super::Reset();

	//SetOwnMonsterOption 보다 늦게 불린다 (asyncload 인듯)
	_isLocalPlayerOwn = false;
	_isMyTeamPlayerOwn = false;
	_ownPlayerName.Empty();

	_isInitialize = true;
}

void UGsUIBillboardNonPlayerTreasure::SetOwnMonsterOption(FString ownerName, int32 remainTimeSec /*= 0*/, int32 fullTimeSec /*= 0*/, bool isLocalPlayerOwn /*= false*/)
{
	Reset();

	_textLifeTime->SetVisibility(isLocalPlayerOwn ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_textBlockName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	_ownPlayerName = ownerName;
	_isLocalPlayerOwn = isLocalPlayerOwn;
	_isMyTeamPlayerOwn = isLocalPlayerOwn;

	FDateTime endTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC() + FTimespan(0, 0, remainTimeSec);
	SetRemainLifeTime(endTime.GetTicks());

	Super::SetOwnMonsterOption(ownerName, remainTimeSec, fullTimeSec, isLocalPlayerOwn);
}

void UGsUIBillboardNonPlayerTreasure::SetRemainLifeTime(time_t endTimeTick)
{
	FDateTime endTime(endTimeTick);
	FTimespan remain = endTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	_endLifeTime = remain.GetTotalSeconds() > 0 ? endTimeTick : 0;

	if (_isLocalPlayerOwn) // 이건 안 변함
	{
		_isMyTeamPlayerOwn = true;
		_showTime = 0.f;

		auto OutColor = FGsUIColorHelper::GetColor(EGsUIColorType::TREASURE_MONSTER_OWN);
		auto TimeColor = FGsUIColorHelper::GetColor(EGsUIColorType::TREASURE_MONSTER_TIME);

		SetLifeTimeColor(TimeColor);
		SetNameColor(OutColor);
		SetBillboardOpacity(1.f);
	}

	UpdateLifeTime(true);

	Super::SetRemainLifeTime(endTimeTick);
}

void UGsUIBillboardNonPlayerTreasure::ChangeOwnMonsterOption()
{
	SetRemainLifeTime(_endLifeTime);
}

void UGsUIBillboardNonPlayerTreasure::UpdateLifeTime(bool forced)
{
	FTimespan remain = FDateTime(_endLifeTime) - FGsTimeSyncUtil::GetServerNowDateTimeUTC();

	auto min = remain.GetMinutes();
	auto sec = remain.GetSeconds();

	_textLifeTime->SetText(FText::FromString(FString::Printf(TEXT("%02d : %02d"), min, sec)));

	if (!_target.IsValid()) return;

	if (remain.GetTotalSeconds() <= 0)
	{
		_endLifeTime = 0;
		_textLifeTime->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (!_isLocalPlayerOwn) //내 것일때는 바뀌지 않음
	{
		auto oldTeamOwn = _isMyTeamPlayerOwn;
		if (UGsGameObjectNonPlayer* nonPlayer = _target->CastGameObject<UGsGameObjectNonPlayer>())
		{
			_isMyTeamPlayerOwn = nonPlayer->IsPlayerTeamTreasureMonster();

			if (_isMyTeamPlayerOwn)
			{
				_showTime = 0.f;
				_textLifeTime->SetVisibility(nonPlayer->IsMyPartyMonster() ?
					ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
				SetBillboardOpacity(1.f);
			}
			else
			{
				_showTime = _backShowTime;
				_textLifeTime->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (forced || oldTeamOwn != _isMyTeamPlayerOwn)
			{
				SetNameColor(FGsUIColorHelper::GetColor(EGsUIColorType::TREASURE_MONSTER_OTHER));
				SetLifeTimeColor(FGsUIColorHelper::GetColor(EGsUIColorType::TREASURE_MONSTER_TIME));
				ShowHPBar(IsShowHPBar());
			}
		}
	}
}

void UGsUIBillboardNonPlayerTreasure::SetLifeTimeColor(const FLinearColor& color)
{
	if (_textLifeTime)
	{
		_textLifeTime->SetColorAndOpacity(color);
	}
}

void UGsUIBillboardNonPlayerTreasure::SetNameColor(const FLinearColor& InColor)
{
	auto TimeColor = FGsUIColorHelper::GetColor(EGsUIColorType::TREASURE_MONSTER_TIME);
	auto OutColor = _isLocalPlayerOwn ? FGsUIColorHelper::GetColor(EGsUIColorType::TREASURE_MONSTER_OWN) : 
		_isMyTeamPlayerOwn ? FGsUIColorHelper::GetColor(EGsUIColorType::TREASURE_MONSTER_TEAM) :
		FGsUIColorHelper::GetColor(EGsUIColorType::TREASURE_MONSTER_OTHER);

	Super::SetNameColor(OutColor);
}

bool UGsUIBillboardNonPlayerTreasure::IsRenderShowStateUpdateMonster()
{
	return IsMonsterLike() && !_ownPlayerName.IsEmpty() && _isMyTeamPlayerOwn;
}
