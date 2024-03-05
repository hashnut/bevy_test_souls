#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "UTIL/GsIntervalChecker.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageGameObject.h"
#include "GsUIHUDSpiritShot.generated.h"


/*
	HUD 천마석(정령탄)
*/


class UImage;

class UGsUIVFX;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItemSimple;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDSpiritShot : public UGsUIHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "GsIcon", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgDimmed;

	UPROPERTY(BlueprintReadOnly, Category = "GsUiVFX", meta = (BindWidget))
	UGsUIVFX* _potionTouchEffect;

	UPROPERTY()
	UGsUIIconItemSimple* _uiIcon;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GsUI")
	float _closeLockTime = 0.5f;
	
public:
	FGsIntervalChecker _checkerCloseLock;

	MsgHudHandleArray _hudHandlerList;
	MsgGameObjHandleArray _gameObjectHandlerList;

	bool _blockProc = false;

protected:
	// UUserWidget override.
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// IGsInvalidateUIInterface override.
	virtual void InvalidateAllInternal() override;

public:
	void OnClickItem(class UGsUIIconItem& InIcon);
	void ClickedSpiritShot();

public:

	void SetEnableButton(bool bInEnable);

protected:
	void ToggleSpiritShotState();
	void OnToggleSpiritShotState();
	void OnUpdateOption(const struct IGsMessageParam*);
};