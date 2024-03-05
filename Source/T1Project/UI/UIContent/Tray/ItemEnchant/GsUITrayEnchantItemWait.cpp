// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayEnchantItemWait.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Management/ScopeGame/GsItemEnchantManager.h"
#include "../Management/GsScopeGame.h"
#include "../Management/GsScopeHolder.h"


void UGsUITrayEnchantItemWait::NativeConstruct()
{
	Super::NativeConstruct();
	
	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayEnchantItemWait::OnClickConfirm);
	
	FGsItemEnchantManager* mgr = GSItemEnchant();
	if (mgr)
	{
		mgr->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::SingleEnchant_NotSafe_Wait);
	}
}

void UGsUITrayEnchantItemWait::NativeDestruct()
{
	_btnConfirm->OnClicked.RemoveDynamic(this, &UGsUITrayEnchantItemWait::OnClickConfirm);

	_confirmCallback = nullptr;
	_currentTime = 0.f;
	_lockMaxTime = 0.f;

	FGsItemEnchantManager* mgr = GSItemEnchant();
	if (mgr)
	{
		mgr->StopEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::SingleEnchant_NotSafe_Wait);
	}

	Super::NativeDestruct();
}

void UGsUITrayEnchantItemWait::OnClickConfirm()
{
	FGsItemEnchantManager* mgr = GSItemEnchant();
	if (mgr)
	{
		if (false == mgr->IsOpenItemEnchangWidget())
		{
			Close();
		}
	}

	if (false == IsPossibleOnClick())
		return;

	if (nullptr != _confirmCallback)
	{
		// 중복 재클릭이 안되도록 임시로 막음
		_currentTime = 0.f;
		_lockMaxTime = 10.0f;

		_confirmCallback();
	}
	else
	{
		Close();
	}
}

bool UGsUITrayEnchantItemWait::IsPossibleOnClick() const
{
	return (_currentTime >= _lockMaxTime);
}

void UGsUITrayEnchantItemWait::SetData(float InBtnLockTime, TFunction<void()> InConfirmCallback)
{
	_lockMaxTime = InBtnLockTime;
	_confirmCallback = InConfirmCallback;
}

// 버튼 클릭까지 1초 정도의 텀을 둠 (바로 광클로 인한 혹이나 꼬임현상 방지 / 리니지2m 동일함)
void UGsUITrayEnchantItemWait::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	_currentTime += InDeltaTime;
}