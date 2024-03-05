// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIUserProfileStatEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


void UGsUIUserProfileStatEntry::SetData(const FSoftObjectPath& InIconPath, const FText& InStatName, const FText& InStatValue)
{
	// 아이콘 설정
	TWeakObjectPtr<UImage> weakThis(_imageStatIcon);
	FGsUIHelper::AsyncLoadSpriteFromPath(InIconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			// 로드 끝난후라 LoadSpriteFromPath 시 새로 로드하지 않고, 로드된 항목을 찾음
			if (UImage* strongThis = weakThis.Get())
			{
				strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
			}
		}));

	_textBlockStatName->SetText(InStatName);
	_textBlockStatValue->SetText(InStatValue);
}


