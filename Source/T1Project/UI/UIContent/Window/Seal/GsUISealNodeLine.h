#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/GsUIWindowSeal.h"

#include "GsUISealNodeLine.generated.h"

class UGsUIVFX;
class UWidgetSwitcher;
struct FGsUIWindowSealEventParameters;

/** 봉인노드간 라인 UI */
UCLASS()
class UGsUISealNodeLine final : public UUserWidget, public IGsUIWindowSealEvent
{
	GENERATED_BODY()

public:
	struct Parameters final
	{
		SealId _sealNodeId = INVALID_SEAL_ID;
		bool _isVisible = false;
		SealSlotColor _sealColor = SealSlotColor::NONE;
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _colorSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UUserWidget* _casting;

private:
	Parameters _parameters;

public:
	void SetParameters(Parameters InParamerters);

	UFUNCTION()
	void OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters) final;
};
