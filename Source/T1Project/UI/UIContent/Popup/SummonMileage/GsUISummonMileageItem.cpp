#include "GsUISummonMileageItem.h"

#include "Management/ScopeGame/GsSummonManager.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"

#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"

// 마일리지 데이터 세팅
void UGsUISummonMileageItem::SetMileageData(bool In_isUniqueReward,
	EGsSummonType In_summonType, Currency In_mileage, int In_itemCount)
{	
	_summonType = In_summonType;
	_isUniqueReward = In_isUniqueReward;

	// 리워드 세팅
	const FGsSchemaItemCommon* rewardItemTbl = 
		GSSummon()->FindSummonMileageRewardItemTableBySummonType(In_isUniqueReward ,In_summonType);
	if (rewardItemTbl == nullptr)
	{
		return;
	}

	int itemId = rewardItemTbl->id;
	int maxSummonMileage = GSSummon()->GetMaxSummonMileageByType(In_isUniqueReward, In_summonType);

	SetRewardData(itemId, In_itemCount);

	// max치 를 나눈 나머지가 나와야 되므로
	Currency progressMileage =0; 
	
	// 에픽은 프로그래스 바 값을 max 값을 나눈 나머지를 표현하는데
	// 유니크는 보상이 1개라서 max넘어가면 그냥 max값으로 표현한다
	if(_isUniqueReward == false)
	{ 
		progressMileage = In_mileage - (In_itemCount * maxSummonMileage);
	}
	else
	{
		progressMileage = FMath::Min((int)In_mileage, maxSummonMileage);
	}

	// 프로그래스바 세팅
	_summonMileageProgressBar->SetProgressText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { int32(progressMileage), int32(maxSummonMileage) })));
	_summonMileageProgressBar->SetProgress(FMath::Clamp<float>(float(progressMileage) / maxSummonMileage, 0.f, 1.0f), 0.0f, false);

	// 타이틀 텍스트 세팅
	SetMileageTitleText(In_isUniqueReward,In_summonType);

	

	// 정보 세팅
	bool rewardExist = (In_itemCount > 0) ? true : false;
	SetMileageRewardInfoText(In_isUniqueReward,rewardExist);

	ESlateVisibility rewardEffectVisibility =
		(rewardExist == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_mileageRewardEffectWidget->SetVisibility(rewardEffectVisibility);
	
}

// 보상 데이터 세팅
void UGsUISummonMileageItem::SetRewardData(int In_rewardItemId, int In_rewardItemCount)
{
	_rewardItemTblId = In_rewardItemId;
	_rewardItemCount = In_rewardItemCount;

	_targetSummonMileageIconSlotHelper->RefreshAll(1);
}

// 마일리지 제목 텍스트 세팅(코스튬, 페어리)
void UGsUISummonMileageItem::SetMileageTitleText(bool In_isUniqueReward, EGsSummonType In_summonType)
{	
	FText findText;
	FTextKey secondKey;

	if (In_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		if (In_isUniqueReward == false)
		{
			secondKey = TEXT("SummonMileage_Fairy");
		}
		else
		{
			secondKey = TEXT("SummonMileage_Fairy_Unique");
		}
	}
	else
	{
		if (In_isUniqueReward == false)
		{
			secondKey = TEXT("SummonMileage_Costume");
		}
		else
		{
			secondKey = TEXT("SummonMileage_Costume_Unique");
		}
	}

	FText::FindText(TEXT("SummonText"), secondKey, findText);

	_textMileageTitle = findText;
}
// 마일리지 보상 텍스트 세팅(획득 가능, 불가)
void UGsUISummonMileageItem::SetMileageRewardInfoText(bool In_isUniqueReward, bool In_isRewardOn)
{
	FText findText;
	FTextKey secondKey;

	if (In_isRewardOn == true)
	{
		secondKey = TEXT("SummonMileage_Guide2");
	}
	else
	{
		if (In_isUniqueReward == false)
		{
			secondKey = TEXT("SummonMileage_Guide1");
		}
		else
		{
			secondKey = TEXT("SummonMileage_Guide3");
		}
	}

	FText::FindText(TEXT("SummonText"), secondKey, findText);

	_textMileageRewardInfo = findText;
}

void UGsUISummonMileageItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_targetSummonMileageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_targetSummonMileageIconSlotHelper->Initialize(_targetSummonMileageItemIconSelector);
	_targetSummonMileageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUISummonMileageItem::OnRefreshTargetIcon);
	_targetSummonMileageIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUISummonMileageItem::OnCreateTargetIcon);
}

void UGsUISummonMileageItem::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}


	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUISummonMileageItem::OnLongPressMileageReward);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUISummonMileageItem::OnClickMileageReward);
	}

	// 에픽은 보상 갯수를 마일리지를 max 로 나눈 값으로 표현
	// 유니크는 보상이 1개라서 0 아니면 1임
	int showCount = 0;
	if (_isUniqueReward == false)
	{
		showCount = _rewardItemCount;
	}
	else
	{
		showCount = (_rewardItemCount == 0)? 0: 1;
	}

	itemIcon->SetNotOwnItem(_rewardItemTblId, showCount);
	// 갯수 0, 1은 표시를 안해서 강제로 하는 처리 추가
	itemIcon->SetItemCountEX(showCount, true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemEnchantLevel(0);
	itemIcon->SetActiveIconBg(false);
}

void UGsUISummonMileageItem::OnCreateTargetIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetActiveIconBg(false);
}
// 보상 아이콘 롱프레스
void UGsUISummonMileageItem::OnLongPressMileageReward(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

// 보상 아이콘 클릭
void UGsUISummonMileageItem::OnClickMileageReward(UGsUIIconItem& InIcon)
{
	// 보상 획득 가능한지
	// 1. 획득할 보상이 있는지
	if (_rewardItemCount == 0)
	{
		FText findText;
		// 마일리지가 부족합니다.
		FText::FindText(TEXT("SummonText"), TEXT("SummonMileage_NotEnough"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}
	// 유니크 보상인가
	// https://jira.com2us.com/jira/browse/CHR-24962

	// 이전에는 tray summon 초기화 시점에 아이템 갯수 저장
	// 팝업에서 보상을 받으므로 서버 send 시점에 아이템 갯수 저장해놓기
	// https://jira.com2us.com/jira/browse/CHR-24962
	GSSummon()->SaveSummonMileageRewardItemCountByType(_isUniqueReward, _summonType);

	// 서버 요청
	if (_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		
		if(_isUniqueReward == false)
		{ 
			FGsNetSendServiceWorld::SendReqFairySummonMileage();
		}
		else
		{
			FGsNetSendServiceWorld::SendReqUniqueFairySummonMileage();
		}
	}
	else
	{
		if (_isUniqueReward == false)
		{
			FGsNetSendServiceWorld::SendReqCostumeSummonMileage();
		}
		else
		{
			FGsNetSendServiceWorld::SendReqUniqueCostumeSummonMileage();
		}
	}

}