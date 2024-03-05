#include "GsUICostumeSkillIconItem.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
#include "Runtime/Engine/Public/Slate/SlateTextureAtlasInterface.h"
#include "PaperSprite.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "WidgetSwitcher.h"
#include "TextBlock.h"
#include "Image.h"


void UGsUICostumeSkillIconItem::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUICostumeSkillIconItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSkillIcon->OnClicked.AddDynamic(this, &UGsUICostumeSkillIconItem::OnClickSkillDetailInfo);
}

void UGsUICostumeSkillIconItem::OnClickSkillDetailInfo()
{
	if(_passivityId >= 0) _OnSelectedIcon.ExecuteIfBound(_isActiveSkill, _passivityId);
}

void UGsUICostumeSkillIconItem::SetData(bool activeSkill, int32 id, CreatureWeaponType weaponType, const FSoftObjectPath iconPath)
{
	_isActiveSkill = activeSkill;
	_passivityId = id;
	if (id != 0)
	{
		FText weaponTypeName = FText::GetEmpty();
		switch (weaponType)
		{
		case CreatureWeaponType::SWORD:		
			FText::FindText(TEXT("WeaponText"), TEXT("Sword"), weaponTypeName); 
			break;
		case CreatureWeaponType::DAGGER:	
			FText::FindText(TEXT("WeaponText"), TEXT("Dagger"), weaponTypeName);
			break;
		case CreatureWeaponType::BOW:		
			FText::FindText(TEXT("WeaponText"), TEXT("Bow"), weaponTypeName);
			break;
		case CreatureWeaponType::WAND:		
			FText::FindText(TEXT("WeaponText"), TEXT("Wand"), weaponTypeName);
			break;
		case CreatureWeaponType::HAND:	
			FText::FindText(TEXT("WeaponText"), TEXT("Hand"), weaponTypeName);
			break;
		case CreatureWeaponType::ORB:
			FText::FindText(TEXT("WeaponText"), TEXT("Orb"), weaponTypeName);
			break;
 		case CreatureWeaponType::SHOTGUN:
 			FText::FindText(TEXT("WeaponText"), TEXT("Shotgun"), weaponTypeName);
 			break;
		default:
			FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_PassiveSkill_Type_All"), weaponTypeName);
			break;
		}
		
		SetIconImageSprite(FGsUIHelper::LoadSpriteFromPath(iconPath));

		_textWeapon->SetText(weaponTypeName);

		_switcherSkillMark->SetActiveWidgetIndex(activeSkill ? 0 : 1);
	}

	_btnSkillIcon->SetIsEnabled(id != 0);
	_btnSkillIcon->SetVisibility(id != 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUICostumeSkillIconItem::SetIconImageSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		_skillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (UMaterialInstanceDynamic* mat = _skillIcon->GetDynamicMaterial())
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
		_skillIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}
