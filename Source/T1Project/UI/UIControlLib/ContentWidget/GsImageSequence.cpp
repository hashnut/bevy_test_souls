// Copyright Epic Games, Inc. All Rights Reserved.

#include "GsImageSequence.h"
#include "UI/UIControlLib/Slate/SImageSequence.h"

#define LOCTEXT_NAMESPACE "UMG"

UGsImageSequence::UGsImageSequence(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UGsImageSequence::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UGsImageSequence::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyImageSequence.Reset();
}

TSharedRef<SWidget> UGsImageSequence::RebuildWidget()
{
	MyImageSequence = SNew(SImageSequence, ImageList, IntervalSecond);

	return MyImageSequence.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE
