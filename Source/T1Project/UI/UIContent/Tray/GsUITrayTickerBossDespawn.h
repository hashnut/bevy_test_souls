#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITicker.h"
#include "GsUITrayTickerBossDespawn.generated.h"

class UWidgetAnimation;
class UImage;

UCLASS()
class T1PROJECT_API UGsUITrayTickerBossDespawn : public UGsUITicker
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* _bossImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textDespawn;
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animationTray;

protected:
	virtual void NativeConstruct() override;

public:
	// 데이터 세팅
	void SetDespawnData(int32 In_creatureId);
	void InvalidBossImage(int32 In_creatrueId);
	// 연출 끝
	UFUNCTION()
	void FinishAnimationBP();
};