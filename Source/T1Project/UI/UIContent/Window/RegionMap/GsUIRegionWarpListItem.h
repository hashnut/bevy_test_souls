#pragma once

#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

#include "GsUIRegionWarpListItem.generated.h"

/*
	지역맵: 워프 리스트(설명, 포탈, 위치 즐겨찾기) 아이템
*/

class UGsButton;

class UImage;
class UWidgetSwitcher;

UCLASS()
class UGsUIRegionWarpListItem :
	public UUserWidget,
	public IGsToggleGroupEntry, 
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	// 스위처
	// 0: 설명
	// 1: 워프 아이템
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherType;

	// 설명
	// 타입 설명
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textTypeDesc;
	// 공백 설명
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textEmptyDesc;

	// 리스트 아이템 정보
	// 자동 이동 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoMove;
	// 텔레포트 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTeleport;
	// 아이템 버튼(전체)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItem;
	// 선택 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textName;
	// 스위치: 아이콘 (포탈, 위치 즐겨 찾기, portal in town)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherIcon;

	bool _bIsSelected = false;

	// 아이템 타입
	EGsRegionWarpListItemType _itemType = EGsRegionWarpListItemType::EmptyDesc;

	// 실제 선택 사용 인덱스
	int _realSelUseIndex = 0;

	// 가상 함수
public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

	// 로직 함수
public:
	UFUNCTION()
	void OnClickAutoMove();
	UFUNCTION()
	void OnClickTeleport();
	// 아이템 클릭
	UFUNCTION()
	void OnClickItem();
	// 텍스트 세팅
	void SetText(const FText& In_name);
	// 데이터 세팅
	void SetData(EGsRegionWarpListItemType In_type, const FText& In_name,
		int In_realSelUseIndex, bool In_isInTown = false);
};