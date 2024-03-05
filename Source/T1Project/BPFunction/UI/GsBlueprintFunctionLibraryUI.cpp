// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBlueprintFunctionLibraryUI.h"
#include "Public/Blueprint/WidgetLayoutLibrary.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"

#include "Option/GsGameUserSettings.h"

#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UILib/Container/GsWidgetPool.h"
#include "UI/UILib/Manager/GsDealScrollManager.h"
#include "UI/UIContent/DealScroll/GsUIDealScrollBase.h"
#include "UI/UIContent/Tray/GsUITrayLongPressBar.h"
#include "UI/UIContent/Tray/GsUITrayItemNameToolTip.h"
#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "UI/UILib/Manager/GsEmoticonManager.h"

#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"
#include "Runtime/Slate/Public/Framework/Application/SlateUser.h"


UGsWidgetPoolManager* UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return nullptr;
	}

	return uiManager->GetWidgetPoolManager();
}

UGsUIDealScrollBase* UGsBlueprintFunctionLibraryUI::AddDealScroll(FName InKey, USceneComponent* InTarget,
	const FString& InValue, const FVector& InDir, int64 InGameId)
{
	if (InKey.IsNone())
	{
		return nullptr;
	}

	UGsGameUserSettings* gameUserSettings = GGameUserSettings();
	if (gameUserSettings == nullptr)
	{
		return nullptr;
	}
	// 옵션 값 확인
	bool isShowDamageFloater =
		(bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::DAMAGE_FLOATER);

	if (isShowDamageFloater == false)
	{
		return nullptr;
	}

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return nullptr;
	}

	UGsDealScrollManager* dealScrollManager = uiManager->GetDealScrollManager();
	if (nullptr != dealScrollManager)
	{
		return dealScrollManager->AddWidget(InKey, InTarget, InValue, InDir, InGameId);
	}

	return nullptr;
}

const FName UGsBlueprintFunctionLibraryUI::GetDealScrollKey(HitResultType InType, bool bInIsDamaged)
{
	if (bInIsDamaged)
	{
		// 피격 딜스크롤 타입
		switch (InType)
		{
		case HitResultType::NORMAL:
		case HitResultType::GREAT:
			return TEXT("Damaged");
		case HitResultType::DODGE:
			return TEXT("DodgeMe");		
		case HitResultType::CRITICAL:
			return TEXT("CriticalDamaged");
		case HitResultType::HEAL:
			return TEXT("Heal");
		case HitResultType::IMMUNE_DAMAGE:
			return TEXT("DamageImmuneMe");
		case HitResultType::IMMUNE_ABNORMALITY:
			return TEXT("ResistMe");
		}

		return TEXT("");
	}

	// 타격 딜스크롤 타입
	switch (InType)
	{
	case HitResultType::NORMAL:
	case HitResultType::GREAT:
		return TEXT("Hit");
	case HitResultType::DODGE:
		return TEXT("Dodge");
	case HitResultType::CRITICAL:
		return TEXT("CriticalHit");
	case HitResultType::HEAL:
		return TEXT("Heal");
	case HitResultType::IMMUNE_DAMAGE:
		return TEXT("DamageImmune");
	case HitResultType::IMMUNE_ABNORMALITY:
		return TEXT("ResistMe");
	}

	return TEXT("");
}

UGsUIHUDEmoticon * UGsBlueprintFunctionLibraryUI::AddEmoticon(EGsEmoticonType InType, UGsGameObjectBase * InTarget, FVector InDir)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return nullptr;
	}

	UGsEmoticonManager* emoticonManager = uiManager->GetEmoticonManager();
	if (nullptr != emoticonManager)
	{
		return emoticonManager->AddWidget(InType, InTarget, InDir);
	}

	return nullptr;
}

