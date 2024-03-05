// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITargetInfoHUDBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"
#include "Components/NamedSlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUIAbnormalityIconList.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "UTIL/GsTimeSyncUtil.h"


void UGsUITargetInfoHUDBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTooltip->OnPressed.AddDynamic(this, &UGsUITargetInfoHUDBase::OnClickTooltip);
	_btnClearTarget->OnClicked.AddDynamic(this, &UGsUITargetInfoHUDBase::OnClickClearTarget);
}

void UGsUITargetInfoHUDBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (0 < _endLifeTime)
	{
		UpdateLifeTime();
	}

	if (_isShowHPBar)
	{
		if (0 < _hpBarAnimRemainTime)
		{
			_hpBarAnimRemainTime -= InDeltaTime * _shadowBarSpeed;

			if (0 >= _hpBarAnimRemainTime)
			{
				_hpBarAnimRemainTime = 0.0f;
				_barShadow->SetPercent(_targetHpRatio);
			}
			else
			{
				_barShadow->SetPercent(FMath::Lerp(_barShadow->Percent, _targetHpRatio, 1 - _hpBarAnimRemainTime));
			}
		}

		if (0 < _delayInvalidateTime)
		{
			_delayInvalidateTime -= InDeltaTime;
			if (0 >= _delayInvalidateTime)
			{
				_delayInvalidateTime = 0.f;
				UpdateHpShieldBar();
			}
		}
	}
}

void UGsUITargetInfoHUDBase::Reset()
{
	// 아직 스탯 이벤트를 받지 못한 상태에서 켜지며 이전값이 잠시 보이는 문제 수정
	_textBlockHpValue->SetText(FText::GetEmpty());
	_textBlockShieldValue->SetText(FText::GetEmpty());
	_delayInvalidateTime = 0.f;
}

