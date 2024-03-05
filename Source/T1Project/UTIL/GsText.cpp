#include "GsText.h"
#include "CoreMinimal.h"

#include "ContentsServer/GsContentsServerHelper.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGlobal.h"

#include "GameObject/Data/GsGameObjectData.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"

#include "DataSchema/InterServer/World/GsSchemaWorldData.h"



int32 FGsTextUtil::GetStringLengthRegex(const FString& InString)
{
	return GLocalization()->GetStringLengthRegex(InString);
}

int32 FGsTextUtil::GetStringLengthNameType(const FString& InString)
{
	return GLocalization()->GetStringLengthNameType(InString);
}

bool FGsTextUtil::IsValidNameType(const FString& InString)
{
	return GLocalization()->IsValidNameType(InString);
}

static TMap<BannedWordCheckUsage, const FString> usageString = {
		TPair<BannedWordCheckUsage, const FString>{ BannedWordCheckUsage::all, TEXT("all") },
		TPair<BannedWordCheckUsage, const FString>{ BannedWordCheckUsage::name, TEXT("name") },
		TPair<BannedWordCheckUsage, const FString>{ BannedWordCheckUsage::sentence, TEXT("sentence") }
};

bool FGsTextUtil::IsBanWord(const FString& InString, BannedWordCheckUsage usage, BannedWordCheckCallBack callback)
{
	return GLocalization()->IsBannedWordInclude(InString, usageString[usage], callback);
}

bool FGsTextUtil::IsBanWordInWordList(TArray<FString>& InStringList, BannedWordCheckUsage usage, BannedWordCheckCallBack callback)
{
	return GLocalization()->IsBannedWordIncludeInWordList(InStringList, usageString[usage], callback);
}

void FGsTextUtil::ChangeBanSentence(const FString& InString, BannedWordCheckCallBack callback)
{
	GLocalization()->ChangeBannedSentence(InString, callback);
}

// get target name(+inter server name)
FString FGsTextUtil::GetTargetName(FGsGameObjectData* In_targetObjectData, EGsGameObjectType In_type)
{
	if (In_targetObjectData == nullptr)
	{
		return "";
	}

	FString targetName = In_targetObjectData->GetName();

	if (EGsGameObjectType::RemotePlayer == In_type)
	{
		WorldId myHomeWorldId = GGameData()->GetUserData()->mHomeWorldId;
		WorldId nowMapWorldId = GGameData()->GetLoadWorldId();
		auto curWorldType = GGameData()->GetLoadWorldType();

		if (SharedFunc::HasOtherUserNamePrefix(curWorldType, nowMapWorldId, myHomeWorldId, In_targetObjectData->GetHomeWorldId()))
		{
			/*FString serverId = In_targetObjectData->GetHomeWorldPrefix();
			targetName = FString::Format(TEXT("{0}({1})"), { *targetName, *serverId,  });*/
			// 대괄호 표시
			FText textName = FText::FromString(targetName);
			targetName = CombineUserAndPrefix(textName, In_targetObjectData->GetHomeWorldId(), true);
		}
				
	}
	return targetName;
}

