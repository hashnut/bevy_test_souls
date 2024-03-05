// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Classes/GsUObjectPool.h"

#include "Components/PanelWidget.h"

#include "Item/Collection/EItemCollectionEnumData.h"

#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/TextDecorator/GsRichTextBlockLinkDecorator.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIItemDetailInfo.generated.h"

class FGsItem;
class FGsItemConsumable;
class FGsItemEquipment;
class FGsItemStat;
class UGsUIItemSetItemListEntry;
class UGsUIItemSetStatListEntry;
class UGsUIItemSkillOptionListEntry;
class UGsUITooltipWeaponSkillInfo;
class UGsUIItemStatListEntry;
class UHorizontalBox;
class UPanelWidget;
class URichTextBlock;
class UTextBlock;
class URichTextBlock;
class UWidgetSwitcher;
class UVerticalBox;
class UGsDynamicPanelSlotHelper;
class UTileView;
class UGsUObjectPool;
class USizeBox;
class UGsSwitcherButton;
class USpacer;
struct FGsSchemaPassivitySet;

/**
 * 아이템 상세정보 공용
 */
UCLASS()
class T1PROJECT_API UGsUIItemDetailInfo: public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnChangeEnchantCount, int32);

protected:
	enum StatCompareResultType : uint8
	{
		NONE = 0,
		UPDATE = 1,
		ADD = 2,
		REMOVE = 3,
	};

	struct StatDisplayInfo
	{
		StatType statType = StatType::NONE;
		FText statName;
		FText statValue;
		StatCompareResultType resultType = StatCompareResultType::NONE;
		int32 compareStatDeltaValue = 0;

	public:
		StatDisplayInfo(StatType InStatType, const FText& InStatName, const FText& InStatValue, StatCompareResultType InResultType, int32 InDeltaValue)
			: statType(InStatType), statName(InStatName), statValue(InStatValue), resultType(InResultType), compareStatDeltaValue(InDeltaValue)
		{

		}
	};

	enum class ExpiredTimeType : uint8
	{
		HIDE, // 보이지 않는다.
		EXPIRED, // 이용 불가
		
		EXPIRED_DELETE, // 이용 후 일괄 삭제
		EXPIRED_NOT_DELETE, // 이용 후 사용 불가

		RECEIVE_EXPIRED_DELETE, // 수령 후 시간 지나면 일괄 삭제
		RECEIVE_EXPIRED_NOT_DELETE, // 수령 후 시간 지나면 사용 불가

		UNDER_DAY_DELETE, // 1일 미만. 일괄 삭제
		UNDER_DAY_NOT_DELETE, // 1일 미만. 사용 불가
	};

protected:
	// Equipment / Supplies 페이지 구분
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherItemType;

	// Equip Page ------------------------------------------------------
	// Equip Tab
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEquipTabRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEquipTabListRoot;

	// 기본정보 / 세트효과 페이지 구분
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherEquipPage;

	// 효과
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelStatRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelTooltipDesc;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USpacer* _spacerTooltip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbStatTooltipName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelStatListRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryClassStatWidget;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperStatList;

	// 특성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEnchantPropRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbEnchantPropName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEnchantPropListRoot;
	// 제련
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRefineRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRefineListRoot;
	// 재질
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbMateraial;
	// 무게
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbWeight;

	// 획득처
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelAccquireRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _rbAcquire;

	// 2023/4/18 PKT - 컬렉션
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelCollectionInfoRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _horizontalBoxMaterialLevelSet;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperMaterialLevelSet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfMaterialLevelEntry;
	
	
	// 사용처
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelUtilizeRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _rbUtilize;

	// 세트 효과
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSetStatListRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSetItemListRoot;

	// 기간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _periodRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _periodText;

	// 스킬 옵션
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _skillOptionRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _skillOptionListRoot;

	// 만료
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _expiredTimeRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _expiredTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _expiredTitleSwitcher;   // 0 삭제 , 1 사용 불가

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBoxGuildCollectionEquip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _richTextOnlyGuildItemCollectionRegisterInfoEquipment;	
	
	// Supp Page -------------------------------------------------------
	// 지속
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuppDurationRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbSuppDuration;

	// 효과
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuppEff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbSuppEff;

	// 내용물 (현재는 상자형 아이템과 소환권만 사용) 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuppContents;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _tbSuppContents;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _suppVerticalBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USizeBox* _sizeBoxForTileViewForMaxHeight;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTileView* _tileViewForAdjustment;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _rbSummonInfo;
	
	// 설명
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuppDesc;
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTextBlock* _tbSuppDescTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbSuppDesc;

	// 설명 리스트인 경우
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuppDescList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuppDescListRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryClassSuppWidget;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperSuppDesc;


	// 무게
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuppWeight;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbSuppWeight;
	
	// 수량
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuppAmount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbSuppAmount;

	// 획득처
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuppAcquireRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _rbSuppAcquire;

	// 사용처
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSuppUtilizeRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _rbSuppUtilize;

	// 기간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _suppPeriodRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _suppPeriodText;

	// 만료
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _suppExpiredTimeRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _suppExpiredTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _suppExpiredTitleSwitcher;   // 0 삭제 , 1 사용 불가

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBoxGuildCollectionSupplies;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _richTextOnlyGuildItemCollectionRegisterInfoSupplies;

	// 마법 부여
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMagicalForge;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _rbMagicalForge;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMagicalForgeSupp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _rbMagicalForgeSupp;

	// 강화 미리보기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEnchant;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockEnchant;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnEnchantMinus;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnEnchantPlus;

