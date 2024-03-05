// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Tray/FairyEnchant/GsUITrayEnchantFairyBase.h"
#include "GsDivFairyEnchant.generated.h"

/**
 * 
 */

class UGsButton;
class UGsUICurrencyButton;
class UGsDynamicPanelSlotHelper;
class URichTextBlock;
class UWidgetSwitcher;
class UScrollBox;

struct FGsSchemaPassivitySet;
struct FGsFairyData;
struct FGsFairyPassivityInfo;

UCLASS()
class T1PROJECT_API UGsDivFairyEnchant : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnStartFairyEnchant, TWeakPtr<FGsFairyData>);
	DECLARE_DELEGATE(FOnCancelEnchant);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnUpgrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textCurLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textNxtLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _maxLevel;

//페어리 스킬
#pragma region FairySkill
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _curFairySkillList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _curSkillListPanel;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _curScrollHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entrySkillIcon;

#pragma endregion FairySkill
//페어리 스탯
#pragma region Upgrade Stat
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switchFairyNextStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _richTextFairyOpenStat; //신성한 루나 lv.4

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _openStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _richTextFairyPreOpenedStat; //신성한 루나 lv.4
#pragma endregion

//페어리 콜렉션 스탯
#pragma region CollectionStat
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	URichTextBlock* _collectionSkillDetail; //신성한 루나 lv.4

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _collectionStatScrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _collectionEffectText;
#pragma endregion

private:
	bool _isEnableEnchant{ false };
	TWeakPtr<FGsFairyData>  _fairyData;
	TArray<TSharedPtr<FGsFairyPassivityInfo>> _fairyLevelData;

public:
	FOnStartFairyEnchant OnStartEnchant;
	FOnCancelEnchant	OnCancelEnchant;

protected:
	UFUNCTION()
	void OnCreateFairySkillEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshFairySkillEntry(int32 index, UWidget* entry);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnStartFairyUpgrade();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnCancelUpgrade();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickFairyCollection();

	void OnTooltipFairySkill(TWeakPtr<FGsFairyPassivityInfo> tooltip);

public:
	void InitEnchantDivision(TWeakPtr<FGsFairyData> data);
	void OnEnchantResult(FairyEnchantResult result);
	void CancelEnchant();

	virtual void BeginDestroy() override;

protected:
	bool CheckEnableFairyUpgrade();
	static FText GetFairySkillName(FText fairyName, FairyGrade grade, FairyLevelId applyLevel);

protected:
	virtual void NativeOnInitialized() override;

private:
	// 성장 정보 Set Private Function
	void SetFairyEnchantBaseInfo();
	void SetFairyUpgradeSkillInfo(TSharedPtr<FGsFairyData> fairyData, FairyLevelId curLevel, FairyLevelId nextLevel);
	void SetFairyUnlockStatInfo(TSharedPtr<FGsFairyData> fairyData, FairyLevelId curLevel, FairyLevelId nextLevel);
	void SetAttainableFairyCollectionStatInfo(TSharedPtr<FGsFairyData> fairyData, FairyLevelId curLevel, FairyLevelId nextLevel);
	
};
