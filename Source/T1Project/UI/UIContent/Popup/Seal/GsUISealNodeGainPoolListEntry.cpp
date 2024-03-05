#include "GsUISealNodeGainPoolListEntry.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "T1Project.h"

void UGsUISealNodeGainPoolListEntry::SetParameters(const Parameters& InParameters)
{
	int32 SwitcherIndex = 0;
	UTextBlock* TargetTitleTxt = nullptr;
	UTextBlock* TargetValueTxt = nullptr;

	switch (InParameters._color)
	{
	case SealSlotColor::NONE:
		SwitcherIndex = 0;
		TargetTitleTxt = _title0Txt;
		TargetValueTxt = _value0Txt;
		break;

	case SealSlotColor::RARE:
		SwitcherIndex = 1;
		TargetTitleTxt = _title1Txt;
		TargetValueTxt = _value1Txt;
		break;

	case SealSlotColor::EPIC:
		SwitcherIndex = 2;
		TargetTitleTxt = _title2Txt;
		TargetValueTxt = _value2Txt;
		break;

	case SealSlotColor::UNIQUE:
		SwitcherIndex = 3;
		TargetTitleTxt = _title3Txt;
		TargetValueTxt = _value3Txt;
		break;

	default:
		GSLOG(Error, TEXT("Invalid InParameters._color:%s"), *SealSlotColorEnumToString(InParameters._color));
		break;
	}

	if (nullptr != TargetTitleTxt)
	{
		TargetTitleTxt->SetText(InParameters._title);
	}

	if (nullptr != TargetValueTxt)
	{
		TargetValueTxt->SetText(InParameters._value);
	}

	_colorSwitcher->SetActiveWidgetIndex(SwitcherIndex);
}
