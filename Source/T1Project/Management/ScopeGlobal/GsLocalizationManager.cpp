// Fill out your copyright notice in the Description page of Project Settings.


#include "GsLocalizationManager.h"
#include "Internationalization/Regex.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "T1Project.h"
#include "Classes/Engine/Texture2D.h"
#include "Data/GsResourceManager.h"
#include "Sound/SoundBase.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "BannedWord/GsBannedWordHandler.h"
#include "Option/GsOptionEnumType.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

// 오픈스펙 로컬지원 안함. 강제 한국어(0, ko) 설정
#define USE_KO_ONLY
// 중국어-번체(대만) 사용
//#define USE_ZH_HANT

const int32 INVALID_LANG_SETTING_VALUE = -1;

const FString LocAssetRootPath = TEXT("LocalizationAssets/");
const FString LauncherLocAssetRootPath = TEXT("Launcher/LocalizationAssets/");

const FString TextureFilePathFormat = TEXT("{0}/Asset/Texture/{1}");
const FString SoundFilePathFormat = TEXT("{0}/Sound/{1}");
const FString CinematicSoundFilePathFormat = TEXT("{0}/Sound/Cinematic/{1}");
const FString DialogueSoundFilePathFormat = TEXT("{0}/Sound/Dialogue/{1}");
const FString SubDialogueSoundFilePathFormat = TEXT("{0}/Sound/SubDialogue/{1}");
const FString FairyVoiceFilePathFormat = TEXT("{0}/Sound/CH/{1}");

const FString LoadFileFormat = TEXT("/Game/{0}.{1}"); // 0: fileName포함된 path, 1: fileName (레퍼런스 복사 시 나오는 주소)
const FString FileExistCheckFormat = TEXT("{0}{1}.uasset"); // 0: Content 폴더 Path, 1: Content미만 경로

const FString DEFAULT_CULTURE_NAME = TEXT("ko");

//----------------------------------------------------------------------------------------------------------------------
FString UGsLocalizationManager::GetCurrCultureName(bool bIsPrioritized)
{
	// 주의 GetCurrentCulture 함수와 GetCurrentLanguage는 동일하며, GetCurrentLanguage 쪽을 사용 권장
	FCultureRef cultureRef = FInternationalization::Get().GetCurrentLanguage();

	// 예: 대만 컬쳐가 zh-TW나 zh-Hant-TW 등으로 들어올 경우 zh-Hant 로 변환하는 로직
	if (bIsPrioritized)
	{
		if (UGsLocalizationManager* mgr = GLocalization())
		{
			return mgr->GetPrioritizedCultureNameInter(cultureRef->GetName());
		}
	}

	return cultureRef.Get().GetName();
}

FString UGsLocalizationManager::GetPrioritizedCultureName(const FString& InCurture)
{
	if (UGsLocalizationManager* mgr = GLocalization())
	{
		return mgr->GetPrioritizedCultureNameInter(InCurture);
	}

	return InCurture;
}

//----------------------------------------------------------------------------------------------------------------------
void UGsLocalizationManager::Initialize()
{
	// 개선: 이 정보를 다른 에셋으로 분리해야 할까?
	// 주의: 옵션의 인덱스와 동일해야 함, 세팅에 쓰인 이후엔 함부로 바뀌면 안됨
	_optionCultureMap.Empty();
	_optionCultureMap.Emplace(0, TEXT("ko"));
#ifndef USE_KO_ONLY
	_optionCultureMap.Emplace(1, TEXT("en"));

#ifdef USE_ZH_HANT
	_optionCultureMap.Emplace(2, TEXT("zh-Hant"));
#endif // USE_ZH_HANT

#endif // USE_KO_ONLY

	SetCultureByGameUserSetting();
}

void UGsLocalizationManager::Finalize()
{
	_optionCultureMap.Empty();
	if (_bannedWordHandler)
	{
		_bannedWordHandler->Finalize();
		_bannedWordHandler = nullptr;
	}
}

