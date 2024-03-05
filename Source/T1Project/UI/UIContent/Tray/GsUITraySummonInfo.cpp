#include "GsUITraySummonInfo.h"

#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"

#include "Fairy/GsFairyData.h"
#include "Costume/GsCostumeData.h"

#include "Summon/GsSummonFunc.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

#include "Components/Image.h"

#include "Core/Public/Math/Color.h"

// 데이터 세팅
void UGsUITraySummonInfo::SetData(EGsSummonType In_type, int In_tblId)
{
	if (In_type == EGsSummonType::SUMMON_FAIRY)
	{
		const TWeakPtr<FGsFairyData> findData = GSFairy()->GetFairyData((FairyId)In_tblId);
		if (findData.IsValid() == false)
		{
			return;
		}

		TSharedPtr<FGsFairyData> data = findData.Pin();

		_textName = data->GetFairyName();

		FairyGrade fairyGrade = data->GetFairyGrade();

		FLinearColor bgColor = FGsUIColorHelper::GetColorSummonFairyGradeBG(fairyGrade);
		_imgGradeBg->SetColorAndOpacity(bgColor);


		FText gradeText = FGsSummonFunc::GetFairyGradeText(fairyGrade);
		_textGrade = gradeText;
	}
	else if (In_type == EGsSummonType::SUMMON_COSTUME)
	{
		const TWeakPtr<FGsCostumeData> findData = GSCostume()->GetCostumeData((CostumeId)In_tblId);
		if (findData.IsValid() == false)
		{
			return;
		}
		TSharedPtr<FGsCostumeData> data = findData.Pin();

		_textName = data->GetCostumeName();

		CostumeGrade costumeGrade = data->GetCostumeGrade();

		FLinearColor bgColor = FGsUIColorHelper::GetColorSummonCostumeGradeBG(costumeGrade);
		_imgGradeBg->SetColorAndOpacity(bgColor);


		FText gradeText = FGsSummonFunc::GetCostumeGradeText(costumeGrade);
		_textGrade = gradeText;
	}
}