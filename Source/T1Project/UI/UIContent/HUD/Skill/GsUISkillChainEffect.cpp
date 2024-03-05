// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillChainEffect.h"
#include "Components/RetainerBox.h"
#include "Components/Image.h"
#include "Skill/GsSkill.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Data/GsResourceManager.h"
#include "Materials/MaterialInstanceDynamic.h"


void UGsUISkillChainEffect::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (0 < _remainTime)
	{
		_remainTime -= InDeltaTime;
		if (0 >= _remainTime)
		{
			OnFinishChainAnimation();
		}
	}

}

void UGsUISkillChainEffect::SetData(const FGsSkill* InSkill, const FGsSkill* InNextSkill, bool bInIsEnabled)
{
	if (nullptr == InSkill ||
		nullptr == InNextSkill)
	{
		return;
	}

	// 아이콘 세팅
	if (UGsResourceManager* resMgr = GResource())
	{
		if (UPaperSprite* icon1 = resMgr->LoadSync<UPaperSprite>(*InSkill->GetIconPath()))
		{
			SetIconImageSprite(icon1, _imgIcon1);
			SetGrayScale(bInIsEnabled, _imgIcon1);
		}

		if (UPaperSprite* icon2 = resMgr->LoadSync<UPaperSprite>(*InNextSkill->GetIconPath()))
		{
			SetIconImageSprite(icon2, _imgIcon2);
			SetGrayScale(bInIsEnabled, _imgIcon2);
		}
	}

	// 애니메이션 시작
	_remainTime = _animTime;
	PlayChainAnimation();
}

bool UGsUISkillChainEffect::IsPlayingEffect() const
{
	if (ESlateVisibility::HitTestInvisible == GetVisibility() ||
		ESlateVisibility::SelfHitTestInvisible == GetVisibility())
	{
		if (0 < _remainTime)
		{
			return true;
		}
	}

	return false;
}

void UGsUISkillChainEffect::OnFinishChainAnimation()
{
	_remainTime = 0.f;

	OnFinishAnimation.ExecuteIfBound();
}

void UGsUISkillChainEffect::SetIconImageSprite(UPaperSprite* InSprite, OUT UImage* OutImage)
{
	if (InSprite)
	{
		if (UMaterialInstanceDynamic* mat = OutImage->GetDynamicMaterial())
		{
			FSlateAtlasData atlasData = InSprite->GetSlateAtlasData();
			mat->SetTextureParameterValue(TEXT("Texture"), atlasData.AtlasTexture);
			mat->SetScalarParameterValue(TEXT("OffsetU"), atlasData.StartUV.X);
			mat->SetScalarParameterValue(TEXT("OffsetV"), atlasData.StartUV.Y);
			mat->SetScalarParameterValue(TEXT("TilingU"), atlasData.SizeUV.X);
			mat->SetScalarParameterValue(TEXT("TilingV"), atlasData.SizeUV.Y);
		}
	}
}

void UGsUISkillChainEffect::SetGrayScale(bool bInIsGray, OUT UImage* OutImage)
{
	OutImage->SetColorAndOpacity((bInIsGray) ?
		FLinearColor(_grayDarkness, _grayDarkness, _grayDarkness, _grayOpacity) : FLinearColor::White);

	if (UMaterialInstanceDynamic* mat = OutImage->GetDynamicMaterial())
	{
		mat->SetScalarParameterValue(TEXT("GrayDensity"), (bInIsGray) ? 1.f : 0.f);
	}
}
