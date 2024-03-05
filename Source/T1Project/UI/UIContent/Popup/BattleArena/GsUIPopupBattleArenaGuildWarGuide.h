#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupBattleArenaGuildWarGuide.generated.h"

UCLASS()
class  T1PROJECT_API UGsUIPopupBattleArenaGuildWarGuide : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnConfirm;

protected:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION()
	void OnClickedConfirm();

protected:
	virtual void OnInputCancel() override;
};
