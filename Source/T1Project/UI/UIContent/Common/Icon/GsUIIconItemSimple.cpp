// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconItemSimple.h"
#include "Management/GsMessageHolder.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/GsItemConsumable.h"
#include "Item/GsItemIngredient.h"
#include "Item/GsItemEquipment.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"
#include "UI/UIControlLib/ContentWidget/GsGrayscaleImage.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Classes/Kismet/KismetMathLibrary.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "TimerManager.h"


void UGsUIIconItemSimple::NativeConstruct()
{
	// 없을 수도 있을 듯 하여 바인딩을 걸지 않고 가져오도록 구현함
	_emptyStateBG = Cast<UImage>(GetWidgetFromName(TEXT("C_Empty_Icon"))); // 없을 수 있다(평타버튼)
	
	ResetCoolTimeInfo();

	SetEmptyStateBG(false);
	SetImageGrayscale(false);
	
	Super::NativeConstruct();
}

void UGsUIIconItemSimple::NativeDestruct()
{
	OnFinishedItemIconCoolTime.Clear();

	FGsMessageHolder* msg = GMessage();
	for (MsgGameObjHandle& elemUI : _messageDelegateList)
	{
		msg->GetGameObject().Remove(elemUI);
	}
	_messageDelegateList.Empty();

	ResetCoolTimeInfo();
	ResetAllAnimation();
	
	Super::NativeDestruct();
}

void UGsUIIconItemSimple::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	SetCoolTime(InDeltaTime);

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIIconItemSimple::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	// Dissolve 애니메이션 종료 시 타이머 제거
	if (_animDissolve.IsValid())
	{
		if (_animDissolve.Get() == Animation)
		{
			ResetDissolveTimer();
		}
	}
}

void UGsUIIconItemSimple::SetEmptyIcon()
{
	Super::SetEmptyIcon();

	OnFinishedItemIconCoolTime.Clear();
	if (0 < _messageDelegateList.Num())
	{
		FGsMessageHolder* msg = GMessage();
		for (MsgGameObjHandle& elemUI : _messageDelegateList)
		{
			msg->GetGameObject().Remove(elemUI);
		}
		_messageDelegateList.Empty();
	}
	ResetCoolTimeInfo();

	_iconImage->SetVisibility(ESlateVisibility::Collapsed);

	SetEmptyStateBG(false);
}

void UGsUIIconItemSimple::SetImageGrayscale(const bool InIsGray)
{
	_iconImage->SetGrayscale(InIsGray);
}

void UGsUIIconItemSimple::SetNotOwnItem(const uint32 InTID, const ItemAmount InAmount, const int32 InEnchantLevel, const uint8 InEnchantShieldCount, const uint8 InEnchantBonusIndex)
{
	Super::SetNotOwnItem(InTID, InAmount, InEnchantLevel,InEnchantShieldCount,InEnchantBonusIndex);

	// 내가 갖고있지 않지만, 디스플레이 되어야 하는 아이템은 
	// 쿨타임 이펙트를 출력하지 않는다
	OnFinishedItemIconCoolTime.Clear();
	if (0 < _messageDelegateList.Num())
	{
		FGsMessageHolder* msg = GMessage();
		for (MsgGameObjHandle& elemUI : _messageDelegateList)
		{
			msg->GetGameObject().Remove(elemUI);
		}
		_messageDelegateList.Empty();
	}
	ResetCoolTimeInfo();

	ResetAllAnimation();
	OnPlayDefaultAnimation();
}

void UGsUIIconItemSimple::SetOwnedItem(const uint64 InItemDBID, const FindItemStorageType InStorageType)
{
	Super::SetOwnedItem(InItemDBID, InStorageType);

	CheckCoolTime();

	ResetAllAnimation();
	OnPlayDefaultAnimation();
}

