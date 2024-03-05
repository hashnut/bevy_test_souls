// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISocialMotionEntry.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "UI/UIContent/Common/GsUICoolTimeEffectProgressBar.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUISocialMotionEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnMotion->OnClicked.AddDynamic(this, &UGsUISocialMotionEntry::OnSelected);
}

void UGsUISocialMotionEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_coolSec = 0.f;
	_coolTimeEndSec = 0.f;
	_btnMotion->SetIsEnabled(true);
	_coolTimeBar->SetProgress(0.f);
}

void UGsUISocialMotionEntry::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUISocialMotionEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (_coolSec > 0.f)
	{
		_coolSec -= InDeltaTime;

		auto percent = _coolTimeEndSec > 0.f ? FMath::Clamp(_coolSec / _coolTimeEndSec, 0.0f, 1.0f) : 0.f;
		_coolTimeBar->SetProgress(percent);
		
		if(_coolSec <= 0.f)
		{
			_btnMotion->SetIsEnabled(true);
			_coolTimeEndSec = _coolSec = 0.f;
		}
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUISocialMotionEntry::SetMotionData(TWeakPtr<FSocialEmotionMotion> data)
{ 
	_motionData = data; 

	if (auto motionData = data.Pin())
	{	
		SetIconImage(motionData->GetEmoticonPath());
	}

	RefreshUI();
}

void UGsUISocialMotionEntry::RefreshUI()
{
	_coolSec = 0.f;
	_coolTimeEndSec = 0.f;
	_btnMotion->SetIsEnabled(true);

	if (auto data = _motionData.Pin())
	{
		if (data->IsPlayableMotion())
		{
			_coolTimeEndSec = float(data->GetEndCoolTime());
			_coolSec = float(data->GetRemainCoolTime());
			_coolTimeBar->SetProgress(_coolSec > 0.f ? 1.f : 0.f);
			_btnMotion->SetIsEnabled(_coolSec == 0.f);

			_lock->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			_coolSec = 0.f;
			_coolTimeEndSec = 0.f;
			_btnMotion->SetIsEnabled(false);
			_lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UGsUISocialMotionEntry::OnSelected()
{
	OnClickSlot.ExecuteIfBound(_motionData);
}
