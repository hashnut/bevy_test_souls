#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "GsUISealNodeGainPoolListEntry.generated.h"

class UWidgetSwitcher;
class UTextBlock;

UCLASS()
class UGsUISealNodeGainPoolListEntry final : public UUserWidget
{
	GENERATED_BODY()

public:
	struct Parameters final
	{
		FText _title;
		FText _value;
		SealSlotColor _color = SealSlotColor::NONE;
		uint32 _sortOrder;
	};
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _colorSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _title0Txt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _value0Txt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _title1Txt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _value1Txt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _title2Txt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _value2Txt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _title3Txt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _value3Txt;

public:
	void SetParameters(const Parameters& InParameters);
};
