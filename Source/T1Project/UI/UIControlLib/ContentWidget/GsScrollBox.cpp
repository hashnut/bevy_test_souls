// Fill out your copyright notice in the Description page of Project Settings.


#include "GsScrollBox.h"
#include "Components/PanelWidget.h"
#include "SlateComponent/SGsScrollBox.h"
#include "Components/ScrollBoxSlot.h"


UGsScrollBox::UGsScrollBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

TSharedRef<SWidget> UGsScrollBox::RebuildWidget()
{
	MyScrollBox = SNew(SGsScrollBox)
		.Style(&WidgetStyle)
		.ScrollBarStyle(&WidgetBarStyle)
		.Orientation(Orientation)
		.ConsumeMouseWheel(ConsumeMouseWheel)
		.NavigationDestination(NavigationDestination)
		.NavigationScrollPadding(NavigationScrollPadding)
		.ScrollWhenFocusChanges(ScrollWhenFocusChanges)
		.AnimateWheelScrolling(bAnimateWheelScrolling)
		.WheelScrollMultiplier(WheelScrollMultiplier)
		.OnUserScrolled(BIND_UOBJECT_DELEGATE(FOnUserScrolled, SlateHandleUserScrolled))
		.OnNativeTick(BIND_UOBJECT_DELEGATE(FOnTIck, SlateHandleTick));

	for ( UPanelSlot* PanelSlot : Slots )
	{
		if ( UScrollBoxSlot* TypedSlot = Cast<UScrollBoxSlot>(PanelSlot) )
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyScrollBox.ToSharedRef());
		}
	}

	return MyScrollBox.ToSharedRef();
}

void UGsScrollBox::SlateHandleTick(const FGeometry& Geometry, const float DeltaTime)
{
}

void UGsScrollBox::BeginDestroy()
{
	ListItems.Reset();
	_widgets.Reset();

	_entryPool.Flush();

	Super::BeginDestroy();
}

void UGsScrollBox::Destruct()
{
	ListItems.Reset();
}

void UGsScrollBox::AddItem(UObject* Item)
{
	ListItems.Add(Item);
	RequestRefresh();
}

void UGsScrollBox::RemoveItem(UObject* Item)
{
	ListItems.Remove(Item);

	RequestRefresh();
}

UObject* UGsScrollBox::GetItemAt(int32 Index) const
{
	return ListItems.IsValidIndex(Index) ? ListItems[Index] : nullptr;
}

int32 UGsScrollBox::GetIndexForItem(UObject* Item) const
{
	return ListItems.Find(Item);
}

void UGsScrollBox::ClearListItems()
{
	if (auto GsSlateScrollBox = StaticCastSharedPtr<SGsScrollBox>(MyScrollBox))
	{
		GsSlateScrollBox->SetClearScroll();
	}

	_selectEntry = nullptr;

	ListItems.Reset();
	RequestRefresh();
}

void UGsScrollBox::InitializeData()
{
	ClearChildren();
}

void UGsScrollBox::RequestRefresh()
{
	while (_widgets.Num() > ListItems.Num())
	{
		auto item = _widgets.Last(0);
		if (auto entry = item->GetWidget().Get())
		{
			entry->SetToggleCallback(nullptr);
			OnEntryWidgetReleasedEvent.Broadcast(*entry);
			RemoveChild(entry);
		}
		_widgets.Remove(item);
	}

	TWeakObjectPtr<UGsScrollBox> thiz = this;
	while (_widgets.Num() < ListItems.Num())
	{
		auto widgetIndex = _widgets.Add(_entryPool.GetItem(GetWorld(), GetEntryWidgetClass()));
		if (auto entry = _widgets[widgetIndex]->GetWidget().Get())
		{
			OnEntryWidgetGeneratedEvent.Broadcast(*entry);
			AddChild(entry);

			if (useSelection)
			{
				entry->SetToggleCallback([thiz](UUserObjectScrollEntry* selectEntry, bool select) {
					if (thiz.IsValid()) thiz->SetSelectEntry(selectEntry, select);
					});
			}
		}
	}
	
	for (auto data : ListItems)
	{
		auto index = ListItems.IndexOfByKey(data);
		if (_widgets.IsValidIndex(index))
		{
			if (auto entry = _widgets[index]->GetWidget().Get())
			{
				entry->NativeOnListItemObjectSet(data);
			}
		}
	}

	InvalidateLayoutAndVolatility();
}

