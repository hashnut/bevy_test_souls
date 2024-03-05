#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "GsUIMonsterPortrait.generated.h"

UCLASS()
class T1PROJECT_API UGsUIMonsterPortrait : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgPortrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherGradeEdge;

protected:
	FSoftObjectPath _lastSoftObjectPath;

protected:
	void SetImageSprite(class UPaperSprite* InSprite);
	void LoadImage(const FSoftObjectPath& InPath);

public:
	void SetData(const struct FGsSchemaNpcData* InData);
};
