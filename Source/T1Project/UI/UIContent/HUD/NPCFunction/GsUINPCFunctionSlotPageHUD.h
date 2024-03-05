#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUINPCFunctionSlotPageHUD.generated.h"

class UGsUINPCFunctionButton;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUINPCFunctionSlotPageHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<UGsUINPCFunctionButton*> _slotList;

	TMap<int32, UGsUINPCFunctionButton*> _slotMap; // key: slotId

protected:
	// True일 경우, 무한스크롤 연출을위해 추가된 페이지. 메시지를 보내지 않도록 막을 것.
	bool _bIsFakePage = false;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetIsFakePage(bool bIsFakePage);

public:
	UGsUINPCFunctionButton* GetSlot(int32 InSlotId) const;
};