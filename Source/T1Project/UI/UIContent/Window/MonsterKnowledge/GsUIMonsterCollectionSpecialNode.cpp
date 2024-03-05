// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIMonsterCollectionSpecialNode.h"
#include "MonsterKnowledgeCollection/GsSlateSpecialNode.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaSlateSpecialNode.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "../../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "T1Project.h"


void UGsUIMonsterCollectionSpecialNode::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSelected->OnClicked.AddDynamic(this, &UGsUIMonsterCollectionSpecialNode::OnClcikedBtn);
}

void UGsUIMonsterCollectionSpecialNode::NativeConstruct()
{
	Super::NativeConstruct();

	_data = nullptr;
	_selectedCallBack = nullptr;

	if (false == _vfxNodeUse->OnVFXAnimationFinished.IsBoundToObject(this))
		_vfxNodeUse->OnVFXAnimationFinished.BindUObject(this, &UGsUIMonsterCollectionSpecialNode::OnFinishedNodeUse);
	if (false == _vfxNodeSuccess->OnVFXAnimationFinished.IsBoundToObject(this))
		_vfxNodeSuccess->OnVFXAnimationFinished.BindUObject(this, &UGsUIMonsterCollectionSpecialNode::OnFinishedNodeResult);
	if (false == _vfxNodeFail->OnVFXAnimationFinished.IsBoundToObject(this))
		_vfxNodeFail->OnVFXAnimationFinished.BindUObject(this, &UGsUIMonsterCollectionSpecialNode::OnFinishedNodeResult);
}

void UGsUIMonsterCollectionSpecialNode::OnClcikedBtn()
{
	if (_data && _selectedCallBack)
	{	// 2023/11/7 PKT - Selected Event 전달
		_selectedCallBack(EMonsterCollectionSlateCallbackReason::SelectedSpecialNode, _data->GetId());
	}
}

void UGsUIMonsterCollectionSpecialNode::OnFinishedNodeUse()
{
	OnFinishNodeUse.Broadcast();
}

void UGsUIMonsterCollectionSpecialNode::OnFinishedNodeResult()
{
	OnFinishNodeResult.Broadcast();
}

void UGsUIMonsterCollectionSpecialNode::SetData(const class FGsSlateSpecialNode* InData, TFunction<void(EMonsterCollectionSlateCallbackReason, uint64)> InSelectedCallBack /* = nullptr */)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_data = InData;
	_selectedCallBack = InSelectedCallBack;

	// 2023/11/7 PKT - CallBack 정보가 없으면 버튼 숨김 처리
	_btnSelected->SetVisibility(_selectedCallBack ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	/**
	 * Image 
	 */
	_imageNodeIconLock->SetVisibility(ESlateVisibility::Collapsed);
	_imageNodeIconUnlock->SetVisibility(ESlateVisibility::Collapsed);
	_imageNodeIconComplete->SetVisibility(ESlateVisibility::Collapsed);

	if (const FGsSchemaSlateSpecialNode* schemaSpecialNodeData = _data->GetSchemaData())
	{
		TWeakObjectPtr<UImage> lockImag = _imageNodeIconLock;
		TWeakObjectPtr<UImage> unlockImag = _imageNodeIconUnlock;
		FGsUIHelper::AsyncLoadSpriteFromPath(schemaSpecialNodeData->nodeSprite, FStreamableDelegateParam::CreateWeakLambda(this, [lockImag, unlockImag](UObject* InSprite)
			{
				if (lockImag.IsValid())
				{
					lockImag.Get()->SetBrushFromAtlasInterface(Cast<UPaperSprite>(InSprite));
					lockImag.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
				}

				if (unlockImag.IsValid())
				{
					unlockImag.Get()->SetBrushFromAtlasInterface(Cast<UPaperSprite>(InSprite));
					unlockImag.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
				}
			}
		)
		);

		TWeakObjectPtr<UImage> activeImag = _imageNodeIconComplete;
		FGsUIHelper::AsyncLoadSpriteFromPath(schemaSpecialNodeData->nodeActiveSprite, FStreamableDelegateParam::CreateWeakLambda(this, [activeImag](UObject* InSprite)
			{
				if (activeImag.IsValid())
				{
					activeImag.Get()->SetBrushFromAtlasInterface(Cast<UPaperSprite>(InSprite));
					activeImag.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
				}
			}
		)
		);
	}	

	SetSelectedEffect(false);

	// 2023/11/7 PKT - 상태에 따른 UI 셋팅
	Invalidate();

	_vfxNodeUse->SetVisibility(ESlateVisibility::Collapsed);
	_vfxNodeSuccess->SetVisibility(ESlateVisibility::Collapsed);
	_vfxNodeFail->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIMonsterCollectionSpecialNode::Invalidate()
{
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;
	}

	// 2023/11/7 PKT - 딱히 좋아 보이진 않지만 int로 바로 Cast( 순서 바뀌면 안됨. 상태 추가 되도 안됨. )
	_widgetSwitcherNodeState->SetActiveWidgetIndex(StaticCast<int32>(_data->GetState()));
}

const FGsSlateSpecialNode* UGsUIMonsterCollectionSpecialNode::GetData() const
{
	return _data;
}

bool UGsUIMonsterCollectionSpecialNode::IsSelected() const
{
	return (ESlateVisibility::Collapsed != _selectedOverlay->GetVisibility());
}

void UGsUIMonsterCollectionSpecialNode::SetSelectedEffect(bool InIsSelected)
{
	// 2023/11/7 PKT - Show Selected Image
	_selectedOverlay->SetVisibility(InIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIMonsterCollectionSpecialNode::PlayNodeUse() const
{
	_vfxNodeUse->ShowVFX();
}

void UGsUIMonsterCollectionSpecialNode::PlayNodeSuccess() const
{
	_vfxNodeSuccess->ShowVFX();
}

void UGsUIMonsterCollectionSpecialNode::PlayNodeFail() const
{
	_vfxNodeFail->ShowVFX();
}
