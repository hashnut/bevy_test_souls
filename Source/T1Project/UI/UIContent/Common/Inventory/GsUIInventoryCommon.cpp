// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIInventoryCommon.h"
#include "GsWrapBoxInventoryIconSelector.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemCollectionSlot.h"
#include "UI/UIContent/Common/Inventory/GsInventoryLogicBase.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "Item/GsItem.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "UI/UIContent/Define/EGsUIColorType.h"


void UGsUIInventoryCommon::SynchronizeProperties()
{
	Super::SynchronizeProperties();
#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		_iconSelector->SetEIconType(_iconType);
		_iconSelector->SetIconKey(_iconKey);
		_iconSelector->SetpreviewCount(_previewCount);
		_iconSelector->SynchronizeProperties();
	}	
#endif //#if WITH_EDITORONLY_DATA
}


void UGsUIInventoryCommon::BeginDestroy()
{
	Super::BeginDestroy();

	if (_iconSlotHelper)
	{
		_iconSlotHelper->OnCreateIcon.RemoveAll(this);
	}
	_iconSlotHelper = nullptr;
}


void UGsUIInventoryCommon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 메인 탭 설정
	_toggleGroupTab.AddToggleWidgetByParentPanel(_panelTab);
	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIInventoryCommon::OnSelectTab);

	// 아이템 리스트 설정
	_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_iconSlotHelper->Initialize(_iconSelector);	
	_iconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIInventoryCommon::OnCreateItemIcon);
	_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIInventoryCommon::OnRefreshItemIcon);	

	// 슬롯 확장 버튼
	_btnItemSlotLock->OnClicked.AddDynamic(this, &UGsUIInventoryCommon::OnClickSlotLock);

	// 튜토리얼 스크롤 락을 위해 이벤트 연결
	_scrollBox->OnUserScrolled.AddDynamic(this, &UGsUIInventoryCommon::OnScrollList);


	_btnMultiEnchantReset->OnClicked.AddDynamic(this, &UGsUIInventoryCommon::OnClickMultiEnchantResetBtn);
	_btnMultiEnchantReset->SetOnClickLockSec(1.0f); // 수정 금지

	_btnMultiEnchantRegisterItem->OnClicked.AddDynamic(this, &UGsUIInventoryCommon::OnClickMultiEnchantRegisterBtn);
	_btnMultiEnchantRegisterItem->SetOnClickLockSec(1.0f); // 수정 금지
}


void UGsUIInventoryCommon::NativePreConstruct()
{
	Super::NativePreConstruct();

	_iconSelector->SetEIconType(_iconType);
	_iconSelector->SetIconKey(_iconKey);
	_iconSelector->SynchronizeProperties();

	_checkBoxAll->SetVisibility(ESlateVisibility::Visible);
	_checkBoxConsume->SetVisibility(ESlateVisibility::Visible);
	_checkBoxEquip->SetVisibility(ESlateVisibility::Visible);
	_checkBoxMaterial->SetVisibility(ESlateVisibility::Visible);

	_imageRedDotMap.Add(EInventoryTab::AllTab, _imageRedDotAll);
	_imageRedDotMap.Add(EInventoryTab::EquipmentTab, _imageRedDotEquip);
	_imageRedDotMap.Add(EInventoryTab::ConsumableTab, _imageRedDotComsume);
	_imageRedDotMap.Add(EInventoryTab::MaterialTab, _imageRedDotEtc);
}


void UGsUIInventoryCommon::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	_panelEmptyList->SetVisibility(ESlateVisibility::Collapsed);
	
	SetMultiEnchantInvenState(false);

	SetInvenTabRedDot(EInventoryTab::AllTab, false);
	SetInvenTabRedDot(EInventoryTab::EquipmentTab, false);
	SetInvenTabRedDot(EInventoryTab::ConsumableTab, false);
	SetInvenTabRedDot(EInventoryTab::MaterialTab, false);
}


