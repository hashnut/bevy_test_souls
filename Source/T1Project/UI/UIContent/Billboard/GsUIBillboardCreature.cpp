// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBillboardCreature.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"


void UGsUIBillboardCreature::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InitFunctionMap();

	if (_targetBlockLeft)
	{
		_defaultTargetBlockColor = _targetBlockLeft->ColorAndOpacity.GetSpecifiedColor();
	}
	if (_textBlockName)
	{
		_defaultTextBlockColor = _textBlockName->ColorAndOpacity.GetSpecifiedColor();
	}
}

void UGsUIBillboardCreature::Reset()
{
	_hpBarValue = 1.0f;
	_bIsTargeting = false;
}

void UGsUIBillboardCreature::InitFunctionMap()
{
	_updateFuncMap.Emplace(EGsBillboardUpdateType::All, [this]() { InvalidateAll(); });
}

// Enemy Only
void UGsUIBillboardCreature::SetTargetingName(bool InTarget)
{
	if (nullptr == _targetBlockLeft || nullptr == _targetBlockRight)
		return;

	if (true == InTarget)
	{
		_targetBlockLeft->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_targetBlockRight->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_targetBlockLeft->SetVisibility(ESlateVisibility::Hidden);
		_targetBlockRight->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIBillboardCreature::SetNameText(const FText& InName)
{
	if (_textBlockName)
	{
		_textBlockName->SetText(InName);
		SetNameColor(_defaultTextBlockColor);
	}

	SetTargetingName(_bIsTargeting);
}

void UGsUIBillboardCreature::OnTargeting(bool InIsTarget, UGsGameObjectBase* InOwner, bool InIsFirstTarget, bool InIsEnemy)
{
	if (false == _target.IsValid())
	{
		GSLOG(Warning, TEXT("[Billboard] _target is not valid. SetTarget again."));
		SetTarget(InOwner);
	}

	_bIsTargeting = InIsTarget;

	SetTargetingName(InIsTarget);
	if (InIsFirstTarget)
	{
		SetTargetBlockColor(_defaultTargetBlockColor);
	}
	else
	{
		//SetTargetBlockColor((InIsEnemy) ? FLinearColor::White : FLinearColor::Green);
		OnSecondTarget();
	}
}
void UGsUIBillboardCreature::OnSecondTarget()
{
	SetTargetBlockColor(FLinearColor::White);
}
void UGsUIBillboardCreature::SetHPPercent(float InPercent)
{
	_hpBarValue = FMath::Clamp(InPercent, 0.0f, 1.0f);
}

void UGsUIBillboardCreature::SetHPShieldProgressBar(UProgressBar* InBarShieldFront,	UProgressBar* InBarShieldBack,
	int InCurrHp, int InMaxHp, int InCurrShield)
{
	if (nullptr == InBarShieldFront ||
		nullptr == InBarShieldBack)
	{
		return;
	}

	float hpRatio = FMath::Clamp(static_cast<float>(InCurrHp) / static_cast<float>(InMaxHp), 0.0f, 1.0f);
	SetHPPercent(hpRatio);

	// 실드가 없는 경우
	if (0 >= InCurrShield)
	{
		InBarShieldFront->SetVisibility(ESlateVisibility::Collapsed);
		InBarShieldBack->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	// maxHp를 초과할 경우
	if (InCurrShield + InCurrHp > InMaxHp)
	{
		InBarShieldFront->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		InBarShieldBack->SetVisibility(ESlateVisibility::Collapsed);

		InBarShieldFront->SetPercent(FMath::Clamp(
			static_cast<float>(InCurrShield) / static_cast<float>(InMaxHp), 0.0f, 1.0f));
	}
	else
	{
		InBarShieldFront->SetVisibility(ESlateVisibility::Collapsed);
		InBarShieldBack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// HP바 뒤로 나와보이도록 더해서 계산
		InBarShieldBack->SetPercent(FMath::Clamp(
			static_cast<float>(InCurrHp + InCurrShield) / static_cast<float>(InMaxHp), 0.0f, 1.0f));
	}
}

void UGsUIBillboardCreature::SetNameColor(const FLinearColor& InColor)
{
	if (_textBlockName)
	{
		_textBlockName->SetColorAndOpacity(InColor);
	}
}

void UGsUIBillboardCreature::SetTargetBlockColor(const FLinearColor& InColor)
{
	if (_targetBlockLeft)
	{
		_targetBlockLeft->SetColorAndOpacity(InColor);
	}

	if (_targetBlockRight)
	{
		_targetBlockRight->SetColorAndOpacity(InColor);
	}
}

UGsGameObjectLocalPlayer* UGsUIBillboardCreature::GetLocalPlayer() const
{
	if (UGsGameObjectManager* spawner = GSGameObject())
	{
		return Cast<UGsGameObjectLocalPlayer>(spawner->FindObject(EGsGameObjectType::LocalPlayer));
	}

	return nullptr;
}

void UGsUIBillboardCreature::UpdateByType(EGsBillboardUpdateType InType)
{
	if (TFunction<void()>* func = _updateFuncMap.Find(InType))
	{
		(*func)();
	}
}

bool UGsUIBillboardCreature::IsTargeting() const
{
	return _bIsTargeting;
}
