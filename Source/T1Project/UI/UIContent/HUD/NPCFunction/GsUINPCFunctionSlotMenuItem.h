#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GsUINPCFunctionSlotMenuItem.generated.h"

class UGsButton;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUINPCFunctionSlotMenuItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClick;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textNpcName;

private:
	int _npcTblId;
protected:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION()
	void OnClickButton();

public:
	void SetSlotMenuItemData( int In_tblId, int In_itemNum);
};