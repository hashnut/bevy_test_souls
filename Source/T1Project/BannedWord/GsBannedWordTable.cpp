#include "GsBannedWordTable.h"
#include "GsBannedWordFile.h"

using ACT = BannedWordFileRowData::ACT;

void FGsBannedWordTable::LoadBannedWordTable(FString filePath)
{
	ResetWordTable();

	TArray<FString> unparseStrings;

	auto bannedFile = MakeShared<FGsBannedWordFile>();
	bannedFile->OpenBannedWordFile(filePath);
	{
		bannedFile->DecryptionData();
		bannedFile->UncompressData();

		bannedFile->GetBannedWordStringList(unparseStrings);
	}
	bannedFile->CloseBannedWordFile();

	TArray<BannedWordFileRowData> dataTable;
	ParseAndAddListWordTable(unparseStrings, dataTable);
}

void FGsBannedWordTable::UpdateBannedWordTable(IN TArray<BannedWordFileRowData>& list)
{
	for (auto e : list)
	{
		UpdateBannedWord(e);
	}
}

void FGsBannedWordTable::ResetWordTable()
{
	_bannedWords.Reset();
}

int32 FGsBannedWordTable::ParseAndAddListWordTable(IN TArray<FString>& unParsedStrings, OUT TArray<BannedWordFileRowData>& dataTable)
{
	for (const auto e : unParsedStrings)
	{
		dataTable.Add(ParseAndAddWordTable(e));
	}

	return dataTable.Num();
}

const BannedWordFileRowData& FGsBannedWordTable::ParseAndAddWordTable(IN const FString& unParsedWord)
{
	TArray<FString> outStr;
	if (unParsedWord.ParseIntoArray(outStr, TEXT("\t"), true) >= 4)
	{
		auto Key = outStr[0];
		UpdateBannedWord(BannedWordFileRowData{ outStr[0], outStr[1], outStr[2], outStr[3], ACT::On });

		if (auto iter = _bannedWords.Find(Key))
			return *iter;
		
	}

	return NoData;
}

const BannedWordFileRowData& FGsBannedWordTable::UpdateBannedWord(BannedWordFileRowData data)
{
	_bannedWords.Remove(data._word);

	if (data._action == ACT::On)
	{
		return _bannedWords.Emplace(data._word, data);
	}

	return NoData;
}

FString FGsBannedWordTable::ReplaceText(FString sentence, TCHAR tochar, FString usage, FString language, FString country)
{
	auto SameType = [](FString str, FString data) { return str.ToLower() == data.ToLower() || data.ToLower() == TEXT("all"); };

	/*input: 삐리리리릭삐리리삐리리삐리리리릭  table: 삐리리, 삐리리리릭  output : ***리릭*********리릭 
	  순차적용의 한계로 원래 얻어야 했던 **************** 를 얻지 못함 */

	FString result = sentence;
	for (auto e : _bannedWords)
	{
		if (e.Value._action == ACT::Off) continue;

		auto word = e.Key;
		auto data = e.Value;

		auto sameType = SameType(usage, data._usage) && SameType(country, data._country) && SameType(language, data._language);

		if (sameType && result.Contains(word))
		{
			FString changedString;
			auto count = word.Len();
			while (count-- > 0)
				changedString.AppendChar(tochar);

			while (result.Contains(word))
			{
				result = result.Replace(*word, *changedString);
			}
		}
	}

	return result;
}
