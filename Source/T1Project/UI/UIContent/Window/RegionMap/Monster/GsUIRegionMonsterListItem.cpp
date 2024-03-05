#include "GsUIRegionMonsterListItem.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ProgressBar.h"

void UGsUIRegionMonsterListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnItem->OnClicked.AddDynamic(this, &UGsUIRegionMonsterListItem::OnClickItem);	
	_btnItem->OnReleased.AddDynamic(this, &UGsUIRegionMonsterListItem::OnReleased);
}

void UGsUIRegionMonsterListItem::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(TEXT("WorldMapText"), TEXT("MonLevel"), _levelTextFormat);

	_btnItem->_onFinishedLongPressGauge.AddUObject(this, &UGsUIRegionMonsterListItem::OnLongPressIcon);

	_btnItem->_callbackOtherProgressBarStart = [this](TFunction<void(bool)> In_callbackFinish)
	{
		_callbackFinishLongPress = In_callbackFinish;
		StartLongPress();
	};

	InitializeLongPress();
}

void UGsUIRegionMonsterListItem::NativeDestruct()
{
	_btnItem->_onFinishedLongPressGauge.Clear();

	// 롱프레스 클리어(상부 전달)
	_callbackLongPress = nullptr;
	// 롱프레스 끝 클리어 (버튼 전달)
	_callbackFinishLongPress = nullptr;

	_btnItem->_callbackOtherProgressBarStart = nullptr;

	Super::NativeDestruct();
}

// 아이템 클릭
void UGsUIRegionMonsterListItem::OnClickItem()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
void UGsUIRegionMonsterListItem::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	if (_imgSelected == nullptr)
	{
		return;
	}

	_imgSelected->SetVisibility((bInIsSelected) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

// 텍스트 세팅
void UGsUIRegionMonsterListItem::SetText(const FText& In_name)
{
	_textName = In_name;
}

void UGsUIRegionMonsterListItem::SetRegionMonsterListItem(const FText& In_name, int32 inLevel)
{
	SetText(In_name);	
	_levelTextBlock->SetText(FText::Format(_levelTextFormat, inLevel));

	InitializeLongPress();
}

// 보스 세팅
void UGsUIRegionMonsterListItem::SetBoss(EGsReionMapMonsterSwitchType In_switchType)
{
	int switchIndex = (int)In_switchType;
	_switcherBoss->SetActiveWidgetIndex(switchIndex);
}
void UGsUIRegionMonsterListItem::OnLongPressIcon()
{
	if (_callbackLongPress)
	{
		_callbackLongPress(_indexInGroup);
	}
}

void UGsUIRegionMonsterListItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isStartLongPress == false)
	{
		return;
	}
	// 0보다 시간이 작으면 바로 처리
	if (_longPressTotalDurationTime <= 0.0f)
	{
		_progressBar->SetPercent(1.0f);
		OnLongPressFinished();
		return;
	}

	// 전 틱에 퍼센트 다찼을때 바로 처리하면
	// 프로그래스바가 다안차고 진행되는거 처럼 보여서
	// 한틱 보내고 처리
	if (_progressBar->Percent >= 1.0f)
	{
		OnLongPressFinished();
		return;
	}

	_time += InDeltaTime;
	float percent = _time / _longPressTotalDurationTime;

	_progressBar->SetPercent(percent);

}
// 롱프레스 끝난 시점 호출(리스트 아이템에서 처리)
void UGsUIRegionMonsterListItem::OnLongPressFinished()
{
	if (_callbackFinishLongPress)
	{
		_callbackFinishLongPress(true);
	}

	InitializeLongPress();
}
// 롱프레스 관련 초기화
void UGsUIRegionMonsterListItem::InitializeLongPress()
{	
	_progressBar->SetVisibility(ESlateVisibility::Hidden);
	_progressBar->SetPercent(0.0f);
	_time = 0.0f;

	_isStartLongPress = false;
}

// 롱프레서 처리 시작
void UGsUIRegionMonsterListItem::StartLongPress()
{
	_isStartLongPress = true;

	_progressBar->SetPercent(0.0f);
	_progressBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_time = 0.0f;
}

// 아이템 릴리즈
void UGsUIRegionMonsterListItem::OnReleased()
{
	if (_isStartLongPress == false)
	{
		return;
	}

	InitializeLongPress();
}