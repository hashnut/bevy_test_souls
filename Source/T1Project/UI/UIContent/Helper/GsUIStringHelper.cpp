// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIStringHelper.h"
#include "Regex.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "UTIL/GsText.h"


void FGsUIStringHelper::GetAmountText(uint64 InAmount, uint64 InLimitAmount, OUT FText& OutText)
{
	if (0 == InAmount ||
		0 >= InLimitAmount)
	{
		OutText = FText::AsNumber(InAmount);
		return;
	}

	if (InLimitAmount > InAmount)
	{
		OutText = FText::AsNumber(InAmount);
		return;
	}

	FString strAmount = FText::AsNumber(InLimitAmount - 1).ToString();
	OutText = FText::FromString(FString::Format(TEXT("{0}+"), {*strAmount}));
}

void FGsUIStringHelper::GetItemAmountShortString(uint64 InAmount, OUT FString& OutString)
{
	const uint64 THOUSAND = 1000;		// 1,000
	const uint64 MILLION = 1000000;		// 1,000,000

	FString strValue;
	FString strUnit;

	if (InAmount < 10 * THOUSAND)
	{
		strValue = FString::Printf(TEXT("%llu"), InAmount);
		strUnit.Empty();
	}
	else
	{
		// 텍스트 언어 확인
		bool isKorean = true;
		if (UGsLocalizationManager* localizationMgr = GLocalization())
		{
			isKorean = localizationMgr->IsCurrentCulture(TEXT("ko"));
		}

		// 단위 설정
		uint64 unitValue = 1;
		FTextKey unitTextKey;
		if (isKorean)
		{
			if (InAmount < 100 * MILLION)
			{	// 억보다 작을 경우
				unitValue = THOUSAND;
				unitTextKey = FTextKey(TEXT("ItemUi_Stack_Unit_Korean"));
			}
			else
			{	// 억보다 클 경우
				unitValue = 10 * MILLION;
				unitTextKey = FTextKey(TEXT("ItemUi_Stack_Unit_2_Korean"));
			}
		}
		else
		{
			unitValue = THOUSAND;
			unitTextKey = FTextKey(TEXT("ItemUi_Stack_Unit_English"));
		}

		// 값 계산
		uint64 tenUnitDiv = InAmount / unitValue;
		if (isKorean)
		{
			uint64 div = tenUnitDiv / 10;
			uint64 mod = tenUnitDiv % 10;
			// 한글 표기 값 : 소수점 한자리 표기. 1000단위에서는 쉼표사용하지 않는다.
			if (mod == 0 || InAmount >= unitValue * 10000)
			{
				strValue = FString::Printf(TEXT("%llu"), div);
			}
			else
			{
				strValue = FString::Printf(TEXT("%llu.%llu"), div, mod);
			}
		}
		else
		{
			// 영어 표기 값 : 단위로 나눈 몫
			strValue = FString::Printf(TEXT("%llu"), tenUnitDiv);
		}

		FText findText;
		FText::FindText(TEXT("ItemUIText"), unitTextKey, findText);
		strUnit = findText.ToString();
	}

	OutString = strValue + strUnit;
}

void FGsUIStringHelper::GetItemAmountShortText(uint64 InAmount, OUT FText& OutText)
{
	FString outString;
	GetItemAmountShortString(InAmount, outString);
	OutText = FText::FromString(outString);
}

void FGsUIStringHelper::GetEllipsisText(int inSize, IN const FText& inText, OUT FText& OutText)
{
	FString text = inText.ToString();
	FString strPattrn = TEXT("[^0-9a-zA-Z\\s]");
	FRegexPattern pattern(strPattrn);

	int32 count = 0;
	for (int32 i = 0; i < text.Len(); ++i)
	{
		FString tempCh;
		tempCh.AppendChar(text[i]);
		FRegexMatcher matcher(pattern, tempCh);
		if (matcher.FindNext())
		{
			count += 2;
		}
		else
		{
			count += 1;
		}

		if (inSize < count)
		{
			OutText = FText::FromString(text.Left(i - 1) + TEXT("..."));
			return;
		}
	}

	OutText = FText::FromString(text);
}

