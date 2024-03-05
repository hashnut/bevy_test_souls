#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Seal/EGsSealState.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/GsUIWindowSeal.h"

#include "GsUISealNode.generated.h"

class UGsToggleButton;
class UGsUIVFX;
class UProgressBar;
class UWidget;
class UWidgetSwitcher;
struct FGsUIWindowSealEventParameters;

UCLASS()
class UGsUISealNode final : public UUserWidget, public IGsUIWindowSealEvent
{
	GENERATED_BODY()

public:
	struct Parameters final
	{
		SealId _sealNodeId = INVALID_SEAL_ID;
		SealSlotColor _sealColor;
		bool _showSlotColor = false;
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _button;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _colorSwitcher;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectedSwitcher;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIVFX* _colorActiveVfx0;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIVFX* _colorActiveVfx1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIVFX* _colorActiveVfx2;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIVFX* _colorActiveVfx3;

	/** "선택" 상태일 때 visible 로 변경할 객체 목록 */
	TArray<TWeakObjectPtr<UWidget>> _widgetsOnSelected;

	/** "선택" 상태일 때 visible 로 변경할 객체 목록 */
	TMap<SealSlotColor, TWeakObjectPtr<UGsUIVFX>> _colorActiveVfxs;

private:
	Parameters _parameters;

protected:
	void NativeOnInitialized() final;

	/************************************************************************/
	/* setter																*/
	/************************************************************************/
	
public:
	void SetParameters(Parameters InParamerters);

	void AddWidgetsOnSelected(UWidget* InWidget);

	/************************************************************************/
	/* getter																*/
	/************************************************************************/

public:
	UGsToggleButton* GetToggleButton() const;
	
	/************************************************************************/
	/* event																*/
	/************************************************************************/

private:
	UFUNCTION()
	void OnToggleButtonSelected(bool InSelected);

public:
	UFUNCTION()
	void OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters) final;
};
