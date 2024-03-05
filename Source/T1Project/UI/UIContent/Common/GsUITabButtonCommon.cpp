#include "GsUITabButtonCommon.h"

#include "UMG/Public/Components/SlateWrapperTypes.h"
#include "UMG/Public/Components/TextBlock.h"

#include "T1Project.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"

void UGsUITabButtonCommon::NativeConstruct()
{
	Super::NativeConstruct();
		
	_txtSelectedSub = Cast<UTextBlock>(GetWidgetFromName(TEXT("_txtSelectedSub")));
	if (nullptr != _txtSelectedSub)
	{
		_txtSelectedSub->SetVisibility(ESlateVisibility::Collapsed);
	}

	_txtUnselectedSub = Cast<UTextBlock>(GetWidgetFromName(TEXT("_txtUnselectedSub")));
	if (nullptr != _txtSelectedSub)
	{
		_txtUnselectedSub->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	_redDot->SetRedDot(false, EGsIconRedDotType::NORMAL);
}

void UGsUITabButtonCommon::SetTitleText(const FText& InText)
{
	if (nullptr != _txtSelected)
	{
		_txtSelected->SetText(InText);
	}

	if (nullptr != _txtUnselected)
	{
		_txtUnselected->SetText(InText);
	}
}

void UGsUITabButtonCommon::SetSubText(const FText& InText)
{
	const ESlateVisibility SlateVisibility = InText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible;

	if (nullptr != _txtSelectedSub)
	{
		_txtSelectedSub->SetText(InText);
		_txtSelectedSub->SetVisibility(SlateVisibility);
	}

	if (nullptr != _txtUnselectedSub)
	{
		_txtUnselectedSub->SetText(InText);
		_txtUnselectedSub->SetVisibility(SlateVisibility);
	}
}

void UGsUITabButtonCommon::SetLockedText(const FText& InText)
{
	if (nullptr == _txtLocked)
	{
		GSLOG(Warning, TEXT("nullptr != _txtLocked, InText:%s"), *InText.ToString());
		return;
	}
	
	_txtLocked->SetText(InText);

	_wgtLocked->SetVisibility(InText.IsEmpty() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UGsUITabButtonCommon::SetIsRedDot(EGsIconRedDotType InType, bool bIsRedDot, int32 InNumber /*= 0*/)
{
	if (nullptr != _redDot)
	{
		_redDot->SetRedDot(bIsRedDot, InType, InNumber);
	}	
}

//void UGsUITabButtonCommon::SetInputBlock(bool InbShouldBlock)
//{
//	SetInputActionBlocking(InbShouldBlock);
//}

IGsToggleGroupEntry* UGsUITabButtonCommon::GetToggleGroupEntry()
{
	return _btnMain;
}