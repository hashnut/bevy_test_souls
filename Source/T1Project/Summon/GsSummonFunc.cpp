#include "GsSummonFunc.h"

#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsFairyManager.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Costume/GsCostumeSummonConfirmData.h"

#include "T1Project.h"

// 소환 대기 카운트 넘어갔는지 체크
bool FGsSummonFunc::CheckSummonWaitCount(EGsSummonType In_summonType)
{
	bool isNotOverCount = false;
	if (In_summonType == EGsSummonType::SUMMON_COSTUME)
	{
		isNotOverCount = CheckCostumeSummonWaitCount();
	}
	else
	{
		isNotOverCount = CheckFairySummonWaitCount();
	}
	return isNotOverCount;
}

// 페어리 소환 확정 대기 카운트 체크
bool FGsSummonFunc::CheckFairySummonWaitCount(const bool InShowPopup, const bool InCheckOnly)
{
	const TArray<TSharedPtr<FGsFairySummonConfirmData>>& listItem =
		GSFairy()->GetFairySummonWaitingList();

	// 최대 갯수랑 같거나 넘어가면 티커 출력
	if (GData()->GetGlobalData()->_summonFairyWaitMaxCount <=
		listItem.Num())
	{
		if (false == InCheckOnly)
		{
			if (false == InShowPopup)
			{
				FText findText;
				FText::FindText(TEXT("FairyText"), TEXT("Alert_SummonConfirm_ListOver"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
			}
			else
			{
				FText findText;
				FText::FindText(TEXT("UICommonText"), TEXT("SummonFixSlot_Full"), findText);
				FGsUIHelper::PopupOK(findText);
			}
		}
		
		return false;
	}

	return true;
}
// 코스튬 소환 확정 대기 카운트 체크
bool FGsSummonFunc::CheckCostumeSummonWaitCount(const bool InShowPopup, const bool InCheckOnly)
{
	const TArray<TSharedPtr<FGsCostumeSummonConfirmData>>& listItem =
		GSCostume()->GetSummonWaitingList();

	// 최대 갯수랑 같거나 넘어가면 티커 출력
	if (GData()->GetGlobalData()->_summonCostumeWaitMaxCount <=
		listItem.Num())
	{
		if (false == InCheckOnly)
		{
			if (false == InShowPopup)
			{
				FText findText;
				FText::FindText(TEXT("CostumeText"), TEXT("Alert_SummonConfirm_ListOver"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
			}
			else
			{
				FText findText;
				FText::FindText(TEXT("UICommonText"), TEXT("SummonFixSlot_Full"), findText);
				FGsUIHelper::PopupOK(findText);
			}
		}
		
		return false;
	}

	return true;
}

// 페어리 등급 텍스트 구하기
FText FGsSummonFunc::GetFairyGradeText(FairyGrade In_fairyGrade)
{
	FText findText;
	FString gradeKey;
	switch (In_fairyGrade)
	{
	case FairyGrade::NORMAL:
	{
		gradeKey = TEXT("Grade_Normal");
		break;
	}
	case FairyGrade::MAGIC:
	{
		gradeKey = TEXT("Grade_Magic");
		break;
	}
	case FairyGrade::RARE:
	{
		gradeKey = TEXT("Grade_Rare");
		break;
	}
	case FairyGrade::EPIC:
	{
		gradeKey = TEXT("Grade_Epic");
		break;
	}
	case FairyGrade::UNIQUE:
	{
		gradeKey = TEXT("Grade_Unique");
		break;
	}
	case FairyGrade::LEGEND:
	{
		gradeKey = TEXT("Grade_Legend");
		break;
	}
	}

	if (!gradeKey.IsEmpty())
	{
		FText::FindText(TEXT("ItemUIText"), gradeKey, findText);
	}

	return findText;
}

// 코스튬 등급 텍스트 구하기
FText FGsSummonFunc::GetCostumeGradeText(CostumeGrade In_costumeGrade)
{
	FText findText;
	FString gradeKey;
	switch (In_costumeGrade)
	{
	case CostumeGrade::NORMAL:
	{
		gradeKey = TEXT("Grade_Normal");
		break;
	}
	case CostumeGrade::MAGIC:
	{
		gradeKey = TEXT("Grade_Magic");
		break;
	}
	case CostumeGrade::RARE:
	{
		gradeKey = TEXT("Grade_Rare");
		break;
	}
	case CostumeGrade::EPIC:
	{
		gradeKey = TEXT("Grade_Epic");
		break;
	}
	case CostumeGrade::UNIQUE:
	{
		gradeKey = TEXT("Grade_Unique");
		break;
	}
	case CostumeGrade::LEGEND:
	{
		gradeKey = TEXT("Grade_Legend");
		break;
	}
	}

	if (!gradeKey.IsEmpty())
	{
		FText::FindText(TEXT("ItemUIText"), gradeKey, findText);
	}

	return findText;
}

//https://jira.com2us.com/jira/browse/C2URWQ-5176
// 소환 stack 갯수(9999)개 넘어 갔는지 체크
bool FGsSummonFunc::CheckSummonStackCount(EGsSummonType In_summonType)
{
	bool isNotOverCount = false;
	if (In_summonType == EGsSummonType::SUMMON_COSTUME)
	{
		isNotOverCount = CheckCostumeSummonStackCount();	
	}
	else
	{
		isNotOverCount = CheckFairySummonStackCount();
		
	}
	return isNotOverCount;
}

// 페어리 소환 stack 갯수(9999)개 넘어 갔는지 체크
bool FGsSummonFunc::CheckFairySummonStackCount(const bool InShowPopup, const bool InCheckOnly)
{
	bool isNotOverCount = GSFairy()->IsEnableStackFairy();
	if (isNotOverCount == false && InCheckOnly == false)
	{	
		if (false == InShowPopup)
		{
			FText findText;
			FText::FindText(TEXT("FairyText"), TEXT("Alert_Ticker_StackFull"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		else
		{
			FText findText;
			FText::FindText(TEXT("FairyText"), TEXT("Alert_Ticker_StackFull"), findText);
			FGsUIHelper::PopupOK(findText);
		}
	}

	return isNotOverCount;
}
// 코스튬 소환 stack 갯수(9999)개 넘어 갔는지 체크
bool FGsSummonFunc::CheckCostumeSummonStackCount(const bool InShowPopup, const bool InCheckOnly)
{
	bool isNotOverCount = GSCostume()->IsEnableStackCostume();
	if(isNotOverCount == false && InCheckOnly == false)
	{ 
		if (false == InShowPopup)
		{
			FText findText;
			FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_StackFull"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		else
		{
			FText findText;
			FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_StackFull"), findText);
			FGsUIHelper::PopupOK(findText);
		}
	}

	return isNotOverCount;
}