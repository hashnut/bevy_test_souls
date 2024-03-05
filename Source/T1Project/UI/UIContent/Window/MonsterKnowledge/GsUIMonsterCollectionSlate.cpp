#include "GsUIMonsterCollectionSlate.h"
#include "../Item/GsItem.h"
#include "../Item/GsItemManager.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../Management/GsMessageHolder.h"
#include "../Message/GsMessageContents.h"
#include "../Message/MessageParam/GsItemContentsMessageParam.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "../UI/UIContent/Define/EGsUIColorType.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "../UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "T1Project.h"


void UGsUIMonsterCollectionSlate::BeginDestroy()
{
	if (_btnSelected)
	{
		_btnSelected->OnClicked.RemoveDynamic(this, &UGsUIMonsterCollectionSlate::OnClicked);
	}

	Super::BeginDestroy();
}

void UGsUIMonsterCollectionSlate::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_data = nullptr;

	_btnSelected->OnClicked.AddDynamic(this, &UGsUIMonsterCollectionSlate::OnClicked);
}

void UGsUIMonsterCollectionSlate::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == _btnSelected->_onFinishedLongPressGauge.IsBoundToObject(this))
	{
		_btnSelected->_onFinishedLongPressGauge.AddUObject(this, &UGsUIMonsterCollectionSlate::OnLongPressedSlate);
	}
}

void UGsUIMonsterCollectionSlate::LoadImage(const FSoftObjectPath& InPath)
{
	// 2021/11/28 PKT - Image가 로드 되기 전까지 화면에서 가린다.
	SetImageSprite(nullptr);

	_lastSoftObjectPath = InPath;

	// 로드되는 사이 이 객체가 어떻게 될지 모르므로 TWeakObjectPtr 사용
	TWeakObjectPtr<UGsUIMonsterCollectionSlate> weakThis(this);
	FGsUIHelper::AsyncLoadSpriteFromPath(InPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, InPath](UObject* inRes)
		{
			// 로드 끝난후라 LoadSpriteFromPath 시 새로 로드하지 않고, 로드된 항목을 찾음
			if (weakThis.IsValid() && weakThis->_lastSoftObjectPath == InPath)
			{
				weakThis->SetImageSprite(Cast<UPaperSprite>(inRes));
			}
		}));
}

void UGsUIMonsterCollectionSlate::SetImageSprite(class UPaperSprite* InSprite)
{
	if (InSprite)
	{
		_imgSlate->SetBrushFromAtlasInterface(InSprite);
		_imgSlate->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_imgSlate->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIMonsterCollectionSlate::OnClicked()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, true);
	}
}

void UGsUIMonsterCollectionSlate::OnLongPressedSlate()
{
	if (nullptr == _data)
	{
		return;
	}

	ItemId costItemId = _data->DisplayCostItemId();
	
	const FGsItemDetailOpenMessageParamTID param(costItemId, true, true, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_TID, &param);
}

void UGsUIMonsterCollectionSlate::SetIsSelected(bool bInIsSelected)
{
	_imgSelected->SetVisibility(bInIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

bool UGsUIMonsterCollectionSlate::GetIsSelected() const
{
	return ESlateVisibility::SelfHitTestInvisible == _imgSelected->GetVisibility();
}

void UGsUIMonsterCollectionSlate::SetData(const FGsMonsterKnowledgeSlate* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_data = InData;

	int32 switcherIndex = StaticCast<int32>(_data->Grade());
	_gradeImgWidgetSwicher->SetActiveWidgetIndex(switcherIndex);
	
	LoadImage(_data->ImagePath());

	SetIsSelected(false);

	// 2023/3/2 PKT - Long Press 활성화
	_btnSelected->OnChangeLongPress(true);

	Invalidate();
}

void UGsUIMonsterCollectionSlate::Invalidate()
{
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;
	}

	bool isActive = (EMonsterKnowledgeSlateState::UnLock == _data->State());
	bool isHaveCost = 0 < _data->HaveCandinateCostAmount();
	bool isRedDot = ((false == isActive) && (true == isHaveCost));

	_panelLock->SetVisibility(isActive ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	_redDot->SetRedDot(isRedDot);
}

const FGsMonsterKnowledgeSlate* UGsUIMonsterCollectionSlate::GetData() const
{
	return _data;
}