void UGsLocalizationManager::SetCultureByGameUserSetting()
{
	// 영문 설정된 기기로 실행 시, culture이 en으로 설정되는데, 번역이 없는 텍스트의 경우엔 원본 텍스트를 출력한다.
	// 그때 다른 리소스에 레퍼런스가 안걸린 텍스트들이 표시가 안되는 문제가 있다.(예: SystemText, NetText, CinematicText)
	// (PC 환경에서는 발생하지 않는다)
	// 따라서 강제로 SetCurrentCulture 를 한 번 불러주어 로딩을 진행한다.

	// 주의: OptionManager보다 늦게 불려야한다. 값이 INVALID_LANG_SETTING_VALUE일 경우 값을 덮어 쓴다.	
	UGsOptionManager* optionMgr = GOption();
	if (nullptr == optionMgr)
	{
		_strVoiceCulture = DEFAULT_CULTURE_NAME;
		FInternationalization::Get().SetCurrentCulture(DEFAULT_CULTURE_NAME);
		GSLOG(Warning, TEXT("[Localization] language: %s, voice: %s"), *DEFAULT_CULTURE_NAME, *_strVoiceCulture);
		return;
	}

	// GameUserSetting의 값 가져오기
	float tempLangIndex = -1.f;
	float tempVoiceIndex = -1.f;
	bool isVisible = true;
	optionMgr->GetValue(EGsOptionItem::Language, tempLangIndex, isVisible);
	optionMgr->GetValue(EGsOptionItem::LanguageVoice, tempVoiceIndex, isVisible);

#ifdef USE_KO_ONLY
	// 오픈스펙 로컬지원 안함. 강제 한국어(0, ko) 설정
	int32 langIndex = 0;
	int32 voiceIndex = 0;
#else
	int32 langIndex = static_cast<int32>(tempLangIndex);
	int32 voiceIndex = static_cast<int32>(tempVoiceIndex);
#endif

	// 처음 설치 시 세팅 값이 없을 때 한 번만 진행하는 부분(index가 -1값)
	// 각 디바이스별로 GetCurrentCulture 시 값이 다를 수 있어서 첫 세팅 경우만 따로 체크
	if (0 > langIndex || 0 > voiceIndex)
	{
		// 설정값이 없으면 현재 culture를 얻어와서 체크
		FString strTempCulture = GetCurrCultureName();

		// 현재 cultrue가 유효하지 않을 경우 기본값 세팅을 위해 false 리턴
		if (false == IsValidCulture(strTempCulture))
		{
			strTempCulture = DEFAULT_CULTURE_NAME;
		}

		langIndex = ConvertCultureToOptionIndex(strTempCulture);
		voiceIndex = langIndex;

		// 옵션의 -1값을 변경
		_bIsOptionSave = true;
		optionMgr->SetValue(EGsOptionItem::Language, static_cast<float>(langIndex));
		optionMgr->SetValue(EGsOptionItem::LanguageVoice, static_cast<float>(voiceIndex));
		_bIsOptionSave = false;

		_strVoiceCulture = strTempCulture;
		FInternationalization::Get().SetCurrentCulture(strTempCulture);
		GSLOG(Warning, TEXT("[Localization] language: %s, voice: %s"), *strTempCulture, *_strVoiceCulture);
		return;
	}	

	// 유효하지 않은 경우 DEFAULT_CULTURE_NAME 으로 설정된다.
	// 유효성 검사를 실패해서 기본값으로 설정된 경우, 옵션 값도 덮어쓴다.
	FString strLang;
	if (false == ConvertOptionIndexToCulture(langIndex, strLang))
	{
		langIndex = ConvertCultureToOptionIndex(strLang);
		_bIsOptionSave = true;
		optionMgr->SetValue(EGsOptionItem::Language, static_cast<float>(langIndex));
		_bIsOptionSave = false;
	}

	FString strVoice;
	if (false == ConvertOptionIndexToCulture(voiceIndex, strVoice))
	{
		voiceIndex = ConvertCultureToOptionIndex(strLang);
		_bIsOptionSave = true;
		optionMgr->SetValue(EGsOptionItem::LanguageVoice, static_cast<float>(voiceIndex));
		_bIsOptionSave = false;
	}

	_strVoiceCulture = strVoice;
	FInternationalization::Get().SetCurrentCulture(strLang);
	GSLOG(Warning, TEXT("[Localization] language: %s, voice: %s"), *strLang, *_strVoiceCulture);
}

