// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIHUD.h"

#include "Message/GsMessageGameObject.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIHUDPreset.generated.h"


/**
 * HUD 프리셋 변경 버튼
 */

class UGsButton;
class UWidgetSwitcher;
class UGsUIPresetSelect;
class UGsUICoolTimeEffectMaterial;
struct IGsMessageParam;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDPreset : public UGsUIHUD
{
	GENERATED_BODY()

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPresetCurrent;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPreset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _effectWait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _effectSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICoolTimeEffectMaterial* _effectCoolTime;

	UPROPERTY()
	UGsUIPresetSelect* _uiPresetSelect;

protected:
	MsgGameObjHandleArray _gameObjectMsgHandlerList;

	bool _hasCoolTime = false;
	bool _isSelectOpen = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

public:
	UFUNCTION()
	void OnClickPreset();

	void OnLocalCooltimeStart(const IGsMessageParam* InParam);
	void OnLocalCooltimeEnd(const IGsMessageParam* InParam);
	void OnUpdateUserPresetId(const IGsMessageParam* InParam);

	void SetPresetSelect(UGsUIPresetSelect* InPresetSelect);

protected:
	void OpenPresetSelect(bool InIsOpen);

protected:
	virtual void InvalidateAllInternal() override;

public:
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

};
