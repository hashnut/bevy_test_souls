// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIReuseItemIconSlotHud.h"
#include "T1Project.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"




void UGsUIReuseItemIconSlotHud::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_removeBtn->OnClicked.AddDynamic(this, &UGsUIReuseItemIconSlotHud::OnClickSlotRemove);

	_dragDownLogic.OnSlotDragDownTranslation.BindUObject(this, &UGsUIReuseItemIconSlotHud::OnSlotDragDownTranslation);
	_dragDownLogic.OnSlotDragDownStateChanged.BindUObject(this, &UGsUIReuseItemIconSlotHud::OnSlotDragDownStateChanged);
}

void UGsUIReuseItemIconSlotHud::NativeConstruct()
{
	Super::NativeConstruct();

	_autoEffectState = -1;
	_dragDownLogic.SetSensitiveValue(_dragDownSensitive);
	_dragDownLogic.SetDragDownValue(_dragDownValue);
}

FReply UGsUIReuseItemIconSlotHud::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	// 버튼때문에 TouchStart는 안들어옴
	// 주의: 버튼이 PreciseClick 옵션일 때만 이곳에 들어온다
	FReply reply = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	if (_bIsCaptureMouse)
	{
		if (false == _pressStartPosAbs.IsZero())
		{
			if (_bCanDragDown)
			{
				_dragDownLogic.OnMouseButtonMove(InGeometry, InGestureEvent);
			}

			FVector2D startPos = InGeometry.AbsoluteToLocal(_pressStartPosAbs);
			FVector2D movePos = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());			
			float diffY = FMath::Abs(startPos.Y - movePos.Y);
			float diffX = FMath::Abs(startPos.X - movePos.X);

			// 스크롤을 방해하는 문제로 X축도 체크
			if (_dragDownSensitive < diffY &&
				_dragDownSensitive > diffX)
			{
				return FReply::Handled().CaptureMouse(TakeWidget());
			}
		}
	}

	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}

FReply UGsUIReuseItemIconSlotHud::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	_pressStartPosAbs = FVector2D::ZeroVector;

	if (_bIsCaptureMouse)
	{
		if (_bCanDragDown)
		{
			_dragDownLogic.OnButtonReleased();
		}

		_bIsCaptureMouse = false;

		return FReply::Handled().ReleaseMouseCapture();
	}

	_bIsCaptureMouse = false;

	return FReply::Unhandled();
}

void UGsUIReuseItemIconSlotHud::OnShow()
{
	Super::OnShow();

	
	SetBlinkEffect(false);			// 슬롯 터치했을때
	SetEditModeEffect(false);		// 슬롯 편집 모드일때

	SetUseAutoEffect(false, false, false);
	_removeBtn->SetVisibility(ESlateVisibility::Collapsed);

	//SetUseAutoEffect(false);		// 소유중인 아이템이 자동모드 설정 되어있을때
	//SetNotOwnedAutoEffect(false);	// 비소유중인 아이템이 자동모드 설정 되어있을때
}

void UGsUIReuseItemIconSlotHud::OnHide()
{

	Super::OnHide();
}