void UGsLocalizationManager::OnSelectLanguage(int32 InOptionIndex, bool bIsVoice)
{
	UGsOptionManager* optionMgr = GOption();
	if (nullptr == optionMgr)
	{
		GSLOG(Error, TEXT("[Localization] OnSelectLanguage: OptionManager is null."));
		return;
	}
	
	_bIsOptionSave = true; 
	if (bIsVoice)
	{
		// 목소리 변경값 저장
		optionMgr->SetValue(EGsOptionItem::LanguageVoice, static_cast<float>(InOptionIndex));
	}
	else
	{
		// 언어 변경값 저장
		optionMgr->SetValue(EGsOptionItem::Language, static_cast<float>(InOptionIndex));
	}
	_bIsOptionSave = false;

	// 옵션 저장
	optionMgr->SaveAll(); // CloseInternal이 안불릴 것이므로 바로 호출

	if (_bannedWordHandler)
	{
		_bannedWordHandler->ResetBannedWordProcess();
		_isErrorCodeBannedWordChecker = 0;
	}

	// Culture을 옵션값대로 다시 세팅
	SetCultureByGameUserSetting();

#if !WITH_EDITOR
	// 하이브 다시 세팅
	GHive()->SetConfig();
#endif

	// 재시작
	GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
}

bool UGsLocalizationManager::CheckAndCreateBanHandler()
{
	// 주의: Initialize 시점 
	if (_bannedWordHandler == nullptr)
	{
		FGsUIHelper::ShowBlockUI();

		_bannedWordHandler = NewObject<UGsBannedWordHandler>();
		_bannedWordHandler->Initialize();
		_bannedWordHandler->OnProcessComplete().BindUObject(this, &UGsLocalizationManager::OnBannedWordProcessComplete);
		_bannedWordHandler->StartBannedWordProcess();

		return false;
	}
	
	if (_bannedWordHandler->IsUsableBannedHandler() == false)
	{
		if (_bannedWordHandler->IsRunningBannedWordProcess() == false) //running 중일때는 들어오지 않는다.
		{
			FGsUIHelper::ShowBlockUI();

			_bannedWordHandler->ResetBannedWordProcess();
			_bannedWordHandler->OnProcessComplete().BindUObject(this, &UGsLocalizationManager::OnBannedWordProcessComplete);
			_bannedWordHandler->StartBannedWordProcess();
		}
		return false;
	}
	
	return _isErrorCodeBannedWordChecker == 0;
}

void UGsLocalizationManager::OnBannedWordProcessComplete(int error)
{
	_isErrorCodeBannedWordChecker = error;
	FGsUIHelper::HideBlockUI();

	if (_isErrorCodeBannedWordChecker != 0)
	{
		FText bannedProcError;
		if (FText::FindText(TEXT("ChatText"), TEXT("ChatError1"), bannedProcError))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, bannedProcError);
		}
	}
}

int32 UGsLocalizationManager::GetStringLengthRegex(const FString& InString) const
{
	return SharedFunc::GetStringLength(InString);
}

int32 UGsLocalizationManager::GetStringLengthNameType(const FString& InString) const
{
	return SharedFunc::GetStringLengthNameType(InString);
}

bool UGsLocalizationManager::IsValidNameType(const FString& InString) const
{
	// 관련문서: https://jira.com2us.com/wiki/x/maBAEg
	return SharedFunc::IsValidCharNameType(InString);
}

// 나중에 chatting 에 language나 country code 가 들어간다면 핸들러를 사용하여 필요한 체크
TWeakObjectPtr<UGsBannedWordHandler> UGsLocalizationManager::GetBannedHandler()
{
	if(CheckAndCreateBanHandler())
		return _bannedWordHandler;
	return nullptr;
}

FString UGsLocalizationManager::GetCurrentCountryName()
{
	auto languageCulture = FInternationalization::Get().GetCurrentLanguage();
	auto strCurCountry = languageCulture.Get().GetRegion();

	if (strCurCountry.IsEmpty()) strCurCountry = TEXT("all");
	return strCurCountry;
}

