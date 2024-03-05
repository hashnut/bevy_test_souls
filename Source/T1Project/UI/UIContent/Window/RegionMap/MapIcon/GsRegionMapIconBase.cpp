#include "GsRegionMapIconBase.h"

#include "UI/UIContent/Common/Icon/GsUIIconBase.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"

#include "UMG/Public/Components/CanvasPanelSlot.h"

#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"

// 위치 갱신
void UGsRegionMapIconBase::UpdatePos()
{
	SetIconPos(GetPos());
}
// 아이콘 위치 갱신
void UGsRegionMapIconBase::SetIconPos(const FVector2D& In_pos)
{
	FVector2D uiPos = GSWorldMap()->GetUIPos(In_pos);
	_icon->SetRenderTranslation(uiPos);
}

// 정리
void UGsRegionMapIconBase::Finalize()
{
	_icon = nullptr;

	if (_timerHandle.IsValid() == true)
	{
		if (UWorld* world = GSMinimap()->GetWorld())
		{
			world->GetTimerManager().ClearTimer(_timerHandle);
			_timerHandle.Invalidate();
		}
	}
}

// 아이콘 세팅
void UGsRegionMapIconBase::SetIcon(UGsUIIconBase* In_icon, 
	UCanvasPanelSlot* In_panelSlot, float In_depth)
{
	_icon = In_icon;

	if (In_panelSlot != nullptr)
	{
		// 센터
		In_panelSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		// 오프셋, 크기
		In_panelSlot->SetOffsets(FMargin(0.f, 0.f,
			0.f, 0.f));
		// 정렬
		In_panelSlot->SetAlignment(FVector2D::ZeroVector);
		// z order
		In_panelSlot->SetZOrder(In_depth);
	}
}

// 타이머 세팅
void UGsRegionMapIconBase::SetTimmer(float In_updateTime)
{
	UWorld* world = GSMinimap()->GetWorld();

	if (world == nullptr)
	{
		return;
	}

	world->GetTimerManager().SetTimer(_timerHandle,
		FTimerDelegate::CreateUObject(this, &UGsRegionMapIconBase::UpdateTimmer), In_updateTime, true);
}

void UGsRegionMapIconBase::SetIconVisibility(bool In_isVisible)
{
	if (nullptr == _icon)
	{
		return;
	}
	ESlateVisibility newVisibility =
		(In_isVisible == true) ?
		ESlateVisibility::SelfHitTestInvisible :
		ESlateVisibility::Hidden;
	_icon->SetVisibility(newVisibility);
}