void UGsScrollBox::SetScrollIndex(int32 topIndex)
{
	if (GetNumItems() <= 0) return;

	_toBottomScrolled = false;

	auto widgetCount = GetChildrenCount();
	for (auto index = 0; index < widgetCount; index++)
	{
		if (auto childWidget = static_cast<UUserObjectScrollEntry*>(GetChildAt(index)))
		{
			childWidget->InvalidateLayout();
		}
	}

	if (auto GsSlateScrollBox = StaticCastSharedPtr<SGsScrollBox>(MyScrollBox))
	{
		TSharedPtr<SWidget> SlateWidgetToFind;
		if (auto WidgetToFind = GetChildAt(topIndex))
		{
			SlateWidgetToFind = WidgetToFind->GetCachedWidget();
			GsSlateScrollBox->ScrollWidgetIntoView(SlateWidgetToFind, false, EDescendantScrollDestination::TopOrLeft);
		}
	}

	InvalidateLayoutAndVolatility();

	if (IsScrollEndPosition()) SetScrollEnd();
}

void UGsScrollBox::SetScrollEnd()
{
	_toBottomScrolled = true;
	if(MyScrollBox.IsValid()) MyScrollBox->ScrollToEnd();
}

bool UGsScrollBox::IsScrollEndPosition()
{	
	return FMath::IsNearlyEqual(GetScrollOffsetOfEnd(), GetScrollOffset(), toEndTolerance);
}

int UGsScrollBox::GetItemIndexByOffset(float offset)
{
	FVector2D compoSize = FVector2D::ZeroVector;
	auto children = GetAllChildren();

	UWidget* offsetWidget = nullptr;
	for (auto child : children)
	{
		offsetWidget = child;
		compoSize += child->GetDesiredSize();
		auto itmeOffset = GetScrollComponentFromVector(compoSize);
		if (itmeOffset > offset)
		{
			break;
		}
	}

	return children.IndexOfByKey(offsetWidget);
}

void UGsScrollBox::SetSelectIndex(int32 index)
{
	TWeakObjectPtr<UGsScrollBox> thiz = this;

	auto selectIndex = _widgets.IsValidIndex(index) ? index : -1;

	if (selectIndex >= 0)
	{
		if (auto entry = _widgets[selectIndex]->GetWidget().Get())
		{
			SetSelectEntry(entry, true);
		}
	}
}

void UGsScrollBox::SetSelectEntry(const UUserObjectScrollEntry* selectEntry, bool forced)
{
	for (auto data : ListItems)
	{
		auto index = ListItems.IndexOfByKey(data);
		if (_widgets.IsValidIndex(index))
		{
			if (auto entry = _widgets[index]->GetWidget().Get())
			{
				bool isSelected = selectEntry == entry;
				if ((entry->_isSelected != isSelected) || (forced && isSelected))
				{	
					_selectEntry = isSelected ? entry : _selectEntry;
					entry->_isSelected = isSelected;
					entry->NativeOnItemSelectionChanged(isSelected);
				}
			}
		}
	}
}

void UGsScrollBox::RefreshAll()
{
	for (auto data : ListItems)
	{
		auto index = ListItems.IndexOfByKey(data);
		if (_widgets.IsValidIndex(index))
		{
			if (auto entry = _widgets[index]->GetWidget().Get())
			{
				entry->NativeOnListItemObjectSet(data);
			}
		}
	}
}
