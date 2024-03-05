#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterKnowledgeCollection/EMonsterStoneSlateTreeNodeCoordinate.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlate.h"
#include "GsUIMonsterCollectionSlateNode.h"
#include "GsUIMonsterCollectionSlateNodeLine.h"
#include "GsUIMonsterCollectionSlateNodeSet.generated.h"


class UGsUIMonsterCollectionSlateNode;
class UGsUIMonsterCollectionSlateNodeLine;

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateNodeSet : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelNodeSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelLineSet;

	TArray<TWeakObjectPtr<UGsUIMonsterCollectionSlateNode>> _nodeSet;

	TArray<TWeakObjectPtr<UGsUIMonsterCollectionSlateNodeLine>> _lineSet;


protected:
	const class FGsMonsterKnowledgeSlate* _data = nullptr;
	EMonsterKnowledgeSlateState _prevState = EMonsterKnowledgeSlateState::Lock;

	const class FGsMonsterKnowledgeSlateNode* _selectedNodeData = nullptr;

public:
	/**
	 * 2023/3/8 PKT - NodeSet 구조 상 NodeSet에 이벤트를 관리자인 MonsterCollectionSlateInfo에 알려야 하는 경우가 있음.(상태 변경 / 선택 등등 )
	 */
	DECLARE_DELEGATE(FOnReportDelegate);
	FOnReportDelegate OnChangedState;
	FOnReportDelegate OnChangedSelected;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	void OnClickedNodeDelegate(const class FGsMonsterKnowledgeSlateNode* InData);

public:
	void SetData(const class FGsMonsterKnowledgeSlate* InSlate, MonsterKnowledgeSlateSlotId InSelectedNodeId);
	void Invalidate(bool InIsForceSelected = false);
	
	const class FGsMonsterKnowledgeSlate* GetData() const;
	const class FGsMonsterKnowledgeSlateNode* GetSelectedNodeData() const;
};