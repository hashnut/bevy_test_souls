#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressBar.h"
#include "GsUIProgressBar.generated.h"

class UCanvasPanel;
class UCanvasPanelSlot;
class UImage;
class UTextBlock;
class UGsUIVFX;

UCLASS()
class T1PROJECT_API UGsUIProgressBar : public UUserWidget
{
	GENERATED_BODY()

public:

	/** The progress bar style */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "BaseStyle"))
	FProgressBarStyle BaseWidgetStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "MultiStyle"))
	FProgressBarStyle MultiWidgetStyle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style", meta = (DisplayName = "DecoStartPos"))
	FVector2D DecoOrigin {0, 0};

	/** Used to determine the fill position of the progress bar ranging 0..1 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Progress, meta = (UIMin = "0", UIMax = "1"))
	float Percent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Progress, meta = (UIMin = "0", UIMax = "1"))
	float SecontPercent;

	/** Defines if this progress bar fills Left to right or right to left */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Progress)
	TEnumAsByte<EProgressBarFillType::Type> BarFillType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Progress)
	bool bIsMarquee;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Progress)
	FVector2D BorderPadding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Progress, meta = (ToolTip = "Progress seconds Speed"))
	float ProgressSpeedPerRate{0.f};

	/** Fill Color and Opacity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FLinearColor BaseFillColorAndOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FLinearColor MultiFillColorAndOpacity;

	/** 애니 실행 도중, 100% 될 떄 마다 발생 */
	FSimpleDelegate _onArrivedFullProgressOnAnimation;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _baseProgressBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _multiProgressBar;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UImage* _deco;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _decoWidget;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtProgress;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _effProgressBarRenewal;

	UPROPERTY()
	UCanvasPanelSlot* _decoSlot;

private:
	float _destPercent{ 0.f };
	float _destSecondPercent{ 0.f };
	float _destPercentTime{ 0.f };
	float _maxPercent{ 1.f };
	bool _isShowEffect{ false };

	FVector2D _maxSize{ 0,0 };

	const float SpeedCalculateRate = 1.f;

protected:
	virtual void NativeOnInitialized() override;
	virtual void SynchronizeProperties() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdatePercent(float InPercent, bool InIsFull = false);
	void UpdateSecondPercent(float InPercent, bool InIsFull = false);
	void UpdateProgress(float InDeltaTime);
	void UpdateDecoPosition(const FVector2D& MaxSize);
	UCanvasPanelSlot* GetDecoImageSlot(UCanvasPanel* panel);

public:
	UGsUIProgressBar(const FObjectInitializer& ObjectInitializer);

	void SetPercent(float InPercent, bool InIsFull = false);
	void SetMaxPercent(float InMaxPercent);
	void SetSecondPercent(float InPercent, bool InIsFull = false);
	void SetProgress(float InPercent, float extProgress = 0.f, bool anim = true); //진행되는 ProgressBar 사용시
	void SetProgressText(FText cur);
	void SetProgressIncreaseEffect();
	bool IsPlayProgress();
	void SetDecoWidgetVisibilityByAlpha(bool InVisible);
};