FString UGsLocalizationManager::GetCurrCultureNameForBannedWord() const
{
	//all, ja(일본), zh-hans(간체), zh-hant(번체), th(태국어), ko
	FString strCulture = GetCurrCultureName();

	return strCulture.ToLower();

	/*
	auto languageCulture = FInternationalization::Get().GetCurrentLanguage();
	FString strLanguage = languageCulture.Get().GetTwoLetterISOLanguageName();
	FString languageScript = languageCulture.Get().GetScript();

	if (!languageScript.IsEmpty()) //all, ja(일본), zh-hans(간체), zh-hant(번체), th(태국어), ko
	{
		strLanguage = strLanguage.ToLower().AppendChar('-') + languageScript.ToLower();
	}

	return strLanguage;
	*/
}

bool UGsLocalizationManager::IsBannedWordInclude(const FString& InString, FString usage, BannedWordCheckCallBack callback /*nullptr*/)
{
	auto res = CheckAndCreateBanHandler();

	if(_bannedWordHandler)
		_bannedWordHandler->PushCheckBannedWord(InString, usage, false, callback, '*', GetCurrCultureNameForBannedWord(), GetCurrentCountryName());

	return res;
}

bool UGsLocalizationManager::ChangeBannedName(const FString& InString, BannedWordCheckCallBack callback /*nullptr*/)
{
	auto res = CheckAndCreateBanHandler();

	if (_bannedWordHandler)
		_bannedWordHandler->PushCheckBannedWord(InString, TEXT("name"), true, callback, '*', GetCurrCultureNameForBannedWord(), GetCurrentCountryName());

	return res;
}

bool UGsLocalizationManager::ChangeBannedSentence(const FString& InString, BannedWordCheckCallBack callback /*nullptr*/, TCHAR toChar)
{
	auto res = CheckAndCreateBanHandler();

	if (_bannedWordHandler)
		_bannedWordHandler->PushCheckBannedWord(InString, TEXT("sentence"), true, callback, toChar, GetCurrCultureNameForBannedWord(), GetCurrentCountryName());

	return res;
}

bool UGsLocalizationManager::ChangeBannedText(const FString& InString, BannedWordCheckCallBack callback /*nullptr*/)
{
	auto res = CheckAndCreateBanHandler();

	if (_bannedWordHandler)
		_bannedWordHandler->PushCheckBannedWord(InString, TEXT("all"), true, callback, '*', GetCurrCultureNameForBannedWord(), GetCurrentCountryName());

	return res;
}

bool UGsLocalizationManager::IsBannedWordIncludeInWordList(TArray<FString>& InString, FString usage /*= TEXT("all")*/, BannedWordCheckCallBack callback /*= nullptr*/)
{
	auto res = CheckAndCreateBanHandler();

	if (_bannedWordHandler)
		_bannedWordHandler->PushCheckBannedWords(InString, usage, false, callback, '*', GetCurrCultureNameForBannedWord(), GetCurrentCountryName());

	return res;
}

int UGsLocalizationManager::GetBannedWordProcessErrorCode()
{
	if (_bannedWordHandler)
		return _bannedWordHandler->GetProcessError();
	return -1;
}

FString UGsLocalizationManager::GetCurrentCultureName(bool bIsPrioritized) const
{
	return GetCurrCultureName(bIsPrioritized);
}

bool UGsLocalizationManager::IsValidCulture(const FString& InCulture) const
{
	for (TPair<int32, FString> itPair : _optionCultureMap)
	{
		if (itPair.Value == InCulture)
		{
			return true;
		}
	}

	return false;
}

bool UGsLocalizationManager::IsCurrentCulture(const FString& InCulture) const
{
	FString strCulture = GetCurrCultureName();
	if (strCulture == InCulture)
	{
		return true;
	}

	return false;
}

FString UGsLocalizationManager::ConvertOptionIndexToCulture(int32 InIndex) const
{
	FString strCulture;
	if (const FString* valuePtr = _optionCultureMap.Find(InIndex))
	{
		strCulture = *valuePtr;
	}
	else
	{
		strCulture = DEFAULT_CULTURE_NAME;
	}

	return strCulture;
}

