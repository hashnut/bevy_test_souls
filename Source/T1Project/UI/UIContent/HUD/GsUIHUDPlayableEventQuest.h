#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"

#include "Core/Public/Internationalization/Text.h"

#include "GsUIHUDPlayableEventQuest.generated.h"

class UGsUIVFX;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDPlayableEventQuest : public UGsUIHUD
{
	GENERATED_BODY()


protected:
	// 목표 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textGoalContents;
	// 진행 카운트 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textProgressCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _questCompleteEffect;

private:
	int _oldQuestId = -1;

	// virtual func
protected:
	// IGsInvalidateUIInterface overrde.
	virtual void InvalidateAllInternal() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// logic func
public:
	void StartAnimation();
	void SetText();
	void FinishEffect();
	void HideEffect();
};