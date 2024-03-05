#include "GsMinimapIconBase.h"

#include "UI/UIContent/Common/Icon/GsUIIconBase.h"

#include "Map/Minimap/SceneImage/GsMinimapSceneImage.h"

#include "UMG/Public/Components/CanvasPanelSlot.h"

UGsMinimapIconBase::~UGsMinimapIconBase()
{
}

// 클리어
void UGsMinimapIconBase::Finalize()
{
	if (nullptr != _icon)
	{
		_icon = nullptr;
	}

	_sceneImage = nullptr;
}

// 위치 갱신
void UGsMinimapIconBase::UpdatePos()
{
	SetIconPos(GetPos());
}

// 아이콘 위치 세팅
void UGsMinimapIconBase::SetIconPos(FVector2D In_objectPos)
{
	if (nullptr == _icon ||
		nullptr == _sceneImage ||
		nullptr == _canvasPanelSlot)
	{
		return;
	}
	FVector2D uiPos = GetUIPos(In_objectPos);
	_icon->SetRenderTranslation(uiPos);
}
// ui 위치 구하기
 FVector2D UGsMinimapIconBase::GetUIPos(const FVector2D& In_objectPos)
{
	 if (_sceneImage == nullptr)
	 {
		 return  FVector2D::ZeroVector;
	 }

	 return _sceneImage->GetUIPos(In_objectPos);
}

// 해제
void UGsMinimapIconBase::Release()
{
	if (nullptr != _icon)
	{
		_icon->RemoveFromParent();
	}

}
void UGsMinimapIconBase::SetCanvasPanelSlot(UCanvasPanelSlot* In_slot)
{
	_canvasPanelSlot = In_slot;
	if (_canvasPanelSlot != nullptr)
	{
		_canvasPanelSlot->SetSize(FVector2D::ZeroVector);
		_canvasPanelSlot->SetZOrder(_iconDepth);
	}
}

bool UGsMinimapIconBase::IsIconHidden() const
{
	return (_icon->GetVisibility() == ESlateVisibility::Collapsed);
}

void UGsMinimapIconBase::SetIconHidden(bool InIsHidden)
{
	if (_icon)
	{
		ESlateVisibility visibility = InIsHidden ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible;
		if (visibility != _icon->GetVisibility())
		{	// 2022/09/30 PKT - 상태가 변경 되었을 경우에만 설정
			_icon->SetVisibility(InIsHidden ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
		}
	}
}