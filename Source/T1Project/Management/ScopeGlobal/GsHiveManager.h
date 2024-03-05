#pragma once

#include "CoreMinimal.h"
#include "../Classes/GsSingleton.h"
#include "../Classes/GsManager.h"

// Hive SDK
#if PLATFORM_IOS
	#include <HIVE_SDK_Plugin/HIVE_CPP.h>	
#else
	#include "HIVE_CPP.h"
#endif

#include "Hercules_App.h"
//#include "Plugins/HIVESDK/Source/HIVESDK/Public/HIVE_ResultAPI.h"

#include "Hive/GsHiveEventMsg.h"

#include "GsHiveManager.generated.h"

using namespace hive;

USTRUCT()
struct FUserEngagementInfoWindows
{
	GENERATED_BODY()

	UPROPERTY()
	FString scheme;
	UPROPERTY()
	FString api;
	UPROPERTY()
	FString param;
};

UENUM()
enum class EGsProviderType : uint8
{
	GUEST,						
	HIVE,
	FACEBOOK,
	GOOGLE,
	APPLE,

	NONE, // << 초기값 사용을 위해 추가
};
UENUM()
enum class EGsEngagementCallbackType : uint8
{
	NONE = 0,
	BMSHOP,

	MAX,
};

UCLASS()
class T1PROJECT_API UGsHiveManager :
	public UObject,
	public TGsSingleton<UGsHiveManager>,
	public IGsManager
{
	GENERATED_BODY()

private:
	bool _isAutoSignIn; // setup이 호출되기전엔 아무값도 의모도 없고 사용해서도 안된다
	bool _isInit; // 초기화를 했는지 체크
	bool _isSignIn;
	bool _LocalPush;
	bool _RemotePush;
	bool _Night;
	bool _Notice;

	PlayerInfo ConflictPlayer;
	PlayerInfo CurrentPlayer;

	// 아직 사용중이진 않지만 사용할거 대비
	FString mStrZone;
	FString mStrAppId;
	bool mBuseLog;
	FString mStrLanguage;
	FString mStrServerID;
	bool mUseAgeGate;
	FString mStrHiveOrientation;
	FString mStrHiveTheme;
	FString modeStrLanguage;
	FString modeStrServerID;

	/** 하이브 프로필 이미지 URL */
	FString _playerImageUrl;

	// 계정삭제 상태값	
	bool _isDeleteAccountSuccessed = false;
	// 계정변경 중?
	bool _isAccountChanging = false;

	/* 하이브 유저 추적용 이벤트 전송 핸들러 */
	TSharedPtr<class FGsHiveEventHandler> _hiveEventHandler;

	bool _InitializedHercules = false;

public:
	virtual void Initialize() override;
	virtual void InitializeAfterResourceDownload() override;
	virtual void Finalize() override;
	virtual void Update(float inTick) override;

	void SetConfig();
	void Init();
	void SignInHelper();
	void SignIn();
	void SignOut();
	void ShowSignIn();
	void AutoSignIn();
	void ShowConflictHelper();
	void ShowConflictHelperLobbyAndInGame(TArray<TMap<FString, FString>> ConflictPlayerGameData, TArray<FString> ServerNameArray, TArray<FString> UserNameArray);
	void CheckMaintenance(bool isRetry = false);

	bool IsInit();
	bool IsSignIn();
	bool IsAutoSignIn();

	PlayerID GetPlayerID();
	FString GetPlayerToken();
	FString GetDid();

	const PlayerInfo& GetConflictPlayer();

	void SetCurPlayerInfo();

	bool IsGuest();
	bool IsHive();
	bool IsGoogle();
	bool IsApple();
	bool IsFacebook();

public:
	bool IsDisplayFreeDiaCountry();

	// idp연동
	void Connect(EGsProviderType In_ProviderType);
	void DisConnect(EGsProviderType In_ProviderType);

	// push
	void PushSetForeground(bool bLocalPush, bool bRemotePush);
	void PushGetForeground();
	void PushSetRemote(bool bNotice, bool bNight);
	void PushGetRemote();

	bool IsPushNight();
	bool IsPushNotice();

	// Promotion
	void PromotionBANNER();
	void PromotionNEWS();
	void PromotionNOTICE();
	void ShowNews();
	void UpdatePromotionData();
	void ShowNativeReview();
	void ShowExit();
	void GetBadgeInfo();
	bool GetOfferwallState();
	void ShowOfferwall();
	void GetAppInvitationData();
	void ShowInquiry();
	void ShowTerms();
	void ShowProfile();
	void GetProfile();

	void CheckProvider(FString providerType);

	void ShowChatbotInquiry();
	void HelperGetIDPList(OUT TArray<FString>& outIDPList);

	void ResetConfig();
	FString ViewConfig();
	void SaveConfig();
	void SetHiveTheme(FString hiveThemeType);
	void SetHiveOrientation(FString hiveOrientation);
	void ConfigurationUpdateGameLanguage(FString language);
	FString GetSavedLanguage(FString langCode);
	FString GetConfigurationGetAppId();
	hive::Account GetCurrentAccountFromHive();
	void ConfigurationUpdateServerID(FString serverId);
	//void IAPV4purchase(FString strMarketPid, FString strAdditionalInfo);
	void SetEngagementReady(bool isReady);
	bool PromotionProcessURI(FString uri);

	void SetEngagementHandlerCB();
	void SetWindowsEngagementHandlerCB();
	void SetServerIdByUserSelectServer(const FString& inServerid);

	FString IAPV4TypeStringFromType(IAPV4Type argIapType);

	FString GetHiveCountry();
	FString GetChannel();
	FString GetMarket();

public:
	// 기기등록
	void ShowDeviceManagement();

public:
	void OnAccountChange();
	void ExitGame();

private:
	void OnEngagementCallback(const EGsEngagementCallbackType InType, FString InJsonValue);
	const bool IsMatchedCallbackTypeByKey(FString InJsonValue, FString InFindKey);

private:
	void CheckAuthenticationState(ResultAPI const& In_Result);

	FString convertResultAPIToFString(const hive::ResultAPI& reulstAPI);
	FString convertPromotionViewInfoToFString(const hive::PromotionViewInfo& info);
	FString convertPromotionBadgeInfoToFString(const hive::PromotionBadgeInfo& info);
	FString convertPromotionBannerInfoToFString(const hive::PromotionBannerInfo& info);

public:
	bool IsIdpConnected(EGsProviderType In_TypeValue);

	/*
	* 차후 hive에서 Hercules 처리를 위해 Hercules manager를 따로 만들지 않고 hive manager로 옮긴다
	* 현재는 단독 모듈 따로 사용
	* Hercules
	*/

protected:
	const FString registerHerculesAppName = TEXT("worldofzenonia");
	bool _herculesInit{ false };

protected:
	void Hercules_Initialize();
	void Hercules_Uninitialize();

	// 계정삭제
public:
	void SetIsDeleteAccountSuccess(bool In_Value) { _isDeleteAccountSuccessed = In_Value; }
	bool GetIsDeleteAccountSuccessed() const { return _isDeleteAccountSuccessed; }

public:
	bool IsSandBox();

public:
	void ShowCommunity();

	/************************************************************************/
	/* Setter		                                                        */
	/************************************************************************/
public:
	void SetPlayerImageUrl(FString&& InPlayerUrl) { _playerImageUrl = InPlayerUrl; }

	/************************************************************************/
	/* Getter			                                                    */
	/************************************************************************/
public:
	const FString& GetPlayerImageUrl() { return _playerImageUrl; }


	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
public:
	void AnalyticsSendEvent(const FString& inEventName);

	/************************************************************************/
	/* Event                                                                */
	/************************************************************************/

public:
	bool IsInitializedHercules() { return _InitializedHercules; }

};

using GsHiveManagerSingle = TGsSingleton<UGsHiveManager>;
#define GHive() GsHiveManagerSingle::Instance()