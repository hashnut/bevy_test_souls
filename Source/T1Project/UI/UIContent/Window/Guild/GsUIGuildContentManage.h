// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Popup/GsUIPopupGuildChangeSnsUrl.h"

#include "GsUIGuildContentManage.generated.h"

class UGsButton;
class UImage;
class UListView;
class UTextBlock;
class UScrollBox;
class UPanelWidget;
class UGsUIRedDotBase;
class UGsUIGuildSnsUrlListItem;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIGuildContentManage : public UGsUIGuildContentBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgEmblem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLimitLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLimitLevel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelAcquisition;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAcquisitionInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelJoinType;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	UTextBlock* _textBlockIntroduce;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _snsLinkListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollNotice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	UTextBlock* _textBlockNotice;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyIntroduce;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyNotice;

	// 권한버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAuthority;
	// 승인버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnApproval;
	// 해산버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDisband;
	// 탈퇴버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLeave;	
	// 소개 변경
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnIntroduce;
	// 공지 변경
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNotice;
	// 문장 변경
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEmblem;
	// 획득 방식 설정 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAcquisition;

	// 승인버튼 레드닷
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotApproval;
	// 획득 방식 설정 버튼 레드닷
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotAcquisition;

protected:
	FGsToggleGroup _toggleGroupJoinType;
	const class FGsGuildDataMine* _guildData = nullptr;
	bool _bClickedApproval = false;
	TArray<UGsUIGuildSnsUrlListItem*> _snsUrlListItems;

public:
	virtual FString GetTabName() const override { return TEXT("Manage"); }

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	virtual void SendRequestPacket() override;
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;
	virtual void Hide() override;

	virtual void OnChangeMyAuthority() override;	
	virtual void OnReceiveChangeGuildSetting(const struct FGsGuildMsgParamSetting* InParam) override;	
	virtual void OnNotifyChangeNotice() override;

public:
	// 승인 대기 유저 리스트 받았을 경우 호출
	void OnReceiveJoinWaitingUserList();
	// 가입 요청 노티가 왔을 경우
	void OnNotifyJoinRequest();
	// 가입 승인/거절 응답 받음
	void OnReceiveJoinApproveResult();
	// 권한 리스트 받았을 경우 호출
	void OnReceiveAuthorityList();	

protected:
	void InvalidateAuthority();
	void InvalidateAcquisition();
	TArray<UGsUIGuildSnsUrlListItem*> MakeSnsLinkListItems(const bool InIsMyGuild);
	void RequestInvalidateSnsLinks();
	void InvalidateSnsLinks();
	bool IsEnableDisband(bool bInShowMessage = true) const;
	bool IsEnableLeave(bool bInShowMessage = true) const;

public:
	/** SNS 링크 비속어 필터링 후에 사용하기 위한 함수. */
	void UpdateSnsLink(const int32 InIndex, const FString& InSnsLink);

protected:
	UFUNCTION()
	void OnClickLimitLevel();
	UFUNCTION()
	void OnClickAuthority();
	UFUNCTION()
	void OnClickApproval();
	UFUNCTION()
	void OnClickDisband();
	UFUNCTION()
	void OnClickLeave();
	UFUNCTION()
	void OnClickIntroduce();
	UFUNCTION()
	void OnClickNotice();
	UFUNCTION()
	void OnClickEmblem();
	UFUNCTION()
	void OnClickAcquisition();
	UFUNCTION()
	void OnClickSnsLink(const int32 InIndex);
	void OnClosePopupGuildChangeSnsLink(const UGsUIPopupGuildChangeSnsUrl::Parameters* InParameters);

	void OnSelectJoinType(int32 InIndex);

protected:
	void SetUIGuildEmblem(GuildEmblemId InEmblemId);
	void SetUIGuildIntroduce(const FString& InIntroduce);
	void SetUIGuildNotice(const FString& InNotice);
	void UpdateRedDotApproval();
	void UpdateRedDotAcquisition();
};