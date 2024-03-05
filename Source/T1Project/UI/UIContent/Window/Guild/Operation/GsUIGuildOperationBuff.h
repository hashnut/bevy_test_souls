// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildOperationBase.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIGuildOperationBuff.generated.h"

class UTextBlock;
class UScrollBox;
class UPanelWidget;
class UWidgetSwitcher;
class UGsButton;
class UGsUITooltip;
class UGsUICurrencyButton;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaGuildBuilding;
struct FGsSchemaGuildBuildingLevelData;
struct FGsGuildBuildingData;


/**
 * 길드 버프탭 페이지 내용
 */
UCLASS()
class T1PROJECT_API UGsUIGuildOperationBuff : public UGsUIGuildOperationBase
{
	GENERATED_BODY()

public:
	enum EGsGuildBuffStateType : uint8
	{	
		None,
		Active,
		WaitReactivate,
		NotOpened, // 버튼 State 없음
	};
		
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelScrollRoot;

	// 버프 선택 시 우측 출력 되는 상세정보 Root패널
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelDetailInfoRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildAsset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelTitleRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherImageState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBuffTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherButtonState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBuffRemainTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockReactivateRemainTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnActivateBuff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltip* _tooltipInfo;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	FGsToggleGroup _toggleGroupBuffList;
	bool _bIsAuthority = false; // 권한에 따라 활성 버튼 상태 변경
	TArray<const FGsSchemaGuildBuilding*> _dataList;
	EGsGuildBuffStateType _buffState = EGsGuildBuffStateType::None;

	bool _bIsTimer = false;
	float _checkRemainSec = 0.f;
	time_t _endTimeSec = 0;

	float _buffDurationSec = 0.f;
	float _maxDurationSec = 0.f;

	// 노티받으면 강제로 끄기위해 포인터를 들고있다가 끈다
	TWeakObjectPtr<class UGsUIWidgetBase> _popupActiveBuff;
	
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;
	virtual void Hide() override;

	virtual void OnChangeMyAuthority() override;
	virtual void OnChangeCurrency(CurrencyType InType) override;
	virtual void OnReceiveBuildingList() override;
	virtual void OnUpdateBuilding(GuildBuildingId InBuildingId) override;

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnPressInfo();

	void OnClickBuffList(int32 InIndex);
	void OnClickActivateBuff();

protected:
	void SetUIGuildAsset();
	void SetUIBuffList(int32 InIndex);
	void SetUIBuffDetail(const FGsSchemaGuildBuilding* InData);
	void SetUIBuffState(const FGsGuildBuildingData* InData, const FGsSchemaGuildBuildingLevelData* InLevelData);

	void SetUIRemainTime();

	const FGsSchemaGuildBuilding* GetSelectedBuildingTableData() const;

	bool IsDataExist(GuildBuildingId InBuildingId);
};
