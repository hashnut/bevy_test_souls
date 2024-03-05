// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconDefault.h"
#include "Components/Image.h"
#include "Classes/PaperSprite.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "T1Project.h"


void UGsUIIconDefault::SetIconImage(const FSoftObjectPath& InPath)
{
	// 로딩되기 전에는 이미지를 가리기 위함
	SetIconImageSprite(nullptr);

	_strPath = InPath.ToString();
	FString tempPath = InPath.ToString();

	// 로드되는 사이 이 객체가 어떻게 될지 모르므로 TWeakObjectPtr 사용
	TWeakObjectPtr<UGsUIIconDefault> weakThis(this);
	FGsUIHelper::AsyncLoadSpriteFromPath(InPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, tempPath](UObject* inRes)
		{
			if (UGsUIIconDefault* StrongThis = weakThis.Get())
			{
				if (StrongThis->IsSamePath(tempPath))
				{
					// 로드 끝난후라 LoadSpriteFromPath 시 새로 로드하지 않고, 로드된 항목을 찾음
					StrongThis->SetIconImageSprite(Cast<UPaperSprite>(inRes));
				}
			}
		}));
}

void UGsUIIconDefault::SetIconImageColor(const FLinearColor& InColor)
{
	_iconImage->SetColorAndOpacity(InColor);
}

void UGsUIIconDefault::SetIconImageSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		_iconImage->SetBrushFromAtlasInterface(InSprite);
		_iconImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_iconImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UGsUIIconDefault::IsSamePath(const FString& InPath)
{
	if (InPath == _strPath)
	{
		return true;
	}
	
	return false;
}
