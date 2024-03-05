#pragma once

#include "CoreMinimal.h"

#include "Map/GsMapIconType.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "SlateCore/Public/Layout/Geometry.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIContinentalMap.generated.h"

class UVerticalBox;
class UWidget;
class UPanelWidget;
class UCanvasPanel;
class UWidgetAnimation;
class UUserWidget;

class UGsButton;
class UGsDynamicPanelSlotHelper;
class UGsWidgetPoolManager;
class UGsUIContinentalBP;
class UGsUIIconBase;
class UGsUIContinentalName;
class UGsUIRegionItem;
/*
	대륙맵
*/

UCLASS()
class T1PROJECT_API UGsUIContinentalMap : public UUserWidget 
{
	GENERATED_BODY()

protected:
	// 대륙 리스트 아이템 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _continentalListItemClass;

	// 대륙 리스트 버티컬 박스(패널)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _continentalListVerticalBox;

	// 대륙 bp 로드해서 붙일곳
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _continentalBPRoot;
	// 사라지는 연출 root
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _disappearBPRoot;
	// 대륙 루프 bp 로드해서 붙일곳
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _continentalLoopBPRoot;

	// 대륙 리스트 아이템 생성하는 넘
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _continentalListScrollBoxHelper;

	// 로드한 대륙 BP
	UPROPERTY()
	UGsUIContinentalBP* _loadedContinentalBP;
	// 이전 bp
	UPROPERTY()
	UGsUIContinentalBP* _beforeContinentalBP;

	// 로드한 loop BP
	UPROPERTY()
	UUserWidget* _loadedContinentalLoopBP;


	// 대륙 BP 풀매니저
	TWeakObjectPtr<UGsWidgetPoolManager> _continentalBPPoolManager;

	// 사라지는 애니매이션
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _disappearAnimation;
	// 추가되는 애니매이션
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _appearAnimation;

	// 관리되는 아이콘 들
	UPROPERTY()
	TArray<UGsUIIconBase*> _arrayIcons;
	// 키값이 id(검색용)
	TMap<int64, UGsUIIconBase*> _mapIcons;

public:
	// 리스트 선택 콜백
	TFunction<void(int)> _callbackSelectList;
	
private:
	// 눌렀는가
	bool _isPressed = false;
	// 이전 마우스 위치
	FVector2D _oldMousePos = FVector2D::ZeroVector;
	// BP 붙일 캔버스 크기
	FVector2D _BPRootCanvasSizes;
	bool _isInitSize = false;
	// BP 크기
	FVector2D _BPSizes;
	// bp와 root 차이값(root size - bp size)
	// 보통은 bp가 더 커야함(음수여야함)
	FVector2D _RootBPDiffSize;

	// https://jira.com2us.com/jira/browse/C2URWQ-4716
	// diff 값이 0보다 더 크면 diff가 max 그외는 diff가 min
	FVector2D _moveMin;
	FVector2D _moveMax;

	// bp 크기 구했나
	bool _isBPInitSize = false;
	// 현재 선택된 대륙 id
	int _currentContinentalId = 0;
	// current index
	int _currentMapSelectIndex = 0;
	// 터치 누르고 있는지
	bool _isTouchPress = false;

	bool _isDirtyFirstShowRegionItemCenter = false;

	UGsUIRegionItem* _currentSelDungeonItem;

	// 맵 이동 보간 처리
private:

	// 끝났는지
	bool _isMapMoveFinish = true;
	// 진행 누적값
	float _mapMoveProgressRate = 0.0f;

	// 위치 목표
	FVector2D _mapMovePosTo = FVector2D::ZeroVector;
	// 위치 시작점
	FVector2D _mapMovePosFrom = FVector2D::ZeroVector;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void BeginDestroy() override;

	// 로직 함수
public:
	void TrySelectNextMap(int In_addIndex);

protected:
	// select map	
	void SelectContinentalMap(int32 In_index);
	
	// 대륙 BP 만들기
	UUserWidget* MakeContinentalBP(const FSoftObjectPath& In_path);
	UUserWidget* Claim(UClass* In_class);
	// 대륙 BP 해제
	void ReleaseContinentalBP(UUserWidget* In_BP);
	// 대륙 bp 사라지는 연출 시작
	void StartDisappearBP(UGsUIContinentalBP* In_BP);
	// 대륙 bp 생기는 연출 시작
	void StartAppearBP();
	// 연출 끝
	UFUNCTION()
	void FinishDiappearBP();

	// root 캔버스 크기 가져오기
	void InitRootCanvasSize();
	// bp size 구하기
	void InitBPSize();

	// 현재맵 모든 아이콘 로드
	void LoadAllIcon();
	// 현재맵 모든 아이콘 해제
	void ReleaseAllIcon();

	// icon 만들기
	void SpawnIcon(int64 In_id, EGsContinentalMapIconType In_iconType, int In_mapId);
	// icon 삭제
	void DespawnIcon(int64 In_id);
	// 아이콘 만들기
	UGsUIIconBase* CreateMapIcon(EGsContinentalMapIconType In_iconType);
	// 드래그 이벤트 연결
	void AddDragEvent();
	// 드래그 이벤트 해제
	void RemoveDragEvent();
	// 맵 이동 갱신
	void UpdateMapMove(float In_delta);

	void MakeCenterCurrentRegionItem();

	void TryMakeCenterRegionItem();

	void CheckDirtyCenterRegionItem();
	
	// event
public:
	UFUNCTION()
	void OnBPTouchMoved(const FVector2D& In_pos);
	UFUNCTION()
	void OnBPTouchStarted(const FVector2D& In_pos);
	UFUNCTION()
	void OnBPTouchEnded(const FVector2D& In_pos);

public:
	// 지역 아이템 클릭
	void MoveCenterRegionItem(UUserWidget* In_widget, bool In_isTargetShowed = true);

	void MakeCenterRegionMapItem(int In_regionMapId, bool In_isTargetShowed = true);

	// get
public:

	// bp 위치 괜찮은지(canvas 내인지)
	bool IsBPPosOK(const FVector2D In_pos, FVector2D& Out_pos);

	bool GetContinentalBPMovePos(UUserWidget* In_widget, FVector2D& Out_movePos, bool In_isTargetShowed = true);

	bool IsCanMoveCenter(UUserWidget* In_widget);
	int GetCurrentContinentalId()
	{
		return _currentContinentalId;
	}

	void SetSelectDungeonItem(UGsUIRegionItem* In_item);

	bool IsShowMapChangeButton(bool In_isLeft);

public:
	//침공 정보 갱신
	void UpdateInvaderCount(const TArray<MapId>& updateMapList);
	void UpdatePortalCount(MapId id, int32 count, int32 maxCount);
};