void UGsUIInventoryCommon::NativeDestruct()
{
	UnbindMessages();

	if (_iconSlotHelper)
	{
		_iconSlotHelper->Clear();
	}
	
	_iconSelector->RemoveAllChildren();

	_imageRedDotMap.Empty();

	if (_logic)
	{
		_logic->OnNativeDestruct();
		_logic = nullptr;
	}

	if (_onMultiEnchantReset.IsBound())
	{
		_onMultiEnchantReset.Unbind();
	}
	if (_onMultiEnchantRegisterItem.IsBound())
	{
		_onMultiEnchantRegisterItem.Unbind();
	}
	
	Super::NativeDestruct();
}


void UGsUIInventoryCommon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	GeneratedPartionItemIcon();
}


void UGsUIInventoryCommon::BindMessages()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	MItem& msgItem = msg->GetItem();
	_msgHandleItemList.Emplace(msgItem.AddUObject(MessageItem::ADDITEMLIST,
		this, &UGsUIInventoryCommon::OnAddItem));
	_msgHandleItemList.Emplace(msgItem.AddUObject(MessageItem::UPDATEITEM,
		this, &UGsUIInventoryCommon::OnUpdateItem));
	_msgHandleItemList.Emplace(msgItem.AddUObject(MessageItem::REMOVEITEM,
		this, &UGsUIInventoryCommon::OnRemoveItem));

	MInventoryCommon& msgInventoryCommon = msg->GetInventoryCommon();
	_msgHandleInventoryCommon.Emplace(msgInventoryCommon.AddUObject(MessageContentCommonInven::INVALIDATE_ALL,
		this, &UGsUIInventoryCommon::OnInvalidateData));
	_msgHandleInventoryCommon.Emplace(msgInventoryCommon.AddUObject(MessageContentCommonInven::INVALIDATE_SELECTED_EFFECT,
		this, &UGsUIInventoryCommon::OnInvalidateSelectedEffect));
}


void UGsUIInventoryCommon::UnbindMessages()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	MItem& msgItem = msg->GetItem();
	for (MsgHandleItem& handle : _msgHandleItemList)
	{
		msgItem.Remove(handle);
	}
	_msgHandleItemList.Empty();

	MInventoryCommon& msgInventoryCommon = msg->GetInventoryCommon();
	for (MsgHandleInventoryCommon& handle : _msgHandleInventoryCommon)
	{
		msgInventoryCommon.Remove(handle);
	}
	_msgHandleInventoryCommon.Empty();
}


void UGsUIInventoryCommon::InvalidateItemList()
{
	if (nullptr == _logic)
	{
		return;
	}

	_logic->OnInvalidateList();

	// 2021/10/12 PKT - 분할 생성 정보 초기화
	_generatedPartion.Clear();
	_reservedUpdateItem.Empty();
	_reservedSelectedIndex = INDEX_NONE;

	if (false == EnumHasAnyFlags(EInventoryCommonProperty::RefreshPartitions, _property))
	{	// 2021/08/09 PKT - 분할 방식을 사용하지 않겠다.
		_iconSlotHelper->RefreshAll(_logic->GetDataCount());
	}
	else
	{	// 2021/08/09 PKT - 분할 생성..
		if (GetWorld())
		{
			_generatedPartion.prevDeltaSeconds = GetWorld()->GetDeltaSeconds();
		}
		else
		{
			_generatedPartion.prevDeltaSeconds = 0.0f;
		}

		_generatedPartion.frameRate = GENERATED_PARTION_RATE;
		_generatedPartion.MaxCount = _logic->GetDataCount();
		_generatedPartion.countPerFrame = FMath::Min(GENERATE_PARTION_MAX_COUNT, _logic->GetDataCount());

		_iconSlotHelper->CreateIconByTick(_generatedPartion.countPerFrame, _generatedPartion.MaxCount);

		const static int32 START_INDEX = 0;
		_iconSlotHelper->RefreshIconByTick(_generatedPartion.countPerFrame, START_INDEX);

		_generatedPartion.currentCount += _generatedPartion.countPerFrame;
	}

	_panelEmptyList->SetVisibility((0 < _logic->GetDataCount()) ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	_scrollBox->EndInertialScrolling();
	_scrollBox->ScrollToStart();

	InvalidateInvenTabSlotCount();
	InvalidateInvenTabRedDot();

	if (IsTutorial() &&
		0 < _logic->GetDataCount())
	{
		OnTutorialListEvent.ExecuteIfBound(_iconSelector->GetChildAt(0));
	}
}

void UGsUIInventoryCommon::InvalidateInvenTabSlotCount()
{
	// 슬롯 개수 표시
	if (true == EnumHasAnyFlags(EInventoryCommonProperty::HideInventoryCount, _property))
	{
		_textBlockSlotCount->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_textBlockSlotCount->SetVisibility(ESlateVisibility::HitTestInvisible);

		FString strSlotCount = FString::Format(TEXT("{0}/{1}"), { _logic->GetOwnedItemCount(), _logic->GetInventorySlotCount() });
		_textBlockSlotCount->SetText(FText::FromString(strSlotCount));
		_textBlockSlotCount->SetColorAndOpacity((_logic->GetOwnedItemCount() > _logic->GetInventorySlotCount()) ?
			FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK) : FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_ENOUGH));
	}
}

