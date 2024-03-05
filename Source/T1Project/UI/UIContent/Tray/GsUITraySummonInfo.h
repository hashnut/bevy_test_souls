#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUITray.h"

#include "Summon/GsSummonDefine.h"

#include "GsUITraySummonInfo.generated.h"

/*
	tray summon: 고등급 이상 등장 시퀀스 내에서 출력할 이름, 등급 정보
*/

class UImage;

UCLASS()
class T1PROJECT_API UGsUITraySummonInfo : public UGsUITray
{
	GENERATED_BODY()
public:
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textName;
	// 등급
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textGrade;
	// 등급 bg
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgGradeBg;
public:
	// 데이터 세팅
	void SetData(EGsSummonType In_type, int In_tblId);
};