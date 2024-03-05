#include "GsAnimatedImage.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

void UGsAnimatedImage::SetCurrentFrame(int32 Frame)
{
	CurrentFrame = Frame;
	if (CurrentFrame < 0) CurrentFrame = 0;
	if (CurrentFrame > TotalFrames - 1) CurrentFrame = TotalFrames - 1;
	SynchronizeProperties();
}

void UGsAnimatedImage::Play()
{
	if (!TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UGsAnimatedImage::TimerTick,
			1.0f / FramesPerSecond,
			true
		);
	}
}

void UGsAnimatedImage::Stop()
{
	if (TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		TimerHandle.Invalidate();
	}
}

void UGsAnimatedImage::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UTexture2D* Texture = Cast<UTexture2D>(Brush.GetResourceObject());
	if (Texture == nullptr) return;
	FVector2D TextureSize(Texture->GetSizeX(), Texture->GetSizeY());

	if (Brush.ImageSize.X == 0)
	{
		return;
	}

	int32 MaxColumns = FMath::FloorToInt(TextureSize.X / Brush.ImageSize.X);

	if (MaxColumns == 0)
	{
		return;
	}

	int32 Row = FMath::FloorToInt(CurrentFrame / MaxColumns);
	int32 Column = CurrentFrame % MaxColumns;

	FVector2D Min(Brush.ImageSize.X * Column, Brush.ImageSize.Y * Row);
	FVector2D Max = Min + Brush.ImageSize;
	FBox2D UVCoordinates(Min / TextureSize, Max / TextureSize);
	UVCoordinates.bIsValid = true;

	Brush.SetUVRegion(MoveTemp(UVCoordinates));
}

void UGsAnimatedImage::TimerTick()
{
	CurrentFrame++;
	if (CurrentFrame > TotalFrames - 1) CurrentFrame = 0;
	SynchronizeProperties();
}