#include "GsUIMonsterCollectionSlateNode.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlateNode.h"

#include "../UI/UILib/Base/GsUIVFX.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIContent/Window/MonsterKnowledge/GsUIMonsterCollectionSlateNodeFadeInVFX.h"

#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "UMG/Public/Components/Widget.h"

#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"

#include "T1Project.h"


void UGsUIMonsterCollectionSlateNode::BeginDestroy()
{
	if (_btnSymbol)
	{
		_btnSymbol->OnClicked.RemoveDynamic(this, &UGsUIMonsterCollectionSlateNode::OnClickedNode);
		_btnSymbol = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIMonsterCollectionSlateNode::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_resourceId = false == _nodeRow.IsNull() ? _nodeRow.GetRow()->id : 0;

	_btnSymbol->OnClicked.AddDynamic(this, &UGsUIMonsterCollectionSlateNode::OnClickedNode);
}

void UGsUIMonsterCollectionSlateNode::NativeConstruct()
{
	Super::NativeConstruct();

	_slateData = nullptr;
	_nodeData = nullptr;
}

void UGsUIMonsterCollectionSlateNode::OnClickedNode()
{
	if (_nodeData)
	{
		OnClickedDelegate.ExecuteIfBound(_nodeData);
	}
}

void UGsUIMonsterCollectionSlateNode::ToggleAnimation(UUserWidget* InWidget, bool InFlag)
{
	if (nullptr == InWidget)
	{
		return;
	}

	bool prevVisible = ESlateVisibility::Collapsed != InWidget->GetVisibility();

	if (false == prevVisible && false == InFlag)
	{
		return;
	}

	if (false == InFlag)
	{
		InWidget->StopAllAnimations();
		InWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UWidgetBlueprintGeneratedClass* genClass = InWidget->GetWidgetTreeOwningClass();
		if (genClass && 0 < genClass->Animations.Num())
		{
			const static int32 FIRST_ANIMATION_DATA = 0;
			if (UWidgetAnimation* animationData = genClass->Animations[FIRST_ANIMATION_DATA])
			{
				InWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				const static int32 NUM_LOOPS_TO_PLAY = 0;
				InWidget->PlayAnimation(animationData, 0.f, NUM_LOOPS_TO_PLAY, EUMGSequencePlayMode::Type::Forward, 1.f);
			}
		}
	}
}

void UGsUIMonsterCollectionSlateNode::SetData(const FGsMonsterKnowledgeSlate* InSlate)
{
	_slateData = nullptr;
	_nodeData = nullptr;

	for (int32 idx = 0; idx < InSlate->NodeMaxCount(); ++idx)
	{
		const auto& item = InSlate->NodeAt(idx);
		if (item && item->ResourceId() == _resourceId)
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

	_prevState = _nodeData->State();

	_effectActiveFadeIn->HideVFX();
	_effectActiveBroken->HideVFX();

	ToggleAnimation(_effectFocus, false);
	ToggleAnimation(_effectSelected, false);

	// 2023/2/16 PKT - Load Image
	UPaperSprite* img = FGsUIHelper::LoadSpriteFromPath(_nodeData->ImagePath());
	if (img)
	{
		_imgActive->SetBrushFromAtlasInterface(img);
		_imgDisable->SetBrushFromAtlasInterface(img);

		_effectActiveFadeIn->SetData(img);
	}

	Invalidate();
}

void UGsUIMonsterCollectionSlateNode::Invalidate(const class FGsMonsterKnowledgeSlateNode* InFocusNode /* = nullptr */, bool InIsForceSelected /* = false */)
{
	if (nullptr == _nodeData)
	{
		return;
	}

	const EMonsterStoneSlateNodeState currentState = _nodeData->State();

	if (_prevState != currentState)
	{
		if (EMonsterStoneSlateNodeState::Complete == currentState)
		{
			_effectActiveFadeIn->ShowVFX();
			_effectActiveBroken->ShowVFX();
		}

		// 2023/2/16 PKT - Save...
		_prevState = currentState;
	}

	const static int32 SWITCHER_ACTIVE_INDEX = 0;
	const static int32 SWITCHER_DISABLE_INDEX = 1;
	_switcherActiveImage->SetActiveWidgetIndex(EMonsterStoneSlateNodeState::Complete != currentState ? SWITCHER_DISABLE_INDEX : SWITCHER_ACTIVE_INDEX);
	_switcherActiveSymbol->SetActiveWidgetIndex(EMonsterStoneSlateNodeState::Complete != currentState ? SWITCHER_DISABLE_INDEX : SWITCHER_ACTIVE_INDEX);

	bool isHightLight = InIsForceSelected;
	if (false == isHightLight)
	{
		if (InFocusNode)
		{
			const auto& chainNodeSet = _slateData->ChainNodeSet(InFocusNode);
			for (auto& item : chainNodeSet)
			{
				if (_nodeData->Id() == item->Id())
				{
					isHightLight = true;
					break;
				}
			}
		}
	}
	ToggleAnimation(_effectFocus, isHightLight);

	bool isSelected = (InFocusNode && _nodeData->Id() == InFocusNode->Id());
	ToggleAnimation(_effectSelected, isSelected);
}

const FGsMonsterKnowledgeSlateNode* UGsUIMonsterCollectionSlateNode::GetData() const
{
	return _nodeData;
}

int32 UGsUIMonsterCollectionSlateNode::ResourceId() const
{
	return _resourceId;
}