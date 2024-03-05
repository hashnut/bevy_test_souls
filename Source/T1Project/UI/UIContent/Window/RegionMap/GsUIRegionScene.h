#pragma once
#include "CoreMinimal.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageUI.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsMonsterLevel.h"
#include "UI/UILib/Define/GsUIDefine.h"

#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "SlateCore/Public/Layout/Geometry.h"
#include "SlateCore/Public/Input/Events.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIRegionScene.generated.h"

class UWidgetSwitcher;
class UWidget;

class UGsRegionMapIconSelect;
class UGsSwitcherButton;
class UGsHorizontalBoxIconSelector;
class UGsButton;
class UGsRegionMapIconSelectSanctum;
class UGsRegionMapIconSelectMonster;

class UGsUISanctumBuffInfoRegionMap;
class UGsUIRegionIconFiltering;
class UGsUIRegionMonsterList;
class UGsUITooltipTitleDesc;
class UGsUIRegionSceneImage;
class UGsUIRegionIconRoot;
class UGsUIIconRegionMapInvasion;
class UGsUIRegionSceneSpoilsPanel;
class UGsUIIconAbnormality;

struct IGsMessageParam;
struct FGsSchemaPassivitySet;

UENUM()
enum class EGsRegionPortalType : uint8
{
	Portal,
	Dungeon,
	Invasion,
};

UCLASS()
class T1PROJECT_API UGsUIRegionScene : public UUserWidget
{
	GENERATED_BODY()

public:
	enum EGsButtonType
	{
		On,
		Off,
		Disable,
	};

		//좌측 몬스터 또는 포탈 패널
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherLeftList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionSceneSpoilsPanel* _spoilsPanel;
	// 몬스터 리스트
	// https://jira.com2us.com/jira/browse/CHR-23756
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionMonsterList* _monsterList;
	// 성소 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISanctumBuffInfoRegionMap* _sanctumInfo;
	//우측 맵 패널
protected:
	// 목적지 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnAutoMovePos;
	// 내위치 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnMyPos;
	// 몬스터 드랍 아이콘 제외한 아이콘들 보이기 토글 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherBtnMonsterDropFilter;

	// 씬 이미지 관리 하는넘
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionSceneImage* _sceneImage;
	// 아이콘 관리 하는넘
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionIconRoot* _iconRoot;

	// 환경 효과 아이콘
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelectorEnv;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelectorEnv2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray< UGsHorizontalBoxIconSelector*> _arrayIconSelectorEnv;

	// 환경 효과 툴팁
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipTitleDesc* _tooltipEnv;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipTitleDesc* _tooltipEnv2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<UGsUITooltipTitleDesc*> _arrayTooltipEnv;

	// 버튼 영역(던전일때 hide 할)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _btnSection;

	//아이템 필터
public:
	// filtering button
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFiltering;
	// icon filtering
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionIconFiltering* _iconFiltering;

private:
	// 이건 실제 이미지/ 실제 씬 사이즈로 비율은 낸거다
	float _uiSceneImageRate;

	// 현재 맵 포털 리스트 크기
	int _currentPortalListSize = 0;
	// 현재 맵 즐겨 찾기 리스트 크기
	int _currentBookmarkListSize = 0;

	// gameobject 메시지 처리용
	MsgGameObjHandleArray _arrGameObjDelegate;
	MsgUIHandleArray _uiMsgArray;

private:
	// 토글 그룹 관리
	FGsToggleGroup _toggleGroup;

private:
	int32 _selectPortalIndex = -1;
	int32 _selectBookmarkIndex = -1;

	// 선택된 침공 포탈 spot id
	int64 _selectInvasionPortalSpotId = -1;
	// 선택된 몬스터 드랍 인덱스
	int _selectMonsterDropIndex = -1;
	// 성소 선택 id
	SanctumAreaId _selectSanctumId = 0;

	// 좌측 스위치 인덱스: 획득처 
	const int LEFT_LIST_INDEX_SPOIL = 0;
	// 좌측 스위치 인덱스: 성소
	const int LEFT_LIST_INDEX_SANCTUM = 2;
	// 좌측 스위치 인덱스: 몬스터
	const int LEFT_LIST_INDEX_MONSTER = 1;
	// 현재 맵타입
	EGsRegionMapType _currentTabMapType = EGsRegionMapType::Spoils;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

	// 로직 함수
public:
	void InitializeMessage();
	void FinalizeMessage();

public:
	// 해당 씬 데이터로 세팅
	// 프리징 이슈 대응
	// https://jira.com2us.com/jira/browse/C2URWQ-6455
	void SetSceneData(EGsRegionMapType inMapType, bool In_byDropItem = false);
	// 탭변경에 의한 데이터 세팅
	// 기존에는 다 세팅하던거 클릭만 일어났을때 분리해서 처리
	void SetMapTypeTabChanged(EGsRegionMapType inMapType, bool In_byDropItem = false);

