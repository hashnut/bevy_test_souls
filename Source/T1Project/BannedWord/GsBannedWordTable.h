#pragma once
#include "CoreMinimal.h"
#include "GsBannedWordInfo.h"

/*
 * bundle file을 읽어 구조를 저장한다. 
 * ReplaceText를 사용할 수 있으나, 순차적용의 한계가 있어 핸들러의 Text Tree 를 이용한다.
 */

class T1PROJECT_API FGsBannedWordTable
{
private:
	TMap<FString, BannedWordFileRowData> _bannedWords;

	const BannedWordFileRowData NoData = BannedWordFileRowData{ TEXT("0"), TEXT("all"), TEXT("all"), TEXT("all"), BannedWordFileRowData::ACT::On };

public:
	void LoadBannedWordTable(FString filePath);
	void UpdateBannedWordTable(IN TArray<BannedWordFileRowData>& list);
	void ResetWordTable();

	int32 ParseAndAddListWordTable(IN TArray<FString>& unParsedStrings, OUT TArray<BannedWordFileRowData>& dataTable);
	const BannedWordFileRowData& ParseAndAddWordTable(IN const FString& unParsedWord);

	const TMap<FString, BannedWordFileRowData>& GetBannedWordsList() const { return _bannedWords; }

	FString ReplaceText(FString sentence, TCHAR tochar = '*', FString usage = TEXT("all"), FString language = TEXT("all"), FString country = TEXT("all"));

protected:
	const BannedWordFileRowData& UpdateBannedWord(BannedWordFileRowData data);
};

