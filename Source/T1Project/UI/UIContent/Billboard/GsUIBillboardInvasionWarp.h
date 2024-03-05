#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"
#include "GsUIBillboardInvasionWarp.generated.h"

class UWidgetSwitcher;
/**
 * 크리쳐 기본 빌보드. 위젯 컴포넌트에서 사용되는 객체.
 */
UCLASS()
class T1PROJECT_API UGsUIBillboardInvasionWarp : public UGsUIBillboard
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPlate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textPropName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textInvaderCount;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//FText _textInvaderCountFull;

protected:
	uint64 _gameId;
	int32  _maxNumInvaders;

protected:
	virtual void NativeConstruct() override;

protected:
	void SetInvaderCount(int32 curNum);

public:
	virtual void SetTarget(class UGsGameObjectBase* InTarget) override;
	void UpdateInvaderInfo();
};
