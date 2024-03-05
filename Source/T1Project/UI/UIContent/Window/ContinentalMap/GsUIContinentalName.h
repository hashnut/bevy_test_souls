#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIContinentalName.generated.h"

// continental name bp

UCLASS()
class T1PROJECT_API UGsUIContinentalName : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textTimeZone;

public:
	void SetText(const FText& In_name, const FText& In_timeZone);
};