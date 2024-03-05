#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "GsUIMultiLevelRankGradeLoopEffect.generated.h"

class UWidgetAnimation;

UCLASS()
class T1PROJECT_API UGsUIMultiLevelRankGradeLoopEffect : public UUserWidget
{
	GENERATED_BODY()

protected:
	TMap<ItemGrade, UWidgetAnimation*> _animationDatas;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	void StartEffect(ItemGrade InGrade);
	void EndEffect();
};