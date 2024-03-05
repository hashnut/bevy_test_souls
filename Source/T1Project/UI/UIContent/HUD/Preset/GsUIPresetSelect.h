#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Message/GsMessageUI.h"

#include "GsUIPresetSelect.generated.h"

/*
	HUD 프리셋 선택 UI
*/

class UGsButton;
class UGsSwitcherButton;
class UImage;

UCLASS(meta = (DisableNativeTick))
class UGsUIPresetSelect : public UUserWidget
{
	GENERATED_BODY()

protected:
	enum PresetButtonIndex
	{
		DIMMED = 0,
		NORMAL = 1,
		SELECTED = 2,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPresetInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnPresetFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnPresetSecond;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnPresetThird;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSecond;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textThird;

protected:
	bool _isOpened;

	MsgUIHandleArray _uiMsgHandlerList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void Show();
	void Hide();
	void SetHUDMode(bool InIsHideMode);

protected:
	UFUNCTION()
	void OnClickPresetInfo();
	UFUNCTION()
	void OnClickPresetFirst();
	UFUNCTION()
	void OnClickPresetSecond();
	UFUNCTION()
	void OnClickPresetThird();

	void OnClickPresetButton(PresetId InPresetId);

public:
	void InvalidateSelectUI();
	void InvalidatePreset(const IGsMessageParam* InParam = nullptr);
	void InvalidatePresetName(const IGsMessageParam* InParam = nullptr);
};