void UGsUITargetInfoHUDBase::SetTarget(UGsGameObjectCreature* InTarget, const FString& InName)
{
	// 툴팁 버튼: NPC 만 보이게 처리
	_btnTooltip->SetVisibility((EGsGameObjectType::NonPlayer == InTarget->GetObjectType()) ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	_abnormalityIconList->SetTarget(InTarget);
	_abnormalityIconList->InvalidateAll();

	_textBlockName->SetText(FText::FromString(InName));
}

void UGsUITargetInfoHUDBase::SetRemainLifeTime(time_t endTimeTick)
{
	FDateTime endTime(endTimeTick);
	FTimespan remain = endTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	_endLifeTime = remain.GetTotalSeconds() > 0 ? endTimeTick : 0;
	_textLifeTime->SetVisibility(_endLifeTime > 0 ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUITargetInfoHUDBase::ShowHPBar(bool In_isShow)
{
	_isShowHPBar = In_isShow;
	_panelHpBarRoot->SetVisibility((In_isShow == false) ?
		ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UGsUITargetInfoHUDBase::SetHP(int32 InCurrent, int32 InMax, bool bInUpdateBar)
{
	_currHp = InCurrent;
	_maxHp = InMax;

	if (bInUpdateBar)
	{
		if (0.f != _delayInvalidateTime)
		{
			UpdateHpShieldBar();
		}
		_delayInvalidateTime = DELAY_HP_DISPLAY_TIME;
	}
	else
	{
		FString valueText = FString::Printf(TEXT("%s/%s"),
			*FString::FormatAsNumber(_currHp), *FString::FormatAsNumber(_maxHp));
		_textBlockHpValue->SetText(FText::FromString(valueText));
	}
}

void UGsUITargetInfoHUDBase::SetShield(int32 InCurrent, bool bInUpdateBar)
{
	_currShield = InCurrent;

	if (0 >= InCurrent)
	{
		_textBlockShieldValue->SetText(FText::GetEmpty());
	}
	else
	{
		FString strShield = FString::Format(TEXT("(+{0})"), { _currShield });
		_textBlockShieldValue->SetText(FText::FromString(strShield));
	}

	if (bInUpdateBar)
	{
		UpdateHpShieldBar();
	}
}

void UGsUITargetInfoHUDBase::UpdateHpShieldBar()
{
	FString valueText = FString::Printf(TEXT("%s/%s"),
		*FString::FormatAsNumber(_currHp), *FString::FormatAsNumber(_maxHp));
	_textBlockHpValue->SetText(FText::FromString(valueText));

	// 실드가 없을 경우 
	if (0 >= _currShield)
	{
		// 실드 모두 가림
		_barShieldFront->SetVisibility(ESlateVisibility::Collapsed);
		_barShieldBack->SetVisibility(ESlateVisibility::Collapsed);

		float hpRatio = FMath::Clamp(static_cast<float>(_currHp) / static_cast<float>(_maxHp), 0.0f, 1.0f);
		if (_targetHpRatio != hpRatio)
		{
			// HP바 애니메이션 시간 세팅
			_hpBarAnimRemainTime = 1.0f;
		}
		_targetHpRatio = hpRatio;

		_barHp->SetPercent(_targetHpRatio);
		// 연출 중 피가 차는 경우 즉시 길이를 맞춤. (그 외엔 애니메이션을 위해 세팅하지 않음)
		if (_barShadow->Percent < _targetHpRatio)
		{
			_barShadow->SetPercent(_targetHpRatio);
		}

		return;
	}

	// 애니메이션 시간 제거, 실드가 있을 땐 애니메이션 없음
	_targetHpRatio = FMath::Clamp(static_cast<float>(_currHp) / static_cast<float>(_maxHp), 0.0f, 1.0f);
	_barHp->SetPercent(_targetHpRatio);
	_barShadow->SetPercent(_targetHpRatio);
	_hpBarAnimRemainTime = 0.f;
	
	// maxHp를 초과할 경우
	if (_currShield + _currHp > _maxHp)
	{
		_barShieldFront->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_barShieldBack->SetVisibility(ESlateVisibility::Collapsed);

		_barShieldFront->SetPercent(FMath::Clamp(
			static_cast<float>(_currShield) / static_cast<float>(_maxHp), 0.0f, 1.0f));
	}
	else
	{
		_barShieldFront->SetVisibility(ESlateVisibility::Collapsed);
		_barShieldBack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// HP바 뒤로 나와보이도록 더해서 계산
		_barShieldBack->SetPercent(FMath::Clamp(
			static_cast<float>(_currHp + _currShield) / static_cast<float>(_maxHp), 0.0f, 1.0f));
	}
}

void UGsUITargetInfoHUDBase::UpdateLifeTime()
{
	FTimespan remain = FDateTime(_endLifeTime) - FGsTimeSyncUtil::GetServerNowDateTimeUTC();

	auto min = remain.GetMinutes();
	auto sec = remain.GetSeconds();

	_textLifeTime->SetText(FText::FromString(FString::Printf(TEXT("%02d : %02d"), min, sec)));

	if (remain.GetTotalSeconds() <= 0)
	{
		_endLifeTime = 0;
		_textLifeTime->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void UGsUITargetInfoHUDBase::OnClickTooltip()
{
	if (OnClickTooltipButton.IsBound())
	{
		OnClickTooltipButton.Execute(_slotTooltip);
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUITargetInfoHUDBase::OnClickClearTarget()
{
	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localBase == nullptr)
	{
		return;
	}
	if (localBase->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_CHANGE_TARGET)) // 도발 타겟 락
	{
		return;
	}
	FGsTargetHandlerLocalPlayer* targetHandler = localBase->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (targetHandler == nullptr)
	{
		return;
	}

	// 모든 타겟 정보 클리어
	targetHandler->ClearTarget(true);

	// 자동전투OFf
	GSAI()->SetActiveAI(false);
}
