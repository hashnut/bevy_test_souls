#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITooltip.h"
#include "GsUICostumeSkillTooltip.generated.h"

class UTextBlock;

UCLASS()
class T1PROJECT_API UGsUICostumeSkillTooltip : public UGsUITooltip
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;

public:
	void OpenIndex(int index);
	void SetData(int index, FText title, FText desc);
	void ClearData();

protected:
	TMap<int, TPair<FText, FText>> _dataList;

protected:
	virtual void NativeOnInitialized() override;
};