bool UGsLocalizationManager::ConvertOptionIndexToCulture(int32 InIndex, OUT FString& OutCulture)
{
	FString strCulture;
	if (const FString* valuePtr = _optionCultureMap.Find(InIndex))
	{
		OutCulture = *valuePtr;
		return true;
	}
	
	OutCulture = DEFAULT_CULTURE_NAME;
	return false;
}

int32 UGsLocalizationManager::ConvertCultureToOptionIndex(const FString& InCulture) const
{
	for (TPair<int32, FString> itPair : _optionCultureMap)
	{
		if (itPair.Value == InCulture)
		{
			return itPair.Key;
		}
	}

	// 실패 시 기본값(한국어)
	return 0;
}

FString UGsLocalizationManager::GetPrioritizedCultureNameInter(const FString& InCurture) const
{
	// 개선: 최적화 할 방법 모색
	TArray<FString> prioritizedList = FInternationalization::Get().GetPrioritizedCultureNames(InCurture);

	if (1 < prioritizedList.Num())
	{
		for (const TPair<int32, FString>& itPair : _optionCultureMap)
		{
			for (int i = 0; i < prioritizedList.Num(); ++i)
			{
				if (itPair.Value == prioritizedList[i])
				{
					return itPair.Value;
				}
			}
		}
	}

	// 없거나 1개뿐이면 현재 받은 이름 그대로 뱉음
	return InCurture;
}

UTexture2D* UGsLocalizationManager::GetTexture(const FString& InPath)
{	
	FString strCulturePath = GetCulturePath(false, false);
	FString strFileName = FPaths::GetBaseFilename(InPath); // 경로에서 파일명 추출
	FString strPath = FString::Format(*TextureFilePathFormat, { *strCulturePath, *InPath });

	if (IsFileExist(strPath))
	{
		FString strLoadPath = GetLoadFilePath(strPath, strFileName);
		if (UTexture2D* texture = GResource()->LoadSync<UTexture2D>(FSoftObjectPath(strLoadPath)))
		{
			return texture;
		}
	}

	// 로드된 것이 없으면 런쳐 경로에서 다시 검색
	strCulturePath = GetCulturePath(false, true);
	strPath = FString::Format(*TextureFilePathFormat, { *strCulturePath, *InPath });

	if (IsFileExist(strPath))
	{
		FString strLoadPath = GetLoadFilePath(strPath, strFileName);
		if (UTexture2D* texture = GResource()->LoadSync<UTexture2D>(FSoftObjectPath(strLoadPath)))
		{
			return texture;
		}
	}

	return nullptr;
}

USoundBase* UGsLocalizationManager::GetSound(const FString& InPath)
{
	// 파일명이 포함된 경로임에 유의
	FString strCulturePath = GetCulturePath(true, false);
	FString strFileName = FPaths::GetBaseFilename(InPath); // 경로에서 파일명 추출
	FString strPath = FString::Format(*SoundFilePathFormat, { *strCulturePath, *InPath });

	if (USoundBase* sound = LoadSound(strPath, strFileName))
	{
		return sound;
	}

	// 로드된 것이 없으면 런쳐 경로에서 다시 검색
	strCulturePath = GetCulturePath(true, true);
	strPath = FString::Format(*SoundFilePathFormat, { *strCulturePath, *InPath });

	if (USoundBase* sound = LoadSound(strPath, strFileName))
	{
		return sound;
	}

	return nullptr;
}

USoundBase* UGsLocalizationManager::GetCinematicSound(const FString& InFileName, bool bIsFemail)
{
	FString strCulturePath = GetCulturePath(true, false);
	FString strFileName;
	FString strPath;

	// 여자 정보 검사
#if WITH_EDITOR
	if (bIsFemail || _bIsFemailVoiceEditor)
#else
	if (bIsFemail)
#endif
	{
		strFileName = InFileName;
		strFileName.Append(TEXT("_f"));
		strPath = FString::Format(*CinematicSoundFilePathFormat, { *strCulturePath, *strFileName });

		if (USoundBase* sound = LoadSound(strPath, strFileName))
		{
			return sound;
		}
	}

	// 기본값이 _m이므로 _f로 못찾으면 다시 찾는다
	strFileName = InFileName;
	strFileName.Append(TEXT("_m"));
	strPath = FString::Format(*CinematicSoundFilePathFormat, { *strCulturePath, *strFileName });

	if (USoundBase* sound = LoadSound(strPath, strFileName))
	{
		return sound;
	}

	return nullptr;
}

