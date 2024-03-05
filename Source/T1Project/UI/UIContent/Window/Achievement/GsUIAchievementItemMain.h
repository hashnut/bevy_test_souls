#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/Achievement/GsUIAchievementItem.h"
#include "Achievement/GsAchievementData.h"
#include "GsUIAchievementItemMain.generated.h"

class UGsUIProgressBar;
class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIAchievementItemMain : public UGsUIAchievementItem
{
	GENERATED_BODY()

private:
	AchievementId _receivedAchievementId = INVALID_ACHIEVEMENT_ID;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _multiProgressBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRecv;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDetail;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _tagComplete;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnReceive();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnAchievementGroupList();

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	virtual void RefreshUI() override;
};
