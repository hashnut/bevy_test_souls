#include "GsBannedWordHandler.h"
#include "GsBannedWordProcess.h"
#include "GsBannedWordTable.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"

#include "Message/GsMessageSystem.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsDivergenceManager.h"

TArray<FString> UGsBannedWordHandler::ValidUsage = { TEXT("all"), TEXT("sentence"), TEXT("name") }; //all, 채팅만, 네임만
TArray<FString> UGsBannedWordHandler::ValidLanguage = { TEXT("all"), TEXT("ja"), TEXT("zh-hans"), TEXT("zh-hant"), TEXT("th") }; // all, 일본, 중국간체, 중국번체, 태국어
TArray<FString> UGsBannedWordHandler::ValidCountry = { TEXT("all"), TEXT("th") }; //all, 태국

void UGsBannedWordHandler::Initialize()
{
	if (_state == BannedWordHandlerState::None)
	{
		if (!_bannedWordTable) _bannedWordTable = MakeShared<FGsBannedWordTable>();
		if (!_rootBannedWord)  _rootBannedWord = MakeUnique<FGsBannedWordNode>();

		_state = BannedWordHandlerState::Init;
	}
}

void UGsBannedWordHandler::Finalize()
{	
	EndBannedWordProcess();
	_procState = BannedWordFileState::None;
	_state = BannedWordHandlerState::None;
	_ObjBannedWordProcess = nullptr;
	
	if (_bannedWordTable)
	{
		_bannedWordTable->ResetWordTable();
		_bannedWordTable = nullptr;
	}

	if (_rootBannedWord.IsValid())
	{
		_rootBannedWord->RemoveChildNode();
		_rootBannedWord.Reset(nullptr);
	}
}

void UGsBannedWordHandler::ResetBannedWordProcess()
{
	EndBannedWordProcess();
	_procState = BannedWordFileState::None;

	if (_bannedWordTable)
	{
		_bannedWordTable->ResetWordTable();
	}

	if (_rootBannedWord.IsValid())
	{
		_rootBannedWord->RemoveChildNode();
	}

	_ObjBannedWordProcess = nullptr;
}

void UGsBannedWordHandler::SetBannedWordGameInfo(FString banwordServerUrl, FString appId, FString gameName, FString bundlePathTEXT /*= TEXT("BannedWord")*/, FString bundleInfoPath /*= TEXT("BannedWord")*/)
{
	if (_bannedWordProcess.IsValid())
	{
		_bannedWordProcess->SetBannedWordProcInfo(banwordServerUrl, appId, gameName, bundlePathTEXT, bundleInfoPath);
	}
}

FName BannedWordProcessName = TEXT("BannedWordProcess");
bool UGsBannedWordHandler::StartBannedWordProcess()
{
	if (GDivergence() != nullptr && !IsRunningBannedWordProcess())
	{
		if (_ObjBannedWordProcess) _ObjBannedWordProcess = nullptr;
		if (!_bannedWordProcess.IsValid())
		{
			_bannedWordProcess = _ObjBannedWordProcess = NewObject<UGsBannedWordProcess>();
			_bannedWordProcess->SetBannedWordProcInfo(TEXT("https://staging-api-gspus.qpyou.cn/gateway.php"), AppID, RegisterGameName);
		}

		//나중에 Config 가져오자 일단은 그냥
		FString srvUrl = TEXT("https://staging-api-gspus.qpyou.cn/gateway.php");
		if (GDivergence() && GDivergence()->GetService() == TEXT("live"))
		{
			srvUrl = TEXT("https://api-gspus.qpyou.cn/gateway.php");
		}

		GSLOG(Log, TEXT("== SetBannedWordProcInfo : %s, %s, %s"), *srvUrl, *AppID, *RegisterGameName);

		_lastProcError = -1;

		_bannedWordProcess->OnProcessComplete().BindLambda([this](TArray<BannedWordFileRowData>& updateWords) { OnBannedWordProcessComplete(updateWords); });
		_bannedWordProcess->SetBannedWordProcInfo(srvUrl, AppID, RegisterGameName);
		_bannedWordProcess->StartProcess();

		_state = BannedWordHandlerState::Process;
	}

	return IsRunningBannedWordProcess();
}