void UGsUIInventoryCommon::InvalidateInvenTabRedDot()
{
	// 인벤토리 탭 레드닷 표시
	if (true == EnumHasAnyFlags(EInventoryCommonProperty::InventoryRedDot, _property))
	{
		if (_logic)
		{
			bool equip = _logic->CheckRedDotByInvenTab(EInventoryTab::EquipmentTab);
			bool consume = _logic->CheckRedDotByInvenTab(EInventoryTab::ConsumableTab);
			bool ingredient = _logic->CheckRedDotByInvenTab(EInventoryTab::MaterialTab);

			SetInvenTabRedDot(EInventoryTab::AllTab, (equip || consume || ingredient));
			SetInvenTabRedDot(EInventoryTab::EquipmentTab, equip);
			SetInvenTabRedDot(EInventoryTab::ConsumableTab, consume);
			SetInvenTabRedDot(EInventoryTab::MaterialTab, ingredient);
		}
	}
	else
	{
		SetInvenTabRedDot(EInventoryTab::AllTab, false);
		SetInvenTabRedDot(EInventoryTab::EquipmentTab, false);
		SetInvenTabRedDot(EInventoryTab::ConsumableTab, false);
		SetInvenTabRedDot(EInventoryTab::MaterialTab, false);
	}
}

void UGsUIInventoryCommon::SetProPerty(EInventoryCommonProperty InProperts)
{
	_property = InProperts;
}

