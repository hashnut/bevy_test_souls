#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIRegionMonster.generated.h"

class FGsRegionMonsterInfo;
class UGsUIRegionMonsterList;
class UGsUIIconBase;
class UGsUITooltipTitleDesc;
class UGsButton;
class UGsUIIconRegionMonsterMaterial;
class UGsUIRegionMonsterDropItemList;
class UGsUIMonsterPortrait;

class UImage;
class UWidgetSwitcher;
class UListView;
class UTextBlock;

struct FGsSchemaFieldBossData;

UCLASS()
class T1PROJECT_API UGsUIRegionMonster : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 몬스터 리스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionMonsterList* _monsterList;
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
	// 드롭 아이템 (장착)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionMonsterDropItemList* _dropItemEquip;
	// 드롭 아이템 (기타)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionMonsterDropItemList* _dropItemEtc;
	// 몬스터 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterPortrait* _monsterPortraitBP;
	// spawn time switcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _spawnTimeWidgetSwitcher;
	// cooperation spawn time
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UListView* _cooperationSpawnTimeListView;
	// competition interval
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _competitionSpawnIntervalTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMoveBossInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidget* _monsterInfoPanel;

private:
	int32 _currentFieldBossId = 0;


	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;

	// 로직 함수
public:
	// 해당 씬 데이터로 세팅
	void SetSceneData();
	// info 정보 초기화
	void InitInfo(FGsRegionMonsterInfo* In_monsterInfo);
	void SetFieldBossTimeInfo(const FGsSchemaFieldBossData* In_fieldBossData);

	// 리스트 선택
	UFUNCTION()
	void OnSelectMonsterList(int32 In_index);
	UFUNCTION()
	void OnClickMoveBossInfo();
};