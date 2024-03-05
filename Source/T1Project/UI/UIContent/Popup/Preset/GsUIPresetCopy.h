#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPresetCopy.generated.h"

/*
	프리셋 복사 UI
*/

class UGsButton;
class UGsSwitcherButton;
class UImage;

UCLASS()
class UGsUIPresetCopy : public UUserWidget
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
	UGsSwitcherButton* _btnCopyFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCopySecond;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCopyThird;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCopyCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCopyOK;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textFirst;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSecond;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textThird;

protected:
	PresetId _copySelectedPresetId = INVALID_PRESET_ID;
	bool _isOpened;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void Show();
	UFUNCTION()
	void Hide();

	bool IsOpened() { return _isOpened; }

protected:
	void OnClickPresetCopy(PresetId InPresetId);
	UFUNCTION()
	void OnClickCopyFirst();
	UFUNCTION()
	void OnClickCopySecond();
	UFUNCTION()
	void OnClickCopyThird();
	UFUNCTION()
	void OnClickCopyOK();

public:
	void InvalidateCopy();
	void InvalidatePresetName();
};