void UGsUIIconItemSimple::CheckCoolTime()
{
	FGsItem* itemData = GetItemIconData();
	if (nullptr == itemData)
		return;

	if (itemData->IsCooldownTimeItem())
	{
		if (0 >= _messageDelegateList.Num())
		{
			_messageDelegateList.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::LOCAL_COOLTIME_START,
				this, &UGsUIIconItemSimple::ExcuteCoolTime));
			_messageDelegateList.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::LOCAL_COOLTIME_END,
				this, &UGsUIIconItemSimple::FinishedCoolTime));
		}
		if (itemData->IsWeaponType())
		{
			FGsItemEquipment* equipData = itemData->GetCastItem<FGsItemEquipment>();
			if (IsExcuteWeaponItemCooltime(equipData))
			{
				float remainTime = GCoolTime()->GetCoolTime(EGsCoolTime::Change_Weapon, itemData->GetCooldownGroupID(), 0);
				_coolTime = itemData->GetMaxCoolTime();
				_remainCoolTime = remainTime;
			}
			else
			{
				if (0.f < _remainCoolTime)
				{
					ResetCoolTimeInfo();
				}
			}
		}	
		else
		{
			float remainTime = GCoolTime()->GetCoolTime(EGsCoolTime::Item, itemData->GetCooldownGroupID(), 0);
			if (0.f >= remainTime)
			{
				ResetCoolTimeInfo();
			}
			else
			{
				_coolTime = itemData->GetMaxCoolTime();
				_remainCoolTime = remainTime;
			}
		}
	}
	else
	{
		// 소모품 아이템 아닌 슬롯은 쿨타임 관련 초기화 한다
		OnFinishedItemIconCoolTime.Clear();
		if (0 < _messageDelegateList.Num())
		{
			FGsMessageHolder* msg = GMessage();
			for (MsgGameObjHandle& elemUI : _messageDelegateList)
			{
				msg->GetGameObject().Remove(elemUI);
			}
			_messageDelegateList.Empty();
		}
		ResetCoolTimeInfo();
	}
}

void UGsUIIconItemSimple::ResetCoolTimeInfo()
{
	_isCoolTime = false;
	_coolTime = 0.f;
	_remainCoolTime = 0.f;
	SetCooltimeUI(0.f, 0.f);
}

void UGsUIIconItemSimple::SetCoolTime(float InDeltaTime)
{
	if (0.0f < _remainCoolTime)
	{
		SetCoolTimeInter();
	}
}

void UGsUIIconItemSimple::SetCoolTimeInter()
{
	float percent = 0.0f;
	_isCoolTime = true;

	FGsItem* itemData = GetItemIconData();
	if (nullptr == itemData || false == itemData->IsCooldownTimeItem())
	{
		// 예외처리
		ResetCoolTimeInfo();
		return;
	}
	float remainTime = GCoolTime()->GetCoolTime(itemData->IsEquipment() ? EGsCoolTime::Change_Weapon : EGsCoolTime::Item, itemData->GetCooldownGroupID(),0);
	_remainCoolTime = remainTime;

	float ratio = (0.0f < _coolTime) ? 1.0f / _coolTime : 0.0f;
	percent = FMath::Clamp(ratio * _remainCoolTime, 0.0f, 1.0f);

	//GSLOG(Error, TEXT("ratio : %f"), ratio);
	//GSLOG(Error, TEXT("percent : %f"), percent);

	SetCooltimeUI(_remainCoolTime, percent);
}

void UGsUIIconItemSimple::SetCooltimeUI(float InRemainSec, float InRate)
{
	OnChangeCoolTime(InRemainSec, InRate);
}

void UGsUIIconItemSimple::FinishedCoolTime(const struct IGsMessageParam* InParam)
{
	const FGsCoolTimeMessageParam* param = InParam->Cast<const FGsCoolTimeMessageParam>();
	if (nullptr == param)
		return;

	if (EGsCoolTime::Item == param->_type || EGsCoolTime::Change_Weapon == param->_type)
	{
		FGsItem* itemData = GetItemIconData();
		if (nullptr == itemData)
			return;

		if (false == itemData->IsCooldownTimeItem())
			return;

		if (itemData->GetCooldownGroupID() == param->_groupId)
		{
			ResetCoolTimeInfo();
			if (true == OnFinishedItemIconCoolTime.IsBound())
			{
				uint64 itemDbId = itemData->GetDBID();
				OnFinishedItemIconCoolTime.Broadcast(itemDbId);
			}
		}
	}
}

