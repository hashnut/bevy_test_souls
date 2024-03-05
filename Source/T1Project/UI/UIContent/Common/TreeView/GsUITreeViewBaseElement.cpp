

#include "GsUITreeViewBaseElement.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "T1Project/T1Project.h"


void UGsUITreeViewBaseElement::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();
	// 

	_btnTrigger->OnClicked.AddDynamic(this, &UGsUITreeViewBaseElement::OnClickedElement);
}

void UGsUITreeViewBaseElement::BeginDestroy()
{
	// 
	UUserWidget::BeginDestroy();
}

void UGsUITreeViewBaseElement::OnClickedElement()
{		
	OnClickedDelegate.ExecuteIfBound(this, false == IsSelected());
}

void UGsUITreeViewBaseElement::SetSelected(bool InIsSelected)
{
	OnSelectedDelegate.ExecuteIfBound(this, InIsSelected);
}

bool UGsUITreeViewBaseElement::GetIsSelected() const
{
	return IsSelected();
}

void UGsUITreeViewBaseElement::SetData(const FGsTreeViewElementData* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_data = InData;

	_btnFirstStateTitle->SetText(_data->name);
	_btnSecondStateTitle->SetText(_data->name);

	// 2021/11/23 PKT - default : (Selected = Faldse)
	SetSelected(false);
}

void UGsUITreeViewBaseElement::SetIsRedDot(bool InIsRedDot)
{
	_redDot->SetRedDot(InIsRedDot);

	// 2021/11/23 PKT - delegate
	OnRedDotDelegate.ExecuteIfBound(this, InIsRedDot);
}

bool UGsUITreeViewBaseElement::GetIsRedDot() const
{
	return (_redDot->GetVisibility() == ESlateVisibility::HitTestInvisible);
}

const FGsTreeViewElementData* UGsUITreeViewBaseElement::GetData() const
{
	return _data;
}