// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTutorial.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/SizeBox.h"
#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Tray/Tutorial/GsUITutorialSubDialog.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "Engine/Public/UnrealClient.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/GsMessageHolder.h"
#include "DataSchema/SubDialog/GsSchemaSubDialogCutData.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"


UWidget* UGsUITrayTutorial::RecursiveFindWidget(UUserWidget* InUserWidget, const FName& InRootName, const FName& InName)
{
	TArray<UWidget*> widgetList;
	InUserWidget->WidgetTree->GetAllWidgets(widgetList);
	bool bIsTargetWidget = false;

	for (UWidget* iter : widgetList)
	{
		if (bIsTargetWidget)
		{
			if (iter->GetFName() == InName)
			{
				return iter;
			}
		}
		else
		{
			if (iter->GetFName() == InRootName)
			{
				bIsTargetWidget = true;
				if (iter->GetFName() == InName)
				{
					return iter;
				}
			}

			if (iter->IsA<UUserWidget>())
			{
				if (UWidget* targetWidget = RecursiveFindWidget(Cast<UUserWidget>(iter), InRootName, InName))
				{
					return targetWidget;
				}
			}
		}
	}

	return nullptr;
}

UWidget* UGsUITrayTutorial::RecursiveFindWidget(UWidget* InWidget, const FName& InName)
{
	if (false == InWidget->IsA<UPanelWidget>())
	{
		return nullptr;
	}

	UPanelWidget* panel = Cast<UPanelWidget>(InWidget);

	for (int i = 0, maxCount = panel->GetChildrenCount(); i < maxCount; ++i)
	{
		UWidget* child = panel->GetChildAt(i);

		if (child->GetFName() == InName)
		{
			return child;
		}

		if (child->IsA<UPanelWidget>())
		{
			if (UWidget* targetWidget = RecursiveFindWidget(child, InName))
			{
				return targetWidget;
			}
		}
	}

	return nullptr;
}

void UGsUITrayTutorial::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSkip->OnClicked.AddDynamic(this, &UGsUITrayTutorial::OnClickSkip);
}

void UGsUITrayTutorial::NativeConstruct()
{
	Super::NativeConstruct();

	_handleResize = FViewport::ViewportResizedEvent.AddUObject(this, &UGsUITrayTutorial::OnResizeViewport);

	_bCheckClick = false;
	_interactionType = EGsTutorialUIInteractionType::NONE;
	_resetSize.X = 10.f;
	_resetSize.Y = 10.f;
}

void UGsUITrayTutorial::NativeDestruct()
{
	if (_handleResize.IsValid())
	{
		FViewport::ViewportResizedEvent.Remove(_handleResize);
	}

	UnbindInteractionEvent();

	_dialogLeft->DestroyActor();
	_dialogRight->DestroyActor();

	_actionId = 0;	
	_backupBtnLockSec = 0.f;

	_bIsTargetFound = false;
	_targetingDelaySec = 0.f;
	_targetingDelayRemainSec = 0.f;
	
	_skipBtnActiveRemainSec = 0.f;

	_prevSizeBoxSize = FVector2D::ZeroVector;
	_prevSizeBoxPos = FVector2D::ZeroVector;
	_targetWidget = nullptr;
	_visualWidget = nullptr;	

	ResetSize();

	Super::NativeDestruct();
}

