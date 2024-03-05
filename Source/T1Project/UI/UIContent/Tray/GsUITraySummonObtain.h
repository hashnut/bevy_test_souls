#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUITray.h"

#include "Summon/GsSummonDefine.h"

#include "GsUITraySummonObtain.generated.h"

/*
	epic 이상 등급 등장 시퀀스 내에서 출력할 카드 tray
*/

class UImage;
class UTextBlock;

class FGsSummonCardData;

UCLASS()
class T1PROJECT_API UGsUITraySummonObtain : public UGsUITray
{
	GENERATED_BODY()

public:

	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textFairyName;
	// 초상화 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgPortrait;
	// 초상화 bg
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgBg;
	// 이름 등급 색상 조절용
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockFairyName;
	// 후면 등급 이펙트 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgBackGradeEffect;
	// 전면 이펙트 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgFrontGradeEffect;
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