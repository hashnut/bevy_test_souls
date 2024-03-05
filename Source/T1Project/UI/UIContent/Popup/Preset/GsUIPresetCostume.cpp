#include "GsUIPresetCostume.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Costume/GsCostumeData.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Paper2D/Classes/PaperSprite.h"


void UGsUIPresetCostume::SetCostume(CostumeId InCostumeId)
{
	UGsCostumeManager* costumeManager = GSCostume();
	if (nullptr == costumeManager)
	{
		return;
	}

	const TWeakPtr<FGsCostumeData> costumeData = costumeManager->GetCostumeData(InCostumeId);
	if (costumeData.IsValid())
	{
		// 코스튬 이미지
		CreatureGenderType genderType = CreatureGenderType::MAX;
		if (const FGsNetUserData* userData = GGameData()->GetUserData())
		{
			genderType = userData->mGender;
		}
		const FSoftObjectPath path = costumeData.Pin()->GetCostumePortraitPath(genderType);
		_imagePortrait->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(path));
		
		// 등급
		CostumeGrade grade = costumeData.Pin()->GetCostumeGrade();
		_switcherGrade->SetActiveWidgetIndex(GetGradeIndex(grade));

		// Equip 표시
		const TWeakPtr<FGsCostumeData> costumeDataEquip = costumeManager->GetWearOnCostumeData();
		if (costumeDataEquip.IsValid())
		{
			_panelEquip->SetVisibility((costumeDataEquip.Pin()->_id == InCostumeId) ?
				ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
}

int32 UGsUIPresetCostume::GetGradeIndex(CostumeGrade InGrade)
{
	switch (InGrade)
	{
	case CostumeGrade::NONE:	return 0;
	case CostumeGrade::NORMAL:	return 0;
	case CostumeGrade::MAGIC:	return 1;
	case CostumeGrade::RARE:	return 2;
	case CostumeGrade::EPIC:	return 3;
	case CostumeGrade::UNIQUE:	return 4;
	case CostumeGrade::LEGEND:	return 5;
	}
	return 0;
}