void UGsBannedWordHandler::EndBannedWordProcess()
{
	if (_bannedWordProcess.IsValid()) //여기서 삭제해도 됨 
	{
		_bannedWordProcess->OnProcessComplete().Unbind();
		_bannedWordProcess->ResetProcInfo();
		_ObjBannedWordProcess = nullptr;
	}

	TArray<BannedWordCheckItem> useCheckList;
	Swap(useCheckList, _bannedWordCheckList);

	for (auto& e : useCheckList)
	{
		PushCheckBannedWords(e._checkTexts, e._usage, e._useToChar, e._callback, e._toChar, e._language, e._country);
	}
}

void UGsBannedWordHandler::OnBannedWordProcessComplete(TArray<BannedWordFileRowData>& updateWords)
{
	auto errorType = 0;
	if (_bannedWordProcess.IsValid()) // _bannedWordProcess 에서 호출하는 함수인데 _bannedWordProcess가 valid 한지 체크
	{
		_bannedWordProcess->OnProcessComplete().Unbind();

		if (_bannedWordProcess->GetBunndleInfo()._bundlePath.IsValidIndex(0))
		{
			_bannedWordTable->LoadBannedWordTable(_bannedWordProcess->GetBunndleInfo()._bundlePath / BANNEDWORD_BUNDDLEFILE);
			_bannedWordTable->UpdateBannedWordTable(updateWords);
		}

		MakeTableToNodeTreeBannedWord();

		UGsBannedWordProcess::ProcessState errorState;
		if (_bannedWordProcess->IsProcessError(errorState))
		{
			errorType = static_cast<int>(errorState);
			if (_bannedWordProcess->GetBunndleInfo()._isExistBundle == false)
			{
				_procState = BannedWordFileState::NoneFileExist; // 사용불가
				//GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::BANNEDWORD_PROCESS_FAILURE, nullptr);
			}
			else
			{
				_procState = BannedWordFileState::UpdateBad;
			}
		}
		else
		{
			_procState = BannedWordFileState::UseGood;
			//GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::BANNEDWORD_PROCESS_SUCCESS, nullptr);
		}
	}

	_state = BannedWordHandlerState::ProcessComplete;
	_lastProcError = errorType;

	EndBannedWordProcess();

	ProcessCompleteDelegate.ExecuteIfBound(errorType);
}

void UGsBannedWordHandler::OnBannedWordProcessUpdate(TArray<BannedWordFileRowData>& updateWords)
{
	_bannedWordTable->UpdateBannedWordTable(updateWords);

	for (const auto& e : updateWords)
		MakeNodeTreeBannedWord(e);
}

int32 UGsBannedWordHandler::CheckAndChangedBannedWord(IN const FString& orgText, OUT FString& outText, FString usage /*= TEXT("all")*/, TCHAR toChar /*= '*'*/)
{
	return CheckAndChangedBannedWordByCultureInfo(orgText, outText, usage, toChar) ;
}

