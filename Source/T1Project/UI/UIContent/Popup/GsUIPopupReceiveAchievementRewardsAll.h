#pragma once

#include "CoreMinimal.h"
#include "GsUIPopupItemReceive.h"

#include "GsUIPopupReceiveAchievementRewardsAll.generated.h"

UCLASS()
class T1PROJECT_API UGsUIPopupReceiveAchievementRewardsAll : public UGsUIPopupItemReceive
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _title;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _btnName;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual	void OnClickOk() override;

public:
	TFunction<void()> _delegeteClosed;

	void SetDetail(FText& title, FText& btnText);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel();
};