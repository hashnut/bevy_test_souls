#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "GsWidgetComponentPooling.generated.h"

class UClass;
class UUserWidget;
class UGsWidgetPoolManager;
/**
* 풀링을 지원하는 위젯 컴포넌트
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UGsWidgetComponentPooling : public UWidgetComponent
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UGsWidgetPoolManager> _poolManager;
	TFunction<void(class UUserWidget*)>	_callbackDestory;

public:	
	void SetWidgetClassWithPooling(TSubclassOf<UUserWidget> InWidgetClass, TFunction<void(UUserWidget*)> InCallbackDestory = nullptr);

	/** Ensures the user widget is initialized */
	virtual void InitWidget() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UUserWidget* Claim(UClass* InClass, TFunction<void(class UUserWidget*)> InCallbackDestory);
};