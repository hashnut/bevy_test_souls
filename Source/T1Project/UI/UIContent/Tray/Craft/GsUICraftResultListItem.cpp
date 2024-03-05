#include "GsUICraftResultListItem.h"

#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"

#include "T1Project.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UMG/Public/Components/TextBlock.h"

/**
 * 초기 구현과 달라진 점 :
 * 
 * 기존에 DynamicPanelSlotHelper 를 사용했던 구조를 TileView 로 바꾸는 과정에서, 네이밍이 이상해짐
 * 일반적으로 ListView 에서 데이터 클래스 뒤에는 Item 을, 엔트리는 Entry 를 붙여 준다.
 * 기존에는 GsUICraftResultListItem 이 데이터/위젯 역할을 전부 하고 있었는데 출시 전 갈아 엎기가 애매해서
 * 데이터를 담당하는 클래스를 추가하고 Item 대신 ItemData 를 붙여 네이밍했다. 
 * (네이밍이 아쉽고 헷갈릴 여지가 존재함)
 */


/**
 * TileView 에 쓰일 실제 데이터 클래스
 */ 

/** UGsCraftResultListItemData */




/**
 * TileView 에 쓰일 엔트리를 위한 클래스 (UGsUITrayCraftResult 에 있는 TileView)
 */
void UGsUICraftResultListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUICraftResultListItem::NativeOnListItemObjectSet(UObject* InListItemData)
{
	UGsCraftResultListItemData* itemData = Cast<UGsCraftResultListItemData>(InListItemData);
	if (nullptr == itemData)
	{
		GSLOG(Error, TEXT("nullptr == itemData"));
		return;
	}

	// 모두 오픈 시, 잠깐 아이템이 그려지고, 그 직후 애니메이션이 재생되는 버그가 있어 예외처리
	// 애니메이션이 끝나고 itemData 의 IsSuccess 값을 조정
	if (!itemData->IsSuccess())
	{
		SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}

	// 만약 모든 아이템이 실패가 없다면, 오프닝 특수효과 없이 바로 모두 오픈 애니메이션 재생 (1회만)
	if (itemData->IsSuccess())
	{
		OpenCraftResultEntry(false, itemData->GetCraftResult());
	}

	// 각종 텍스트 및 효과 설정
	SetParameters(itemData->GetParameters());

	// 성공, 실패, 대실패 세팅
	_switcherResult->SetActiveWidgetIndex(StaticCast<int32>(itemData->GetCraftResult()));

	// 컨테이너 커버 숨길지 말지 설정
	SetOpened(itemData->IsOpened());

	IUserObjectListEntry::NativeOnListItemObjectSet(InListItemData);
}

bool UGsUICraftResultListItem::OpenCraftResultEntry(const bool InPlayOpening, const CraftResult InCraftResult)
{
	SetOpened(true);

	OnPlayAnimation();

	// 오프닝 연출 특수효과 재생할지 결정
	if (InPlayOpening)
	{
		_vfxOpening->ShowVFX();
	}
	else
	{
		_vfxOpening->HideVFX();
	}

	_switcherResult->SetActiveWidgetIndex(StaticCast<int32>(InCraftResult));
	UWidget* ActiveWidget = _switcherResult->GetActiveWidget();
	if (nullptr == ActiveWidget)
	{
		GSLOG(Error, TEXT("nullptr == ActiveWidget, _switcherResult->GetActiveWidgetIndex():%d"), _switcherResult->GetActiveWidgetIndex());
	}
	else
	{
		UGsUIVFX* InnerVfxs[] = { _vfxFail, _vfxSuccess, _vfxGreatSuccess };
		for (UGsUIVFX* InnerVfx : InnerVfxs)
		{
			if (InnerVfx->IsChildOf(ActiveWidget))
			{
				if (InPlayOpening)
				{
					InnerVfx->ShowVFX();
				}
				else
				{
					InnerVfx->HideVFX();
				}

				break;
			}
		}
	}

	return true;
}

void UGsUICraftResultListItem::SetParameters(const FCraftResultParameters& InParameters)
{
 	_txtTitle->SetText(InParameters._title);

	// 만약 장비의 경우, 강화단계를 표시한다
	if (InParameters._isEquipment)
	{
		SetEnchantLevelText(InParameters._enchantLevel, InParameters._itemGrade);
	}
	else
	{
		_txtAmount->SetText(FText::AsNumber(InParameters._amount));
	}

	// 아무것도 돌려 주지 않는 경우, 텍스트 숫자를 0으로 표시 (어색할 수는 있으나, 실패시 아이템 지급하는 경우와 구별해야 함)
	//_txtAmount->SetVisibility((0 < InParameters._amount) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_failMarker->SetVisibility((CraftResult::CRAFT_FAIL == InParameters._craftResult) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (!InParameters._icon.IsValid())
	{
		GSLOG(Error, TEXT("!InParameters._icon.IsValid(), InParameters._title:%s"), *InParameters._title.ToString());
	}
	else
	{
		UPaperSprite* IconPaperSprite = FGsUIHelper::LoadSpriteFromPath(InParameters._icon);
		if (nullptr == IconPaperSprite)
		{
			GSLOG(Error, TEXT("nullptr == IconPaperSprite, InParameters._icon:%s"), *InParameters._icon.ToString());
		}
		else
		{
			_imgIcon->SetBrushFromAtlasInterface(IconPaperSprite);
		}
	}

	// 아이템 등급에 따라 배경/텍스트 색상 변경
	if (ItemGrade::NONE == InParameters._itemGrade)
	{
		_rankImgWidgetSwicher->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		uint8 slotIndex = UGsUIIconItem::GetGradeNumByGradeType(InParameters._itemGrade);
		_rankImgWidgetSwicher->SetActiveWidgetIndex(slotIndex);
		_rankImgWidgetSwicher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	_txtTitle->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InParameters._itemGrade));
}

void UGsUICraftResultListItem::SetEnchantLevelText(Level InLevel, ItemGrade InItemGrade)
{
	if (nullptr != _txtAmount)
	{
		bool isShowingText = 1 <= InLevel ? true : false;
		_txtAmount->SetVisibility(true == isShowingText ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (true == isShowingText)
		{
			FString resultString;
			FText markText;
			FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), markText);
			resultString = markText.ToString() + FString::FormatAsNumber(InLevel);
			_txtAmount->SetText(FText::FromString(resultString));

			_txtAmount->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InItemGrade));
		}
	}
}

void UGsUICraftResultListItem::OnEntryWidgetVisibleEvent()
{
	UGsCraftResultListItemData* itemData = Cast<UGsCraftResultListItemData>(GetListItem());
	if (nullptr == itemData)
	{
		GSLOG(Error, TEXT("nullptr == itemData"));
		return;
	}

	itemData->SetIsSuccess(false);
}

void UGsUICraftResultListItem::SetOpened(const bool InIsOpened)
{
	_containerCover->SetVisibility(InIsOpened ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}