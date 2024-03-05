// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

/*
 * FGsUIHelper
 * 각종 UI 편의용 static 함수 등록
 */
struct T1PROJECT_API FGsUIStringHelper
{
public:

	// 3글자마다 콤마표시, InLimitAmount를 넘어가는 항목은 +로 표시
	// 예: InLimitAmount가 10000이면, InAmount는 10000부터 9,999+로 표시
	static void GetAmountText(uint64 InAmount, uint64 InLimitAmount, OUT FText& OutText);

	// 1000 단위의 콤마(,) 표시 안함. {0}만(한글), {0}K(영문) 표현. 내림 표현
	// 예: InAmount가 1500 -> 1500, 16889 -> 1.6만 (16K), 10000 -> 1만 (10K)
	static void GetItemAmountShortString(uint64 InAmount, OUT FString& OutString);

	// 1000 단위의 콤마(,) 표시 안함. {0}만(한글), {0}K(영문) 표현. 내림 표현
	// 예: InAmount가 1500 -> 1500, 16889 -> 1.6만 (16K), 10000 -> 1만 (10K)
	static void GetItemAmountShortText(uint64 InAmount, OUT FText& OutText);

	// 원하는 위치에 스트링을 끊고 "..." 삽입
	static void GetEllipsisText(int inSize, IN const FText& inText, OUT FText& OutText);

	// 소수 자릿수 표기 제어(값, 소수점 최소 자릿수, 최대 자릿수, 리턴받을 Text)
	static void GetFormatedFractionalDigitsText(float InValue, int32 InMinFractionalDigits, int32 InMaxFractionalDigits, 
		OUT FText& OutText);

	static void GetFormatedFractionalDigitsTextD(double InValue, int32 InMinFractionalDigits, int32 InMaxFractionalDigits,
		OUT FText& OutText);

	static void GetPatchSizeText(float inSize, OUT FString& OutString);

	// "LeftValue/RightValue" 반환. 인벤토리 보유 수량 등 사용 (예: 22/100)
	static void GetTwoValueSlashedText(int64 InLeftValue, int64 InRightValue, OUT FText& OutText);

	// 입력된 문자열에 길이제한과 개행제거 옵션을 적용해 변경된 텍스트 및 변경 여부를 반환한다.
	// InLength 가 0 일 경우 길이제한이 없는 것으로 판단한다.
	// Return : 텍스트 변경 여부
	static bool GetFormatedEditableText(const FString& InString, OUT FString& OutString, int32 InMaxLength = 0, bool InIsMultiLine = false, bool InIsNameType = false, bool InIsSimpleCheck = false);

	template<typename... TArgTypes>
	static FText MakeFormatedText(const FName InKeyName, TArgTypes... Args)
	{
		TMap<FName, FText> TableSimul({
			{ TEXT("Lv.{0}"), NSLOCTEXT("mytemp", "mykey", "Lv.{0}") },
			{ TEXT("{0} %"), NSLOCTEXT("mytemp", "mykey2", "{0} %") }
		});

		const FText& FormatText = TableSimul.FindRef(InKeyName);
		return FText::Format(FormatText, Forward<TArgTypes>(Args)...);
	}
};
