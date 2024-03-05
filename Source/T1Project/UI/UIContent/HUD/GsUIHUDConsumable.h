// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageGameObject.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIHUDConsumable.generated.h"

class FGsItem;
/**
 * 자동 물약 HUD
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDConsumable : public UGsUIHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsIcon", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgDimmed;

	UPROPERTY(BlueprintReadOnly, Category = "GsUiVFX", meta = (BindWidget))
	class UGsUIVFX* _potionTouchEffect;

public:
	UPROPERTY()
	class UGsUIIconItemSimple* _uiIcon;

private:
	bool _blockProc = false;
	bool _isActiveAutoUse = false;

	MsgHudHandleArray _hudHandlerList;
	MsgGameObjHandleArray _gameObjectHandlerList;

public:
	UGsUIHUDConsumable(const FObjectInitializer& ObjectInitializer);

protected:
	// UUserWidget override.
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// IGsInvalidateUIInterface override.
	virtual void InvalidateAllInternal() override;

private:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

public:
	void OnClickItemWindowsKey();

public:
	void OnClickItem(class UGsUIIconItem& InIcon);
	void SetEnableButton(bool bInEnable);
	class UGsHorizontalBoxIconSelector* GetIconSelector() { return _iconSelector; }
	const FGsItem* GetIconItemData();

protected:
	void ToggleAutoPotionState();
	void OnOpenOption();
	void OnToggleAutoPotionState();
	void OnUpdateOption(const struct IGsMessageParam*);
};
