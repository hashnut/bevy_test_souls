#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Message/GsMessageUI.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIPopupMonsterInfo.generated.h"

class UGsDynamicPanelSlotHelper;
class UGsButton;
class UGsUIMonsterPortrait;
class UGsUIMonsterInfoCombobox;

class UVerticalBox;
class UScrollBox;
class UWidget;
class UWidgetSwitcher;

struct IGsMessageParam;


// 몬스터 정보(지역맵에서 있던 그것)
// 선공 or 비선공, 약점, 보상템등 표시


UCLASS()
class T1PROJECT_API UGsUIPopupMonsterInfo : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;
	// 스크롤 박스
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	// 몬스터 인덱스
	int _monsterIndex;

	// 몬스터 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMonsterName;
	// 몬스터 등급(일반, 엘리트, 보스)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMonsterGrade;
	// 몬스터 선공 여부(선공, 비선공)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMonsterOffensive;
	// 몬스터 종족
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMonsterCreatureRace;
	// 몬스터 약점 재질
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMonsterWeaknessMaterial;
	// 몬스터 속성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textMonsterElement;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOK;

	// 몬스터 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterPortrait* _monsterPortraitBP;
	// 리스트 타입 스위처
	// 0: 리스트 있음
	// 1: 리스트 없음
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherListType;

	// 콤보 박스
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterInfoCombobox* _comboboxMonsterInfo;
private:
	MsgUIHandleArray _msgUIHandlerList;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 이건 back 키
	// https://jira.com2us.com/jira/browse/C2URWQ-7203
	virtual bool OnBack() override;
	// 이건 esc 키
	// https://jira.com2us.com/jira/browse/C2URWQ-7203
	virtual void OnInputCancel() override;

public:
	virtual void BeginDestroy() override;

	// 이벤트
public:
	// 레벨 변경
	void OnSelectLevelPopupMonsterInfo(const IGsMessageParam* InParam);

	// 로직함수
public:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnClickOK();
	// 몬스터 리스트 초기화
	void InitMonsterInfoList(int In_monsterIndex, bool In_isShowLevelCombobx);
};
