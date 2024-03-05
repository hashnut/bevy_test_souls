#include "GsUIIconMinimap.h"

#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Blueprint/UserWidget.h"

// 캔버스에 붙임
void UGsUIIconMinimap::AddCanvas(UUserWidget* In_widget)
{
	if (_iconRoot == nullptr ||
		In_widget == nullptr)
	{
		return;
	}
	UCanvasPanelSlot* canvasPanelSlot =
		_iconRoot->AddChildToCanvas(In_widget);

	if (nullptr != canvasPanelSlot)
	{
		canvasPanelSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		canvasPanelSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
	}
	
}