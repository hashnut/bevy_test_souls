#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"

#include "Summon/GsSummonDefine.h"

#include "GsUITrayTickerMileageSummon.generated.h"

class UWidgetAnimation;

UCLASS()
class T1PROJECT_API UGsUITrayTickerMileageSummon : public UGsUITray
{
	GENERATED_BODY()

public:
	// 트레이 애니메이션
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animationTray;
	// 리워드 메시지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textRewardMessage;
protected:	
	virtual void NativeConstruct() override;

public:
	// 데이터 세팅 및 애니 시작
	void StartDataTickerMileageSummon(bool In_isUniqueReward, EGsSummonType In_summonType);
	// 연출 끝
	UFUNCTION()
	void FinishAnimationBP();
};
