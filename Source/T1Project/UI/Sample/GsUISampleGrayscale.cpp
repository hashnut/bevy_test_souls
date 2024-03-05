// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISampleGrayscale.h"
#include "UI/UIControlLib/ContentWidget/GsGrayscaleImage.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "PaperSprite.h"


void UGsUISampleGrayscale::OnClickSetImage()
{
	if (UPaperSprite* sprite0 = FGsUIHelper::LoadSpriteFromPath(_path0))
	{
		_imgTest0->SetPaperSprite(sprite0);
	}

	if (UPaperSprite* sprite1 = FGsUIHelper::LoadSpriteFromPath(_path1))
	{
		_imgTest1->SetPaperSprite(sprite1);
	}
}

void UGsUISampleGrayscale::OnClickGrayscale0(bool bInIsGray)
{
	_imgTest0->SetGrayscale(bInIsGray);
}

void UGsUISampleGrayscale::OnClickGrayscale1(bool bInIsGray)
{
	_imgTest1->SetGrayscale(bInIsGray);
}