	// 데이터 초기화
	void InitRegionSceneData();

	// 통합 인덱스로 type과 data index 찾기
	void FindInfoByTotalIndex(int In_totalIndex, EGsRegionWarpListItemType& Out_itemType,
		int& Out_dataIndex);
	// 인포로 통합 인덱스 구하기
	int FindTotalIndexByInfo(EGsRegionWarpListItemType In_itemType,
		int In_dataIndex);

	// release select list and hide icon menu
	void ReleaseSelectList();

	// event call
public:
	void OnClickPortalIcon(UGsRegionMapIconSelect* In_clickedIconObj);	

	void OnClickInvasionPortalIcon(UGsUIIconRegionMapInvasion* In_icon, int64 In_spotId, FVector In_pos, FString In_text);

	void OnSelectPortal(const IGsMessageParam* inParam);

	void OnSelectBookmark(const  IGsMessageParam* inParam);

	void OnClickMonsterDropIcon(UGsRegionMapIconSelectMonster* In_clickedIconObj, FVector In_pos);

	// 성소 클릭
	void OnClickSanctum(UGsRegionMapIconSelectSanctum* In_clickSanctumObj);

	// 아무 성소아이콘 선택되게 처리(탭변경시 처리)
	void OnForcedSelectSanctum();

	// widget binding
public:
	// 목적지 클릭
	UFUNCTION()
	void OnClickAutoMovePos();
	// 내위치 클릭
	UFUNCTION()
	void OnClickMyPos();
	// click left list item	
	// 환경 속성 아이콘 클릭(버튼 위에 그려지므로 아이콘도 클릭처리 추가)
	UFUNCTION()
	void OnPressIconEnv(UGsUIIconAbnormality* InIcon);
	UFUNCTION()
	void OnPressIconInvasionInvadeEnv( UGsUIIconAbnormality* InIcon);
	UFUNCTION()
	void OnPressIconInvasionDefenseEnv( UGsUIIconAbnormality* InIcon);
	// click filtering btn
	UFUNCTION()
	void OnClickFiltering();

	// click MonsterDrop Filtering btn
	UFUNCTION()
	void OnClickMonsterDropFiltering();

	// 몬스터 리스트 선택
	// https://jira.com2us.com/jira/browse/CHR-23756
	UFUNCTION()
	void OnSelectMonsterList(int32 In_index);

	void OnInitMonsterDropFiltering(const IGsMessageParam* In_param = nullptr);
	void OnActiveMonsterDropFiltering(const IGsMessageParam* In_param = nullptr);
	void HideIconFilterring();	

private:
	void SetMonsterDropFiltering(EGsToggleType inToggleType = EGsToggleType::Auto);

public:
	// 포탈 id 관리
	void ResetSelectPortalIndex() { _selectPortalIndex = -1; }
	void SelectPortalIndex(int32 inIndex) { _selectPortalIndex = inIndex; }

	// 북마크 선택 버튼 id 관리
	void ResetSelectBookmarkIndex() { _selectBookmarkIndex = -1; }
	void SelectBookmarkIndex(int32 inIndex) { _selectBookmarkIndex = inIndex; }

	// 침공 넘어간 포탈 spot id 
	void SelectInvasionPortalSpotId(int64 In_spotId){ _selectInvasionPortalSpotId = In_spotId;}
	void ResetSelectInvasionPortalSpotId(){_selectInvasionPortalSpotId = -1;}

	// 몬스터 드랍 id 관리
	void SelectMonsterDropId(int In_id){_selectMonsterDropIndex = In_id;}
	void ResetSelectMonsterDropId(){_selectMonsterDropIndex = -1;}

	// 성소 선택 id 관리
	void SelectSanctumId(SanctumAreaId In_id) {_selectSanctumId = In_id;}
	void ResetSelectSanctumId();


	// get, set
protected:
	void SetEnvIcon(int32 InMapId);
	const FGsSchemaPassivitySet* GetEnvPassivitySetByMapId(int32 InMapId) const;

	UGsUIIconAbnormality* GetEnvIconByIndex(int In_index);

	UGsUITooltipTitleDesc* GetEnvTooltipByIndex(int In_index);

	const FGsSchemaPassivitySet* GetInvadeEnvPassivitySetByMapId(int32 InMapId, bool In_isInvade) const;

public:
	void OnShowItemDrop(const IGsMessageParam* inParam);
	void OnHideItemDrop(const IGsMessageParam* inParam);
	void ShowItemDrop(const TArray<FGsSchemaWorldMapSpoilsMonsterLevel>& inData);
	void HideItemDrop();
};