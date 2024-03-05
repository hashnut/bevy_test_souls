#pragma once
#include "CoreMinimal.h"
#include "GsUIRegionMonsterDropItem.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsUIRegionSpoilsEntry.generated.h"

/*
	지역맵: 몬스터 드롭 아이템 리스트 아이템
*/
class UGsUIIconBase;
class UGsUIIconItemInventory;

UCLASS()
class UGsUIRegionSpoilsEntry : public UGsUIRegionMonsterDropItem, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:	
	// 이름 색상 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSelected;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSpoils;

public:
	TFunction<void(int32)> _clickEvent;

private:
	UPROPERTY()
	bool _isSelect;


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
public:
	virtual void SetIsSelected(bool inSelect) override;
	virtual bool GetIsSelected() const override;
	
public:
	UFUNCTION()
	void OnClickSelect();
};