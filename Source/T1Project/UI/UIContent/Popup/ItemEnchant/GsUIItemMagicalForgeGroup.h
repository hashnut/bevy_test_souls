// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIItemMagicalForgeGroup.generated.h"

class UGsButton;
class UTextBlock;
class UPanelWidget;
class UGsUIIconBase;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class FGsItemEquipment;
class FGsItemIngredient;
class UGsUIItemMagicalForgeEntry;
struct FGsSchemaEffectTextPassivityEffect;
struct FGsSchemaItemEffectMagicalForge;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClickMagicalForgeIconSlot, const bool, const ItemDBId);

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemMagicalForgeGroup : public UUserWidget
{
	GENERATED_BODY()

public:
	enum class MagicalForgeFailType : uint8
	{
		NONE = 0, // 성공
		EQUIP_EMPTY, // 장비 부족
		INGREDIENT_EMPTY, // 재료 부족
		MAGICAL_FORGE_LEVEL_MAX, // 같은 효과의 레벨 MAX
		NOT_MATCHED, // 올바르지 않은 아이템 매칭
		EQUIP_GRADE, // 유효하지 않은 장비 등급
	};

protected:
	// 장비 및 재료 아이템 아이콘
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _targetItemIconSelector;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _ingredientItemIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _targetIconSlotHelper;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _ingredientIconSlotHelper;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTargetItem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnIngredientItem;

	// 아이템 이름 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockItemName;

	// 중단 및 하단 설명 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuideMiddle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuideBottom;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelGuideBottom;

	// 마법 부여 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _contentsSwitcher; // 0 : display stat 1 : guideText

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCurrentName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockExpectName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemMagicalForgeEntry* _entryCondition;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemMagicalForgeEntry* _entryEffect;

	// 마법 부여 확률 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnProbability;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnProbabilityPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelProbability;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockItemNameProbability;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIEnchantRateListEntry* _uiProbabilityEntry;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuideProbability;

	// 마법 부여 및 취소 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencyButton* _btnEnchant;

protected:
	// 강화 연출
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIVFX* _mainSlotsafeEnchantEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _enchantAnimSpeedValue = 1.f;

private:
	const int32 SHOW_DETAIL_INFO = 0;
	const int32 HIDE_DETAIL_INFO = 1;

	bool _isInProgress = false; // 강화 진행중인가?
	ItemDBId _targetItemDBId = 0;
	ItemDBId _ingredientItemDBId = 0;

public:
	FOnClickMagicalForgeIconSlot OnClickMagicalForgeIconSlot;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void ResetContents();

	void SetItemDBId(const ItemDBId InTargetItemDBId, const ItemDBId InIngredientItemDBId);
	void SetTargetItemDBId(const ItemDBId InItemDBId);
	void SetIngredientItemDBId(const ItemDBId InItemDBId);

	ItemDBId GetTargetItemDBId() { return _targetItemDBId; }
	ItemDBId GetIngredientItemDBId() { return _ingredientItemDBId; }

	bool IsPlayingAnim();

	void UpdateMagicalForgeResultInfo(ItemDBId InTargetItemDBId, ItemMagicalForgeResult InResultType);

	void InvalidateAll();
	void InvalidateCurrencyInfo();

	bool UpdateItemInfo(ItemDBId InItemDBId);
	void RemoveItemInfo(ItemDBId InItemDBId);

private:
	void InvalidateAllInternal();

	void SetEquipItemInfo(const FGsItemEquipment* InEquipData);
	void SetMagicalForgeExpectInfo(const FGsItemEquipment* InEquipData, const FGsItemIngredient* InIngredientData);
	void SetCurrentEffectInfo(const FGsSchemaItemEffectMagicalForge* InMagicalForgeData, Level InLevel);
	void SetExpectEffectInfo(const FGsSchemaItemEffectMagicalForge* InMagicalForgeData, Level InLevel);
	void SetGuideText(bool InIsSafe);

	void GetNameText(const FText& InNameText, Level InLevel, OUT FText& OutNameText);
	void GetConditionAndEffectText(int32 InPassivityId, OUT FText& OutConditionText, OUT FText& OutEffectText);

	// 설명 문구
	void HideDetailInfo(const FTextKey& InTextKey);

	// 마법 부여 버튼
	void SetCurrency(Currency InCurrency);
	void SetMagicalForgeOn();
	void SetMagicalForgeOff();

	// 아이템 등록 가능 판단
	bool CanRegisterEquipment(const ItemDBId InItemDBId, bool InIsShowTicker = false);
	bool CanRegisterIngredient(const ItemDBId InItemDBId);

	const FGsSchemaEffectTextPassivityEffect* GetEffectTextData(int32 InPassivityId);

private:
	UFUNCTION()
	void OnClickTargetItem();
	UFUNCTION()
	void OnClickIngredientItem();
	UFUNCTION()
	void OnClickEnchant();
	UFUNCTION()
	void OnClickProbability();
	UFUNCTION()
	void OnClickProbabilityPanel();
	UFUNCTION()
	void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIngredientIcon(int32 InIndex, UGsUIIconBase* InIcon);

	void OnConfirmEnchant();

	void OnEnchantEffectAnimationFinished();
};