// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICurveAnimation.h"
#include "Components/Widget.h"
#include "Classes/Curves/CurveFloat.h"
#include "Classes/Curves/CurveVector.h"
#include "T1Project.h"


//----------------------------------------------------------------------------------------------------------------------
void FGsUICurveAnimationData::Reset()
{
	_currTime = 0.0f;
	_maxTime = 0.0f;
}

void FGsUICurveAnimationData::SetTarget(class UWidget* InTarget)
{
	_target = InTarget;
}

bool FGsUICurveAnimationData::IsFinished()
{
	return (GetMaxTime() <= _currTime) ? true : false;
}

float FGsUICurveAnimationData::GetMaxTime()
{
	if (_curve)
	{	
		if (0.0f == _maxTime)
		{
			float min = 0.0f;			
			_curve->GetTimeRange(min, _maxTime);
		}

		return _maxTime;
	}

	return 0.0f;
}

void FGsUICurveAnimationData::InvalidateValue(float InTime)
{
	if (nullptr == _target)
	{
		return;
	}

	// 이미 max이면 수행 하지 않음
	if (_currTime >= GetMaxTime())
	{
		return;
	}

	// maxTime을 초과하지 않는다
	_currTime = (InTime < GetMaxTime()) ? InTime : _maxTime;

	if (EGsUICurveAnimationType::SCALE == _animationType)
	{
		if (_curve)
		{
			if (UCurveFloat* curveFloat = Cast<UCurveFloat>(_curve))
			{
				float currValue = curveFloat->GetFloatValue(_currTime);
				_target->SetRenderScale(FVector2D(currValue, currValue));
			}
		}
	}
	else if (EGsUICurveAnimationType::RENDER_OPACITY == _animationType)
	{
		if (_curve)
		{
			if (UCurveFloat* curveFloat = Cast<UCurveFloat>(_curve))
			{
				_target->SetRenderOpacity(curveFloat->GetFloatValue(_currTime));
			}
		}
	}
	else if (EGsUICurveAnimationType::TRANSLATION == _animationType)
	{
		if (_curve)
		{
			if (UCurveVector* curveFloat = Cast<UCurveVector>(_curve))
			{
				const FVector& currValue = curveFloat->GetVectorValue(_currTime);
				_target->SetRenderTranslation(FVector2D(currValue));
			}
		}
	}
	//else if (EGsUICurveAnimationType::SCALE_VECTOR == _animationType)
	//{
	//	if (_curve)
	//	{
	//		if (UCurveVector* curveFloat = Cast<UCurveVector>(_curve))
	//		{
	//			const FVector& currValue = curveFloat->GetVectorValue(_currTime);
	//			_target->SetRenderScale(FVector2D(currValue));
	//		}
	//	}
	//}
}
//----------------------------------------------------------------------------------------------------------------------

void FGsUICurveAnimation::Reset()
{
	_currTime = 0.0f;
	_bIsPlaying = false;
}

void FGsUICurveAnimation::Clear()
{
	_currTime = 0.0f;
	_bIsPlaying = false;

	// 주의: UI를 재활용하기 때문에 블루프린트의 OnInitialize에서 넣어준 _target을 비우면 안된다. 

	OnCurveAnimationFinished.Unbind();
}

void FGsUICurveAnimation::SetTarget(UWidget* InTarget)
{
	_target = InTarget;
}

void FGsUICurveAnimation::Play(class UWidget* InOwner)
{
	UWidget* targetWidget = (nullptr != _target) ? _target : InOwner;
	if (nullptr == targetWidget)
	{
		return;
	}

	_bIsPlaying = true;

	for (FGsUICurveAnimationData& animData : _animationList)
	{
		animData.Reset();
		animData.SetTarget(targetWidget);
		animData.InvalidateValue(0.0f);
	}
}

void FGsUICurveAnimation::Update(float InDeltaTime)
{
	_currTime += InDeltaTime;

	int finishCount = 0;
	for (FGsUICurveAnimationData& animData : _animationList)
	{
		animData.InvalidateValue(_currTime);

		if (animData.IsFinished())
		{
			++finishCount;
		}
	}

	// 리스트의 애니메이션이 모두 끝나면 종료판정
	if (_animationList.Num() == finishCount)
	{
		_bIsPlaying = false;

		// 애니메이션 종료 메시지 보냄
		OnCurveAnimationFinished.ExecuteIfBound(_triggerType);
	}
}
//----------------------------------------------------------------------------------------------------------------------
void FGsUICurveAnimationList::Clear()
{
	for (FGsUICurveAnimation& curveAnim : _animationList)
	{
		curveAnim.Clear();
	}
}

void FGsUICurveAnimationList::SetTarget(EGsUIAnimationTriggerType InTriggerType, UWidget* InTarget)
{
	for (FGsUICurveAnimation& curveAnim : _animationList)
	{
		if (InTriggerType == curveAnim._triggerType)
		{
			curveAnim.SetTarget(InTarget);
		}
	}
}

bool FGsUICurveAnimationList::Play(EGsUIAnimationTriggerType InTriggerType, UWidget* InOwner)
{
	bool bExist = false;
	for (FGsUICurveAnimation& curveAnim : _animationList)
	{
		if (InTriggerType == curveAnim._triggerType)
		{
			bExist = true;

			curveAnim.Reset();
			curveAnim.Play(InOwner);
		}
		else
		{
			curveAnim.Reset();
		}
	}

	return bExist;
}

void FGsUICurveAnimationList::Update(float InDeltaTime)
{
	for (FGsUICurveAnimation& curveAnim : _animationList)
	{
		if (curveAnim.IsPlaying())
		{
			curveAnim.Update(InDeltaTime);
		}
	}
}

bool FGsUICurveAnimationList::IsPlaying(EGsUIAnimationTriggerType InTriggerType)
{
	for (FGsUICurveAnimation& curveAnim : _animationList)
	{
		if (InTriggerType == curveAnim._triggerType)
		{
			if (curveAnim.IsPlaying())
			{
				return true;
			}
		}
	}

	return false;
}