protected:
	UPROPERTY(Transient)
	TArray<UGsUIItemStatListEntry*> _enchantPropSlotList;
	UPROPERTY(Transient)
	TArray<UGsUIItemStatListEntry*> _refineSlotList;
	UPROPERTY(Transient)
	TArray<UGsUIItemSetStatListEntry*> _setStatSlotList;
	UPROPERTY(Transient)
	TArray<UGsUIItemSetItemListEntry*> _setItemSlotList;
	UPROPERTY(Transient)
	TArray<UGsUIItemSkillOptionListEntry*> _skillOptionSlotList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipWeaponSkillInfo* _tooltipWeaponSkillInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipWeaponSkillInfo* _tooltipWeaponSkillInfoSupp;

	//UPROPERTY(Transient)
	//UGsUObjectPool* _supplyTileViewPool;

	//UPROPERTY(Transient)
	//UGsUObjectPool* _supplyTileViewEntryPool;

protected:
	FGsToggleGroup _toggleGroupEquipTab;

private:
	bool _isRunningExpiredTimeFlag = false;

	bool _compareVisibility = false;
	TArray<StatDisplayInfo> _displayedStatList;

	TArray<int32> _skillBookUserSkillSetIdList;

	TArray<TPair<Level, ECollectionMaterialActionType>> _collectionMarkingDataList;

	TSharedPtr<FGsItem> _displayedItem;
	TSharedPtr<FGsItem> _compareItem;
	bool _isCompareMode = false;

	int32 _displayedEnchantLevel = 0;
	int32 _defaultEnchantLevel = 0;

	RewardId _displayedRewardId = INVALID_REWARD_ID;

public:
	FOnChangeEnchantCount OnChangeEnchantCount;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetData(const FGsItem* InItem);
	void SetData(const CurrencyType InCurrencyType, const int64 InAmount);
	void SetCompareData(const FGsItem* InItem, const FGsItem* InCompareItem);
	void SetCompareValueVisibility(bool InState);
	void SetComareItemEnchantCount(int32 InCount);

	// SkillInfo
	void SetWeaponSkillInfo(FText&& InTitleText, FText&& InDescText, bool InIsPassive = false);

	// 2022/02/25 PKT - Guild Item Collection Register Info
	void SetGuildItemCollectionRegisterInfo(const FText& InText);	

	void SetItemCollectionMarking(const FGsItem* InItem, bool InUseMarking);
	void SetItemCollectionMarking(const CurrencyType InCurrencyType, bool InUseMarking);

