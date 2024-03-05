// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeUICaptureData.h"
#include "GsUIMonsterCollectionSlateNodeDetailPanelBase.generated.h"

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateNodeDetailPanelBase : public UUserWidget
{
	GENERATED_BODY()

public:
	enum class ENodeDetailType
	{
		None,
		Normal,		// 2023/11/8 PKT - 일반 노드들의 정보
		Special,	// 2023/11/8 PKT - 특수 노드들의 정보
	};


protected:
	ENodeDetailType _detailType = ENodeDetailType::None;
	
	MonsterKnowledgeSlateId _slateId = INVALID_MONSTER_KNOWLEDGE_SLATE_ID;

	TFunction<void(EMonsterCollectionSlateCallbackReason, uint64)> _callbackFunc = nullptr;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	const class FGsMonsterKnowledgeSlate* FindData() const;

public:
	virtual void SetData(const class FGsMonsterKnowledgeSlate* InData, TFunction<void(EMonsterCollectionSlateCallbackReason, uint64)> InCallback);
	virtual void Invalidate() {}
	virtual void InvalidateCurrency() {};

	ENodeDetailType GetType() const { return _detailType; };
	const MonsterKnowledgeSlateId GetId() const { return _slateId; }
};
