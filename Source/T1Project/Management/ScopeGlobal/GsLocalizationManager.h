// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsManager.h"
#include "Management/GsScopeHolder.h"
#include "GsLocalizationManager.generated.h"

class UGsBannedWordHandler;
class USoundBase;
class UTexture2D;
struct IGsMessageParam;

using BannedWordCheckCallBack = TFunction<void(bool, const FString&, const FString&)>;

/**
 * 현지화 처리를 위한 매니저
 */
UCLASS()
class T1PROJECT_API UGsLocalizationManager final : public UObject, public IGsManager
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UGsBannedWordHandler* _bannedWordHandler;
	TMap<int32, FString> _optionCultureMap;

	FString _strVoiceCulture;
	bool _bIsOptionSave = false; // OptionManager에서 SaveValue 시 팝업과 저장의 분기를 위해 _bIsOptionSave 값 세팅
	
	// Editor Only
	bool _bIsFemailVoiceEditor = false;

	int _isErrorCodeBannedWordChecker{ 0 };

public:
	static FString GetCurrCultureName(bool bIsPrioritized = true);
	static FString GetPrioritizedCultureName(const FString& InCurture);

public:
	// IGsManager override
	virtual void Initialize() override;
	virtual void Finalize() override;

private:	
	// Initialize 시, 옵션 값 변경 시 수행
	void SetCultureByGameUserSetting();

public:
	// 게임 옵션 값을 바꿨을 경우
	void OnSelectLanguage(int32 InOptionIndex, bool bIsVoice);
	bool IsOptionSave() const { return _bIsOptionSave; }

private:
	bool CheckAndCreateBanHandler();
	void OnBannedWordProcessComplete(int error);

public:
	// 주의: 추후 나라별로 변경되어야 하는 항목	

	// 영문, 숫자, 공백을 제외한 모든 문자를 2자로 계산
	int32 GetStringLengthRegex(const FString& InString) const;
	// B1 : 닉네임, 기사단명이 길어지는 현상 때문에 영어 숫자 상관없이 2로 처리 요청(CHR-18925)
	int32 GetStringLengthNameType(const FString& InString) const;

	// 이름에 쓰는 항목들 체크. 영문, 숫자, 한글만 가능, 조합중인 한글(자음만, 모음만 존재할 경우)불가
	bool IsValidNameType(const FString& InString) const;

public:
	// FGsBanTextHandlerBase 사용항목
	TWeakObjectPtr<UGsBannedWordHandler> GetBannedHandler();

	FString GetCurrentCountryName();
	FString GetCurrCultureNameForBannedWord() const;

	bool IsBannedWordInclude(const FString& InString, FString usage = TEXT("all"), BannedWordCheckCallBack callback = nullptr);
	bool ChangeBannedName(const FString& InString, BannedWordCheckCallBack callback /*nullptr*/);
	bool ChangeBannedSentence(const FString& InString, BannedWordCheckCallBack callback /*nullptr*/, TCHAR toChar = '*');
	bool ChangeBannedText(const FString& InString, BannedWordCheckCallBack callback /*nullptr*/);
	bool IsBannedWordIncludeInWordList(TArray<FString>& InString, FString usage = TEXT("all"), BannedWordCheckCallBack callback = nullptr);
	int GetBannedWordProcessErrorCode();

public:
	FString GetCurrentCultureName(bool bIsPrioritized = true) const;
	FString GetVoiceCultureName() const { return _strVoiceCulture; }
	bool IsValidCulture(const FString& InCulture) const;
	bool IsCurrentCulture(const FString& InCulture) const;

	FString ConvertOptionIndexToCulture(int32 InIndex) const;
	bool ConvertOptionIndexToCulture(int32 InIndex, OUT FString& OutCulture);
	int32 ConvertCultureToOptionIndex(const FString& InCulture) const;

private:
	// 대만의 경우 zh-Hant(중국어번체)로 세팅했는데, zh-Hant-TW나 zh-TW가 넘어올 경우에 대한 대응
	FString GetPrioritizedCultureNameInter(const FString& InCurture) const;

	// 리소스 얻어오기
	// LocalizationAssets 하위를 먼저 찾아보고, 없을 경우 Launcher/LocalizationAssets 하위 경로를 찾아 로드함
public:
	// Texture 하위 경로
	// LocalizationAssets 하위를 먼저 찾아보고, 없을 경우 Launcher/LocalizationAssets 하위 경로를 찾아 로드함
	UTexture2D* GetTexture(const FString& InPath);
	// Sound 하위 경로
	USoundBase* GetSound(const FString& InPath);
	USoundBase* GetCinematicSound(const FString& InFileName, bool bIsFemail = false);	
	USoundBase* GetDialogueSound(const FString& InFileName, bool bIsFemail = false);
	USoundBase* GetSubDialogueSound(const FString& InFileName, bool bIsFemail = false);
	// Character 폴더 하위 사운드 파일 경로
	USoundBase* GetCharacterSound(const FString& InFilePath);

private:
	USoundBase* LoadSound(const FString& InFilePath, const FString& InFileName);

public:
	FString GetCulturePath(bool bIsVoiceCulture, bool bIsLauncherPath) const;	
	FString GetLoadFilePath(const FString& InFilePath, const FString& InFileName) const;
	// Content 미만 폴더 경로를 적을 것
	bool IsFileExist(const FString& InFilePath);

	// Editor Only
public:
	void SetVoiceGenderEditor(bool bIsFemailVoice) { _bIsFemailVoiceEditor = bIsFemailVoice; }
};

#define GLocalization() UGsScopeHolder::GetGlobalManager<UGsLocalizationManager>(UGsScopeGlobal::EManagerType::Localization)
