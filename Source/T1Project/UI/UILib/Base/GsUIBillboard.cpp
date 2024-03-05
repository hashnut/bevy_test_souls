// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBillboard.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "Management/ScopeGame/GsGameObjectManager.h"


void UGsUIBillboard::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	IGsInvalidateUIInterface::CheckInvalidate();
}

void UGsUIBillboard::NativeDestruct()
{
	// 주의: 타겟을 비우면 안됨. (이름없는 캐릭터 버그 생김)
	// UWidgetComponent::TickComponent 에서 RemoveWidgetFromScreen 후에 다시 AddToViewport시키는 경우가있다.
	// 이미 타겟 세팅이 끝난 후에 지우는 경우가 있어, 비우지 않는다.
	//_target = nullptr;
	//_targetGameId = 0;

	Super::NativeDestruct();
}

void UGsUIBillboard::InvalidateAllInternal()
{
	// _target의 연결이 끊어졌는지를 체크
	CheckAndSetTarget();
}

void UGsUIBillboard::SetTarget(class UGsGameObjectBase* InTarget)
{
	_target = InTarget;

	if (_target.IsValid())
	{
		if (FGsGameObjectData* data = _target->GetData())
		{
			_targetGameId = data->GetGameId();			
		}
	}

	InvalidateAll();
}

void UGsUIBillboard::CheckAndSetTarget()
{
	if (_target.IsValid())
	{
		return;
	}

	if (0 != _targetGameId)
	{
		EGsGameObjectType objectType = GetOwnerObjetType();
		if (EGsGameObjectType::Base != objectType)
		{
			if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
			{
				_target = gameObjectMgr->FindObject(objectType, _targetGameId);
			}

#if WITH_EDITOR
			if (_target.IsValid())
			{
				GSLOG(Warning, TEXT("[TestName] Billboard CheckAndSetTarget find success. type: %d"), objectType);
			}
			else
			{
				GSLOG(Warning, TEXT("[TestName] Billboard CheckAndSetTarget find fail. type: %d"), objectType);
			}
#endif
		}
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[TestName] Billboard CheckAndSetTarget fail. _targetGameId is 0."));
#endif
	}
}