void UGsUIReuseItemIconSlotHud::OnClickSlotRemove()
{
	if (true == _isBlockState)
		return;

	FGsOnClickRemoveQuickSlotIcon paramData(_slotIndex);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::QUICKSLOT_REMOVE, &paramData);

	_removeBtn->SetVisibility(ESlateVisibility::Collapsed);

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIReuseItemIconSlotHud::SetSlotItemIcon(const uint64 InItemDBID, const int32 InItemTID, const int64 InAmount, const uint8 InSlotIndex)
{
	_bCanDragDown = false;

	int32 prevAutoState = _autoEffectState;

	Super::SetSlotItemIcon(InItemDBID, InItemTID, InAmount, InSlotIndex);

	UGsQuickSlotManager* mgr = GSQuickSlot();
	const QuickSlotInfo* slotInfo = mgr->GetQuickSlotInfoBySlotID(_slotIndex);
	if (nullptr == slotInfo)
		return;

	//OnChangeSlotRemoveBtnVisible();
	_bCanDragDown = mgr->IsAllowedAutoUseItem(_slotIndex);
	SetAutoUseImage(_bCanDragDown);
	SetGrayScaleImage(((0 >= slotInfo->mItemDBId) && (0 < slotInfo->mTemplateId)));
	SetEditModeEffect(mgr->IsEditMode());

	// 아이템이 자동 사용되고 있을 때, 갯수가 줄어들며 강제로 오토 세팅이 되는 문제 수정(C2URWQ-6258)
	// 이전 정보와 다를 때만 세팅함
	int32 currAutoState = GetAutoEffectState(slotInfo->mAutoUse, 
		(0 < slotInfo->mItemDBId) ? true : false,
		(0 < slotInfo->mTemplateId) ? true : false);
	if (prevAutoState != currAutoState)
	{
		SetUseAutoEffect(slotInfo->mAutoUse,
			(0 < slotInfo->mItemDBId) ? true : false,
			(0 < slotInfo->mTemplateId) ? true : false);
	}
}

