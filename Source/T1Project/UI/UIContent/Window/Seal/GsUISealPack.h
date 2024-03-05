#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Management/ScopeGame/GsSealManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/GsUIWindowSeal.h"

#include "GsUISealPack.generated.h"

class FGsSealPack;
class UGsUISealNode;
class UGsUISealNodeLine;
class UGsUISealSlot;
class UGsUISealSlotListEntry;
class UGsUIVFX;
class UWidget;
class UWidgetSwitcher;
struct FGsUIWindowSealEventParameters;

UCLASS()
class UGsUISealPack final : public UUserWidget, public IGsUIWindowSealEvent
{
	GENERATED_BODY()

public:
	struct Parameters final
	{
		SealPackId _packId;
	};

protected:
	/** 이펙트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidget* _sealNodeAllTheSameColorVfx0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidget* _sealNodeAllTheSameColorVfx1;
	
	/** 봉인팩0의 모든 봉인노드가 동일 색상에 도달했을 때 연출 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UGsUIVFX* _sealAllNodeSameColorOnceVfx;

	/** 봉인노드 연결선. */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealNodeLine* _sealNodeLineA;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealNodeLine* _sealNodeLineB;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealNodeLine* _sealNodeLineC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealNodeLine* _sealNodeLineD;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealNodeLine* _sealNodeLineE;
	
	/** 봉인 노드 A 와 해당 봉인 슬롯 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealNode* _sealNodeA;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidget* _sealNodeSelectionA;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _sealNodeColorSwitcherA;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _sealNodeIconA;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeASlot0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeASlot1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeASlot2;
	
	/** 봉인 노드 B 와 해당 봉인 슬롯 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealNode* _sealNodeB;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidget* _sealNodeSelectionB;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _sealNodeColorSwitcherB;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _sealNodeIconB;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeBSlot0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeBSlot1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeBSlot2;
	
	/** 봉인 노드 C 와 해당 봉인 슬롯 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealNode* _sealNodeC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidget* _sealNodeSelectionC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _sealNodeColorSwitcherC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _sealNodeIconC;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeCSlot0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeCSlot1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeCSlot2;
	
	/** 봉인 노드 D 와 해당 봉인 슬롯 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealNode* _sealNodeD;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidget* _sealNodeSelectionD;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _sealNodeColorSwitcherD;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _sealNodeIconD;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeDSlot0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeDSlot1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeDSlot2;
	
	/** 봉인 노드 E 와 해당 봉인 슬롯 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealNode* _sealNodeE;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidget* _sealNodeSelectionE;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _sealNodeColorSwitcherE;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	UWidgetSwitcher* _sealNodeIconE;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeESlot0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeESlot1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISealSlot* _sealNodeESlot2;
	
private:
	Parameters _parameters;

	FGsToggleGroup _sealNodeToggleGroup;

	/** 현재 출력중인 봉인 노드들의 데이터 */
	TArray<TWeakObjectPtr<UGsUISealNode>> _uiNodes;

	/** 각 노드의 색상이 특정된 경우(모든 봉인슬롯의 색상이 일치할 경우), 이 위젯으로 해당 색상을 출력함. */
	TArray<TWeakObjectPtr<UWidgetSwitcher>> _uiNodeColorSwitchers;

	TArray<TWeakObjectPtr<UWidgetSwitcher>> _uiNodeIcons;

	/** 현재 출력중인 봉인 슬롯들의 데이터. 먼저 노드 순서대로 분류하고, 슬롯 순서대로 저장되어 있음. */
	TArray<TArray<TWeakObjectPtr<UGsUISealSlot>>> _uiNodeSlots;

	/** 봉인 노드 라인 위젯 목록. */
	TArray<TWeakObjectPtr<UGsUISealNodeLine>> _uiSealNodeLines;

	UGsUIWindowSeal::FGsOnUIEvent _onUiEvent;

public:
	/** 노드 선택 변경 이벤트 */
	FSimpleDelegate _onSelectChangedNode;

protected:
	void NativeOnInitialized() final;

public:
	void ResetSelections();

public:
	void SetParameters(Parameters InParamerters);

	void SetSealPackVfxVisible(const bool InSealPackVfxVisible);

public:
	int32 GetSelectedNodeIndex() const;

private:
	const FGsSealPack* GetCurrentPack() const;

	const int32 FindNodeIndexByNodeId(const SealId InNodeId) const;
	
	const FGsSealNode* GetNodeAt(const int32 InNodeIndex) const;

	TWeakPtr<const FGsSealSlot> GetSlotAt(const int32 InNodeIndex, const int32 InSlotIndex) const;

	int32 GetSlotCountInNodeAt(const int32 InNodeIndex) const;

private:
	void InvalidateSealNode(const SealId InNodeId);

	void InvalidateSealNodeByIndex(const int32 InNodeIndex);

	void InvalidateSealNodeLine(const int32 InNodeIndex);

public:
	void InvalidateSealSlot(const SealId InNodeId, const int32 InSlotIndex);

public:
	UFUNCTION()
	void OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters) final;

protected:
	UFUNCTION()
	void OnSelectChangedNode(int32 InIndex);
};
