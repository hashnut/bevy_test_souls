#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIHUD.h"

#include "Message/GsMessageGameObject.h"

#include "GsUIHUDParty.generated.h"

/*
	HUD 파티
*/
class UGsButton;
class UGsDynamicPanelSlotHelper;
class UGsSwitcherButton;

class UWidgetSwitcher;
class UVerticalBox;
class UWidget;
class UTextBlock;
class URichTextBlock;

struct IGsMessageParam;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDParty : public UGsUIHUD 
{
	GENERATED_BODY()

	// 2022/06/20 PKT - 파티 설명 타입
	enum class EInfoDescType
	{
		Normal,		// 2022/06/20 PKT - 일반 파티
		Dungeon,	// 2022/06/20 PKT - 파티 던전
	};


	enum class ESWITCER_ADDIN_INDEX
	{
		Shotcut,			// 2022/06/20 PKT - 바로 가기
		AskHelpEnable,		// 2022/06/20 PKT - 헬프 요청 : 활성화
		AskHelpUnEnable,	// 2022/06/20 PKT - 헬프 요청 : 비활성화
	};

	enum class ESWITCHER_READY_INDEX
	{
		Start,				// 2022/06/20 PKT - 시작하기
		ReadyOn,			// 2022/06/20 PKT - Ready On
		ReadyCancel,		// 2022/06/20 PKT - Ready Cancel
	};

protected:

	// 파티 참여했는지 스위치
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPartyJoinState;
	
	/**
	 * 2022/06/16 PKT - 파티 정보 설명
	 * 0 : 파티
	 * 1 : 매칭 중
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherWidgetInfoDesc;

	/**
	 * 2022/06/16 PKT - 파티 정보 설명
	 * 0 : 일반 파티
	 * 1 : 던전 파티
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherWidgetPartyDesc;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textNormalPartyDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _richTextDungeonPartyDesc;
	

	// 멤버 리스트 원본
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOpenSetting;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLeave;
	// 멤버 리스트 만드는 넘
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textPartyInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _autoMatchingOn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherButtonPartyDungonWork;

	// switcher
	// 0: party list
	// 1: request help(enable)
	// 2: request help(disable)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherWidgetAddIn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherButtonPartyDungonStart;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textAskHelpEnable;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textAskHelpDisable;

	// switcher button
	// 0: stat(leader)
	// 1: ready(party member)
	// 2: ready success(party member)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherWidgetReadty;

	// 메시지 관리
	MsgGameObjHandleArray _arrDelegateGameObjectMsg;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

	// 메시지
public:
	// 모든 리스트 변경
	void OnAllListChange(const IGsMessageParam*);
	// 한개 리스트 변경
	void OnOneListChange(const IGsMessageParam* In_param);
	void OnPartyInfoChange(const IGsMessageParam*);
	void OnAddAbnormality(const IGsMessageParam* InParam);
	void OnRemoveAbnormality(const IGsMessageParam* InParam);

	// ui 이벤트
protected:
	UFUNCTION()
	void OnMemberListRefreshEntry(int32 InIndex, UWidget* InEntry);
	// open setting popup
	UFUNCTION()
	void OnClickOpenSetting();
	// leave
	UFUNCTION()
	void OnClickLeave();
	// 2022/06/17 PKT - 매칭 중 클릭
	UFUNCTION()
	void OnClickMatchingOn();
	// 2022/06/14 PKT - 던전 파티 전용 좌측 상단 클릭
	UFUNCTION()
	void OnClickAddIn();
	// 2022/06/14 PKT - 던전 파티 전용 좌측 하단 클릭
	UFUNCTION()
	void OnClickReady();

protected:
	// ui 갱신(스위치와 멤버 리스트)
	void RefreshUI();
	// 2022/06/16 PKT - 파티 정보
	void InvalidateInfo();
	// 2022/06/16 PKT - 파티 정보 설명	
	void InvalidateInfoDesc(EInfoDescType InType);
	// 2022/06/16 PKT - 던전 파티 관련 좌측 상단 버튼
	void InvalidateAddIn();
	// 2022/06/16 PKT - 던전 파티 관련 좌측 하단 버튼
	void InvalidateReady();

protected:
	// 2022/06/24 PKT - 파티 던전 전용 좌측 상단 UI 세팅
	bool DisplayPartyDungeonAddIn();
	// 2022/06/24 PKT - 파티 던전 전용 좌측 하단 UI 세팅
	bool DisplayPartyDungeonReady();
	
protected:
	void SetTextPartyInfo(EInfoDescType InType);
	// 멤버 리스트 데이터 세팅
	void SetMemberListData();
	// 상태이상 컨퓨즈(내 HP/MP 정보 가림)
	void UpdateAbnormalityConfuse();

public:
	// 모든 드로우 메뉴 닫기
	void CloseAllDrawMenu();	
};