void UGsUITrayTutorial::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 스킵버튼 시간 체크
	if (0 < _skipBtnActiveRemainSec)
	{
		_skipBtnActiveRemainSec -= InDeltaTime;
		if (0 >= _skipBtnActiveRemainSec)
		{
			SetActiveSkipButton(true);
		}
	}

	if (false == _visualWidget.IsValid())
	{
		return;
	}

	// 타깃이 아직 영역 계산이 안되어있을 경우를 방지
	const FGeometry targetGeometry = _visualWidget->GetCachedGeometry();
	if (FVector2D::ZeroVector == targetGeometry.GetLocalSize())
	{
		return;
	}

	// 위젯 갱신이 안되어있을 경우를 방지
	const FGeometry sizeBoxGeometry = _sizeBoxTarget->GetCachedGeometry();
	if (FVector2D::ZeroVector == sizeBoxGeometry.GetLocalSize())
	{
		return;
	}

	// 값변경있으면 무조건 갱신
	if (_prevSizeBoxSize != sizeBoxGeometry.GetLocalSize() ||
		_prevSizeBoxPos != sizeBoxGeometry.AbsolutePosition)
	{
		CalcSize();
	}
	
	_prevSizeBoxSize = sizeBoxGeometry.GetLocalSize();
	_prevSizeBoxPos = sizeBoxGeometry.AbsolutePosition;

	// 타겟 표시 딜레이 로직
	if (_bIsTargetFound)
	{
		if (0 < _targetingDelayRemainSec)
		{
			_targetingDelayRemainSec -= InDeltaTime;
			if (0 >= _targetingDelayRemainSec)
			{
				_targetingDelayRemainSec = 0.f;

				_btnLock->SetVisibility(ESlateVisibility::Collapsed);
				SetVisibleTargetEffect(true);
			}
		}
	}
}

FReply UGsUITrayTutorial::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	// 입력이 있으면 시간 스킵 시간 리셋
	SetActiveSkipButton(false);

	if (EGsTutorialUIInteractionType::CLICK_REGION == _interactionType ||
		EGsTutorialUIInteractionType::CLICK_REGION_INTERFACE_EVENT == _interactionType ||
		false == _bUseTarget)
	{
		// 터치 이펙트 출력
		if (_clickVFXPath.IsValid())
		{
			UClass* vfxClass = GPath()->GetClassByObjectPath(_clickVFXPath);
			if (vfxClass)
			{
				UGsBlueprintFunctionLibraryUI::AddVFXToPointerPos(vfxClass, 1000);
			}
		}

		_bCheckClick = true;
	}

	return FReply::Handled();
}

FReply UGsUITrayTutorial::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	// 입력이 있으면 시간 스킵 시간 리셋
	SetActiveSkipButton(false);

	return FReply::Handled();
}

FReply UGsUITrayTutorial::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (EGsTutorialUIInteractionType::CLICK_REGION == _interactionType ||
		EGsTutorialUIInteractionType::CLICK_REGION_INTERFACE_EVENT == _interactionType)
	{
		if (_bCheckClick)
		{
			OnClickRegion();
		}
	}
	else if (false == _bUseTarget)
	{
		if (_bCheckClick)
		{
			EndInteraction();
		}
	}

	_bCheckClick = false;

	return FReply::Unhandled();
}

bool UGsUITrayTutorial::SetNoTargetData(int32 InActionId)
{
	if (0 != _actionId)
	{
		EndInteraction();
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	_bUseTarget = false;
	_actionId = InActionId;
	_bIsVisibleAfterInteraction = true;
	_interactionType = EGsTutorialUIInteractionType::NONE;

	_bIsTargetFound = false;
	_targetingDelaySec = 0.f;
	_targetingDelayRemainSec = 0.f;	

	_targetWidget.Reset();
	_visualWidget.Reset();

	_btnLock->SetVisibility(ESlateVisibility::Collapsed);
	SetVisibleTargetEffect(false);
	SetActiveSkipButton(false);

	return true;
}

bool UGsUITrayTutorial::SetTargetData(int32 InActionId, const FGsSchemaTutorialActionUIInteraction* InData,
	UWidget* InTargetWidget, UWidget* InVisualWidget)
{
	if (nullptr == InData)
	{
		return false;
	}

	if (0 != _actionId)
	{
		EndInteraction();
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	_bUseTarget = true;
	_actionId = InActionId;
	_bIsVisibleAfterInteraction = InData->isVisibleAfterInteraction;
	_interactionType = InData->targetInteractionType;
	
	_bIsTargetFound = false;
	_targetingDelaySec = InData->targetingDelay;
	_targetingDelayRemainSec = 0.f;

	// 인터렉션할 타겟 설정
	if (InTargetWidget)
	{
		_targetWidget = InTargetWidget;
	}
	else
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(InData->wbpKey);
		if (false == widget.IsValid())
		{
			return false;
		}

		_targetWidget = RecursiveFindWidget(widget.Get(), FName(InData->targetRootWidget), FName(InData->targetWidget));
	}

	// 영역 설정. 다른 값이 없으면 타겟과 동일
	if (InVisualWidget)
	{
		_visualWidget = InVisualWidget;
	}
	else
	{
		if (InData->visualRootWidget.IsEmpty())
		{
			_visualWidget = _targetWidget;
		}
		else
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(InData->wbpKey);
			if (false == widget.IsValid())
			{
				return false;
			}

			_visualWidget = RecursiveFindWidget(widget.Get(), FName(InData->visualRootWidget), FName(InData->visualTargetWidget));
		}
	}

	if (false == _targetWidget.IsValid() &&
		false == _visualWidget.IsValid())
	{
		return false;
	}
	
	ResetSize();

	SetInteraction(InData->targetInteractionType, _targetWidget.Get());
	SetArrow(InData->arrowDirection);
	SetEffect(InData->effectType);
	SetActiveSkipButton(false);

	SetCheckSize();	

	return true;
}

