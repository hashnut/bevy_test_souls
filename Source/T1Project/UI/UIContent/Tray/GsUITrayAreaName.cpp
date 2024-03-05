// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayAreaName.h"
#include "T1Project.h"

#include "Management/ScopeGlobal/GsSoundManager.h"

#include "Management/ScopeGame/GsEventProgressManager.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UMG/Public/Animation/WidgetAnimation.h"

#include "Core/Public/Misc/DateTime.h"

void UGsUITrayAreaName::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UGsUITrayAreaName::NativeDestruct()
{
	if (FGsEventProgressManager* eventProgress = GSEventProgress())
	{
		eventProgress->OnReqFinishEvent(EGsEventProgressType::UI_TRAY_AREA_NAME);

	}
	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	//GSLOG(Log, TEXT("Call UGsUITrayAreaName::NativeDestruct !"));
	Super::NativeDestruct();
}


void UGsUITrayAreaName::SetAreaName(const FText& In_name)
{
	KorName->SetText(In_name);

	StartAnimation();
}
void UGsUITrayAreaName::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUITrayAreaName::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isWaitEnd == true)
	{
		return;
	}


	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTime;

	if (diffTick >= _timeTickWaitTime)
	{
		_isWaitEnd = true;
		// finish
		Close();
	}
}

void UGsUITrayAreaName::StartAnimation()
{
	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	PlayAnimation(_aniFadeOut);

	_startTime = FDateTime::UtcNow().GetTicks();
	_isWaitEnd = false;

	// 5 seond
	_timeTickWaitTime = _waitSecond * ETimespan::TicksPerSecond;
}