void UGsUIReuseItemIconSlotHud::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	if (true == _isBlockState)
		return;

	if (IsTutorialInteraction())
	{
		return;
	}

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}
	UGsQuickSlotManager* slotMgr = GSQuickSlot();

	SetBlinkEffect(true);
	FGsItem* itemData = itemIcon->GetItemIconData();
	int32 slotID = itemIcon->GetSlotIntParamData();
	UGsQuickSlotManager::SlotRegisterItemStateType  type = slotMgr->GetSlotRegisterStateType(slotID);
	slotMgr->SetQuickSlotSelectionItem(slotID, type == UGsQuickSlotManager::SlotRegisterItemStateType::Empty);
	if (slotMgr->IsEditMode())
	{
		if (true == FGsItem::IsValidItem(itemData) && type == UGsQuickSlotManager::SlotRegisterItemStateType::OwnedItem)
		{
			/*
			//itemData->IsUseItem()
			itemIcon->SetSlotEquipImage((false == slotMgr->IsConsumableItem(slotID))
				&& (false == slotMgr->IsEquippedItem(slotID)) && (false == slotMgr->IsIngredientItem(slotID)));

			itemIcon->SetSlotUnEquipImage((false == slotMgr->IsConsumableItem(slotID))
				&& (true == slotMgr->IsEquippedItem(slotID)) && (false == slotMgr->IsIngredientItem(slotID)));

			itemIcon->SetSlotUseImage((true == slotMgr->IsConsumableItem(slotID))
				&& (false == slotMgr->IsIngredientItem(slotID)));
				*/
			
			//itemIcon->SetSlotEquipImage((itemData->IsEquipment()) && (false == itemData->IsEquipped()));
			//itemIcon->SetSlotUnEquipImage((itemData->IsEquipment()) && (itemData->IsEquipped()));
			//itemIcon->SetSlotUseImage(itemData->IsUseItem());
		}
		_removeBtn->SetVisibility(type != UGsQuickSlotManager::SlotRegisterItemStateType::Empty ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		slotMgr->CheckSelectionInfo();
	}
	else
	{
		// 드래그로 오토설정을 했는데 영역 안이라 클릭 판정이 날 경우 클릭 무시
		if (_bAutoChanged)
		{
			slotMgr->ResetQuickSlotSelectionInfo();
			return;
		}

		// Edit 상태가 아닐때는 바로 사용 처리
		if (true == FGsItem::IsValidItem(itemData) && type == UGsQuickSlotManager::SlotRegisterItemStateType::OwnedItem)
		{
			FGsOnClickUseQuickSlotItem paramData(slotID);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::QUICKSLOT_USE, &paramData);	
		}
		slotMgr->ResetQuickSlotSelectionInfo();
	}
	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIReuseItemIconSlotHud::ResetSelection()
{
	SetEquipTextImage(false);
	SetUnEquipTextImage(false);
	SetUseTextImage(false);

	_removeBtn->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIReuseItemIconSlotHud::SetSlotDimmed(const bool InState)
{
	_isBlockState = InState;

	if (nullptr == _iconSlotHelper->GetIcon(0))
		return;

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(_iconSlotHelper->GetIcon(0));
	if (nullptr == itemIcon)
		return;

	itemIcon->SetDimmedImage(InState);
}

void UGsUIReuseItemIconSlotHud::SetGrayScaleImage(const bool InState)
{
	if (nullptr == _iconSlotHelper->GetIcon(0))
		return;

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(_iconSlotHelper->GetIcon(0));
	if (nullptr == itemIcon)
		return;

	itemIcon->SetImageGrayscale(InState);
}

void UGsUIReuseItemIconSlotHud::SetAutoUseImage(const bool InState)
{
	if (nullptr == _iconSlotHelper->GetIcon(0))
		return;

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(_iconSlotHelper->GetIcon(0));
	if (nullptr == itemIcon)
		return;

	itemIcon->SetAutoImage(InState);
}

void UGsUIReuseItemIconSlotHud::SetEquipTextImage(const bool InState)
{
	if (nullptr == _iconSlotHelper->GetIcon(0))
		return;

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(_iconSlotHelper->GetIcon(0));
	if (nullptr == itemIcon)
		return;

	itemIcon->SetSlotEquipImage(InState);
}

void UGsUIReuseItemIconSlotHud::SetUnEquipTextImage(const bool InState)
{
	if (nullptr == _iconSlotHelper->GetIcon(0))
		return;

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(_iconSlotHelper->GetIcon(0));
	if (nullptr == itemIcon)
		return;

	itemIcon->SetSlotUnEquipImage(InState);
}

void UGsUIReuseItemIconSlotHud::SetUseTextImage(const bool InState)
{
	if (nullptr == _iconSlotHelper->GetIcon(0))
		return;

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(_iconSlotHelper->GetIcon(0));
	if (nullptr == itemIcon)
		return;

	itemIcon->SetSlotUseImage(InState);
}

void UGsUIReuseItemIconSlotHud::SetBlinkEffect(bool InState)
{
	if (true == InState)
	{
		if (_quickSlotBlinkEffect->IsPlayingAnimation())
		{
			_quickSlotBlinkEffect->HideVFX();
		}
		_quickSlotBlinkEffect->ShowVFX();
	}
	else
	{
		_quickSlotBlinkEffect->HideVFX();
	}
}

void UGsUIReuseItemIconSlotHud::SetEditModeEffect(bool InState)
{
	if (true == InState)
	{
		if (_quickSlotEditEffect->IsPlayingAnimation())
		{
			return;
		}
		_quickSlotEditEffect->ShowVFX();
	}
	else
	{
		_quickSlotEditEffect->HideVFX();
	}
}

void UGsUIReuseItemIconSlotHud::SetUseAutoEffect(bool InAutoUse, bool IsItemDBId, bool IsTemplateId)
{
	_autoEffectState = GetAutoEffectState(InAutoUse, IsItemDBId, IsTemplateId);

	_dragDownLogic.SetIsDown(InAutoUse, false);

	switch (_autoEffectState)
	{
	case 1:
		_quickSlotUseAutoEffect->ShowVFX();
		_notOwnedUseAutoEffect->HideVFX();
		break;
	case 2:
		_quickSlotUseAutoEffect->HideVFX();
		_notOwnedUseAutoEffect->ShowVFX();
		break;
	default:
		_quickSlotUseAutoEffect->HideVFX();
		_notOwnedUseAutoEffect->HideVFX();
		break;
	}
}

int32 UGsUIReuseItemIconSlotHud::GetAutoEffectState(bool InAutoUse, bool IsItemDBId, bool IsTemplateId)
{
	if (InAutoUse)
	{
		if (IsItemDBId)
		{
			// 소유중인 아이템이 자동모드 설정 되어있을때
			return 1;
		}
		else
		{
			if (IsTemplateId)
			{
				// 비소유중인 아이템이 자동모드 설정 되어있을때
				return 2;
			}
		}
	}

	return 0;
}

void UGsUIReuseItemIconSlotHud::InvalidateAutoState()
{
	if (UGsQuickSlotManager* quickSlotMgr = GSQuickSlot())
	{
		if (const QuickSlotInfo* slotInfo = quickSlotMgr->GetQuickSlotInfoBySlotID(_slotIndex))
		{
			SetUseAutoEffect(slotInfo->mAutoUse,
				(0 < slotInfo->mItemDBId) ? true : false,
				(0 < slotInfo->mTemplateId) ? true : false);
		}
	}
}

void UGsUIReuseItemIconSlotHud::OnRefreshIconWrapBox(int32 InIndex, UGsUIIconBase* InIcon)
{
	Super::OnRefreshIconWrapBox(InIndex, InIcon);

	// 빈 슬롯 판정난 경우
	if (0 >= _itemDBID && 0 >= _itemTID)
	{
		SetUseAutoEffect(false, false, false);
	}

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}	

	if (false == itemIcon->OnButtonDownSlot.IsBoundToObject(this))
	{
		itemIcon->OnButtonDownSlot.Unbind();
		itemIcon->OnButtonDownSlot.BindUObject(this, &UGsUIReuseItemIconSlotHud::OnItemSlotButtonDown);
	}

	if (false == itemIcon->OnButtonReleaseSlot.IsBoundToObject(this))
	{
		itemIcon->OnButtonReleaseSlot.Unbind();
		itemIcon->OnButtonReleaseSlot.BindUObject(this, &UGsUIReuseItemIconSlotHud::OnItemSlotButtonRelease);
	}
}

