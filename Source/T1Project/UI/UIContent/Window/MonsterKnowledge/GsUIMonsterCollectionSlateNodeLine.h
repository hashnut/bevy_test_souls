// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlateStatNode.h"
#include "GsUIMonsterCollectionSlateNodeLine.generated.h"

class UGsUIVFX;
class UWidgetSwitcher;

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateNodeLine : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _vfxLine;

	UPROPERTY(EditInstanceOnly, Category = "Node Num", meta = (BindWidget))
	FGsSchemaMonsterKnowledgeSlateStatNodeRow _prevNode;

	UPROPERTY(EditInstanceOnly, Category = "Node Num", meta = (BindWidget))
	FGsSchemaMonsterKnowledgeSlateStatNodeRow _nextNode;

	int32 prevBindNodeId = 0;
	int32 nextBindNodeId = 0;

protected:
	const class FGsMonsterKnowledgeSlate* _slateData = nullptr;
	const class FGsMonsterKnowledgeSlateNode* _nodeData = nullptr;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	void ToggleActive(bool InIsActive);
	void PlayAnimation(bool InIsplay);

public:
	void SetData(const class FGsMonsterKnowledgeSlate* InSlate);

	void Invalidate(const class FGsMonsterKnowledgeSlateNode* InFocusNode = nullptr, bool InIsForceSelected = false);

	UFUNCTION()
	int32 PrevBindNodeId() const;

	UFUNCTION()
	int32 NextBindNodeId() const;
};
