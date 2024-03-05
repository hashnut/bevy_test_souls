#pragma once

#include "Runtime/Engine/Classes/Engine/EngineTypes.h"

#include "UI/UILib/Base/GsUITray.h"

#include "GsUITrayTickerCollection.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class T1PROJECT_API UGsUITrayTickerCollection : public UGsUITray
{
	GENERATED_BODY()

public:
	struct FInitData
	{
		const FText _name;
		const FText _desc;
	};

protected:
	FTimerHandle _timerOnAnimationEnd;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* _animationStart;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDesc;

public:
	void SetData(const FInitData& InInitData);
	void SetData(FText InTitle, FText InName, FText InDesc);

private:
	void OnAnimationEnded();
};