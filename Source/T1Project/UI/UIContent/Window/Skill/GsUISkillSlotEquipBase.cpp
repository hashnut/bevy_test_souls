// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillSlotEquipBase.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Skill/GsSkill.h"
#include "Materials/MaterialInstanceDynamic.h"


void UGsUISkillSlotEquipBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUISkillSlotEquipBase::OnClickSlot);
}

void UGsUISkillSlotEquipBase::NativeDestruct()
{
	_skill = nullptr;

	Super::NativeDestruct();
}

void UGsUISkillSlotEquipBase::InitializeSlot(SkillSlotId InSlotId)
{
	_slotId = InSlotId;
}

bool UGsUISkillSlotEquipBase::SetData(const FGsSkill* InData)
{
	if (nullptr == InData || 
		nullptr == InData->_userSkillSetData)
	{
		SetEmpty();
		return false;
	}

	_skill = InData;

	// 아이콘 로딩
	FSoftObjectPath iconPath(InData->_userSkillSetData->icon);
	
	SkillId skillId = _skill->GetSkillId();
	TWeakObjectPtr<UGsUISkillSlotEquipBase> weakThis(this);
	FGsUIHelper::AsyncLoadSpriteFromPath(iconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, skillId](UObject* inRes)
		{
			if (UGsUISkillSlotEquipBase* strongThis = weakThis.Get())
			{
				if (skillId == strongThis->GetSkillId())
				{
					strongThis->SetIconImageSprite(Cast<UPaperSprite>(inRes));
				}
			}
		}));

	return true;
}

void UGsUISkillSlotEquipBase::SetEmpty()
{
	_skill = nullptr;
	_imgSkill->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISkillSlotEquipBase::SetIconImageSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		_imgSkill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (UMaterialInstanceDynamic* mat = _imgSkill->GetDynamicMaterial())
		{
			FSlateAtlasData atlasData = InSprite->GetSlateAtlasData();
			mat->SetTextureParameterValue(TEXT("Texture"), atlasData.AtlasTexture);
			mat->SetScalarParameterValue(TEXT("OffsetU"), atlasData.StartUV.X);
			mat->SetScalarParameterValue(TEXT("OffsetV"), atlasData.StartUV.Y);
			mat->SetScalarParameterValue(TEXT("TilingU"), atlasData.SizeUV.X);
			mat->SetScalarParameterValue(TEXT("TilingV"), atlasData.SizeUV.Y);
		}
	}
	else
	{
		_imgSkill->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUISkillSlotEquipBase::SetIconDimmed(bool bIsDimmed)
{
	if (UMaterialInstanceDynamic* mat = _imgSkill->GetDynamicMaterial())
	{
		mat->SetScalarParameterValue(TEXT("GrayDensity"), (bIsDimmed) ? 1.f : 0.f);
	}
}

bool UGsUISkillSlotEquipBase::IsEmpty() const
{
	return (nullptr == _skill) ? true : false;
}

SkillId UGsUISkillSlotEquipBase::GetSkillId() const
{
	return _skill ? _skill->GetSkillId() : INVALID_SKILL_ID;
}
