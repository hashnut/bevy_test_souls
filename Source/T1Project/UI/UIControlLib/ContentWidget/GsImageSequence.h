// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Misc/Attribute.h"
#include "Styling/SlateBrush.h"
#include "Widgets/SWidget.h"
#include "Components/Widget.h"
#include "UObject/ScriptInterface.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "GsImageSequence.generated.h"

class SImageSequence;
class UTexture2D;

/*
 * SImageSequence를 UMG에서 편집하기 위해 만든 위젯
 */
UCLASS()
class UGsImageSequence : public UWidget
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsImageSequence")
	TArray<UTexture2D*> ImageList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsImageSequence")
	float IntervalSecond = 0.05f;

private:
	/** The ImageSequence widget managed by this object. */
	TSharedPtr<SImageSequence> MyImageSequence;

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface
};