void UGsUITrayTutorial::SetSubDialog(const FGsSchemaSubDialogCutData* InData, EGsTutorialUIDialogDirection InDirection)
{
	if (nullptr == InData)
	{
		InDirection = EGsTutorialUIDialogDirection::NONE;
	}

	UGsUITutorialSubDialog* dialog = nullptr;
	switch (InDirection)
	{
	case EGsTutorialUIDialogDirection::LEFT:
		dialog = _dialogLeft;
		_dialogLeft->SetVisibility(ESlateVisibility::HitTestInvisible);
		_dialogRight->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EGsTutorialUIDialogDirection::RIGHT:
		dialog = _dialogRight;
		_dialogLeft->SetVisibility(ESlateVisibility::Collapsed);
		_dialogRight->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;
	default:
		_dialogLeft->SetVisibility(ESlateVisibility::Collapsed);
		_dialogRight->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}

	if (dialog)
	{
		dialog->StartAction(InData);
	}
}

void UGsUITrayTutorial::SetArrow(EGsTutorialUIArrowDirection InDirection)
{
	if (EGsTutorialUIArrowDirection::NONE == InDirection)
	{
		_switcherArrow->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_switcherArrow->SetVisibility(ESlateVisibility::HitTestInvisible);

	int32 index = static_cast<int32>(InDirection) - 1;
	_switcherArrow->SetActiveWidgetIndex(index);
}

void UGsUITrayTutorial::SetEffect(EGsTutorialUIEffectType InType)
{
	if (EGsTutorialUIEffectType::NONE == InType)
	{
		_switcherEffect->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_switcherEffect->SetVisibility(ESlateVisibility::HitTestInvisible);

	int32 index = static_cast<int32>(InType) - 1;
	_switcherEffect->SetActiveWidgetIndex(index);
}

void UGsUITrayTutorial::SetVisibleTargetEffect(bool bVisible)
{
	// 반대로
	_imgCenter->SetVisibility(bVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	float renderOp = bVisible ? 1.f : 0.f;

	// 끄면 연산이 안될까봐 연산을 위해
	_uiRectEffect->SetRenderOpacity(renderOp);
	_switcherArrow->SetRenderOpacity(renderOp);
	_switcherEffect->SetRenderOpacity(renderOp);
}

void UGsUITrayTutorial::SetActiveSkipButton(bool bIsActive)
{
	if (bIsActive)
	{
		_skipBtnActiveRemainSec = 0.f;
		_btnSkip->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_skipBtnActiveRemainSec = _skipButtonActiveSecond;
		_btnSkip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUITrayTutorial::SetInteraction(EGsTutorialUIInteractionType InType, UWidget* InTargetWidget)
{
	_sizeBoxTarget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	switch (InType)
	{
	case EGsTutorialUIInteractionType::CLICK_REGION:
	{
		_sizeBoxTarget->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	case EGsTutorialUIInteractionType::CLICK_BUTTON:
	{
		if (UGsButton* button = Cast<UGsButton>(InTargetWidget))
		{
			_backupBtnLockSec = button->_clickLockSecond;
			button->OnClicked.AddDynamic(this, &UGsUITrayTutorial::OnClickTarget);
			button->_clickLockSecond = 1.f; // 광클 방지를 위해 강제로 걸어버림
		}
	}
	break;
	case EGsTutorialUIInteractionType::LONG_PRESS_ITEM_ICON:
	{
		if (UGsUIIconItem* itemIcon = Cast<UGsUIIconItem>(InTargetWidget))
		{
			_handleLongPressItemIcon =
				itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUITrayTutorial::OnLongPressItemIcon);
		}
	}
	break;
	case EGsTutorialUIInteractionType::INTERFACE_EVENT:
	{
		if (IGsTutorialInteractionInterface* interactionInterface =
			Cast<IGsTutorialInteractionInterface>(InTargetWidget))
		{
			interactionInterface->SetIsTutorialInteraction(true);
			interactionInterface->GetTutorialInteraction().BindUObject(this, &UGsUITrayTutorial::OnInteractionInterface);
		}
	}
	break;
	case EGsTutorialUIInteractionType::CLICK_REGION_INTERFACE_EVENT:
	{
		_sizeBoxTarget->SetVisibility(ESlateVisibility::Visible);

		if (IGsTutorialInteractionInterface* interactionInterface =
			Cast<IGsTutorialInteractionInterface>(InTargetWidget))
		{
			interactionInterface->SetIsTutorialInteraction(true);
			interactionInterface->GetTutorialInteraction().BindUObject(this, &UGsUITrayTutorial::OnInteractionInterface);
		}
	}
	break;
	default:
		break;
	}
}

void UGsUITrayTutorial::EndInteraction()
{
	if (0 == _actionId)
	{
		return;
	}

	_btnLock->SetVisibility(ESlateVisibility::Visible);

	UnbindInteractionEvent();

	_dialogLeft->DestroyActor();
	_dialogRight->DestroyActor();

	if (false == _bIsVisibleAfterInteraction)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}

	FGsTutorialMsgParamInt param(_actionId);

	// 순서에 유의. 메시지를 보내기 전에 비워야 다음 액션 시 정보가 남아있지 않음
	_actionId = 0;
	_targetWidget = nullptr;
	_visualWidget = nullptr;
	_prevSizeBoxSize = FVector2D::ZeroVector;
	_prevSizeBoxPos = FVector2D::ZeroVector;

	GMessage()->GetTutorial().SendMessage(MessageContentTutorial::END_UI_INTERACTION, &param);
}

void UGsUITrayTutorial::UnbindInteractionEvent()
{
	switch (_interactionType)
	{
	case EGsTutorialUIInteractionType::CLICK_BUTTON:
	{
		if (_targetWidget.IsValid())
		{
			if (UGsButton* button = Cast<UGsButton>(_targetWidget.Get()))
			{
				button->OnClicked.RemoveDynamic(this, &UGsUITrayTutorial::OnClickTarget);
				button->_clickLockSecond = _backupBtnLockSec;
			}
		}
	}
	break;
	case EGsTutorialUIInteractionType::LONG_PRESS_ITEM_ICON:
	{
		if (_targetWidget.IsValid())
		{
			if (UGsUIIconItem* itemIcon = Cast<UGsUIIconItem>(_targetWidget.Get()))
			{
				itemIcon->OnFinishedLongPressEvent.Remove(_handleLongPressItemIcon);
			}
		}
	}
	break;
	case EGsTutorialUIInteractionType::INTERFACE_EVENT:
	case EGsTutorialUIInteractionType::CLICK_REGION_INTERFACE_EVENT:
	{
		if (_targetWidget.IsValid())
		{
			if (IGsTutorialInteractionInterface* interactionInterface =
				Cast<IGsTutorialInteractionInterface>(_targetWidget.Get()))
			{
				interactionInterface->SetIsTutorialInteraction(false);
				interactionInterface->GetTutorialInteraction().Unbind();
			}
		}
	}
	break;
	}
}

void UGsUITrayTutorial::SetCheckSize()
{
	_prevSizeBoxSize = FVector2D::ZeroVector;
	_prevSizeBoxPos = FVector2D::ZeroVector;
}

void UGsUITrayTutorial::CalcSize()
{
	if (false == _visualWidget.IsValid())
	{
		return;
	}

	const FGeometry targetGeometry = _visualWidget.Get()->GetCachedGeometry();	
	FVector2D targetLocalSize = targetGeometry.GetLocalSize();	

	// 타겟 좌표 맞추기 Y축
	const FGeometry topGeometry = _imgTop->GetCachedGeometry();
	FVector2D topSizeAbs = topGeometry.GetAbsoluteSize();
	topSizeAbs.Y = targetGeometry.AbsolutePosition.Y;
	_imgTop->SetBrushSize(topGeometry.AbsoluteToLocal(topSizeAbs));

	// 타겟 좌표 맞추기 X축
	const FGeometry leftGeometry = _imgLeft->GetCachedGeometry();
	FVector2D leftSizeAbs = leftGeometry.GetAbsoluteSize();
	leftSizeAbs.X = targetGeometry.AbsolutePosition.X;
	FVector2D leftSizeLoc = leftGeometry.AbsoluteToLocal(leftSizeAbs);
	leftSizeLoc.Y = targetLocalSize.Y;
	_imgLeft->SetBrushSize(leftSizeLoc);

	// ResetSize 상태이므로 찾은게 아님
	_bIsTargetFound = true;
	if (10.f == _sizeBoxTarget->WidthOverride &&
		10.f == _sizeBoxTarget->HeightOverride)
	{
		_bIsTargetFound = false;
	}

	// 타겟 사이즈 설정(혹시 순서 문제가 되어도 여기에 맞추게 가장 나중에 세팅)
	_sizeBoxTarget->SetWidthOverride(targetLocalSize.X);
	_sizeBoxTarget->SetHeightOverride(targetLocalSize.Y);

	// 대상 찾아서 타게팅 성공할 경우 켜주는 로직
	if (_bIsTargetFound)
	{
		if (0 < _targetingDelaySec)
		{
			// 딜레이 체크 시작
			_targetingDelayRemainSec = _targetingDelaySec;
		}
		else
		{
			// 타겟 보이게 처리
			_btnLock->SetVisibility(ESlateVisibility::Collapsed);
			SetVisibleTargetEffect(true);
		}
	}
}

void UGsUITrayTutorial::ResetSize()
{
	// 주의: 튜토리얼에서 HUD와 창을 넘나들면서 UIScale에 의해 화면 영역 밖으로 타게팅 되는 경우 발생할 경우,
	// sizeBoxGeometry.GetLocalSize()가 항상 0이나와 타겟팅을 못하는 상황 발생.
	// 예: 우하단 NPC리스트 튜토리얼로 잡화상점을 눌러서 잡화상점을 열어서 잡화상점을 연 경우, 
	// 우하단이 UIScale이 바뀌며 영역밖으로 판정되어 문제상황 발생.
	// 강제로 좌상단으로 맞춰주도록 처리	

	const FGeometry topGeometry = _imgTop->GetCachedGeometry();
	FVector2D topSizeLocal = topGeometry.GetLocalSize();
	topSizeLocal.Y = _resetSize.Y;
	_imgTop->SetBrushSize(topSizeLocal);

	_imgLeft->SetBrushSize(_resetSize);
	_sizeBoxTarget->SetWidthOverride(_resetSize.X);
	_sizeBoxTarget->SetHeightOverride(_resetSize.Y);

	SetVisibleTargetEffect(false);
}

void UGsUITrayTutorial::OnClickSkip()
{
	GMessage()->GetTutorial().SendMessage(MessageContentTutorial::FORCED_END_TUTORIAL, nullptr);
}

void UGsUITrayTutorial::OnClickTarget()
{
	EndInteraction();
}

void UGsUITrayTutorial::OnClickRegion()
{
	if (EGsTutorialUIInteractionType::CLICK_REGION_INTERFACE_EVENT == _interactionType)
	{
		if (_targetWidget.IsValid())
		{
			if (IGsTutorialInteractionInterface* interactionInterface =
				Cast<IGsTutorialInteractionInterface>(_targetWidget.Get()))
			{
				interactionInterface->OnClickTutorialRegion();
			}
		}
	}

	EndInteraction();
}

void UGsUITrayTutorial::OnInteractionInterface()
{
	EndInteraction();
}

void UGsUITrayTutorial::OnLongPressItemIcon(UGsUIIconItem& InIcon)
{
	EndInteraction();
}

void UGsUITrayTutorial::OnResizeViewport(class FViewport* InViewport, uint32 InNum)
{
	SetCheckSize();
}