void UGsUIIconItemSimple::ExcuteCoolTime(const struct IGsMessageParam* InParam)
{
	const FGsCoolTimeMessageParam* param = InParam->Cast<const FGsCoolTimeMessageParam>();
	if (nullptr == param)
		return;

	FGsItem* itemData = GetItemIconData();
	if (nullptr == itemData)
		return;

	if (false == itemData->IsCooldownTimeItem())
		return;

	if (itemData->GetCooldownGroupID() != param->_groupId)
		return;

	if (EGsCoolTime::Item == param->_type)
	{
		if (itemData->IsEquipment())
			return;

		float remainTime = GCoolTime()->GetCoolTime(EGsCoolTime::Item, itemData->GetCooldownGroupID(), 0);
		_coolTime = itemData->GetMaxCoolTime();
		_remainCoolTime = remainTime;
	}
	else if (EGsCoolTime::Change_Weapon == param->_type)
	{
		if (false == itemData->IsEquipment() && false == itemData->IsWeaponType())
			return;

		FGsItemEquipment* equipData = itemData->GetCastItem<FGsItemEquipment>();
		if (IsExcuteWeaponItemCooltime(equipData))
		{
			float remainTime = GCoolTime()->GetCoolTime(EGsCoolTime::Change_Weapon, itemData->GetCooldownGroupID(), 0);
			_coolTime = itemData->GetMaxCoolTime();
			_remainCoolTime = remainTime;
		}
	}
}

bool UGsUIIconItemSimple::IsExcuteWeaponItemCooltime(FGsItemEquipment* itemData)
{
	if (nullptr == itemData)
		return false;

	if (GCoolTime()->IsCoolTime(EGsCoolTime::Change_Weapon, itemData->GetCooldownGroupID(), 0))
	{
		UGsItemManager* itemMgr = GItem();
		CreatureWeaponType loginCreatureWeaponType = itemMgr->GetLoginCreatureWeaponType();
		TWeakPtr<FGsItem> item = itemMgr->GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
		if (item.IsValid())
		{
			FGsItemEquipment* equipItem = item.Pin()->GetCastItem<FGsItemEquipment>();
			CreatureWeaponType type = equipItem->GetCreatureWeaponType();
			return itemData->GetCreatureWeaponType() != type;
		}
		else
		{
			//GSLOG(Error, TEXT("[IsExcuteWeaponItemCooltime] - loginCreatureWeaponType : %d"), (uint8)loginCreatureWeaponType);
			return itemData->GetCreatureWeaponType() != loginCreatureWeaponType;
		}
	}
	return false;
}

void UGsUIIconItemSimple::SetEmptyStateBG(bool InActive)
{
	if (_emptyStateBG)
	{
		_emptyStateBG->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UGsUIIconItemSimple::SetIconImageSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		_iconImage->SetPaperSprite(InSprite);
		_iconImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_iconImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIIconItemSimple::SetActiveOnEffect(bool In_isActive)
{
	if (_toggleOnEffect)
	{
		_toggleOnEffect->SetVisibility(true == In_isActive ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UGsUIIconItemSimple::PlayDissolveEffect()
{
	ResetDissolveTimer();
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	
	OnPlayDissolveAnimation();
}

void UGsUIIconItemSimple::OnTimerDissolve()
{
	if (false == _animDissolve.IsValid())
	{
		ResetDissolveTimer();
		_iconImage->SetDissolveIntensity(1.f);
		return;
	}

	if (0 == _animDissolve->GetEndTime())
	{
		ResetDissolveTimer();
		_iconImage->SetDissolveIntensity(1.f);
		return;
	}

	float currAnimTime = GetAnimationCurrentTime(_animDissolve.Get());
	if (0 == currAnimTime)
	{
		_iconImage->SetDissolveIntensity(1.f);
	}
	else
	{
		float intensityVal = currAnimTime / _animDissolve->GetEndTime();
		intensityVal = 1 - UKismetMathLibrary::NormalizeToRange(intensityVal, 0.f, 1.f);
		intensityVal = FMath::Lerp(-1.f, 1.f, intensityVal);

		_iconImage->SetDissolveIntensity(intensityVal);
	}
}

void UGsUIIconItemSimple::ResetDissolveTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(_timerHandleDissolve);
}

void UGsUIIconItemSimple::StartDissolveEffect(UWidgetAnimation* InAnimation)
{
	_animDissolve = InAnimation;

	GetWorld()->GetTimerManager().SetTimer(_timerHandleDissolve, this, &UGsUIIconItemSimple::OnTimerDissolve, 0.05f, true);
}

void UGsUIIconItemSimple::ResetAllAnimation()
{
	ResetDissolveTimer();
	_iconImage->SetDissolveIntensity(1.f);

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	_animDissolve = nullptr;
}