void UGsUIReuseItemIconSlotHud::OnClickIconWindowInput()
{
	if (nullptr != _iconSlotHelper)
	{
		UGsUIIconBase* iconBase = _iconSlotHelper->GetIcon(0);
		if (nullptr == iconBase)
			return;

		UGsUIIconItem* iconItem = Cast<UGsUIIconItem>(iconBase);
		if (nullptr == iconItem)
			return;

		OnClickIconSlot(*iconItem);
	}
}

void UGsUIReuseItemIconSlotHud::OnItemSlotButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	_bAutoChanged = false;

	// 주의: 버튼이 있으면 NativeOnTouchStarted 가 안들어오므로 여기서 저장. 
	// 계산을 버튼이 아닌 위젯으로 할거라 로컬좌표로 변환하지 않은 절대좌표 저장
	if (false == _bIsCaptureMouse)
	{
		_bIsCaptureMouse = true;
		_pressStartPosAbs = MouseEvent.GetScreenSpacePosition();

		if (_bCanDragDown)
		{
			_dragDownLogic.OnMouseButtonDown(Geometry, MouseEvent);
		}
	}
}

void UGsUIReuseItemIconSlotHud::OnItemSlotButtonRelease()
{
	// 스크롤 판정 혹은 캡쳐되지 않은 상태에서 영역 밖으로 나간경우에 초기화 위함
	if (_bIsCaptureMouse)
	{
		if (false == HasMouseCapture())
		{
			_bIsCaptureMouse = false;
			_pressStartPosAbs = FVector2D::ZeroVector;
		}
	}
}

void UGsUIReuseItemIconSlotHud::OnSlotDragDownTranslation(const FVector2D& InTranslation)
{	
	SetRenderTranslation(InTranslation);
}

void UGsUIReuseItemIconSlotHud::OnSlotDragDownStateChanged()
{
	// 오토 세팅 이벤트 보내기 관련 처리
	if (_bCanDragDown)
	{
		_bAutoChanged = true;

		if (UGsUIIconBase* icon = _iconSlotHelper->GetIcon(0))
		{
			if (UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(icon))
			{
				int32 slotId = itemIcon->GetSlotIntParamData();
				GSQuickSlot()->SetAutoState(slotId);
			}
		}

		if (IsTutorialInteraction())
		{
			OnTutorialInteraction.ExecuteIfBound();
		}
	}
}
