// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRankingRewardListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Paper2D/Classes/PaperSprite.h"

#include "UI/UIContent/Helper/GsUIHelper.h"


void UGsUIRankingRewardListEntry::SetData(FText InGradeText, FText InRewardText, const FSoftObjectPath* InIconPath)
{
	_textBlockGrade->SetText(InGradeText);
	_textBlockReward->SetText(InRewardText);
	
	if (_imageBadge)
	{
		_imageBadge->SetVisibility(ESlateVisibility::Collapsed);
		if (nullptr != InIconPath)
		{
			TWeakObjectPtr<UImage> weakThis(_imageBadge);
			FGsUIHelper::AsyncLoadSpriteFromPath(*InIconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
				{
					if (UImage* strongThis = weakThis.Get())
					{
						if (UPaperSprite* badgeSprite = Cast<UPaperSprite>(inRes))
						{
							strongThis->SetBrushFromAtlasInterface(badgeSprite);
							strongThis->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						}
					}
				}));
		}
	}
}