#pragma once

#include "CoreMinimal.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "UMG/Public/Blueprint/UserWidget.h"

#include "Message/GsMessageGameObject.h"

#include "GsUIRegionMap.generated.h"

/*
	지역맵
*/
class UWidgetSwitcher;

class UGsUIRegionTab;
class UGsUIRegionScene;
class UGsUIRegionMonster;
class UGsUIRegionCombobox;

UCLASS()
class UGsUIRegionMap : public UUserWidget
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	// 탭 스위치
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherRegionType;
	// 탭 패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelTab;
	//전리품
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionTab* _tabSpoils;
	// 탭 몬스터
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionTab* _tabMonster;
	// 탬: 성소 
	// https://jira.com2us.com/jira/browse/CHR-23698
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionTab* _tabSanctum;

	// 컨텐츠 타입에따라 보여줌 처리
	// 0: 보여줌
	// 1: 해당 컨텐츠는 안보여줌
	// https://jira.com2us.com/jira/browse/C2URWQ-7853
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherContentsType;

protected:
	// 씬 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionScene* _regionScene;
	// 몬스터 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionMonster* _regionMonster;


	// 콤보 박스
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionCombobox* _combobox;

	// 토글 그룹 관리
	FGsToggleGroup _toggleGroup;

	// gameobject 메시지 처리용
	MsgGameObjHandleArray _msgGameObjectHandleList;

public:
	const int TAB_SPOILS		= 0;	// 전리품 탭
	const int TAB_MONSTER_LIST	= 1;	// 몬스터 리스트 탭
	const int TAB_SANCTUM		= 2;	// 성소 탭

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

	// 로직 함수
protected:
	// 탭 클릭
	UFUNCTION()
	void OnClickTab(int32 In_index);
	UFUNCTION()
	void OnLeavePreSelected(int32 In_index);

	void OnClickRegionMapTypeTab(const IGsMessageParam* InParam);

public:
	void InitializeMessage();
	void FinalizeMessage();

public:
	// 지역맵 변경(현재 탭껄로 갱신)
	// 프리징 이슈 대응
	// https://jira.com2us.com/jira/browse/C2URWQ-6455
	void ChangeRegionMap(bool In_byDropItem = false);
	// 지역맵 타입으로 변경(씬, 몬스터)
	void ChangeRegionMapByType(EGsRegionMapType In_type);

	// 타입 데이터 세팅
	// 프리징 이슈 대응
	// https://jira.com2us.com/jira/browse/C2URWQ-6455
	void SetRegionMapTypeData(EGsRegionMapType In_type, bool In_byDropItem = false);

	void SelectPanelType(EGsRegionMapType inType);
	// 성소 정보 요청
	void SendSanctumInfoReq();
};