#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Message/GsMessageGameObject.h"

#include "GsUIPopupPartySetting.generated.h"


/*
	Poup 파티 설정
*/

class UUserWidget;
class UPanelWidget;
class UVerticalBox;
class UWidgetSwitcher;
class UWidget;

class UGsDynamicPanelSlotHelper;
class UGsButton;
class UGsUIPartySettingTab;
class UGsUIPartyOwnTab;

struct IGsMessageParam;

UCLASS()
class T1PROJECT_API UGsUIPopupPartySetting : public UGsUIPopup
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;
	
	// 
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;
	// 탭 스위치
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSettingType;

	// ok 버튼 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textOkBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	//// 최상위 탭(설정, 멤버)	
	//// 설정 탭 패널
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UPanelWidget* _panelSettingTab;
	//// 탭 설정
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIPartySettingTab* _tabSetting;
	//// 탭 멤버
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIPartySettingTab* _tabMember;

	//// 토글 그룹 관리
	//FGsToggleGroup _toggleGroup;


	// 획득 룰 탭(랜덤, 순차, 자유)
	// 획득 룰 탭 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelOwnTab;
	// 랜덤 탭
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartyOwnTab* _tabRandom;
	// 순차 탭
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartyOwnTab* _tabSequence;
	// 자유 탭
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPartyOwnTab* _tabFree;
	// 구글 피처드 이슈: 빈화면 출력 처리
	// https://jira.com2us.com/jira/browse/CHR-20622
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _noList;
	// 획득룰 탭 관리용 배열
	TArray<UGsUIPartyOwnTab*> _arrayOwnTab;

	// 획득 룰 토글 그룹 관리
	FGsToggleGroup _toggleGroupOwn;

	// 파티 획득룰
	PartyDropOwnershipType _partyDropOwnershipType;

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

	// 로직 함수
protected:
	UFUNCTION()
	void OnMemberListRefreshEntry(int32 InIndex, UWidget* InEntry);
	// 탭 클릭
	UFUNCTION()
	void OnClickTab(int32 In_index);
	// 획득 룰 탭 클릭
	UFUNCTION()
	void OnClickOwnTab(int32 In_index);
	// ok 클릭
	UFUNCTION()
	void OnClickOk();
	// cancel 클릭
	UFUNCTION()
	void OnClickCancel();
	// 멤버 리스트 데이터 세팅
	void SetMemberListData();
	// 탭 초기화
	void InitTab();
	// ok 버튼 초기화
	void InitOkButton();

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};