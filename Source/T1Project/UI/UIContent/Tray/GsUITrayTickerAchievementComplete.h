#pragma once
#include "CoreMinimal.h"
#include "GsUITrayTickerCollectionMulti.h"
#include "GsUITrayTickerAchievementComplete.generated.h"

class UTextBlock;

UCLASS()
class T1PROJECT_API UGsUITrayTickerAchievementComplete : public UGsUITrayTickerCollectionMulti
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

public:
	enum class AchievementCompleteType { Theme, Achievement, ThemeAll, AchievementAll };

	struct FInitData
	{	
		AchievementCompleteType _eType{ AchievementCompleteType::Theme };
		const FText _name;
		const FText _desc;
	};

public:
	void SetData(const FInitData& InInitData);

private:
	void OnInitWork(const FInitData& data);
//	void OnAnimationEnded();
};

