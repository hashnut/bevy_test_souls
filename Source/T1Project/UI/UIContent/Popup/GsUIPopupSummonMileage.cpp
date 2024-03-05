#include "GsUIPopupSummonMileage.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsSummonManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Popup/SummonMileage/GsUISummonMileageItem.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "Currency/GsCurrencyHelper.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/Widget.h"

void UGsUIPopupSummonMileage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupSummonMileage::OnRefreshEntry);

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupSummonMileage::OnClickOK);
}

// OK 클릭
void UGsUIPopupSummonMileage::OnClickOK()
{
	Close();
}

void UGsUIPopupSummonMileage::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	// 0: 에픽 소환권
	// 1: 유니크 소환권
	bool isUniqueReward = (InIndex == 0)? false: true;

	UGsUISummonMileageItem* item = Cast<UGsUISummonMileageItem>(InEntry);
	if (item == nullptr)
	{
		return;
	}

	// currency 구하기
	CurrencyType findCurrencyType = CurrencyType::MAX;
	if (_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		if (isUniqueReward == false)
		{
			findCurrencyType = CurrencyType::SUMMON_FAIRY_MILEAGE;
		}
		else
		{
			findCurrencyType = CurrencyType::SUMMON_UNIQUE_FAIRY_MILEAGE;
		}
	}
	else
	{
		if (isUniqueReward == false)
		{
			findCurrencyType = CurrencyType::SUMMON_COSTUME_MILEAGE;
		}
		else
		{
			findCurrencyType = CurrencyType::SUMMON_UNIQUE_COSTUME_MILEAGE;
		}
	}

	Currency nowCurrency = FGsCurrencyHelper::GetCurrencyAmount(findCurrencyType);

	// item count 구하기
	int itemCount = GSSummon()->GetSummonMileageRewardItemCountByType(isUniqueReward, _summonType);
	item->SetMileageData(isUniqueReward, _summonType, nowCurrency, itemCount);
}

// 초기화
void UGsUIPopupSummonMileage::InitSummonMileage(EGsSummonType In_summonType)
{
	_summonType = In_summonType;

	SetCurrentMileageData();
}
void UGsUIPopupSummonMileage::NativeConstruct()
{
	Super::NativeConstruct();

	FGsMessageHolder* msg = GMessage();
	_handlerUserInfo = msg->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIPopupSummonMileage::OnUpdateCurrency);

	MUI& msgUI = msg->GetUI();
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::CHANGED_SUMMON_MILEAGE_DATA, this, &UGsUIPopupSummonMileage::OnChangedSummonMileageData));

}
void UGsUIPopupSummonMileage::NativeDestruct()
{
	// 메시지 해제
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		msgMgr->GetUserBasicInfo().Remove(_handlerUserInfo);

		if (0 != _uiMsgArray.Num())
		{
			for (auto iter : _uiMsgArray)
			{
				msgMgr->GetUI().Remove(iter);
			}
			_uiMsgArray.Empty();
		}
	}

	Super::NativeDestruct();
}
void UGsUIPopupSummonMileage::OnUpdateCurrency(uint64 InType)
{
	CurrencyType targetCurrency = static_cast<CurrencyType>(InType);

	// 유니크, 일반 체크
	if (targetCurrency != CurrencyType::SUMMON_COSTUME_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_FAIRY_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_UNIQUE_COSTUME_MILEAGE &&
		targetCurrency != CurrencyType::SUMMON_UNIQUE_FAIRY_MILEAGE
		)
	{
		return;
	}

	SetCurrentMileageData();
}
// 현재 마일리지 데이터로 세팅
void UGsUIPopupSummonMileage::SetCurrentMileageData()
{
	// 1: 유니크보상을 이미 받았는가
	bool isObtained = GSSummon()->IsUniqueMileageObtained(_summonType);

	// 안받았으면 리스트 2개(유니크 포함)
	int listCount = (isObtained == false) ? 2 : 1;

	_scrollBoxHelper->RefreshAll(listCount);
}

// 소환 마일리지 데이터 갱신
void UGsUIPopupSummonMileage::OnChangedSummonMileageData(const  IGsMessageParam* inParam)
{
	SetCurrentMileageData();
}
// 이건 esc 키
// https://jira.com2us.com/jira/browse/C2URWQ-8128
void UGsUIPopupSummonMileage::OnInputCancel()
{
	Close();
}