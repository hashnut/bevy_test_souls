#pragma once


#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUITray.h"

#include "Summon/GsSummonDefine.h"

#include "GsUITraySummonDialogue.generated.h"

/*
	epic 이상 등급 등장 시퀀스 내에서 출력할 대사 tray
*/

class UWidgetAnimation;

UCLASS()
class T1PROJECT_API UGsUITraySummonDialogue : public UGsUITray
{
	GENERATED_BODY()

public:
	// 대사
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textDialogue;

	// 연출 애니매이션
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animation;

public:
	// 데이터 세팅
	void SetData(EGsSummonType In_type, int In_tblId);

	// 연출 끝
	UFUNCTION()
	void FinishAnimationBP();
};