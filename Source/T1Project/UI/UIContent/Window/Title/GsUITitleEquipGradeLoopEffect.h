#pragma once


#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUITitleEquipGradeLoopEffect.generated.h"


UCLASS()
class T1PROJECT_API UGsUITitleEquipGradeLoopEffect : public UUserWidget
{
	GENERATED_BODY()

protected:

	TMap<TitleGrade, class UWidgetAnimation*> _animationDatas;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetData(bool InIsShow, TitleGrade InGrade);
};