UGsUIVFX* UGsBlueprintFunctionLibraryUI::AddVFX(TSubclassOf<UGsUIVFX> InClass, const FVector2D& InPosition, int32 InZOrder)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return nullptr;
	}

	UGsWidgetPoolManager* poolMgr = uiManager->GetWidgetPoolManager();
	if (nullptr == poolMgr)
	{
		return nullptr;
	}

	UGsWidgetPool* pool = poolMgr->FindPool(InClass);
	if (nullptr == pool)
	{
		pool = poolMgr->CreatePool(InClass);
		if (nullptr == pool)
		{
			return nullptr;
		}
	}

	UGsUIVFX* widget = pool->Claim<UGsUIVFX>();
	if (widget)
	{
		widget->SetDynamicCreate(true, InPosition, true);
		widget->AddToViewport(InZOrder);
		widget->ShowVFX();
	}

	return widget;
}

UGsUIVFX* UGsBlueprintFunctionLibraryUI::AddVFXToPointerPos(TSubclassOf<UGsUIVFX> InClass, int32 InZOrder)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return nullptr;
	}

	UGsWidgetPoolManager* poolMgr = uiManager->GetWidgetPoolManager();
	if (nullptr == poolMgr)
	{
		return nullptr;
	}

	UGsWidgetPool* pool = poolMgr->FindPool(InClass);
	if (nullptr == pool)
	{
		pool = poolMgr->CreatePool(InClass);
		if (nullptr == pool)
		{
			return nullptr;
		}
	}

	FVector2D pointPosLocal;
	if (false == GetPointerPositionLocal(uiManager->GetWorld(), pointPosLocal))
	{
		return nullptr;
	}

	UGsUIVFX* widget = pool->Claim<UGsUIVFX>();
	if (widget)
	{
		// 주의: 센터 맞추기 하지 않음 위젯에서 좌상단 중심으로 맞추기
		widget->SetDynamicCreate(true, pointPosLocal, true);
		widget->AddToViewport(InZOrder);
		widget->ShowVFX();
	}

	return widget;
}

UGsUITrayLongPressBar* UGsBlueprintFunctionLibraryUI::AddLongPressBar(class UWidget* InTarget, int32 InZOrder)
{
	if (nullptr == InTarget)
	{
		return nullptr;
	}

	UClass* widgetClass = GPath()->GetClass(EGsPathType::UI, TEXT("TrayLongPress"));
		
	FVector2D targetPos;
	UGsUIWidgetBase* widget = AddWidgetToTargetTop(InTarget, widgetClass, targetPos);
	if (nullptr == widget)
	{
		return nullptr;
	}

	UGsUITrayLongPressBar* barWidget = Cast<UGsUITrayLongPressBar>(widget);
	if (barWidget)
	{
		barWidget->SetTargetPosition(targetPos);
		barWidget->AddToViewport(InZOrder);
	}

	return barWidget;
}

UGsUITrayItemNameToolTip* UGsBlueprintFunctionLibraryUI::AddNameTooltip(UWidget* InTarget, int32 InZOrder)
{
	if (nullptr == InTarget)
	{
		return nullptr;
	}

	UClass* widgetClass = GPath()->GetClass(EGsPathType::UI, TEXT("TrayItemNameToolTip"));

	FVector2D targetPos;
	UGsUIWidgetBase* widget = AddWidgetToTargetTop(InTarget, widgetClass, targetPos);
	if (nullptr == widget)
	{
		return nullptr;
	}

	UGsUITrayItemNameToolTip* toolTipWidget = Cast<UGsUITrayItemNameToolTip>(widget);
	if (toolTipWidget)
	{
		toolTipWidget->SetTargetPosition(targetPos);
		toolTipWidget->AddToViewport(InZOrder);
	}

	return toolTipWidget;
}

