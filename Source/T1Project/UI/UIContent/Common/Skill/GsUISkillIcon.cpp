// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillIcon.h"
#include "PaperSprite.h"
#include "Components/Image.h"
#include "Skill/GsSkill.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Materials/MaterialInstanceDynamic.h"


void UGsUISkillIcon::SetData(const FGsSkill* InSkill)
{
	if (nullptr == InSkill ||
		nullptr == InSkill->_userSkillSetData)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	_skillId = InSkill->GetSkillId();

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 이미지 적용 후 켜기 위해 꺼둔다
	_imgIcon->SetVisibility(ESlateVisibility::Hidden);

	// 아이콘 로딩
	FSoftObjectPath iconPath(InSkill->_userSkillSetData->icon);
	SkillId skillId = _skillId;

	TWeakObjectPtr<UGsUISkillIcon> weakThis(this);
	FGsUIHelper::AsyncLoadSpriteFromPath(iconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, skillId](UObject* inRes)
		{
			if (UGsUISkillIcon* strongThis = weakThis.Get())
			{
				// 아이콘 밀리는 문제 수정을 위한 테스트
				if (skillId == strongThis->GetSkillId())
				{
					// 로드 끝난후라 LoadSpriteFromPath 시 새로 로드하지 않고, 로드된 항목을 찾음
					strongThis->SetIconImageSprite(Cast<UPaperSprite>(inRes));
				}
			}
		}));
}

void UGsUISkillIcon::SetIconImageSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		_imgIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (UMaterialInstanceDynamic* mat = _imgIcon->GetDynamicMaterial())
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

void UGsUISkillIcon::SetGrayscale(bool bInIsGray)
{
	if (UMaterialInstanceDynamic* mat = _imgIcon->GetDynamicMaterial())
	{
		mat->SetScalarParameterValue(TEXT("GrayDensity"), (bInIsGray) ? 1.f : 0.f);
	}
}
