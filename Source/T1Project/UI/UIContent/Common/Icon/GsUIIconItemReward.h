// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsUIIconItemReward.generated.h"

class FGsItem;
/**
 * 아이템/재화 보상 아이콘 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIIconItemReward : public UGsUIIconItem, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UImage* _iconImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgBG;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _skillOptionImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _rankImgWidgetSwicher;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _lockImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UWidget* _widgetAcceptable;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UWidget* _widgetRewarded;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UWidget* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UWidget* _widgeIneligible;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UGsUIItemIconSlotEffect* _slotEffectMagicalForge;

protected:
	bool _bIsSelected = false;
	bool _bEnableDetailPopup = true;
	bool _bCenterPopup = true;
	bool _bIsRewarded = false;
	bool _bIsAcceptable = false;

	bool _bIsRewardLock = false;
	bool _bIsRewardNoti = false;

public:
	// UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;
	
	virtual void SetIconImageSprite(class UPaperSprite* InSprite) override;

	virtual void SetIsRewardLock(bool isRewardLock);
	virtual void SetIsRewardNoti(bool isRewardNoti);

protected:
	// UGsUIIconItem overide
	virtual void SetItemGrade(ItemGrade InGrade);
	virtual void SetItemSlotEffect(const EGsItemIconSlotEffectType InType) override;
	virtual void OnClickItemIcon() override;

public:
	// 슬롯 BG 이미지 변경, 값세팅
	void SetBGImage(class UPaperSprite* InImg);
	void SetBGVisibility(bool bVisible);
	void SetBGColorAndOpacity(const FLinearColor& InColor);
	void SetEnableDetailPopup(bool bInEnable) { _bEnableDetailPopup = bInEnable; }
	void SetCenterPopup(bool bInCenterPopup) { _bCenterPopup = bInCenterPopup; }
	void SetIsRewarded(bool bInRewarded);
	void SetIsAcceptable(bool bInAcceptable);
	void SetSkillOptionImage(FGsItem* InItem);
	virtual void SetSlotEffectMagicalForge(FGsItem* InItem) override;

	virtual void SetIsEnabled(bool bInIsEnabled) override;
	void SetIsOpenReward(bool bIsOpenReward);
public:
	bool GetIsRewarded() { return _bIsRewarded; }
	bool GetIsAcceptable() { return _bIsAcceptable; }
};
