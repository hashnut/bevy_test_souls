#pragma once
#include "CoreMinimal.h"
#include "GsBannedWordInfo.h"
#include "GsBannedWordHandler.generated.h"

class FGsBannedWordTable;
class UGsBannedWordProcess;

//multiple tree
class FGsBannedWordNode
{
private:
	bool  _isLeaf = false; //반환가능한 문자라는 의미.
	int32 _depth = 0; //반환시의 길이
	FString _usage = TEXT("all");
	FString _language = TEXT("all");
	FString _country = TEXT("all");

	TMap<TCHAR, TSharedPtr<FGsBannedWordNode>> _underList; //leaf여도, 아래 항목이 있을 수 있다. ex> fuck, fucking

public:
	FGsBannedWordNode() = default;
	FGsBannedWordNode(int32 depth, FString usage, FString language, FString country, bool isLeaf) 
		: _isLeaf(isLeaf), _depth(depth), _usage(usage), _language(language), _country(country)
	{}
	virtual ~FGsBannedWordNode() {}

	const bool isLeafNode() const { return _isLeaf; }
	void setLeafNode() { _isLeaf = true; }

	FORCEINLINE bool SameLanguage(FString lang) { return lang == _language || _language == TEXT("all"); }
	FORCEINLINE bool SameCountry(FString country) { return country == _country || _country == TEXT("all"); }
	FORCEINLINE bool SameUsage(FString usage) { return usage == _usage || _usage == TEXT("all"); }

	const bool isSameTypeLeaf(FString usage, FString lang, FString country) 
	{ 
		return _isLeaf && SameUsage(usage) && SameLanguage(lang) && SameCountry(country); 
	}

	void setLeafType(FString usage, FString lang, FString country) 
	{ 
		_isLeaf = true;  
		_usage = usage; 
		_language = lang;
		_country = country;
	}

	const int32 getDepth()  const { return _depth; }

	TSharedPtr<FGsBannedWordNode> AddChildNode(TCHAR c)
	{
		auto weakNode = FindChildNode(c);
		if (auto node = weakNode.Pin())
		{
			return node;
		}
		auto node = _underList.Emplace(c, MakeShared<FGsBannedWordNode>(_depth + 1, _usage, _language, _country, false));
		return node;
	}

	TWeakPtr<FGsBannedWordNode> FindChildNode(TCHAR c)
	{
		if (_underList.Find(c) != nullptr)
			return _underList[c];
		return nullptr;
	}

	void RemoveChildNode()
	{
		for (auto& node : _underList)
			node.Value->RemoveChildNode();
		_underList.Reset();
	}
};

/*
 * 금칙어 파일 핸들러
 *		1. Bannned Word Process 를 통해 저장된 Banned Table을 읽음
 *		2. 단어 단위의 Tree 형식으로 저장
 *		3. 금칙어 체크후 그 길이만큼 Replace
 * 
 * auto bannedWordHandler = MakeShared<FGsBannedWordHandler>();
 * bannedWordHandler->Initialize();
 * bannedWordHandler->StartBannedWordProcess();
 */
UCLASS()
class T1PROJECT_API UGsBannedWordHandler : public UObject //, public FTickableGameObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FBannedWordProcCompleteDelegate, int)

	const FString AppID = TEXT("com.com2us.worldofzenonia");
	const FString RegisterGameName = TEXT("WorldOfZenonia");

protected:
	struct BannedWordCheckItem
	{
		bool _useToChar{ false };
		TCHAR _toChar{'*'};
		FString _usage{ TEXT("all")};
		FString _language{ TEXT("all") };
		FString _country{ TEXT("all") };
		TArray<FString> _checkTexts; //바뀌기 전
		TFunction<void(bool, const FString& /*InString*/, const FString& /*outString*/)> _callback;
	};

	TArray<BannedWordCheckItem> _bannedWordCheckList;

