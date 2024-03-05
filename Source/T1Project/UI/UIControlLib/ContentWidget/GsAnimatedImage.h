#pragma once
#include "CoreMinimal.h"
#include "Components/Image.h"
#include "GsAnimatedImage.generated.h"

UCLASS()
class UGsAnimatedImage : public UImage
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = Animation)
	void SetCurrentFrame(int32 Frame);

	UFUNCTION(BlueprintCallable, Category = Animation)
	void Play();

	UFUNCTION(BlueprintCallable, Category = Animation)
	void Stop();

	virtual void SynchronizeProperties();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	int32 FramesPerSecond = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	int32 CurrentFrame = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	int32 TotalFrames = 1;

	void TimerTick();

	FTimerHandle TimerHandle;

};