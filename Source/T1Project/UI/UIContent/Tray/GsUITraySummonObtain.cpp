#include "GsUITraySummonObtain.h"

#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsSummonManager.h"

#include "Fairy/GsFairyData.h"

#include "Costume/GsCostumeData.h"

#include "Summon/GsSummonCardData.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

#include "Components/Image.h"

#include "Paper2D/Classes/PaperSprite.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Animation/WidgetAnimation.h"

#include "Core/Public/Math/Color.h"



// 데이터 세팅
void UGsUITraySummonObtain::SetData(EGsSummonType In_type, int In_tblId)
{
	if (In_type == EGsSummonType::SUMMON_FAIRY)
	{
		const TWeakPtr<FGsFairyData> findData = GSFairy()->GetFairyData((FairyId)In_tblId);
		if (findData.IsValid() == false)
		{
			return;
		}

		TSharedPtr<FGsFairyData> data = findData.Pin();

		// 이름 세팅
		_textFairyName = data->GetFairyName();

		// 초상화 이미지 세팅
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(data->GetFairyPortraitPath());
		_imgPortrait->SetBrushFromAtlasInterface(spriteImg);

		FairyGrade fairyGrade = data->GetFairyGrade();

		// bg 색상
		FLinearColor bgColor = FGsUIColorHelper::GetColorSummonFairyGradeBG(fairyGrade);
		_imgBg->SetColorAndOpacity(bgColor);
		_imgBackGradeEffect->SetColorAndOpacity(bgColor);
		_imgFrontGradeEffect->SetColorAndOpacity(bgColor);


		// text 색상
		_textBlockFairyName->SetColorAndOpacity(FGsUIColorHelper::GetColorSummonFairyGradeText(fairyGrade));
	}
	else
	{
		const TWeakPtr<FGsCostumeData> findData = GSCostume()->GetCostumeData((CostumeId)In_tblId);
		if (findData.IsValid() == false)
		{
			return;
		}

		TSharedPtr<FGsCostumeData> data = findData.Pin();

		_textFairyName = data->GetCostumeName();

		FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
		if (summonHandler == nullptr)
		{
			return;
		}

		CreatureGenderType localGender = summonHandler->GetLocalGender();

		// 초상화 이미지 세팅
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(data->GetCostumePortraitPath(localGender));
		_imgPortrait->SetBrushFromAtlasInterface(spriteImg);

		CostumeGrade costumeGrade = data->GetCostumeGrade();
		// bg 색상
		FLinearColor bgColor = FGsUIColorHelper::GetColorSummonCostumeGradeBG(costumeGrade);
		_imgBg->SetColorAndOpacity(bgColor);
		_imgBackGradeEffect->SetColorAndOpacity(bgColor);
		_imgFrontGradeEffect->SetColorAndOpacity(bgColor);

		// text 색상
		_textBlockFairyName->SetColorAndOpacity(FGsUIColorHelper::GetColorSummonCostumeGradeText(costumeGrade));
	}

	// 애니메이션 연출
	PlayAnimation(_animation);

	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	BindToAnimationFinished(_animation, FWidgetAnimationDynamicEvent(animationDelegate));
}


// 연출 끝
void UGsUITraySummonObtain::FinishAnimationBP()
{
	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, "FinishAnimationBP");

	UnbindFromAnimationFinished(_animation, FWidgetAnimationDynamicEvent(animationDelegate));
}