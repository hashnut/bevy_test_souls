// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GsUIBillboard.generated.h"

/**
 * WidgetComonent에서 사용되는 항목들. 편의상 빌보드로 부른다.
 * GsUIManager에서 생성/관리하지 않는다
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIBillboard : public UGsUIWidgetBase, public IGsInvalidateUIInterface
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<class UGsGameObjectBase> _target;
	// 타겟 연결이 끊겼을 때 확인을 위해 저장
	int64 _targetGameId = 0;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

	virtual void InvalidateAllInternal() override;

public:
	virtual void OnChangeRenderState(bool bIsRendered) {}

public:
	virtual void SetTarget(class UGsGameObjectBase* InTarget);
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_WIDGETCOMPONENT; }

protected:
	virtual EGsGameObjectType GetOwnerObjetType() const { return EGsGameObjectType::Base; }

protected:
	// _target 연결이 끊어질 경우때문에 Invalidate 시 체크(UGsUIBillboard의 주체가 Actor이므로 주기가 안맞을 수있음)
	void CheckAndSetTarget();

public:
	TWeakObjectPtr<class UGsGameObjectBase> GetTarget() { return _target; }
};
