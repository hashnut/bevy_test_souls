#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Seal/EGsSealState.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/GsUIWindowSeal.h"

#include "GsUISealSlot.generated.h"

class UGsToggleButton;
class UGsUIVFX;
class UWidgetSwitcher;

UCLASS()
class UGsUISealSlot final : public UUserWidget, public IGsUIWindowSealEvent
{
	GENERATED_BODY()

public:
	struct Parameters final
	{
		SealId _nodeId = INVALID_SEAL_ID;
		int32 _slotIndex = -1;
		bool _isOpened = false;
		bool _isStatUnlocked = true;
		bool _isColorUnlocked = true;
		SealSlotColor _sealColor = SealSlotColor::NONE;
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _locked;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _colorSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _casting;

private:
	Parameters _parameters;

public:
	void SetParameters(Parameters InParamerters);

	UFUNCTION()
	void OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters) final;
};
