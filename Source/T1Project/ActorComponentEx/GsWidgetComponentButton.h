#pragma once

#include "CoreMinimal.h"

#include "UMG/Public/Components/WidgetComponent.h"

#include "GsWidgetComponentButton.generated.h"

class UWidget;

/*
* 위젯 컴퍼넌트 클릭 처리용(버튼)
* https://answers.unrealengine.com/questions/896176/view.html
*/

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UGsWidgetComponentButton : public UWidgetComponent
{
	GENERATED_BODY()

private:
	FVector2D _prevTouchPos;
	FVector2D _touchPos;

	// 클릭 콜백
	TFunction<void(bool)> _callbackClick;

	

	// static 함수
public:
	// 자식들까지 체크
	static void ForWidgetAndChildren(
		UWidget* In_widget, TFunctionRef<void(UWidget*)> In_predicate,
		bool In_recursive = true);

	// 가상 함수
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	// 로직 함수
public:
	// 터치 이벤트 처리
	bool ManageTouchEvent();
	// 위젯 존인가 체크
	bool IsInWidgetZone(FVector2D In_touchPos, UWidget* In_widget);

	// set
public:
	void SetClickCallback(TFunction<void(bool) > In_callback)
	{
		_callbackClick = In_callback;
	}
};