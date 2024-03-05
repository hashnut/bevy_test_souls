#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Message/GsMessageUI.h"
#include "GsUIWindowContinentalMap.generated.h"

/*
	window 대륙맵
*/

class UGsButton;
class UGsUIContinentalMap;
class UGsUIContinentalDungeonSide;
class UGsUIContinentalName;
class UGsUIContinentalSpaceCrackList;
class UGsUIRegionItem;

class UUserWidget;
class UPanelWidget;
class UWidgetSwitcher;

struct IGsMessageParam;

UCLASS()
class T1PROJECT_API UGsUIWindowContinentalMap : public UGsUIWindow
{
	GENERATED_BODY()
	// 멤버 변수
protected: 
	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	// 백 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	// 목적지 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoMovePos;
	// 내위치 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMyPos;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _toggleBtnSpaceCrack;
	// 대륙맵 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textContinentalMapName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIContinentalMap* _continentalMap;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIContinentalDungeonSide* _dungeonSide;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _invadeInfoPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _invasionWorld;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _invasionTime;

	MsgUIHandleArray		_msgHandleUIList;

	// name bp
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIContinentalName* _nameBP;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMapSelectLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMapSelectRight;
	// space crack list
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIContinentalSpaceCrackList* _spaceCrackList;
	// space crack btn toggle image
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSpaceCrackBtn;


	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool OnBack() override;

	// 스택에 의해 이 창이 복구될 때 호출
	virtual bool SyncContentState() override;

	// init function
private:
	void RegisterMessage();
	void RemoveMessage();


	// bp call function
public:
	// 목적지 클릭
	UFUNCTION()
	void OnClickAutoMovePos();
	// 내위치 클릭
	UFUNCTION()
	void OnClickMyPos();
	UFUNCTION()
	void OnClickMapSelectLeft();
	UFUNCTION()
	void OnClickMapSelectRight();
	UFUNCTION()
	void OnClickSpaceCrack();

	// 로직 함수
public:
	// continental name update
	void UpdateContinentalName(int In_continentalTblId);
	// 던전 우측 켤지 말지
	void ShowDungeonSide(bool In_isShow);

	// 지역 아이템 클릭
	void MoveCenterRegionItem(UUserWidget* In_widget);

	void InitializeSpaceCrackInfo();

	void MakeCenterRegionMapItem(int In_regionMapId, bool In_isTargetShowed);

	void ReqInvasionInfo(bool reqStart = true);

	void UpdateSelectButtonState();

	void ChangeMapProc(int In_continentalTblId);

	// 시공의 틈새 버튼 보일지 처리(현재 대륙에 시공의 틈새가 있으면 표시, 없으면 표시 안함)
	void UpdateSpaceCrackButton(int In_continentalTblId);

	// bind message 
public:
	void OnUpdateInvasionInfo(const IGsMessageParam* InParam);

	// set, get
public:
	// 던전 입구 이름 세팅
	void SetDungeonEntranceName(const FText& In_name);
	// 던전 side가 보이는가
	bool GetDungeonSideShow();

	bool IsCanMoveCenter(UUserWidget* In_widget);

	void SetSelectDungeonItem(UGsUIRegionItem* In_item);

	void SetInvasionInfo();
	void SetInvasionRemainTime(FTimespan remainTime);
};