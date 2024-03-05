// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Helper/GsRedDotChecker.h"
#include "GsUIGuildContentOperation.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UGsUIRedDotBase;
class UGsSwitcherButton;
class UGsUIGuildOperationBase;

/**
 * 길드 운영 페이지
 */
UCLASS()
class T1PROJECT_API UGsUIGuildContentOperation : public UGsUIGuildContentBase
{
	GENERATED_BODY()

public:
	enum EGsOperationTab
	{
		BUILDING = 0,
		SANCTUM = 1,
		BUFF = 2,

		ALL,
		MAX = ALL,
	};
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnBuff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSanctum;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildOperationBuilding* _uiPageBuilding;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildOperationSanctum* _uiPageSanctum;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildOperationBuff* _uiPageBuff;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotBuilding;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotSanctum;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDotBuff;

protected:
	FGsToggleGroup _toggleSubTab;
	bool _bIsMyGuild = false;
	EGsOperationTab _prevTab = EGsOperationTab::MAX;

	TMap<EGsOperationTab, UGsUIGuildOperationBase*> _pageMap;
	TMap<EGsOperationTab, FGsRedDotChecker> _redDotMap;

public:
	virtual FString GetTabName() const override { return TEXT("Operation"); }

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	virtual void SendRequestPacket() override;
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;
	virtual void Hide() override;	
	
	virtual void OnChangeCurrency(CurrencyType InType) override;
	virtual void OnChangeGuildLevel() override;
	virtual void OnUpdateBuilding(GuildBuildingId InBuildingId) override;
	virtual void OnChangeMyAuthority() override;

public:
	// 길드 건물 리스트 정보 받음
	void OnReceiveBuildingList();
	void OnCampConstruction();
	void OnCampDestruction();
	void OnSanctumConstructionStateChanged();
	void OnSanctumDestruction();

protected:
	void OnSelectTab(int32 InIndex);

	void UpdateRedDot(EGsOperationTab InTab);

	class UGsUIGuildOperationBase* GetCurrentPage() const;	
};
