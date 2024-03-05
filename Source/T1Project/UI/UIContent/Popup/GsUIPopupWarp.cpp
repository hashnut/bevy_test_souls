#include "GsUIPopupWarp.h"

#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "ContentsEvent/GsContentsEventTeleportCost.h"
#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Map/Bookmark/GsBookmarkFunc.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Warp/GsWarpFunc.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

void UGsUIPopupWarp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupWarp::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupWarp::OnClickCancel);
}

void UGsUIPopupWarp::OnInputOk()
{
	bool isUseable = false;

	const FGsCostElement* FirstCostElement = _costPackage->GetFirstCostElement();
	if (nullptr == FirstCostElement)
	{
		GSLOG(Error, TEXT("설정된 비용 정보가 없음."));
		Close();
		return;
	}

	if (CostType::CURRENCY == FirstCostElement->_costType)
	{
		// R2: 비용 부족 시 구매 유도 팝업  연결
		if (!FirstCostElement->PlayerHasEnough())
		{
			_isCallEventProcessFinish = false;
			Close(true);

			FGsCurrencyHelper::OpenLackCurrencyPopup(FirstCostElement->GetCurrencyType(), FirstCostElement->GetCostLackAmount());
			return;
		}

		isUseable = true;
	}
	else
	{
		// 아이템
		uint64 itemDBId = 0;
		ItemId CurrentItemId = INVALID_ITEM_ID;
		isUseable = FGsBookmarkFunc::IsBookmarkScrollExist(itemDBId, CurrentItemId) ? true : false;
	}

	if (isUseable == true &&
		nullptr != _callbackOkClick)
	{
		_callbackOkClick();
		_isCallEventProcessFinish = false;
	}
	else
	{
		_isCallEventProcessFinish = true;
	}

	Close();
}

void UGsUIPopupWarp::OnInputCancel()
{
	_isCallEventProcessFinish = true;
	Close();
}

// ok 클릭
void UGsUIPopupWarp::OnClickOk()
{
	OnInputOk();
}
// cancel 클릭
void UGsUIPopupWarp::OnClickCancel()
{
	OnInputCancel();
}

#define POPUP_WARP_SWITCHER_CURRENCY 0
#define POPUP_WARP_SWITCHER_ITEM 1

void UGsUIPopupWarp::SetData(const FString& In_text, TSharedRef<FGsCostPackage> InCostPackage, TFunction<void()>In_callback /*= nullptr*/)
{
	_textMoveTarget = FText::FromString(In_text);
	_callbackOkClick = In_callback;
	_costPackage = MoveTemp(InCostPackage);

	// 컨텐츠이벤트 - 워프골드할인 적용
	TSharedPtr<FGsCostOption> CostOption = FGsContentsEventTeleportCost::CreateActivatedTeleportCostOption();
	if (CostOption.IsValid())
	{
		_costPackage->AddCostOption(CostOption.ToSharedRef());
	}

	const FGsCostElement* FirstCostElement = _costPackage->GetFirstCostElement();
	if (nullptr == FirstCostElement)
	{
		GSLOG(Error, TEXT("설정된 비용 정보가 없음."));
		return;
	}

	const CostType CurrentCostType = FirstCostElement->_costType;
	const CurrencyType CurrentCurrencyType = FirstCostElement->GetCurrencyType();

	// 스위칭
	if (CostType::CURRENCY == CurrentCostType)
	{
		_switcherCostType->SetActiveWidgetIndex(POPUP_WARP_SWITCHER_CURRENCY);

		_currencySlotNow->SetData(CurrentCurrencyType, FirstCostElement->PlayerHasAmountMax());
		_currencySlotUse->SetData(CurrentCurrencyType, FirstCostElement->GetCostAmount());
		_currencySlotUse->SetAmountTextColor(FirstCostElement->PlayerHasEnough() ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
	}
	else if (CostType::ITEM == CurrentCostType)
	{
		_switcherCostType->SetActiveWidgetIndex(POPUP_WARP_SWITCHER_ITEM);

		FString itemCountUse = FString::FromInt(FirstCostElement->GetCostAmount());
		_textItemCountUse = FText::FromString(itemCountUse);

		FString itemCountNow = FString::FromInt(FirstCostElement->PlayerHasAmountMax());
		_textItemCountNow = FText::FromString(itemCountNow);

		_colorItemCountUse = FirstCostElement->PlayerHasEnough() ? 
			FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_ENOUGH) : FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK);
	}

	_isCallEventProcessFinish = true;
}

#undef POPUP_WARP_SWITCHER_CURRENCY
#undef POPUP_WARP_SWITCHER_ITEM

void UGsUIPopupWarp::NativeDestruct()
{
	FGsEventProgressManager* eventProgressManager = GSEventProgress();
	FGsWorldMapManager* worldmapManager = GSWorldMap();

	if(eventProgressManager != nullptr && worldmapManager != nullptr)
	{ 	
		if (_isCallEventProcessFinish == false)
		{
			// 성공하면 예약 이벤트 클리어
			// 워프가야되어서 다음꺼 진행하면 안됨
			eventProgressManager->ClearAllData();
		}
		else
		{
			EGsEventProgressType currentProgressType = worldmapManager->GetCurrentProgressType();
			// 이벤트 진행 순서에 전송(다음 이벤트 진행)
			eventProgressManager->OnReqFinishEvent(currentProgressType);
		}
	}
	Super::NativeDestruct();
}