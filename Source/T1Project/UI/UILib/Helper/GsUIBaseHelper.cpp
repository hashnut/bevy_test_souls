// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIBaseHelper.h"
#include "Components/PanelWidget.h"
#include "Components/InvalidationBox.h"
#include "Blueprint/UserWidget.h"


void FGsUIBaseHelper::RecursiveFindInvalidationBox(UWidget* InWidget, OUT TArray<UInvalidationBox*>& OutArray)
{
	if (nullptr == InWidget)
	{
		return;
	}

	if (false == InWidget->IsA<UPanelWidget>())
	{
		return;
	}

	// 자신이 InvalidationBox일 경우 저장하고 넘긴다
	if (InWidget->IsA<UInvalidationBox>())
	{
		OutArray.Emplace(Cast<UInvalidationBox>(InWidget));
		return;
	}

	UPanelWidget* panelWidget = Cast<UPanelWidget>(InWidget);

	for (int i = 0; i < panelWidget->GetChildrenCount(); ++i)
	{
		UWidget* widget = panelWidget->GetChildAt(i);
		if (widget->IsA<UInvalidationBox>())
		{
			// 찾았으면 자손들의 InvalidationBox를 더 찾지 않음
			// 부모단에서 InvalidateLayoutAndVolatility 를 하면 자손도 함께 갱신되므로
			OutArray.Emplace(Cast<UInvalidationBox>(widget));
		}
		else
		{
			RecursiveFindInvalidationBox(widget, OutArray);
		}
	}
}

void FGsUIBaseHelper::InvalidateInvalidationBox(TArray<UInvalidationBox*>& InArray)
{
	// InvalidationBox의 자식 위젯을 갱신
	if (0 < InArray.Num())
	{
		for (UInvalidationBox* invalidationBox : InArray)
		{
			if (UWidget* childWidget = invalidationBox->GetChildAt(0))
			{
				childWidget->InvalidateLayoutAndVolatility();
			}
		}
	}
}