void UGsUIInventoryCommon::SetData(FGsInventoryLogicBase* InData)
{
	_logic = InData;
	if (_logic)
	{
		_btnItemSlotLock->SetVisibility((0 >= _logic->GetEnoughExtendSlotCount()) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}

	_toggleGroupTab.SetSelectedIndex(0, true);
}

void UGsUIInventoryCommon::InvalidateData()
{
	InvalidateItemList();
}

void UGsUIInventoryCommon::ChangeInvenSlotCount(uint32 InMaxSlotCount, uint32 InTotalExtendCount)
{
	if (_logic)
	{
		_logic->OnChangeInvenSlotCount(InMaxSlotCount, InTotalExtendCount);
		// 보유 가능한 만큼 빈 슬롯 생성
		_iconSlotHelper->RefreshAll(_logic->GetDataCount());
		// 탭 개수 정보 갱신
		InvalidateInvenTabSlotCount();
		// 확장 버튼 갱신
		_btnItemSlotLock->SetVisibility((0 >= _logic->GetEnoughExtendSlotCount()) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}

void UGsUIInventoryCommon::SetEmptyDesc(const FText& InDesc)
{
	_textEmptyDesc->SetText(InDesc);
}

void UGsUIInventoryCommon::SetInvenTabRedDot(EInventoryTab InTab, bool InValue)
{
	if (true == _imageRedDotMap.Contains(InTab))
	{
		if (nullptr != _imageRedDotMap[InTab])
		{
			ESlateVisibility slateType = (true == InValue) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
			_imageRedDotMap[InTab]->SetVisibility(slateType);
		}
	}
}

void UGsUIInventoryCommon::OnAddItem(FGsItemMessageParamBase& InParam)
{
	if (true == EnumHasAnyFlags(EInventoryCommonProperty::ItemActionMessageUnBind, _property))
	{	// 2021/08/09 PKT - ItemActionMesdsageBind하지 않겠다.
		return;
	}

	InvalidateItemList();
}

void UGsUIInventoryCommon::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	if (true == EnumHasAnyFlags(EInventoryCommonProperty::ItemActionMessageUnBind, _property))
	{	// 2021/08/09 PKT - ItemActionMesdsageBind하지 않겠다.
		return;
	}

	FGsUpdateItem* paramData = static_cast<FGsUpdateItem*>(&InParam);
	if (paramData)
	{
		if (_logic)
		{
			if (true == EnumHasAnyFlags(EInventoryCommonProperty::RefreshPartitions, _property))
			{	// 2021/08/09 PKT - 분할 생성 프로퍼티이면 모든 데이터를 로드 했는지를 보고 아직 미 완료 상태라면 업데이트 예약을 걸어둔다.
				if (_generatedPartion.currentCount < _logic->GetDataCount())
				{
					_reservedUpdateItem.Emplace(paramData->_itemDBID);
					return;
				}
			}

			_iconSlotHelper->RefreshByIndex(_logic->GetDataIndex(paramData->_itemDBID));
		}
	}
}

void UGsUIInventoryCommon::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	if (true == EnumHasAnyFlags(EInventoryCommonProperty::ItemActionMessageUnBind, _property))
	{	// 2021/08/09 PKT - ItemActionMesdsageBind하지 않겠다.
		return;
	}

	InvalidateItemList();
}

void UGsUIInventoryCommon::OnInvalidateData(const IGsMessageParam* InParam)
{	
	// 2021/10/12 PKT - Inventory 갱신
	InvalidateItemList();
}

void UGsUIInventoryCommon::OnInvalidateSelectedEffect(const IGsMessageParam* InParam)
{
	const FGsInventoryCommonInvalidate* param = StaticCast<const FGsInventoryCommonInvalidate*>(InParam);

	_logic->SetSelectedItemDBId(param->_itemDBID);

	const int32 maxCount = _logic->GetDataCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUIIconItemCollectionSlot> itemIcon = StaticCast<UGsUIIconItemCollectionSlot*>(_iconSlotHelper->GetIcon(idx));
		if (itemIcon.IsValid() && itemIcon->GetItemIconData())
		{
			if (param->_itemDBID == itemIcon->GetItemIconData()->GetDBID())
			{
				itemIcon->SetSlotSelectionImage(true);
			}
			else
			{
				itemIcon->SetSlotSelectionImage(false);
			}
		}
	}
}

