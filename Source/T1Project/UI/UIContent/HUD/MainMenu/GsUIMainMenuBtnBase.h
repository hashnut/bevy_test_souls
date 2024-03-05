// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "Unlock/GsUnlockDefine.h"
#include "GsUIMainMenuBtnBase.generated.h"

class IGsRedDotCheck;
class UGsSwitcherButton;

/**
 * HUD 메뉴버튼
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class T1PROJECT_API UGsUIMainMenuBtnBase : public UUserWidget, public IGsTutorialInteractionInterface
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnCickMenu);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UGsSwitcherButton* _btnSlot;

protected:
	EGsUnlockContentsType _unlockContentsType = EGsUnlockContentsType::NONE;
	bool _bForcedDisable = false;

public:
	FOnCickMenu OnClickMenu;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	virtual void OnClickSlot();

	// 레드닷
public:	
	virtual void SetRedDotCheck(IGsRedDotCheck* InLogic, EGsIconRedDotType InType = EGsIconRedDotType::CHECK) {}
	virtual void SetReinforceCheck(IGsRedDotCheck* InLogic) {}
	virtual bool UpdateRedDot(bool bUpdate) { return false; }

	virtual IGsRedDotCheck* GetRedDotCheck() const { return nullptr; }
	virtual EGsIconRedDotType GetRedDotType() const { return EGsIconRedDotType::NORMAL;	}

	// 언락
public:
	virtual void SetContentIsNew(bool bIsNew) {}
	virtual void SetMenuOpen(bool bIsOpen) {}

protected:
	virtual void SetButtonLock(bool bIsLocked);

public:
	void SetUnlockContentsType(EGsUnlockContentsType InType) { _unlockContentsType = InType; }
	EGsUnlockContentsType GetUnlockContentsType() const { return _unlockContentsType; }
	
	void SetForcedDisable(bool bInDisable) { _bForcedDisable = bInDisable; }
	bool IsForcedDisable() const { return _bForcedDisable; }

public:
	void UpdateContentLock();
	bool IsEnableButton() const;
	bool OnCheckCanClick();

	UGsSwitcherButton* GetButton() const { return _btnSlot; }

	//------------------------------------------------------------------------------------------------------------------
	// Tutorial
private:
	FGsOnTutorialInteraction OnTutorialInteraction;
	float _backupBtnLockSec = 0.f;

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial);
	virtual bool IsTutorialInteraction() const { return OnTutorialInteraction.IsBound(); }
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() { return OnTutorialInteraction; }
	//------------------------------------------------------------------------------------------------------------------
};
