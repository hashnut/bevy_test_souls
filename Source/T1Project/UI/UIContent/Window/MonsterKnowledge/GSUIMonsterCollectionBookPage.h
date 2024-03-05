#pragma once

#include "CoreMinimal.h"
#include "GSUIMonsterCollectionBasePage.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "GSUIMonsterCollectionBookPage.generated.h"

class UTileView;
class UGsButton;
class UScrollBox;
class UGsPoolUObject;
class UGsDynamicPanelSlotHelper;

UCLASS()
class T1PROJECT_API UGSUIMonsterCollectionBookPage : public UGSUIMonsterCollectionBasePage, 
	public IGsTutorialInteractionInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollCategoryView;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCategoryView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfCategoryView;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRewardDetail;

	// 2021/11/24 PKT - 전체 레벨 달성 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLevelUpAll;

	// 2021/11/24 PKT - 아이템 일괄 사용 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnUseItemAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTileView* _tileViewPortaitSet;
	// 2022/01/05 PKT - collection list Data Entry Buffer
	UPROPERTY()
	UGsPoolUObject* _poolPortraitDataSet;

protected:
	// 2023/1/31 PKT - Main Tab Toggle
	FGsToggleGroup _toggleGroupCategoryTab;
	// 2023/2/3 PKT - List View Scroll Offset
	float _listScrollOffset = 0.f;
	// 2023/2/20 PKT - Lsat Detail Info
	uint32 _lastBookId = INVALID_MONSTER_KNOWLEDGE_ID;

	// 2023/2/3 PKT - EXP Used Item Option
	uint64 _savedOptionData = 0;

	bool _prevActiveUseItemPopup = false;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
public:
	virtual void Enter(struct FGsMonsterKnowledgeUICaptureData* InData) override;
	virtual void Leave() override;
	virtual void Capture(struct FGsMonsterKnowledgeUICaptureData* OutData) override;
			
	virtual void InvalidateBook(uint32 InData) override;
	virtual void InvalidateBookSet(const TArray<FGsMonsterCollectionBookResult>* InData);
	
protected:	
	virtual void InvalidateRedDot() override;	
	virtual void BuildData(int32 InCategoryIndex) override;		

protected:
	UFUNCTION()
	void OnRefreshEntryCategoryTab(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnSelectedCategoryTab(int32 InIndex);

	UFUNCTION()
	void OnClickedBtnLevelUpAll();

	UFUNCTION()
	void OnClickedBtnUseItemAll();

	UFUNCTION()
	void OnScrollListView(float InOffsetInItems, float InDistanceRemaining);
		
	UFUNCTION()
	void OnClickedTileViewItem(class UObject* InListItem);

	UFUNCTION()
	void OnSavedUsedItemOption(uint64 InData);
	
	UFUNCTION()
	void OnCloseResultPopup();

	UFUNCTION()
	void OnOpenRewardDetail();

	void OnCloseDetailPopup(uint32 InId);

private:
	void RefreshDisplayListViewEntryWidget(uint32 InId);

	void ActiveDetailPopup(uint32 InId);
	void ActiveUsedItemPopup();

private:
	int32 MaxTypeCount()
	{
		return StaticCast<int32>(MonsterKnowledgeBookGrade::MAX);
	}

	MonsterKnowledgeBookGrade ConvertIndexToType(int32 InIndex)
	{
		// 2023/2/21 PKT - 전체(MAX) 에서 역순 -> 유니크 .. 일반
		return StaticCast<MonsterKnowledgeBookGrade>(MaxTypeCount() - InIndex);
	}

	int32 ConvertTypeToIndex(MonsterKnowledgeBookGrade InType)
	{
		// 2023/2/21 PKT - 전체(MAX) 에서 역순 -> 유니크 .. 일반
		return MaxTypeCount() - StaticCast<int32>(InType);
	}


//----------------------------------------------------------------------------------------------------------------------
	// Tutorial
private:
	bool _bIsTutorial = false;
	bool _bIsTutorialInteraction = false;

	FGsOnTutorialInteraction OnTutorialInteraction;

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial) override;
	virtual bool IsTutorialInteraction() const override { return _bIsTutorialInteraction; }
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() override { return OnTutorialInteraction; }
	virtual void OnClickTutorialRegion() override;

public:
	UWidget* SetTutorial(bool bIsTutorial);
	bool IsTutorial() const { return _bIsTutorial; }
};
