#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"
#include "GsUIBillboardEliteDungeonWarp.generated.h"

UCLASS()
class T1PROJECT_API UGsUIBillboardEliteDungeonWarp : public UGsUIBillboard
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherPlate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textPropName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textCount;

protected:
	uint64 _gameId = 0;
	int32  _maxLimit = 0;

protected:
	void SetCount(int32 InCount);

public:
	virtual void SetTarget(class UGsGameObjectBase* InTarget) override;
	virtual void UpdateCount();
};