protected:
	//현재 제한되어 있는 부분
	static TArray<FString> ValidUsage;
	static TArray<FString> ValidLanguage;
	static TArray<FString> ValidCountry;

	struct FGsResultBannedWord
	{
		int32	_result = 0; //0이면 금칙어 처리 안함 - 바뀐 문자열 길이
		FString _orgText; //바뀌기 전
		FString _changedText; //바뀐 후
	};

	enum class BannedWordHandlerState : uint8 { None, Init, Process, ProcessComplete};
	enum class BannedWordFileState : uint8 { None, NoneFileExist, UpdateBad, UseGood };

private:
	TWeakObjectPtr<UGsBannedWordProcess> _bannedWordProcess{ nullptr };

	BannedWordHandlerState _state{ BannedWordHandlerState::None };
	BannedWordFileState	_procState{ BannedWordFileState::None };

	TSharedPtr<FGsBannedWordTable>   _bannedWordTable;
	TUniquePtr<FGsBannedWordNode>	 _rootBannedWord;

	UPROPERTY()
	UGsBannedWordProcess* _ObjBannedWordProcess{ nullptr };

	FBannedWordProcCompleteDelegate  ProcessCompleteDelegate;

	int _lastProcError{ 0 };

protected:
	// bannedWordProcess의 모든 절차가 끝나 저장된 파일을 읽고 사용 가능
	void OnBannedWordProcessComplete(TArray<BannedWordFileRowData>& updateWords);

	// 저장된 파일에 추가 변동 사항이 있을때 처리됨.
	void OnBannedWordProcessUpdate(TArray<BannedWordFileRowData>& updateWords);

private:
	void MakeTableToNodeTreeBannedWord();
	void MakeNodeTreeBannedWord(const BannedWordFileRowData& word);
	int32 FindBanMatchWord(FString str, FString usage = TEXT("all"), FString lang = TEXT("all"), FString country = TEXT("all"));

public:
	void Initialize();
	void Finalize();
	void SetBannedWordGameInfo(FString banwordServerUrl, FString appId, FString gameName, FString bundlePathTEXT = TEXT("BannedWord"), FString bundleInfoPath = TEXT("BannedWord"));

	bool IsInitBannedWordProcess() { return _state == BannedWordHandlerState::Init; }
	bool IsRunningBannedWordProcess() { return _state == BannedWordHandlerState::Process; }

	bool IsUsableBannedHandler();

	bool StartBannedWordProcess(); //cancel 안됨
	void ResetBannedWordProcess();

	FBannedWordProcCompleteDelegate& OnProcessComplete() { return ProcessCompleteDelegate;  };

	void PushCheckBannedWord(IN const FString& orgText, FString usage = TEXT("all"), 
		bool useChange = false, TFunction<void(bool, const FString&, const FString&)> func = nullptr,
		TCHAR toChar = '*', FString language = TEXT("all"), FString country = TEXT("all"));

	void PushCheckBannedWords(IN TArray<FString>& orgTexts, FString usage = TEXT("all"),
		bool useChange = false, TFunction<void(bool, const FString&, const FString&)> func = nullptr,
		TCHAR toChar = '*', FString language = TEXT("all"), FString country = TEXT("all"));


	int GetProcessError();

protected:
	void EndBannedWordProcess();

protected:
	int32 CheckAndChangedBannedWord(IN const FString& orgText, OUT FString& outText, FString usage = TEXT("all"), TCHAR toChar = '*');
	int32 CheckAndChangedBannedWordByCultureInfo(IN const FString& orgText, OUT FString& outText, FString usage = TEXT("all"), TCHAR toChar = '*', FString language = TEXT("all"), FString country = TEXT("all"));
	FGsResultBannedWord CheckBannedWord(FString checkText, TCHAR changeChar = '*', FString usage = TEXT("all"), FString language = TEXT("all"), FString country = TEXT("all"));

	bool IsBannedWordIncluded(FString checkText, FString usage = TEXT("all"), FString lang = TEXT("all"), FString country = TEXT("all"));

	friend class UGsLocalizationManager;
};

