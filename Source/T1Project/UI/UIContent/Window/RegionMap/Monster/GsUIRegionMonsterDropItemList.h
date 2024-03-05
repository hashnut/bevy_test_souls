#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

#include "GsUIRegionMonsterDropItemList.generated.h"

/*
	지역맵: 몬스터 드롭 아이템 리스트
*/

class UGsDynamicPanelSlotHelper;
class FGsRegionMonsterInfo;

class UVerticalBox;
class UWidget;
class UScrollBox;

struct FGsSchemaItemCommon;

UCLASS()
class T1PROJECT_API UGsUIRegionMonsterDropItemList : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickMonsterDropItemList, int32);

public:
	FOnClickMonsterDropItemList OnClickMonsterDropItemList;

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

	// 몬스터 드롭 아이템 타입
	EGsRegionMonsterDropItemType _dropItemType;
	// 몬스터 인덱스
	int _monsterIndex;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void BeginDestroy() override;

	// 로직함수
public:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
	// 리스트 초기화
	void InitList(EGsRegionMonsterDropItemType In_type, int In_monsterIndex);

	// get
public:

	EGsRegionItemInfoSetType GetNowMapItemInfoSetType(FGsRegionMonsterInfo* In_monsterInfo);
};