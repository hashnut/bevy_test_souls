#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeUICaptureData.h"
#include "GsUIMonsterCollectionSlateNodeSet.h"
#include "GsUIMonsterCollectionSlateNormalNodeDetailPanel.h"
#include "GsUIMonsterCollectionSlateSpecialNodeDetailPanel.h"
#include "GsUIMonsterCollectionSlateInfo.generated.h"

class UGsUIVFX;
class UGsButton;
class UTextBlock;
class UScrollBox;
class UPanelWidget;
class UCanvasPanel;
class UWidgetSwitcher;
class UGsSwitcherButton;
class UGsUICurrencyButton;
class UGsDynamicPanelSlotHelper;
class UGsUIMonsterCollectionSpecialNode;

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	struct FGsCaptureSmallOption
	{
		bool _isCheckNodeAll = false;
		MonsterKnowledgeSlateSlotId _isSelectedNodeId = INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID;
		SlateSpecialNodeId _isSelectedSpecialNodeId = INVALID_SLATE_SPECIAL_NODE_ID;
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxOpenEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelNodeSetList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDetailPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _specialNodeBtnRootPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _specialNodeBtnEntryWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _specialNodeBtnPanelSlotHelper;

protected:
	TWeakObjectPtr<UGsUIMonsterCollectionSlateNodeSet> _currentNodeSet;

	TWeakObjectPtr<UGsUIMonsterCollectionSlateNormalNodeDetailPanel> _normalDetailPanel;
	TWeakObjectPtr<UGsUIMonsterCollectionSlateSpecialNodeDetailPanel> _specialDetailPanel;
	

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UWidget* FindOrAddNodeWidget(const FSoftObjectPath& InPath);

protected:
	UFUNCTION()
	void OnRefreshSpecialNodeBtnEntry(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnVFXPlayOpenSlate();

	UFUNCTION()
	void OnSelectedNode();

	void OnEventStateChange(EMonsterCollectionSlateCallbackReason InReason, uint64 InParam);

public:
	void SetData(const class FGsMonsterKnowledgeSlate* InData, const FGsCaptureSmallOption& InOption);
	void Invalidate();
	void InvalidateCurrency();

public:
	const FGsMonsterKnowledgeSlate* GetData() const;
	bool IsCheckedSelectedNodeAll() const;		
	MonsterKnowledgeSlateSlotId SelectedNodeId() const;
	SlateSpecialNodeId SelectedSpecialNodeId() const;
};