UGsUIWidgetBase* UGsBlueprintFunctionLibraryUI::AddWidgetToTargetTop(UWidget* InTarget, UClass* InWidgetClass,
	OUT FVector2D& OutTargetPos)
{
	if (nullptr == InWidgetClass)
	{
		return nullptr;
	}

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return nullptr;
	}
	
	UGsWidgetPoolManager* poolMgr = uiManager->GetWidgetPoolManager();
	if (nullptr == poolMgr)
	{
		return nullptr;
	}

	UGsWidgetPool* pool = poolMgr->FindPool(InWidgetClass);
	if (nullptr == pool)
	{
		pool = poolMgr->CreatePool(InWidgetClass);
		if (nullptr == pool)
		{
			return nullptr;
		}
	}

	UGsUIWidgetBase* widget = pool->Claim<UGsUIWidgetBase>();
	if (nullptr == widget)
	{
		return nullptr;
	}

	const FGeometry targetGeometry = InTarget->GetCachedGeometry();
	FVector2D locSize = targetGeometry.GetLocalSize();
	locSize.X *= 0.5f;
	locSize.Y = -12.f; // 타겟의 상단에 로컬기준 12 띄움

	OutTargetPos = targetGeometry.LocalToAbsolute(locSize);

	return widget;
}

void UGsBlueprintFunctionLibraryUI::RemoveVFX(UGsUIVFX* InVfx)
{
	if (nullptr == InVfx)
	{
		return;
	}

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return;
	}

	UGsWidgetPoolManager* poolMgr = uiManager->GetWidgetPoolManager();
	if (nullptr == poolMgr)
	{
		return;
	}

	InVfx->RemoveFromParent();
	poolMgr->ReleaseWidget(InVfx->GetClass(), InVfx);
}

void UGsBlueprintFunctionLibraryUI::RemoveWidget(UGsUIWidgetBase* InWidget)
{
	if (nullptr == InWidget)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return;
	}

	UGsWidgetPoolManager* poolMgr = uiManager->GetWidgetPoolManager();
	if (nullptr == poolMgr)
	{
		return;
	}

	InWidget->RemoveFromParent();
	poolMgr->ReleaseWidget(InWidget->GetClass(), InWidget);
}

bool UGsBlueprintFunctionLibraryUI::GetPointerPositionAbsolute(OUT FVector2D& OutAbsolutePosition)
{
#if WITH_EDITOR || PLATFORM_WINDOWS
	OutAbsolutePosition = FSlateApplication::Get().GetCursorPos();
	return true;
#else
#ifdef RAON_ENGINE_LAST_TOUCH_POINTER_INDEX_YJCHOUNG
	TSharedPtr<FSlateUser> cursorUser = FSlateApplication::Get().GetCursorUser();
	if (cursorUser.IsValid())
	{
		if (cursorUser->GetLastTouchedPointerPosition(OutAbsolutePosition))
		{
			return true;
		}
	}
#endif //RAON_ENGINE_LAST_TOUCH_POINTER_INDEX_YJCHOUNG

	OutAbsolutePosition = FVector2D::ZeroVector;
	return false;
#endif
}

bool UGsBlueprintFunctionLibraryUI::GetPointerPositionLocal(UObject* WorldContextObject, OUT FVector2D& OutLocalPosition)
{
	if (WorldContextObject)
	{
		FVector2D absPos;
		if (GetPointerPositionAbsolute(absPos))
		{
			FGeometry viewportGeometry = UWidgetLayoutLibrary::GetViewportWidgetGeometry(WorldContextObject);
			OutLocalPosition = viewportGeometry.AbsoluteToLocal(absPos);

			return true;
		}
	}

	return false;
}

UGsUIWidgetBase* UGsBlueprintFunctionLibraryUI::OpenUI(const FName& InKey, bool bInCheckDuplicate)
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(InKey);
		if (widget.IsValid())
		{
			return widget.Get();
		}
	}

	return nullptr;
}

UGsUIWidgetBase* UGsBlueprintFunctionLibraryUI::GetUI(const FName& InKey)
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(InKey);
		if (widget.IsValid())
		{
			return widget.Get();
		}
	}

	return nullptr;
}

void UGsBlueprintFunctionLibraryUI::CloseUI(const FName& InKey)
{
	if (UGsUIWidgetBase* widget = GetUI(InKey))
	{
		widget->Close();
	}
}

void UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(UUserWidget* InWidget)
{
	if (InWidget)
	{
		InWidget->StopAllAnimations();
		//InWidget->StoppedSequencePlayers.Empty();
	}
}
