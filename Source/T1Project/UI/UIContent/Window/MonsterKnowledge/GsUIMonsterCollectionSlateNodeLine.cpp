#include "GsUIMonsterCollectionSlateNodeLine.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlateNode.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../UI/UILib/Base/GsUIVFX.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UMG/Public/Blueprint/WidgetBlueprintGeneratedClass.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "T1Project.h"


void UGsUIMonsterCollectionSlateNodeLine::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const FGsSchemaMonsterKnowledgeSlateStatNode* prevNodeDat = _prevNode.GetRow();
	const FGsSchemaMonsterKnowledgeSlateStatNode* nextNodeDat = _nextNode.GetRow();
	
	if (nullptr == prevNodeDat || nullptr == nextNodeDat)
	{
		GSLOG(Error, TEXT("nullptr == prevNodeDat || nullptr == nextNodeDat"));
		return;
	}

	prevBindNodeId = prevNodeDat->id;
	nextBindNodeId = nextNodeDat->id;
}

void UGsUIMonsterCollectionSlateNodeLine::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(false);
}

void UGsUIMonsterCollectionSlateNodeLine::ToggleActive(bool InIsActive)
{
	const int32 SWITCHER_ACTIVE_INDEX = 0;
	const int32 SWITCHER_DISABLE_INDEX = 1;

	_switcher->SetActiveWidgetIndex(InIsActive ? SWITCHER_ACTIVE_INDEX : SWITCHER_DISABLE_INDEX);
}

void UGsUIMonsterCollectionSlateNodeLine::PlayAnimation(bool InIsplay)
{
	if (false == InIsplay)
	{
		_vfxLine->StopAllAnimations();
		_vfxLine->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		UWidgetBlueprintGeneratedClass* genClass = _vfxLine->GetWidgetTreeOwningClass();
		if (genClass && 0 < genClass->Animations.Num())
		{
			const static int32 FIRST_ANIMATION_DATA = 0;
			if (UWidgetAnimation* animationData = genClass->Animations[FIRST_ANIMATION_DATA])
			{
				_vfxLine->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				const static int32 NUM_LOOPS_TO_PLAY = 0;
				_vfxLine->PlayAnimation(animationData, 0.f, NUM_LOOPS_TO_PLAY, EUMGSequencePlayMode::Type::Forward, 1.f);
			}
		}
	}
}

void UGsUIMonsterCollectionSlateNodeLine::SetData(const FGsMonsterKnowledgeSlate* InSlate)
{
	_slateData = nullptr;
	_nodeData = nullptr;

	PlayAnimation(false);

	for (int32 idx = 0; idx < InSlate->NodeMaxCount(); ++idx)
	{
		const auto& item = InSlate->NodeAt(idx);
		if (item && item->Id() == NextBindNodeId() && item->PrevNodeId() == PrevBindNodeId())
		{
			_slateData = InSlate;
			_nodeData = item;

			break;
		}
	}

	if (nullptr == _nodeData)
	{
		GSLOG(Error, TEXT("nullptr == _nodeData"));
		return;
	}

	Invalidate();
}

void UGsUIMonsterCollectionSlateNodeLine::Invalidate(const class FGsMonsterKnowledgeSlateNode* InFocusNode /* = nullptr */, bool InIsForceSelected /* = false */)
{
	EMonsterStoneSlateNodeState prevNodeState = _nodeData->PrevNodeData()->State();
	EMonsterStoneSlateNodeState nextNodeState = _nodeData->State();

	// 2023/3/7 PKT - 양쪽다 활성화 되어 있는가?
	bool isActive = false;
	if (EMonsterStoneSlateNodeState::Complete == prevNodeState && prevNodeState == nextNodeState)
	{
		isActive = true;
	}

	// 2023/3/7 PKT - 모두 활성화 되어 있지 않았을 경우 상황에 따른 Animation
	bool isAnimation = false;
	if (false == isActive)
	{
		isAnimation = InIsForceSelected;
		if (false == isAnimation && InFocusNode)
		{
			const auto& chainNodeSet = _slateData->ChainNodeSet(InFocusNode);
			for (auto& item : chainNodeSet)
			{
				if (_nodeData->Id() == item->Id())
				{
					isAnimation = true;
					break;
				}
			}
		}
	}

	// 2023/2/17 PKT - Animation을 플레이 해야 하는 상황이라면 Switcher를 바꿔야 함
	ToggleActive(isActive || isAnimation);
	PlayAnimation(isAnimation);
}

int32 UGsUIMonsterCollectionSlateNodeLine::PrevBindNodeId() const
{
	return prevBindNodeId;
}

int32 UGsUIMonsterCollectionSlateNodeLine::NextBindNodeId() const
{
	return nextBindNodeId;
}