int32 UGsBannedWordHandler::CheckAndChangedBannedWordByCultureInfo(IN const FString& orgText, OUT FString& outText, FString usage /*= TEXT("all")*/, TCHAR toChar /*= '*'*/, FString language /*= TEXT("all")*/, FString country /*= TEXT("all")*/)
{
	if (!ValidUsage.Find(usage)) usage = TEXT("all");
	if (!ValidLanguage.Find(language)) language = TEXT("all");
	if (!ValidCountry.Find(country)) country = TEXT("all");


	int32 changedLength = 0;
	FString resultText = orgText;
	FString useString = orgText;

	useString.ConvertTabsToSpacesInline(1);

	TArray<FString> outStr;
	useString.ParseIntoArray(outStr, TEXT(" "), false); //위치정보

	for (auto str : outStr)
	{
		FString procText = str;
		int32 pos = 0;
		while (!procText.IsEmpty())
		{
			while (procText.StartsWith(&toChar))
			{
				pos += 1;
				procText = procText.Right(str.Len() - pos);
			}

			auto result = CheckBannedWord(procText, toChar, usage, language, country);
			if (result._result > 0)
			{
				FString left; FString right;
				resultText.Split(result._orgText, &left, &right);
				resultText = left + result._changedText + right;

				pos += result._result;
				changedLength += result._result;
			}
			else if (!procText.IsEmpty())
				pos += 1; //heART삐리리 : heART삐리리, eART삐리리, ART삐리리, RT삐리리, T삐리리, 삐리리 -> heART***

			procText = procText.Right(str.Len() - pos); //삐리리나쁜말금칙어 -> *********
		}
	}
	outText = resultText;
	return changedLength;
}

UGsBannedWordHandler::FGsResultBannedWord UGsBannedWordHandler::CheckBannedWord(FString checkText, TCHAR changeChar, FString usage /*=TEXT("all")*/, FString lang /*=TEXT("all")*/, FString country /*=TEXT("all")*/)
{
	FGsResultBannedWord res = FGsResultBannedWord{ 0, checkText, checkText };
	if (checkText.IsEmpty()) return res;

	int32 count = FindBanMatchWord(checkText, usage, lang, country);

	if (count > 0)
	{
		res._result = count;

		res._orgText.Reset();
		res._orgText = checkText.Left(count);

		res._changedText.Reset();
		while(count-- > 0) res._changedText.AppendChar(changeChar);
	}

	return res;
}

bool UGsBannedWordHandler::IsBannedWordIncluded(FString checkText, FString usage /*=TEXT("all")*/, FString lang /*=TEXT("all")*/, FString country /*=TEXT("all")*/)
{
	UGsBannedWordHandler::FGsResultBannedWord result;

	FString resultText = checkText;
	resultText.ConvertTabsToSpacesInline(1);

	TArray<FString> outStr;
	resultText.ParseIntoArray(outStr, TEXT(" "), false); //위치정보
	for (auto str : outStr)
	{
		FString procText = str;
		int32 pos = 0;
		while (!procText.IsEmpty())
		{
			result = CheckBannedWord(procText, '*', usage, lang, country);
			if (result._result > 0)
			{
				break;
			}

			pos += 1;
			procText = procText.Right(str.Len() - pos);
		}
		if (result._result > 0)
			break;
	}
	return result._result > 0;
}

void UGsBannedWordHandler::MakeTableToNodeTreeBannedWord()
{
	_rootBannedWord->RemoveChildNode();
	const auto& list = _bannedWordTable->GetBannedWordsList();
	for (auto e : list)
	{
		auto arr = e.Key.ToUpper().GetCharArray();
		auto val = e.Value;

		TSharedPtr<FGsBannedWordNode> node = nullptr;
		for (auto& ch : arr)
		{
			if (ch == '\0') continue;
			node = node == nullptr ? _rootBannedWord->AddChildNode(ch) : node->AddChildNode(ch);
		}

		if (node)
		{
			node->setLeafType(val._usage, val._language, val._country);

			if (!ValidLanguage.Find(val._language)) ValidLanguage.Emplace(val._language);
			if (!ValidCountry.Find(val._country)) ValidCountry.Emplace(val._country);
		}
	}
}

void UGsBannedWordHandler::MakeNodeTreeBannedWord(const BannedWordFileRowData& word)
{
	auto arr = word._word.ToUpper().GetCharArray();

	TSharedPtr<FGsBannedWordNode> node = nullptr;
	for (auto& ch : arr)
	{
		if (ch == '\0') continue;
		node = node == nullptr ? _rootBannedWord->AddChildNode(ch) : node->AddChildNode(ch);
	}

	if (node)
	{
		node->setLeafType(word._usage, word._language, word._country);

		if (!ValidLanguage.Find(word._language)) ValidLanguage.Emplace(word._language);
		if (!ValidCountry.Find(word._country)) ValidCountry.Emplace(word._country);
	}
}

