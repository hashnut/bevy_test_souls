#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPresetSkill.generated.h"

/*
	프리셋 스킬 UI
*/

class UGsButton;
class UPanelWidget;
class UImage;
class FGsSkill;

UCLASS()
class UGsUIPresetSkill : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSkill;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSkill;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageSkill;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageAuto;

protected:
	int32 _slotId = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	void SetSkill(const FGsSkill* InSkill, PresetId InPresetId);
	void SetSlotId(int32 InSlotId) { _slotId = InSlotId; }

protected:
	UFUNCTION()
	void OnClickSkill();
};