// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCharacterSandbag.h"
#include "Data/GsDataContainManager.h"

// Sets default values
AGsCharacterSandbag::AGsCharacterSandbag() : Super()
{
}

void AGsCharacterSandbag::BeginPlay()
{
	Super::BeginPlay();

	FTransform trans = GetMesh()->GetSocketTransform(TEXT("OverHeadSocket"), RTS_Component);

	// 퀘스트 타겟 마크
	if (_questTargetMark)
	{
		_questTargetMark->SetRelativeLocation(trans.GetTranslation() +
			GData()->GetGlobalData()->_interactionButtonPosOffset);
	}
}