int32 UGsBannedWordHandler::FindBanMatchWord(FString str, FString usage /*=TEXT("all")*/, FString lang /*=TEXT("all")*/, FString country /*=TEXT("all")*/)
{
	auto arr = str.ToUpper().GetCharArray();

	TWeakPtr<FGsBannedWordNode> preNode;
	TWeakPtr<FGsBannedWordNode> node;
	for (auto& ch : arr)
	{
		if (ch == '\0') continue;
		node = node.IsValid() == false ? _rootBannedWord->FindChildNode(ch) : node.Pin()->FindChildNode(ch);
		if (node.IsValid() == false)
		{
			break;
		}

		if (node.Pin()->isSameTypeLeaf(usage, lang, country))
			preNode = node;
	}

	if (preNode.IsValid())
		return preNode.Pin()->getDepth();

	return 0;
}

bool UGsBannedWordHandler::IsUsableBannedHandler()
{	
	return	(_procState == BannedWordFileState::UpdateBad || _procState == BannedWordFileState::UseGood);
}

void UGsBannedWordHandler::PushCheckBannedWord(IN const FString& orgText, FString usage /*= TEXT("all")*/, bool useChange /*= false*/, 
	TFunction<void(bool, const FString&, const FString&)> func /*= nullptr*/, TCHAR toChar /*= '*'*/, FString language /*= TEXT("all")*/, FString country /*= TEXT("all")*/)
{
	if (IsUsableBannedHandler())
	{
		int errorCode = GetProcessError();
		if (useChange)
		{
			FString outString;
			auto reslen = CheckAndChangedBannedWordByCultureInfo(orgText, outString, usage, toChar, language, country);
			if (func) func(errorCode == 0 && reslen == 0, orgText, outString);
		}
		else
		{
			FString outString;
			auto res = IsBannedWordIncluded(orgText, usage, language, country);
			if (func) func(errorCode == 0 && res == false, orgText, orgText);
		}
	}
	else
	{
		TArray<FString> orgTextList = { orgText };
		_bannedWordCheckList.Add(BannedWordCheckItem{ useChange, toChar, usage, language, country, orgTextList, func });
	}
}

void UGsBannedWordHandler::PushCheckBannedWords(IN TArray<FString>& orgTexts, FString usage /*= TEXT("all")*/, bool useChange /*= false*/, 
	TFunction<void(bool, const FString&, const FString&)> func /*= nullptr*/, TCHAR toChar /*= '*'*/, FString language /*= TEXT("all")*/, FString country /*= TEXT("all")*/)
{
	if (IsUsableBannedHandler())
	{
		int errorCode = GetProcessError();
		if (useChange)
		{
			for (const auto& orgText : orgTexts)
			{
				FString outString;
				auto reslen = CheckAndChangedBannedWordByCultureInfo(orgText, outString, usage, toChar, language, country);
				if (func) func(errorCode == 0 && reslen == 0, orgText, outString);
			}
		}
		else
		{
			for (const auto& orgText : orgTexts)
			{
				FString outString;
				auto res = IsBannedWordIncluded(orgText, usage, language, country);
				if (func) func(errorCode == 0 && res == false, orgText, orgText);
			}
		}
	}
	else
	{
		_bannedWordCheckList.Add(BannedWordCheckItem{ useChange, toChar, usage, language, country, orgTexts, func });
	}
}

int UGsBannedWordHandler::GetProcessError()
{
	UGsBannedWordProcess::ProcessState errorState;

	if (_bannedWordProcess.IsValid())
	{
		if (_bannedWordProcess->IsProcessError(errorState))
		{
			return _bannedWordProcess->GetErrorCode();
		}
		return 0;
	}
	return _lastProcError;
}

