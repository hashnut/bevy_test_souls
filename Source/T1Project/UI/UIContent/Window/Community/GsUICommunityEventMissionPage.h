// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "CommunityEventMission/GsCommunityEventMissionData.h"
#include "GsUICommunityEventMissionPage.generated.h"

class UPanelWidget;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaCommunityEvent;
struct FGsCommunityEventMissionTab;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItemReward;
class UCanvasPanel;
class UGsUICommunityEventMissionTab;
class UGsUICommunityEventMissionEntry;
class UTextBlock;
class UGsUITooltipDesc;

/**
 * 커뮤니티 이벤트 미션 페이지
 */
UCLASS()
class T1PROJECT_API UGsUICommunityEventMissionPage : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _bonusCanvasPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))	
	UTextBlock* _bonusText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _tooltipButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUITooltipDesc* _tooltipDesc;

	///////////////////////////////////////////////////////////////////
	// 탭 추가되는 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _tabScrollPanel;
	// tab 스크롤헬퍼
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _tabScrollBoxHelper;
	// 탭리소스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<class UUserWidget> _tabScrollBoxEntryClass;
	// 선택 on/off 헬퍼
	FGsToggleGroup _tabToggleGroup;
	// 탭 전체 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _tabCanvasPanel;
	// 이벤트 시간이 시작되지 않았을때
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _timeLockCanvasPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _missionIdCanvasPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMissionId;

	///////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _commonEntryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWrapBox* _missionCommonlist;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _commonSlotHelper;
	
	// 선택 on/off 헬퍼
	FGsToggleGroup _commonToggleGroup;

	/////////////////////////////////////////////////////////////////
	int _tabIndex = 0;
	int _indexCommunityEvent = 0;
	const FGsSchemaCommunityEvent* _schemaCommunityEvent = nullptr;
	const FGsSchemaCommunityEventMission* _SchemaCommunityEventMission = nullptr;

	CommunityEventId _communityEventId = 0;
	CommunityEventMissionId _missionId = 0;
	CommunityEventMissionTabId _missionTabId = 0;

	TSharedPtr<FGsCommunityEventMissionData> _missionData;
	TSharedPtr<FGsCommunityEventMissionTab> _missionTabData;

	bool _expire = false;

protected:
	FTimerHandle _updateTimerHandle;
	
public:
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;

protected:
	// tab 스크롤 업데이트
	UFUNCTION()
	void OnCreateTabButtonEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshTabButtonEntry(int32 inIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnCreateCommonEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshCommonEntry(int32 InIndex, class UWidget* InEntry);

	// tab 클릭
	UFUNCTION()
	void OnClickTab(int32 InIndex);

	// common 클릭
	UFUNCTION()
	void OnClickCommon(int32 InIndex);

	// common 클릭
	void OnClickCommonReward(CommunityEventMissionCommonId InCommonId);

	// 툴팁
	UFUNCTION()
	void OnClickedTooltip();

protected:
	UGsUIIconItemReward* GetIcon();
	void OnClickIconSlot(class UGsUIIconItem& InIcon);
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

protected:	
	void UpdateTimer();
	void UpdatePage(int32 InIndex);

	void SetTabData(int InIndex, UGsUICommunityEventMissionTab* InEntryTab);
	void SetCommonData(int InIndex, UGsUICommunityEventMissionEntry* InEntryCommon);

public:
	void InvalidateAll();

public:
	void OnSelectMission(int index, const FGsSchemaCommunityEvent* inData);
	void SetExpire();

protected:
	void SetToolTipDecPos();
};
