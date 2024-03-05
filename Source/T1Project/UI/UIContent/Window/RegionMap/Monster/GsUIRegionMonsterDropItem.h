#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "GsUIRegionMonsterDropItem.generated.h"

/*
	지역맵: 몬스터 드롭 아이템 리스트 아이템
*/

class UGsUIIconBase;
class UGsUIIconItemInventory;
class UGsButton;
class UGsHorizontalBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsUIIconItem;

class UTextBlock;

UCLASS()
class UGsUIRegionMonsterDropItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textName;

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _targetItemIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _targetIconSlotHelper;
	// 이름 색상 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	// 아이템 tbl id
	int _itemTblId = 0;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	virtual void BeginDestroy() override;

	// 아이콘 초기화
	virtual void InitItemIcon(UGsUIIconItemInventory* In_icon);

	// 로직 함수
public:
	void SetText(const FText& In_name);

	UFUNCTION()
	void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateTargetIcon(UGsUIIconBase* InIcon);
	// 아이템 id 세팅
	void SetItemTblId(int In_tblId, const FText& In_name, ItemGrade In_itemGrade);

	void OnLongPressIconSlot( UGsUIIconItem& InIcon);
};