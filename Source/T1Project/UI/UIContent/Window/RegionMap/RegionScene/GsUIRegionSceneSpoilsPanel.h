#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Map/WorldMap/GsWorldMapDefine.h"
#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

#include "SlateCore/Public/Layout/Geometry.h"
#include "SlateCore/Public/Input/Events.h"

#include "Message/GsMessageGameObject.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsItem.h"

#include "GsUIRegionSceneSpoilsPanel.generated.h"

class UPanelWidget;
class UUserWidget;
class UWidgetSwitcher;
class UVerticalBox;
class UScrollBox;

class UGsDynamicPanelSlotHelper;

struct FGsSchemaItemCommon;

UCLASS()
class UGsUIRegionSceneSpoilsPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _equipSpoilVerticalBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _equipSpoilScrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _equipSpoilSwitcher;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _equipScrollBoxHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _etcSpoilVerticalBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _etcSpoilScrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _etcSpoilSwitcher;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _etcScrollBoxHelper;	
		
	// 종류 탭(장비, 기타)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelTab;

private:
	// 토글 그룹 관리
	FGsToggleGroup _equipSpoilToggleGroup;
	// 토글 그룹 관리
	FGsToggleGroup _etcSpoilToggleGroup;

	// 토글 그룹 관리(기존 장비, 기타 통합본)
	FGsToggleGroup _spoilToggleGroup;

	// 탭(장비, 기타) 선택 토글 그룹
	FGsToggleGroup _spoilTabToggleGroup;

private:
	int32 _equipItemNum;
	int32 _etcItemNum;	
	FGsSchemaWorldMapSpoilsItem _selectItem;
	const struct FGsSchemaWorldMapSpoilsData* _spoilsData;
	// 현재 탭 타입
	EGsWorldMapSpoilTabType _spoilTabType = EGsWorldMapSpoilTabType::Equip;

public:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;

public:
	void InitializeRegionSceneSpoilsPanelWidget();

private:
	void InitSpoil();
	void InitEquipSpoil();
	void InitEtcSpoil();
	
	UFUNCTION()
	void OnCreateEquipItemEntry(class UWidget* inWidget);
	UFUNCTION()
	void OnCreateEtcItemEntry(class UWidget* inWidget);
	UFUNCTION()
	void OnRefreshEquipItemEntry(int32 InIndex, class UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEtcItemEntry(int32 InIndex, class UWidget* InEntry);

	// 리스트 하나로 변경
	// https://jira.com2us.com/jira/browse/CHR-25993
	UFUNCTION()
	void OnCreateItemEntry(class UWidget* inWidget);
	UFUNCTION()
	void OnRefreshItemEntry(int32 InIndex, class UWidget* InEntry);

	// 탭 클릭
	UFUNCTION()
	void OnClickTab(int32 In_index);

	void ShowItemDrop(const FGsSchemaWorldMapSpoilsItem& inItem);
	void PreSelectIndex();

	// get
private:
	// 현재 인덱스로 아이템 테이블 접근
	const FGsSchemaItemCommon* GetItemCommon(int In_listIndex);
};