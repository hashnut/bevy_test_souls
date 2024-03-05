// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Message/GsMessageSystem.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "HIVE_AuthV4.h"
#include "Online/HTTP/Public/Http.h"
#include "GsUIOptionAccountPage.generated.h"

class UGsButton;
struct IGsMessageParam;
class UWidgetSwitcher;
class UImage;
class UTexture2D;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIOptionAccountPage : public UUserWidget
{
	GENERATED_BODY()

private:
	MsgSystemHandleArray					_listSystemDelegate;
	MsgSystemHandleArray					_listSystemIntDelegate;

protected:
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//class UGsButton* _btnGotoTitle;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//class UGsButton* _btnGotoLobby;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _canvasPanelName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textServerName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnCsCodeCopy;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textVID;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textGameVersion;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnHiveConnect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnFacebookConnect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGoogleConnect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnAppleConnect;


	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnHiveDisConnect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnFacebookDisConnect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGoogleDisConnect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnAppleDisConnect;


	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGameExit;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnDeleteAccount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnLogoutAccount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swicherBtnHive;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swicherBtnFacebook;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swicherBtnApple;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _swicherBtnGoogle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGameInquiry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGameTerms;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnUserProfile;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgUserProfile;


#if FLOW_DUBUG_TEST	
	int _testIndex = 0;
#endif

protected:
	EGsProviderType _disConnecctType;
	EGsProviderType _connecctType;

	/** 유저 프로필 이미지 갱신을 위한 Request */
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> _requestProfileImgDownload{ nullptr };

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	/** 계정 페이지 정보 세팅. 탭 전환시 프로필 이미지 갱신해 주어야 함 */
	void SetData();

protected:
	UFUNCTION()
	void OnClickGotoTitle();

	UFUNCTION()
	void OnClickGotoLobby();

	UFUNCTION()
	void OnClickHiveConnect();

	UFUNCTION()
	void OnClickFacebookConnect();

	UFUNCTION()
	void OnClickGoogleConnect();

	UFUNCTION()
	void OnClickAppleConnect();

	UFUNCTION()
	void OnClickGameExit();

	UFUNCTION()
	void OnClickDeleteAccount();

	UFUNCTION()
	void OnClickLogoutAccount();

	UFUNCTION()
	void OnClickHiveDisConnect();

	UFUNCTION()
	void OnClickFacebookDisConnect();

	UFUNCTION()
	void OnClickGoogleDisConnect();

	UFUNCTION()
	void OnClickAppleDisConnect();

	UFUNCTION()
	void OnClickCsCodeCopy();

	UFUNCTION()
	void OnClickInquiry();

	UFUNCTION()
	void OnClickTerms();

	UFUNCTION()
	void OnClickUserProfile();

protected:
	void GetVersion(OUT FString& outVersion);

	void HiveConnectFailed(int32 InParam);
	void HiveConnectSucceeded(int32 InParam);

	void HiveDisConnectSucceeded(int32 InParam);
	void HiveDisConnectFailed(int32 InParam);

	void HiveLogOutSucceeded(const struct IGsMessageParam* InParam);
	void HiveLogOutFailed(const struct IGsMessageParam* InParam);
	void HiveInfoUpdate();
	void HiveUserProfileUpdateRequest(const struct IGsMessageParam* InParam);
	void HiveIdpConflictPlayer(const struct IGsMessageParam* InParam);
	void ServerToRecvConflictPlayerInfo();
	void HIVESignOut(const struct IGsMessageParam* InParam);
	void HIVEIdepConnectCancle(const struct IGsMessageParam* InParam);

private:
	void GetConnectedType(OUT EGsProviderType& Out_ProviderType);
	void SetVisibleDeleteAccountBtn(bool In_Visible);
	void SetVisibleLogoutAccountBtn(bool In_Visible);
	void SetVisibleIdpBtn();
	void OnDisConnect();

	bool IsConnectedInType(EGsProviderType In_Type);	
	bool IsConnected();
	void OnIdpConnect(EGsProviderType In_Type);
	void OnIdpDisConnect(EGsProviderType In_Type);
	void SetIdpButtonSetting(bool In_IsOn, EGsProviderType In_ProviderTypeType);

	int GetConnectedCount();
	EGsProviderType GetClientProviderType(IN hive::ProviderType inHiveProviderType);
	class UWidgetSwitcher* GetIdpButtonInProviderType(EGsProviderType In_ProviderType);
	void SetVisibleIdpBtnInProviderType(EGsProviderType In_ProviderType, bool In_isVisible);

	void InvalidateConnectButtons();

	/** 프로필 이미지 url 요청 시, 다운로드 콜백 함수 */
	void OnHttpResponseProfileUpdate(FHttpRequestPtr httpRequest, FHttpResponsePtr httpResponse, bool isSuccess);

	/** 인자로 넘겨진 url 로 프로필 업데이트 시도. 기본값은 기본 이미지 링크이다 */
	void SetImageProfileWithUrl(const FString& InImageUrl = TEXT("https://sandbox-hive-fn.qpyou.cn/hubweb/avatar_img/public/noimage.png"));
};