void FGsUIStringHelper::GetFormatedFractionalDigitsText(float InValue, int32 InMinFractionalDigits, int32 InMaxFractionalDigits, 
	OUT FText& OutText)
{
	FNumberFormattingOptions formatOption = FNumberFormattingOptions()
		.SetUseGrouping(true)
		.SetMinimumFractionalDigits(InMinFractionalDigits)
		.SetMaximumFractionalDigits(InMaxFractionalDigits);	

	OutText = FText::AsNumber(InValue, &formatOption);
}

void FGsUIStringHelper::GetFormatedFractionalDigitsTextD(double InValue, int32 InMinFractionalDigits, int32 InMaxFractionalDigits,
	OUT FText& OutText)
{
	FNumberFormattingOptions formatOption = FNumberFormattingOptions()
		.SetUseGrouping(true)
		.SetMinimumFractionalDigits(InMinFractionalDigits)
		.SetMaximumFractionalDigits(InMaxFractionalDigits);

	OutText = FText::AsNumber(InValue, &formatOption);
}

void FGsUIStringHelper::GetPatchSizeText(float inSize, OUT FString& OutString)
{
	float tb = 1099511627776;
	float gb = 1073741824;
	float mb = 1048576;
	float kb = 1024;

	if (inSize >= tb)
		OutString = FString::Printf(TEXT("%.2f MB"), (inSize / tb));
	else if (inSize >= gb && inSize < tb)
		OutString = FString::Printf(TEXT("%.2f GB"), (inSize / gb));
	else if (inSize >= mb && inSize < gb)
		OutString = FString::Printf(TEXT("%.2f MB"), (inSize / mb));
	else if (inSize >= kb && inSize < mb)
		OutString = FString::Printf(TEXT("%.2f KB"), (inSize / kb));
	else if (inSize < kb)
		OutString = FString::Printf(TEXT("%.2f Bytes"), inSize);
	else
		OutString = FString::Printf(TEXT("%.2f Bytes"), inSize);
}

void FGsUIStringHelper::GetTwoValueSlashedText(int64 InLeftValue, int64 InRightValue, OUT FText& OutText)
{
	FText slashText;
	FText::FindText(TEXT("MarkFormatingText"), TEXT("Slash"), slashText);

	FString leftStr = FString::FromInt(InLeftValue);
	FString rightStr = FString::FromInt(InRightValue);
	FString resultStr = leftStr + slashText.ToString() + rightStr;

	OutText = FText::FromString(resultStr);
}

bool FGsUIStringHelper::GetFormatedEditableText(const FString& InString, OUT FString& OutString, int32 InMaxLength /*= 0*/, bool InIsMultiLine /*= false*/, bool InIsNameType /*= false*/, bool InIsSimpleCheck /*= false*/)
{
	FString changedText = InString;

	// 개행제한 적용
	bool isTextChanged = false;
	if (false == InIsMultiLine)
	{
		if ((INDEX_NONE != InString.Find(TEXT("\n"))) ||
			(INDEX_NONE != InString.Find(TEXT("\r"))))
		{
			changedText = InString.Replace(TEXT("\n"), TEXT(""));
			changedText = changedText.Replace(TEXT("\r"), TEXT(""));
			isTextChanged = true;
		}
	}

	// 길이제한 적용
	if (InMaxLength > 0)
	{
		int32 length = InIsNameType ? FGsTextUtil::GetStringLengthNameType(changedText) : InIsSimpleCheck ? changedText.Len() : FGsTextUtil::GetStringLengthRegex(changedText);
		if (InMaxLength < length)
		{
			changedText.RemoveAt(changedText.Len() - 1, length - InMaxLength);
			isTextChanged = true;
		}
	}

	OutString = changedText;
	return isTextChanged;
}