void UGsUIInventoryCommon::GeneratedPartionItemIcon()
{
	if (false == EnumHasAnyFlags(EInventoryCommonProperty::RefreshPartitions, _property))
	{
		return;
	}

	if (_generatedPartion.currentCount == _logic->GetDataCount())
	{	// 2021/08/09 PKT - 전체 로딩 끝

		/**
		 * 기존 분할 생성 과정에서 생긴 이벤트에 대한 처리
		 */
		for (ItemDBId itemDBId : _reservedUpdateItem)
		{
			_iconSlotHelper->RefreshByIndex(_logic->GetDataIndex(itemDBId));
		}
		_reservedUpdateItem.Empty();

		if (INDEX_NONE != _reservedSelectedIndex)
		{
			//_iconSelector->Get
			//OnClickItemIcon
		}

		_reservedSelectedIndex = INDEX_NONE;

		return;
	}

	const float prevDeltaSeconds = _generatedPartion.prevDeltaSeconds;
	if (GetWorld())
	{
		_generatedPartion.prevDeltaSeconds = GetWorld()->GetDeltaSeconds();
	}
	else
	{
		_generatedPartion.prevDeltaSeconds = GENERATED_PARTION_RATE + prevDeltaSeconds;
	}

	float rate = GENERATED_PARTION_RATE / (_generatedPartion.prevDeltaSeconds - prevDeltaSeconds);
	_generatedPartion.countPerFrame = _generatedPartion.countPerFrame * rate;
	_generatedPartion.countPerFrame = FMath::Min(_generatedPartion.countPerFrame, GENERATE_PARTION_MAX_COUNT);
	_generatedPartion.countPerFrame = FMath::Max(_generatedPartion.countPerFrame, GENERATE_PARTION_MIN_COUNT);

	if (_generatedPartion.countPerFrame + _generatedPartion.currentCount > _generatedPartion.MaxCount)
	{
		_generatedPartion.countPerFrame = _generatedPartion.MaxCount - _generatedPartion.currentCount;
	}

	// 2021/10/12 PKT - 분할 생성.
	_iconSlotHelper->CreateIconByTick(_generatedPartion.countPerFrame + _generatedPartion.currentCount, _generatedPartion.MaxCount);
	_iconSlotHelper->RefreshIconByTick(_generatedPartion.countPerFrame, _generatedPartion.currentCount);
	_generatedPartion.currentCount += _generatedPartion.countPerFrame;
}


void UGsUIInventoryCommon::OnCreateItemIcon(UGsUIIconBase* InIcon)
{
	if (UGsUIIconItem* itemIcon = Cast<UGsUIIconItem>(InIcon))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIInventoryCommon::OnClickItemIcon);
	}
	
	if (_logic)
	{
		_logic->OnCreateItemIcon(InIcon);
	}
}

void UGsUIInventoryCommon::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (UGsUIIconItem* itemIcon = Cast<UGsUIIconItem>(InIcon))
	{
		if (false == itemIcon->OnClickSlot.IsBound())
		{
			itemIcon->OnClickSlot.AddUObject(this, &UGsUIInventoryCommon::OnClickItemIcon);
		}
	}

	if (_logic)
	{
		_logic->OnRefreshItemIcon(InIndex, InIcon);
	}
}

void UGsUIInventoryCommon::OnSelectTab(int32 InIndex)
{
	if (_logic)
	{
		EInventoryTab tabType = static_cast<EInventoryTab>(InIndex);
		if (tabType == _logic->GetCurrTabType())
		{	// 2021/08/09 PKT - 같은 Tab을 클릭했을 시 갱신을 해줄 필요가 없다.
			return;
		}
		
		_logic->OnSelectTab(tabType);
	}

	InvalidateItemList();
}

void UGsUIInventoryCommon::SetInvenTab(const uint8 InIndex)
{
	_toggleGroupTab.SetSelectedIndex(InIndex, true);
}

void UGsUIInventoryCommon::OnClickItemIcon(UGsUIIconItem& InIcon)
{
	if (nullptr == _logic)
	{
		return;
	}
	
	// 클릭
	_logic->OnClickItemIcon(InIcon);

	// 슬롯 토글 처리는 각 컨텐츠마다 다를 수 있으므로 로직쪽에서 처리
	// 예: 무조건 선택 or 동일슬롯 클록시 선택 해제 or 동일슬롯 클릭시 아이템 사용 등
	ItemDBId prevItemDBId = _logic->GetSelectedItemDBId();
	UGsUIIconItem* prevIcon = nullptr;	
	if (INVALID_ITEM_DB_ID != prevItemDBId)
	{
		int32 prevIndex = _logic->GetDataIndex(prevItemDBId);
		if (0 <= prevIndex)
		{
			prevIcon = Cast<UGsUIIconItem>(_iconSlotHelper->GetIcon(prevIndex));
		}
	}
	
	_logic->OnSelectItemIcon(prevIcon, &InIcon);
	
	// 클릭된 아이템에 대한 기본적인 Delegate 이벤트
	if (FGsItem* itemData = InIcon.GetItemIconData())
	{
		_onClickItemIcon.ExecuteIfBound(itemData->GetDBID());
	}
}

void UGsUIInventoryCommon::OnClickSlotLock()
{
	if (_logic)
	{
		_logic->OnClickSlotLock();
	}
}

void UGsUIInventoryCommon::SetEnchantInvenState()
{
	_checkBoxAll->SetVisibility(ESlateVisibility::Collapsed);
	_checkBoxConsume->SetVisibility(ESlateVisibility::Collapsed);
	_checkBoxEquip->SetVisibility(ESlateVisibility::Visible);
	_checkBoxMaterial->SetVisibility(ESlateVisibility::Visible);

	//_toggleGroupTab.SetSelectedIndex(1, true);
}

void UGsUIInventoryCommon::SetItemFusionInvenState()
{
	_checkBoxAll->SetVisibility(ESlateVisibility::Collapsed);
	_checkBoxConsume->SetVisibility(ESlateVisibility::Collapsed);
	_checkBoxEquip->SetVisibility(ESlateVisibility::HitTestInvisible);
	_checkBoxMaterial->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIInventoryCommon::SetTutorial(bool bIsTutorial)
{
	_bIsTutorial = bIsTutorial;
	if (false == _bIsTutorial)
	{
		_scrollBox->SetAllowOverscroll(true);
		return;
	}

	_scrollBox->SetAllowOverscroll(false);
}

void UGsUIInventoryCommon::OnScrollList(float InCurrentOffset)
{
	if (IsTutorial())
	{
		_scrollBox->SetScrollOffset(0.f);
	}
}

void UGsUIInventoryCommon::InvalidateBySlotIndex(int32 InSlotIndex)
{
	if (_iconSlotHelper)
	{
		_iconSlotHelper->RefreshByIndex(InSlotIndex);
	}
}

void UGsUIInventoryCommon::SetInvenSlotDimmed(const bool InDimmedState, const ItemDBId InItemDbid)
{
	int32 index = _logic->GetDataIndex(InItemDbid);
	if (0 <= index && _iconSlotHelper->GetIcon(index))
	{
		UGsUIIconItemInventory* itemIconInven = Cast<UGsUIIconItemInventory>(_iconSlotHelper->GetIcon(index));
		if (nullptr != itemIconInven)
		{
			itemIconInven->SetDimmedImage(InDimmedState);
		}
	}
}

void UGsUIInventoryCommon::SetInvenSlotSelectionImg(const bool InIsSelected, const ItemDBId InItemDbid)
{
	int32 index = _logic->GetDataIndex(InItemDbid);
	if (0 <= index && _iconSlotHelper->GetIcon(index))
	{
		UGsUIIconItemInventory* itemIconInven = Cast<UGsUIIconItemInventory>(_iconSlotHelper->GetIcon(index));
		if (nullptr != itemIconInven)
		{
			itemIconInven->SetSlotSelectionImage(InIsSelected);
		}
	}
}

void UGsUIInventoryCommon::OnClickMultiEnchantResetBtn()
{
	_onMultiEnchantReset.ExecuteIfBound();
}

void UGsUIInventoryCommon::OnClickMultiEnchantRegisterBtn()
{
	_onMultiEnchantRegisterItem.ExecuteIfBound();
}

void UGsUIInventoryCommon::SetMultiEnchantInvenState(const bool InState)
{
	_multiEnchantOptionRootPanel->SetVisibility(InState ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}