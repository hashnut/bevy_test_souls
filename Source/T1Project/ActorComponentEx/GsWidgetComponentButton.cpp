#include "GsWidgetComponentButton.h"

#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/NamedSlotInterface.h"
#include "UMG/Public/Components/PanelWidget.h"

#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/Engine/GameViewportClient.h"
#include "Engine/Classes/GameFramework/HUD.h"
#include "Engine/Public/Slate/SceneViewport.h"

#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/GsScopeHolder.h"

#include "Engine.h"


// 자식들까지 체크
void UGsWidgetComponentButton::ForWidgetAndChildren(
	UWidget* In_widget, TFunctionRef<void(UWidget*)> In_predicate,
	bool In_recursive)
{
    // Search for any named slot with content that we need to dive into.
    if (INamedSlotInterface* NamedSlotHost = Cast<INamedSlotInterface>(In_widget))
    {
        TArray<FName> SlotNames;
        NamedSlotHost->GetSlotNames(SlotNames);

        for (FName SlotName : SlotNames)
        {
            if (UWidget* SlotContent = NamedSlotHost->GetContentForSlot(SlotName))
            {
                In_predicate(SlotContent);

                if (In_recursive) {
                    ForWidgetAndChildren(SlotContent, In_predicate);
                }
            }
        }
    }

    // Search standard children.
    if (UPanelWidget* PanelParent = Cast<UPanelWidget>(In_widget))
    {
        for (int32 ChildIndex = 0; ChildIndex < PanelParent->GetChildrenCount(); ChildIndex++)
        {
            if (UWidget* ChildWidget = PanelParent->GetChildAt(ChildIndex))
            {
                In_predicate(ChildWidget);

                if (In_recursive) {
                    ForWidgetAndChildren(ChildWidget, In_predicate);
                }
            }
        }
    }
}

bool UGsWidgetComponentButton::ManageTouchEvent()
{
    // 액티브안되어있으면 처리 안함    
    if (IsActive() == false)
    {
        return false;
    }

    APlayerController* Controller = GetWorld()->GetFirstPlayerController();

    UUserWidget* widget = GetWidget();

    if (widget == nullptr) return false;
    if (Controller == nullptr) return false;
    if (!GEngine || !GEngine->GameViewport || !GEngine->GameViewport->GetGameViewport()) 
        return false;

    float TouchX, TouchY;
    bool bIsCurrentlyPressed;
    Controller->GetInputTouchState(ETouchIndex::Touch1, TouchX, TouchY, bIsCurrentlyPressed);


    _prevTouchPos = _touchPos;

    _touchPos.X = TouchX;
    _touchPos.Y = TouchY;

    // No interaction here!
    if (widget->GetVisibility() != ESlateVisibility::SelfHitTestInvisible
        && widget->GetVisibility() != ESlateVisibility::Visible) {
        return false;
    }

    if (!IsInWidgetZone(_touchPos, widget->GetRootWidget()))
    {
        return false;
    }
    // 터치 성공
    if (_callbackClick != nullptr)
    {
        _callbackClick(true);
    }
    return true;
}

void UGsWidgetComponentButton::BeginPlay()
{
    Super::BeginPlay();
    if (nullptr != GSInteraction())
    {
        GSInteraction()->AddWidgetComponentButton(this);
    }
        

}
void UGsWidgetComponentButton::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (nullptr != GSInteraction())
    {
        GSInteraction()->DelWidgetComponentButton(this);
    }
}

bool UGsWidgetComponentButton::IsInWidgetZone(FVector2D In_touchPos, UWidget* In_widget)
{
    // No need to go further here
    if (In_widget->GetVisibility() != ESlateVisibility::SelfHitTestInvisible
        && In_widget->GetVisibility() != ESlateVisibility::Visible) {
        return false;
    }

    // Check if in zone
    if (In_widget->GetVisibility() == ESlateVisibility::Visible) {
        FGeometry ViewportGeo = GEngine->GameViewport->GetGameViewport()->GetCachedGeometry();
        // This works only for a visible widget.
        FGeometry Geo = In_widget->GetCachedGeometry();
        FVector2D WidgetSize = Geo.GetAbsoluteSize();
        FVector2D WidgetPos = Geo.GetAbsolutePositionAtCoordinates(FVector2D(0, 0));


        // 위젯 위치에서 뷰포트 절대값 빼면 viewport (0,0) 에 맞춤
        FVector2D WidgetShiftPos = WidgetPos - ViewportGeo.GetAbsolutePosition();


        if (true == GSInteraction()->GetTestShowInteractionButtonPos())
        {
            GSInteraction()->SetTestTouchScreenPos(_touchPos);
            GSInteraction()->SetTestWidgetShiftPos(WidgetShiftPos);
            GSInteraction()->SetTestWidgetSize(WidgetSize);
        }

        // Check if touch position is in the widget zone
        if (_touchPos > WidgetShiftPos && _touchPos < WidgetShiftPos + WidgetSize) {
            return true;
        }
    }

    // If touch position is not on the widget or his visibility is set to SelfHitTestInvisible,
    // maybe we have better chance in it's children 

    TArray<UWidget*> AllWidgets;
    // Retrieve each direct children to test them
    ForWidgetAndChildren(In_widget, [&AllWidgets](UWidget* ChildWidget) {
        AllWidgets.Add(ChildWidget);
        }, false);

    for (UWidget* SubWidget : AllWidgets)
    {
        // Go to check recursively in this widget's children
        bool bIsInZone = IsInWidgetZone(_touchPos, SubWidget);

        // If a child is hit, no need to go further
        if (bIsInZone) {
            return true;
        }
        else {
            continue;
        }
    }

    return false;
}
