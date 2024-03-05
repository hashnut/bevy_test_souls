#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../UI/UILib/Base/GsUIVFX.h"
#include "GsUIMonsterCollectionSlateNodeFadeInVFX.generated.h"

class UImage;
class UGsUIVFX;
class UGsButton;
class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateNodeFadeInVFX : public UGsUIVFX
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSymbol;

public:
	void SetData(class UPaperSprite* InData);
};