USoundBase* UGsLocalizationManager::GetDialogueSound(const FString& InFileName, bool bIsFemail)
{
	FString strCulturePath = GetCulturePath(true, false);
	FString strFileName;
	FString strPath;
	
#if WITH_EDITOR
	if (bIsFemail || _bIsFemailVoiceEditor)
#else
	if (bIsFemail)
#endif
	{
		strFileName = InFileName;
		strFileName.Append(TEXT("_f"));
		strPath = FString::Format(*DialogueSoundFilePathFormat, { *strCulturePath, *strFileName });

		if (USoundBase* sound = LoadSound(strPath, strFileName))
		{
			return sound;
		}
	}

	// 기본값이 _m이므로 _f로 못찾으면 다시 찾는다
	strFileName = InFileName;
	strFileName.Append(TEXT("_m"));
	strPath = FString::Format(*DialogueSoundFilePathFormat, { *strCulturePath, *strFileName });

	if (USoundBase* sound = LoadSound(strPath, strFileName))
	{
		return sound;
	}

	return nullptr;
}

USoundBase* UGsLocalizationManager::GetSubDialogueSound(const FString& InFileName, bool bIsFemail)
{
	FString strCulturePath = GetCulturePath(true, false);
	FString strFileName;
	FString strPath;

	if (bIsFemail)
	{
		strFileName = InFileName;
		strFileName.Append(TEXT("_f"));
		strPath = FString::Format(*SubDialogueSoundFilePathFormat, { *strCulturePath, *strFileName });

		if (USoundBase* sound = LoadSound(strPath, strFileName))
		{
			return sound;
		}
	}

	strFileName = InFileName;
	strFileName.Append(TEXT("_m"));	
	strPath = FString::Format(*SubDialogueSoundFilePathFormat, { *strCulturePath, *strFileName });

	if (USoundBase* sound = LoadSound(strPath, strFileName))
	{
		return sound;
	}

	return nullptr;
}

USoundBase* UGsLocalizationManager::GetCharacterSound(const FString& InFilePath)
{
	if (InFilePath.IsEmpty())
	{
		return nullptr;
	}

	FString strCulturePath = GetCulturePath(true, false);
	FString strFileName = FPaths::GetBaseFilename(InFilePath); // 경로에서 파일명 추출
	FString strPath = FString::Format(*FairyVoiceFilePathFormat, { *strCulturePath, *InFilePath });

	if (USoundBase* sound = LoadSound(strPath, strFileName))
	{
		return sound;
	}

	return nullptr;
}

USoundBase* UGsLocalizationManager::LoadSound(const FString& InFilePath, const FString& InFileName)
{
	if (IsFileExist(InFilePath))
	{
		FString strLoadPath = GetLoadFilePath(InFilePath, InFileName);
		if (USoundBase* sound = GResource()->LoadSync<USoundBase>(FSoftObjectPath(strLoadPath)))
		{
			return sound;
		}
	}

	return nullptr;
}

FString UGsLocalizationManager::GetCulturePath(bool bIsVoiceCulture, bool bIsLauncherPath) const
{
	FString targetPath = (bIsLauncherPath) ? LauncherLocAssetRootPath : LocAssetRootPath;
	targetPath.Append(bIsVoiceCulture ? GetVoiceCultureName() : GetCurrCultureName());

	return targetPath;
}

FString UGsLocalizationManager::GetLoadFilePath(const FString& InFilePath, const FString& InFileName) const
{
	// InFilePath는 파일명이 포함된 경로임에 유의
	FString strOut = FString::Format(*LoadFileFormat, { *InFilePath, *InFileName });
	return strOut;
}

bool UGsLocalizationManager::IsFileExist(const FString& InFilePath)
{
	// InFilePath는 파일명이 포함된 경로임에 유의
	FString strPathContent = FPaths::ProjectContentDir();
	FString strCheckPath = FString::Format(*FileExistCheckFormat, { *strPathContent, *InFilePath } );

	return FPaths::FileExists(strCheckPath);
}
