// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCinematicCaptionController.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Tray/GsUITrayCinematicCaption.h"
#include "Sequence/GsSequenceManager.h"


// Sets default values
AGsCinematicCaptionController::AGsCinematicCaptionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGsCinematicCaptionController::BeginPlay()
{
	Super::BeginPlay();
	
	OpenCaptionUI();
}

void AGsCinematicCaptionController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CloseCaptionUI();

	Super::EndPlay(EndPlayReason);
}

void AGsCinematicCaptionController::OnKeyFrameEvent(FString InTextKey, EGsCinematicCaptionType InType)
{
	if (_trayCaption.IsValid())
	{
		FText textCaption;
		FText::FindText(_strTextNamespace, InTextKey, textCaption);

		_trayCaption->SetCaption(textCaption, InType);
	}
}

void AGsCinematicCaptionController::StartSound()
{
	// R3: 사운드 싱크를 위해 플레이 후 1프레임에 키 이벤트를 받아 사운드 재생
	if (UGsSequencePlayer* sequencePlayer = GSequencePlayer())
	{
		sequencePlayer->StartSound();
	}
}

void AGsCinematicCaptionController::OpenCaptionUI()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayCinematicCaption"));
	if (widget.IsValid())
	{
		_trayCaption = Cast<UGsUITrayCinematicCaption>(widget.Get());
	}
}

void AGsCinematicCaptionController::CloseCaptionUI()
{
	if (_trayCaption.IsValid())
	{
		_trayCaption->Close();
		_trayCaption = nullptr;
	}
}
