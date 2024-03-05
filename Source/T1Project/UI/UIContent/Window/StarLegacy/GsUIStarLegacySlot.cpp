#include "GsUIStarLegacySlot.h"

#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"

#include "Management/ScopeGame/GsStarLegacyManager.h"

#include "GameFlow/GameContents/ContentsGame/GsGameStateStarLegacy.h"
#include "GameFlow/GameContents/ContentsGame/StarLegacy/GsStarLegacySlotArrangementInfo.h"

#include "Item/GsItem.h"

#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"

void UGsUIStarLegacySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_targetStarLegacyRewardIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_targetStarLegacyRewardIconSlotHelper->Initialize(_targetStarLegacyRewardIconSelector);
	_targetStarLegacyRewardIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIStarLegacySlot::OnRefreshTargetIcon);
	_targetStarLegacyRewardIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIStarLegacySlot::OnCreateTargetIcon);

	_btnMagnitudeOpen->OnClicked.AddDynamic(this, &UGsUIStarLegacySlot::OnClickMagnitude);
}


	
void UGsUIStarLegacySlot::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIStarLegacySlot::OnLongPressIconSlot);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIStarLegacySlot::OnClickIconSlot);
	}

	if (_slotInfo == nullptr)
	{
		return;
	}
	int itemCount = 0;
	// 장비면 갯수 0으로 표시
	if (_slotInfo->_itemType == ItemType::EQUIP)
	{
		itemCount = 0;
	}
	else
	{
		itemCount = _slotInfo->_itemCount;
	}


	itemIcon->SetNotOwnItem(_slotInfo->_itemId, itemCount, _slotInfo->_enchantCount);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemEnchantLevel(0);
	itemIcon->SetActiveIconBg(true);

	// 컬랙션 표시 처리
	itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);
}
	
void UGsUIStarLegacySlot::OnCreateTargetIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetActiveIconBg(true);
}

// 등급 상태 표시
void UGsUIStarLegacySlot::SetMagnitudeInfo(bool In_isOpen)
{
	_isOpen = In_isOpen;

	if (_slotInfo == nullptr)
	{
		return;
	}

	int switcherIndex = 0;
	if (_isOpen == false)
	{
		switcherIndex = 0;
	}
	else
	{
		switch (_slotInfo->_magnitude)
		{
		case 5:
		{
			switcherIndex = 1;
		}
		break;
		case 4:
		{
			switcherIndex = 2;
		}
		break;
		case 3:
		{
			switcherIndex = 3;
		}
		break;
		case 2:
		{
			switcherIndex = 4;
		}
		break;
		case 1:
		{
			switcherIndex = 5;
		}
		break;
		}
	}
	_switcherMagnitudeInfo->SetActiveWidgetIndex(switcherIndex);

}

// 등급 오픈 클릭	
void UGsUIStarLegacySlot::OnClickMagnitude()
{
	FGsGameStateStarLegacy* stateStarLegacy = GSStarLegacy()->GetGameStateStarLegacy();

	if (stateStarLegacy == nullptr || 
		_slotInfo == nullptr)
	{
		return;
	}
	stateStarLegacy->OpenMagnitude(_slotInfo->_slotOrderIndex);
}

// 슬롯 정보 인포로 세팅
void UGsUIStarLegacySlot::SetSlotInfo(FGsStarLegacySlotArrangementInfo* In_info)
{
	if (In_info == nullptr)
	{
		return;
	}
	_slotInfo = In_info;	
	SetMagnitudeInfo(false);
	// 슬롯 갱신
	_targetStarLegacyRewardIconSlotHelper->RefreshAll(1);

	// 선택, 룰렛 표시, 성공, 실패 hide
	_panelSelect->SetVisibility(ESlateVisibility::Collapsed);
	_panelRoulette->SetVisibility(ESlateVisibility::Collapsed);
	_panelSuccess->SetVisibility(ESlateVisibility::Collapsed);
	_panelFail->SetVisibility(ESlateVisibility::Collapsed);

	InitAnimation();
}
// 아이콘 롱프레스 누를때
void UGsUIStarLegacySlot::OnLongPressIconSlot(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true, true);
}

// 아이콘 클릭 누를때(선택)
void UGsUIStarLegacySlot::OnClickIconSlot(UGsUIIconItem& InIcon)
{
	FGsGameStateStarLegacy* stateStarLegacy = GSStarLegacy()->GetGameStateStarLegacy();
	if (stateStarLegacy == nullptr ||
		_slotInfo == nullptr)
	{
		return;
	}
	stateStarLegacy->SelectSlot(_slotInfo->_slotOrderIndex);
}
// 선택 처리
void UGsUIStarLegacySlot::SetSelectVisible(bool In_isSelect)
{
	ESlateVisibility selectVisibility =
		(In_isSelect == true)? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	
	_panelSelect->SetVisibility(selectVisibility);
}

void UGsUIStarLegacySlot::NativeConstruct()
{
	Super::NativeConstruct();

	
	InitAnimation();
}

// 룰렛 애니 시작
void UGsUIStarLegacySlot::StartRouletteAnimation()
{
	_panelRoulette->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	InitAnimation();

	PlayAnimation(_rouletteAnimation);
}

// 애니 초기화
void UGsUIStarLegacySlot::InitAnimation()
{
	// 애니 초기화
	float endTime = _rouletteAnimation->GetEndTime();
	PlayAnimation(_rouletteAnimation, endTime, 1, EUMGSequencePlayMode::Reverse);
}

// 성공, 실패 표시
void UGsUIStarLegacySlot::SetSuccessOrFailVisible(bool In_isSuccess)
{
	if (In_isSuccess == true)
	{
		_panelSuccess->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_panelFail->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_panelFail->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_panelSuccess->SetVisibility(ESlateVisibility::Hidden);		
	}
}

// 룰렛 연출 표시 처리
void UGsUIStarLegacySlot::SetRouletteSlotVisible(bool In_isVisibleOn)
{
	ESlateVisibility visible = (In_isVisibleOn == true)? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_panelRoulette->SetVisibility(visible);
}