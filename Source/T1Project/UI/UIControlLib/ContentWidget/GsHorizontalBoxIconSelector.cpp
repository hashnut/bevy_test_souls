// Fill out your copyright notice in the Description page of Project Settings.


#include "GsHorizontalBoxIconSelector.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/UIContent/Factory/GsIconFactory.h"
#include "UI/UIControlLib/Define/EGsIconType.h"

void UGsHorizontalBoxIconSelector::SynchronizeProperties()
{
	Super::SynchronizeProperties();

#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		int32 currCount = GetChildrenCount();
		int32 createCount = _previewCount - currCount;
		if (0 < createCount)
		{
			for (int i = 0; i < createCount; i++)
			{
				CreateIcon(_iconType, _iconKey);
			}
		}
		else if (0 > createCount)
		{
			int count = createCount * -1;
			for (int i = 0; i < count; i++)
			{
				RemoveChildAt(currCount - i - 1);
			}

			InvalidateLayoutAndVolatility();
		}
	}
#endif
}

UGsUIIconBase* UGsHorizontalBoxIconSelector::CreateIcon(EGsIconType InIconType, uint8 InIconKey)
{
#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		UGsUIIconBase* icon = FGsIconFactory::CreateIcon(InIconType, InIconKey, this);
		if (icon)
		{
			AddChild(icon);
		}

		return icon;
	}

	return FGsIconFactory::CreateIcon(InIconType, InIconKey, this);
#else
	return FGsIconFactory::CreateIcon(InIconType, InIconKey, this);
#endif
}

UGsUIIconBase* UGsHorizontalBoxIconSelector::CreateIcon()
{
	return CreateIcon(_iconType, _iconKey);
}

UPanelWidget* UGsHorizontalBoxIconSelector::GetPanel()
{
	return this;
}

void UGsHorizontalBoxIconSelector::WarmUp(int32 InWarmUpIconCount)
{
	FGsIconFactory::WarmUpIcon(_iconType, InWarmUpIconCount);
}