protected:
	// Equipment
	void SetUIItemStat(const FGsItemEquipment* InDisplayedItem, const FGsItemEquipment* InComparedItem = nullptr);
	void SetUIEnchantProperty(const FGsItemEquipment* InEquipItem);
	void SetUIRefineOption(const FGsItemEquipment* InEquipItem);
	void SetUIMaterial(const FGsItemEquipment* InEquipItem);
	void SetUIEquipWeight(const FGsItemEquipment* InEquipItem);
	void SetUISetStatInfo(const FGsItemEquipment* InEquipItem);
	void SetUISetItemInfo(const FGsItemEquipment* InEquipItem);
	void SetSkillOptionData(const FGsItemEquipment* InEquipItem);
	void SetUIEnchant(const FGsItemEquipment* InEquipItem);

	// Supplies
	void SetUISuppliesInfo(const FGsItem* InItem);
	void SetUIConsumableInfo(const FGsItem* InItem);
	void SetUIEff(const FGsItem* InItem);
	void ClearUIContents();
	void SetUIContents(const FText& InTxt);
	void SetUIContentsIcon(const FGsItem* InItem);
	void SetUIContentsPopupLink(const FGsItem* InItem);
	void SetUIDesc(const FText& InTxt);
	void SetUIDesc(const FGsItem* InItem);
	void SetUIAmount(const int64 InAmount);

	// Currency
	void SetUICurrencyInfo(const CurrencyType InCurrencyType, const int64 InAmount);

	// 공용
	void SetUIWeightInter(float InValue, OUT UTextBlock* OutTextBlock);
	void SetUIAcquire(const FText& InTxt);
	void SetUIUtilize(const FText& InTxt);
	void SetUIPeriod(const FGsItem* InItem);
	void SetUIMagicalForge(const FGsItem* InItem);

protected:
	void OnSelectEquipTab(int32 InIndex);

protected:
	template <typename T = UWidget>
	void GetAllChildListEntry(UPanelWidget* InPanelRoot, OUT TArray<T*>& OutList);

private:
	bool GetCalcCompareData(FGsItemStat* InStatDataList, TArray<FGsItemStat*> InCompareStatDataList, OUT int32& outCalcValue);
	bool isExistCompareStat(TArray<FGsItemStat*> InCompareStatDataList, StatType InStatType , OUT int32& outCompareIndex);
	void GetExistExpectNewOrRemoveStat(TArray<FGsItemStat*> InStatList, TArray<FGsItemStat*> InCompareStatList, StatCompareResultType InType, OUT TMap<FGsItemStat*, TPair<StatCompareResultType, int32>>& outResultMap);

	void GetDisplayedStatList(const FGsItemEquipment* InDisplayedItem);
	void GetComparedStatList(const FGsItemEquipment* InDisplayedItem, const FGsItemEquipment* InComparedItem);

	// 마법 부여
private:
	void RegisterHyperLinkDelegateMagicalForge(const FGsSchemaPassivitySet& InPassivitySet, ItemGrade InGrade, URichTextBlock* InTextBlock);
	void UnRegisterHyperLinkDelegateMagicalForge(URichTextBlock* InTextBlock);

	// 강화 미리보기
private:
	void InvalidateEnchantPanel();

private:
	UFUNCTION()
	void OnRefreshEntryStatList(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnRefreshEntrySuppDesc(int32 InIndex, UWidget* InEntry);
	
	UFUNCTION()
	void OnRefreshEntryCollectionMaterialLevel(int32 InIndex, UWidget* InEntry);

	void OnClickHyperLinkId(const UGsRichTextBlockLinkDecorator::FGsOnClickParamId& InParamIdMap);
	void OnClickHyperCustomLinkId(const UGsRichTextBlockLinkDecorator::FGsOnClickParamId& InParamIdMap);

	UFUNCTION()
	void OnClickEnchantMinus();
	UFUNCTION()
	void OnClickEnchantPlus();
};

template <typename T>
void UGsUIItemDetailInfo::GetAllChildListEntry(UPanelWidget* InPanelRoot, OUT TArray<T*>& OutList)
{
	int32 count = InPanelRoot->GetChildrenCount();
	for (int32 i = 0; i < count; ++i)
	{
		UWidget* widget = InPanelRoot->GetChildAt(i);
		if (T* listEntry = Cast<T>(widget))
		{
			OutList.Emplace(listEntry);
		}
	}
}
