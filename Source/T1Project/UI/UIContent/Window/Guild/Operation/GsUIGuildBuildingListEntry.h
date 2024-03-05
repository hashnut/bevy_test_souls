// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "GsUIGuildBuildingListEntry.generated.h"

class UTextBlock;
class UGsButton;
class UImage;
class UGsGrayscaleImage;
class UWidgetSwitcher;
class UGsUIRedDotBase;
struct FGsSchemaGuildBuilding;
struct FGsSchemaGuildBuildingLevelData;
struct FGsGuildBuildingData;

/**
 * 기사 건물 리스트의 entry
 * - 주의: 다른 길드 정보 상세보기 시에도 동일UI 활용함에 유의
 */
UCLASS()
class T1PROJECT_API UGsUIGuildBuildingListEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsGrayscaleImage* _imgIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	UTextBlock* _textBlockEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNeedGuildLevel;

	// 0: 건설, 레벨업 버튼, 1: 업그레이드 중 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherLevelUp;

	// 레벨업 버튼을 어떻게 만드시려나. 레벨업 중 표시가 필요하다
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUICurrencyButton* _btnLevelUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUpgrade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUpgradeRemainTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgDimmed;

protected:
	const float TIME_UPDATE_SEC = 0.2f;
	const FGsSchemaGuildBuilding* _tableData = nullptr;	

	bool _bShowLevelUpTime = false;
	float _checkRemainSec = 0.f;
	//FDateTime _upgradeEndTimeUTC; // 시간을 비교하기 위함
	int32 _needLevelUpCost = 0;
	bool _bIsAuthority = false;
	bool _bIsMyGuild = false;

	time_t _upgradeEndSec = 0;
	int32 _maxLevelUpSec = 0;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void OnHide() override;

public:
	void SetData(const FGsSchemaGuildBuilding* InTableData, bool bIsAuthority, bool IsMyGuild);

protected:
	UFUNCTION()
	void OnClickLevelUp();

protected:
	void InvalidateAll();

	void SetUINeedLevel(int32 InNeedLevel, GuildStatType InStatType, bool InCanLevelUp);
	void SetUILevelUpButton(bool bInIsFirst, bool bInIsMaxLevel);
	void SetUIUpgradeState(bool bInIsFirst);
	void SetUIUpgradeRemainTime();
	
	// T: 다음 레벨 정보 있음, F: 다음 레벨 정보 없음
	bool GetLevelData(int32 InBuildingLevel, 
		OUT int32& OutCurrLevel, OUT int32& OutNeedLevel, OUT int32& OutNeedLevelUpCost);

	bool GetBuildingEffectString(const FGsSchemaGuildBuilding* InData, int32 InBuildingLevel, OUT FText& OutText);
};
