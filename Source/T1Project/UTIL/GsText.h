#pragma once
#include <string>
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
/*
	문자열 변환에 관련된 처리를 모아놓은 유틸
	관련 코드는 여기에서 확장바랍니다.
*/

namespace
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 허용되는 유저 이름 검증 등을 위한 상수들
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static constexpr wchar_t MIN_WCHAR_NUMBER = 48;					// 숫자 시작 값
	static constexpr wchar_t MAX_WCHAR_NUMBER = 57;					// 숫자 끝 값

	static constexpr wchar_t MIN_WCHAR_ENGLISH_LOWER_CASE = 65;		// 영문 소문자 시작 값
	static constexpr wchar_t MAX_WCHAR_ENGLISH_LOWER_CASE = 90;		// 영문 소문자 끝 값

	static constexpr wchar_t MIN_WCHAR_ENGLISH_UPPER_CASE = 97;		// 영문 대문자 시작 값
	static constexpr wchar_t MAX_WCHAR_ENGLISH_UPPER_CASE = 122;	// 영문 대문자 끝 값

	static constexpr wchar_t MIN_WCHAR_KOREAN = 44032;				// 한글 시작 값
	static constexpr wchar_t MAX_WCHAR_KOREAN = 55203;				// 한글 끝 값
}


enum class BannedWordCheckUsage : uint8 { all, sentence, name };

class FGsGameObjectData;

class FGsTextUtil
{
	// Example usage
	//GetEnumValueAsString<EVictoryEnum>("EVictoryEnum", VictoryEnum)));
public:

	//반환 값 InvadeDropType::ATTACK 값을 변환한다면 InvadeDropType::ATTACK이 나옴
	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!enumPtr)
		{
			return FString("Invalid");
		}
		return enumPtr->GetNameByValue((int64)Value).ToString();
	}

	//반환 값 InvadeDropType::ATTACK 값을 변환한다면 ATTACK 만 나옴
	template<typename TEnum>
	static FORCEINLINE FString GetStringFromEnum(const FString& Name, TEnum Value)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);

		if (!enumPtr)
		{
			return FString("Invalid");
		}

		return enumPtr->GetNameStringByIndex((int32)Value);
	}

	static FORCEINLINE std::string FString_TO_string(const FString& fstring)
	{
		return std::string(TCHAR_TO_UTF8(*fstring));
	}
	
	static FORCEINLINE FString string_TO_FString(const std::string& stdstring)
	{
		return FString(stdstring.c_str());
	}

	// 영문, 숫자, 공백 제외한 문자를 모두 2자로 계산
	static int32 GetStringLengthRegex(const FString& InString);
	// B1 : 닉네임, 기사단명이 길어지는 현상 때문에 영어 숫자 상관없이 2로 처리 요청(CHR-18925)
	static int32 GetStringLengthNameType(const FString& InString);

	// 이름에 쓰는 항목들 체크. 영문, 숫자, 한글만 가능, 조합중인 한글(자음만, 모음만 존재할 경우)불가
	static bool IsValidNameType(const FString& InString);
	// 금칙어가 있는지 체크
	static bool IsBanWord(const FString& InString, BannedWordCheckUsage usage, BannedWordCheckCallBack callback);
	static bool IsBanWordInWordList(TArray<FString>& InStringList, BannedWordCheckUsage usage, BannedWordCheckCallBack callback);
	// 금칙어 ***변환
	static void ChangeBanSentence(const FString& InString, BannedWordCheckCallBack callback);

	// get target name(+inter server name)
	static FString GetTargetName(FGsGameObjectData* In_targetObjectData, EGsGameObjectType In_type);

	
	// 2023/8/30 PKT - https://jira.com2us.com/jira/browse/CHR-21412
	//				  Inter Server 관련 이름 검색 고도화 작업( 특문 포함 및 이름 검색 길이 수정 등 )
	//				  현재 이름 검색(친구/PKBook/파티 등)에 대한 구현이 여러 곳에 산개되어 있어 일괄 처리를 위해 제공
	enum class EUserNameValid { Valid, InvalidLength, InvalidFormat, InvalidWorldId};
	static const TTuple<EUserNameValid,WorldId,FText,FText> IsValidUserSearchNameOnly(const FString& InName, bool IsPrintMsg = true);	
	
	// 2023/8/30 PKT - 유저 이름 검색 시 설정한 최대 길이를 넘지 않도록 검사
	static EUserNameValid CheckedUserNameLengthLimit(const FString& InName, FString& OutName, const int32 InMaxLength = MAX_CHAR_SERVER_NAME_LEN);

	// 2023/9/4 PKT - 유저(Prefix) 
	static FString CombineUserAndPrefix(const FText& InUserName, const WorldId InWorldId = INVALID_WORLD_ID, bool InPrefixForce = false);
};

#define GetEnumToString(EnumClassName, ValueOfEnum) FGsTextUtil::GetEnumValueAsString<EnumClassName>(FString(TEXT(#EnumClassName)), (ValueOfEnum))
#define TOstring(Fstring) FGsTextUtil::FString_TO_string(Fstring)
#define TOFString(STDstring) FGsTextUtil::string_TO_FString(STDstring)
