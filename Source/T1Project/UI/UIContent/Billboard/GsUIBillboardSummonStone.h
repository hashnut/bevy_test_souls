#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"
#include "Summon/GsSummonDefine.h"

#include "GsUIBillboardSummonStone.generated.h"

class UWidgetSwitcher;

// 뽑기방에서 소환석에 나오는 widget(인풋을 touch, drag 할지 표시)
// billboard를 상속받으면 hide flag에 걸려서 안나오므로 그냥 widget 사용
UCLASS()
class UGsUIBillboardSummonStone : public UUserWidget
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	// 입력 타입
	// 0: touch(터치), 1: pull(당기기)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherInputType;

	// 로직 함수
public:
	// 소환석 입력 타입 세팅
	void SetSummonStoneInputType(EGsUISummonStoneInputType In_type);

};