const TTuple<FGsTextUtil::EUserNameValid, WorldId, FText, FText> FGsTextUtil::IsValidUserSearchNameOnly(const FString& InName, bool IsPrintMsg /*= true*/)
{
	auto IsCheckString = [](const FString& InString, const int32 InMaxLength) -> EUserNameValid
	{
		EUserNameValid outResult = EUserNameValid::Valid;

		if (InString.IsEmpty() || InMaxLength < FGsTextUtil::GetStringLengthNameType(InString))
		{	// 2023/8/30 PKT - String 길이 검사
			outResult = EUserNameValid::InvalidLength;
		}
		else if (false == IsValidNameType(InString))
		{	// 2023/8/30 PKT - 특문 등 의 검사.( 관련문서: https://jira.com2us.com/wiki/x/maBAEg )
			outResult = EUserNameValid::InvalidFormat;
		}

		return outResult;
	};

	// 2023/8/30 PKT - Out Value's
	WorldId outWorldId = INVALID_WORLD_ID;
	FText outUserName = FText::GetEmpty();
	FText outServerName = FText::GetEmpty();
	EUserNameValid outReason = EUserNameValid::Valid;

	do 
	{
		// 2023/9/8 PKT - 대괄호가 있는가?
		bool isSquareTag = (0 <= InName.Find(TEXT("["), ESearchCase::CaseSensitive) || 0 <= InName.Find(TEXT("]"), ESearchCase::CaseSensitive));
		bool isParenthesesTag = (0 <= InName.Find(TEXT("("), ESearchCase::CaseSensitive) || 0 <= InName.Find(TEXT(")"), ESearchCase::CaseSensitive));

		// 2023/9/8 PKT - 소괄호든 대괄호든 둘중 하나만 써야 함.( 둘다 지원 하는건 맞지만 같은 형식으로 와야 함. )
		if (isSquareTag && isParenthesesTag)
		{
			outReason = EUserNameValid::InvalidFormat;
			break;
		}


		// 2023/9/7 PKT - 소괄호 대괄호 모두 지원을 위해 해당 문자를 바꿈.
		FString tempName = InName.Replace(TEXT("("), TEXT("["));
		tempName.ReplaceInline(TEXT(")"), TEXT("]"));

		int32 tagBeginIndex = tempName.Find(TEXT("["), ESearchCase::CaseSensitive);
		int32 tagEndIndex = tempName.Find(TEXT("]"), ESearchCase::CaseSensitive);
		
		// 2023/8/30 PKT - 서버 이름에 대한 tag가 붙어 있는데 위치가 이상하다면 포맷 형식이 맞지 않음.
		if (0 <= tagBeginIndex)
		{	
			if (tagBeginIndex >= tagEndIndex || 0 >= tagEndIndex)
			{
				outReason = EUserNameValid::InvalidFormat;
				break;
			}
		}

		// 2023/8/30 PKT - 유저 이름 검색
		FString userName = (0 <= tagBeginIndex) ? tempName.LeftChop(tempName.Len() - tagBeginIndex) : tempName;
		outReason = IsCheckString(userName, MAX_CHAR_NAME_LEN);
		if (EUserNameValid::Valid != outReason)
		{
			break;
		}

		FString serverName = TEXT("");
		if (0 < tagBeginIndex && 0 < tagEndIndex)
		{
			serverName = tempName.Mid(tagBeginIndex + 1, (tagEndIndex - 1) - tagBeginIndex);
			outReason = IsCheckString(serverName, MAX_SERVER_NAME_LEN);
			if (EUserNameValid::Valid != outReason)
			{
				break;
			}			
		}		

		// 2023/8/30 PKT - 문자열 전체 길이와 각각뽑아낸 정보의 문자열 길이가 서로 맞지 않는다. 이상한 형식의 문자열이 포한되었다고 간주 한다.
		const static int32 BRACKETS_LENGHT = 2;
		if (tempName.Len() != (userName.Len() + (0 < serverName.Len() ? serverName.Len() + BRACKETS_LENGHT : 0)))
		{
			outReason = EUserNameValid::InvalidFormat;
			break;
		}

		// 2023/8/30 PKT - World는 기본적으로 Local Player와 같지만, 별도 서버를 지칭하는 경우 변경한다.
		WorldId targetWorldId = GGameData()->GetUserData()->mHomeWorldId;
		if (false == serverName.IsEmpty())
		{
			// 2023/9/8 PKT - Prefix가 있는 상황에서는 WorldId도 체크 대상이 된다.
			targetWorldId = FGsContentsServerHelper::GetWorldIdWithWorldPrefix(FText::FromString(serverName));
			if (INVALID_WORLD_ID >= targetWorldId)
			{
				outReason = EUserNameValid::InvalidWorldId;
				break;
			}
		}

		// 2023/9/11 PKT - Valid
		outReason = EUserNameValid::Valid;
		outWorldId = targetWorldId;
		outUserName = FText::FromString(userName);
		outServerName = FText::FromString(serverName);

	} while (0);

	if (IsPrintMsg && EUserNameValid::Valid != outReason)
	{
		FText msg = FText::GetEmpty();
		if (outReason == EUserNameValid::InvalidLength)
		{	// 2023/8/30 PKT - 글자 수 조건이 맞지 않습니다.
			FText::FindText(TEXT("UICommonText"), TEXT("CharacterName_Check_WrongCount"), msg);
		}
		else if (outReason == EUserNameValid::InvalidFormat)
		{	// 2023/8/30 PKT - 올바르지 않는 형식의 문자가 포함되어 있습니다.
			FText::FindText(TEXT("UICommonText"), TEXT("CharacterName_Check_WrongWord"), msg);
		}
		else if (outReason == EUserNameValid::InvalidWorldId)
		{	// 2023/8/30 PKT - 
			FText::FindText(TEXT("UICommonText"), TEXT("CharacterName_Check_WrongWorldId"), msg);
		}

		FGsUIHelper::TrayMessageTicker(msg);
	}

	return MakeTuple(outReason, outWorldId, outUserName, outServerName);
}

FGsTextUtil::EUserNameValid FGsTextUtil::CheckedUserNameLengthLimit(const FString& InName, FString& OutName, const int32 InMaxLength /* = MAX_CHAR_SERVER_NAME_LEN */)
{
	EUserNameValid reason = EUserNameValid::Valid;

	OutName = InName.Replace(TEXT(" "), TEXT(""));
	int32 length = FGsTextUtil::GetStringLengthNameType(OutName);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (InMaxLength < length)
	{
		OutName.RemoveAt(OutName.Len() - 1);
		reason = EUserNameValid::InvalidLength;
	}

	return reason;
}

FString FGsTextUtil::CombineUserAndPrefix(const FText& InUserName, const WorldId InWorldId /* = INVALID_WORLD_ID */, bool InPrefixForce /* = false */)
{
	FString OutString = InUserName.ToString();

	if (GGameData() && GGameData()->GetUserData())
	{
		// 2023/9/4 PKT - 1. Prefix가 뭐든 강제로 이름 뒤에 붙인다. or 2. WorldId가 로컬 플레이어와 다를 경우에 찍는다.
		if (true == InPrefixForce || INVALID_WORLD_ID != InWorldId && InWorldId != GGameData()->GetUserData()->mHomeWorldId)
		{
			FText textPrefix = FGsContentsServerHelper::GetWorldIPrefixdWithWorldId(InWorldId);

			if (false == textPrefix.IsEmpty())			
			{
				// 2023/8/31 PKT - UserName(Prefix)
				OutString += (TEXT("[") + textPrefix.ToString() + TEXT("]"));
			}
		}
	}
	
	return OutString;
}