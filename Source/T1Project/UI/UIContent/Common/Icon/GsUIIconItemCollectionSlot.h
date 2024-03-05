#pragma once

#include "GsUIIconItemSimple.h"
#include "Item/Collection/EItemCollectionSlotState.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "DataSchema/GsSchemaEnums.h"


#include "GsUIIconItemCollectionSlot.generated.h"

class UImage;
class UWidget;
class UGsUIVFX;
class UWidgetSwitcher;
class UGsUIRedDotBase;

/**
 * 아이템 컬렉션 슬롯 전용 아이템 아이콘
 */
UCLASS()
class T1PROJECT_API UGsUIIconItemCollectionSlot : public UGsUIIconItemSimple
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UWidgetSwitcher* _swicherGrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UWidget* _selectImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UWidget* _widgetImpossible;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UGsUIRedDotBase* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UWidget* _widgetCompleted;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _skillOptionImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UTextBlock* _textPiledCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UGsUIVFX* _registeredEffect;

private:
	bool _isActiveShiftAbleBtn;

	CollectionConditionId _collectionConditionId = INVALID_COLLECTION_CONDITION_ID;

	/************************************************************************/
	/* instancing                                                           */
	/************************************************************************/
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/************************************************************************/
	/* setter                                                               */
	/************************************************************************/
protected:
	void SetItemGrade(ItemGrade InGrade) override;
	void SetItemSlotEffect(const EGsItemIconSlotEffectType InType) override;

private:
	void SetSkillOptionImage(const FGsItem* InItemData);

public:
	void SetEmptyIcon() override;
	void SetData(const class FGsItemCollectionCondition& InCondition);
	void SetSlotSelectionImage(const bool InActive);
	bool IsSelected() const;

	void PlayRegisteredEffect();

	CollectionConditionId GetCollectionConditionId() const {
		return _collectionConditionId;
	}
};