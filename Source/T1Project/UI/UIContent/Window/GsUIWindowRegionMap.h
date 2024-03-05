#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Message/GsMessageGameObject.h"

#include "GsUIWindowRegionMap.generated.h"


/*
	window 지역맵
*/

class UGsUIRegionMap;
class UGsButton;

class UWidgetSwitcher;

UCLASS()
class UGsUIWindowRegionMap : public UGsUIWindow
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
	// 대륙맵 가는 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGoToContinentalMap;
	// 대륙맵 이름(현재 지역맵이 속한)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textContinentalMapName;
	// 지역맵 이름(일반)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textRegionMapNameNormal;
	// 지역맵 이름(던전)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textRegionMapNameDungeon;
	// 스위처: 상단 이름(0: 일반, 1: 던전)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherUpperName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionMap* _regionMap;

	// 메시지 관리 변수
private:
	MsgGameObjHandleArray _msgGameObjectHandleList;


	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

	virtual bool SyncContentState() override;
	virtual bool OnBack() override;

	// 메시지 바인딩/언바인딩
protected:
	void BindMessage();
	void UnBindMessage();

	// 로직 함수
public:
	// 지역맵 변경(현재 탭)
	// 프리징 이슈 대응
	// https://jira.com2us.com/jira/browse/C2URWQ-6455
	void ChangeRegionMap(bool In_byDropItem = false);

	// 지역맵 타입으로 변경(씬, 몬스터)
	void ChangeRegionMapByType(EGsRegionMapType In_type);
	// 상단 이름 변경
	void UpdateUpperName();

	// 대륙맵 클릭
	UFUNCTION()
	void OnClickContinentalMap();

	// 맵이 열려 있는 상태에서 몬스터 획득처로 이동 시, 상단과 콤보박스 UI 갱신 목적
	void OnMonsterDrop(const IGsMessageParam* InParam);
};