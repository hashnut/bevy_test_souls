#include "GsUITraySummonSkip.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Tray/SummonSkip/GsUISummonWidgetSwitcherEx.h"

#include "Management/GsMessageHolder.h"

#include "Message/GsMessageSystem.h"

#include "UMG/Public/Components/Widget.h"

void UGsUITraySummonSkip::NativeOnInitialized()
{
	_btnSkip->OnClicked.AddDynamic(this, &UGsUITraySummonSkip::OnClickSkip);

	Super::NativeOnInitialized();
}

void UGsUITraySummonSkip::OnClickSkip()
{
	GMessage()->GetSystemParam().SendMessage(MessageSystem::SUMMON_COMPOSE_SKIP, nullptr);
}

void UGsUITraySummonSkip::OnChangedProgressRate(float In_progressRate)
{
	ChangeProgressRateSwitchers(In_progressRate);
	ChangeProgressRateMoveWidget(In_progressRate);
}
// switcher
void UGsUITraySummonSkip::ChangeProgressRateSwitchers(float In_progressRate)
{
	for (UGsUISummonWidgetSwitcherEx* iter : _arrayProgressSwitcher)
	{
		if (iter == nullptr)
		{
			continue;
		}

		iter->SetProgressRate(In_progressRate);
	}
}

// move widget
void UGsUITraySummonSkip::ChangeProgressRateMoveWidget(float In_progressRate)
{
	if (_arrayWidgetPos.Num() != 2 )
	{
		return;
	}
	FGeometry parentGeometry = _dragCanvas->GetCachedGeometry();

	FVector2D parentSize =  parentGeometry.GetLocalSize();
	if (parentSize.IsZero() == true)
	{
		if(_dirtySetProgress == false)
		{ 
			_dirtySetProgress = true;
			_currentProgressRate = In_progressRate;
		}
		return;
	}

	FGeometry startGeometry = _arrayWidgetPos[0]->GetCachedGeometry();
	FVector2D startAbsolutePos = startGeometry.GetAbsolutePosition();

	FGeometry endGeometry = _arrayWidgetPos[1]->GetCachedGeometry();
	FVector2D endAbsolutePos = endGeometry.GetAbsolutePosition();

	

	FVector2D startLocalPos = parentGeometry.AbsoluteToLocal(startAbsolutePos);
	FVector2D endLocalPos = parentGeometry.AbsoluteToLocal(endAbsolutePos);

	float nowYPos = FMath::Lerp(startLocalPos.Y, endLocalPos.Y, In_progressRate);

	FVector2D newPos = FVector2D(0, nowYPos);

	_moveProgressWidget->SetRenderTranslation(newPos);

	if (_dirtySetProgress == true)
	{
		_dirtySetProgress = false;
	}
}

void UGsUITraySummonSkip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_dirtySetProgress == true)
	{
		ChangeProgressRateMoveWidget(_currentProgressRate);
	}
}