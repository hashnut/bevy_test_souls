// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDConsumable.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

#include "DataCenter/Public/Option/GsOptionEnumType.h"
#include "DataSchema/Option/GsSchemaAutoPotionTabIndexInfo.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "Data/GsDataContainManager.h"

#include "GameObject/Define/GsGameObjectDefine.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemConsumable.h"

#include "Input/GsInputEventMsgBase.h"

#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContentHud.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Net/GsNetSendServiceItem.h"

#include "Option/GsServerOption.h"

#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemSimple.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "T1Project.h"


UGsUIHUDConsumable::UGsUIHUDConsumable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGsUIHUDConsumable::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_potionTouchEffect->HideVFX();
}

void UGsUIHUDConsumable::NativeConstruct()
{
	if (_imgDimmed)
	{
		_imgDimmed->SetVisibility(ESlateVisibility::Hidden);
	}

	_blockProc = false;

	_hudHandlerList.Emplace(GMessage()->GetContentsHud().AddUObject(MessageContentHud::TOGGLE_AUTO_POTION,
		this, &UGsUIHUDConsumable::OnToggleAutoPotionState));

	_gameObjectHandlerList.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::LOCAL_UPDATE_AUTO_POTION,
		this, &UGsUIHUDConsumable::OnUpdateOption));

	Super::NativeConstruct();
}

void UGsUIHUDConsumable::NativeDestruct()
{
	_iconSelector->ClearChildren();

	// 아이콘 Panel에서 제거, Pool에 반납이 이루어진다.
	_iconSelector->RemoveAllChildren();

	_uiIcon = nullptr;

	FGsMessageHolder* msg = GMessage();
	MHud& contentsHUD = msg->GetContentsHud();
	for (auto& iter : _hudHandlerList)
	{
		contentsHUD.Remove(iter);
	}
	_hudHandlerList.Empty();

	MGameObject& gameObject = msg->GetGameObject();
	for (auto& iter : _gameObjectHandlerList)
	{
		gameObject.Remove(iter);
	}
	_gameObjectHandlerList.Empty();

	Super::NativeDestruct();
}

void UGsUIHUDConsumable::InvalidateAllInternal()
{
	if (nullptr == _uiIcon)
	{
		_uiIcon = Cast<UGsUIIconItemSimple>(_iconSelector->CreateIcon());
		_iconSelector->AddChild(_uiIcon);
	}

	if (nullptr == _uiIcon)
		return;

	// 패킷에서 사용중인 아이템 dbid 내려준다고 함.
	// 그때까지는 임시로 클라에서 처리함
	TArray<TWeakPtr<FGsItem>> findItemList = GItem()->FindItem(ItemType::CONSUME, ItemCategorySub::HP_POTION);
	if (0 >= findItemList.Num())
	{
		ItemId itemid = 0;
		if (const FGsSchemaItemCommon* item = GItem()->FindItemSchemaDataBySubCategory(ItemCategorySub::HP_POTION))
		{
			itemid = item->id;
		}
		_uiIcon->SetNotOwnItem(itemid, 0);
		_uiIcon->SetImageGrayscale(true);
		//_uiIcon->SetEmptyStateBG(true);

		if (false == _uiIcon->OnClickSlot.IsBoundToObject(this))
		{
			_uiIcon->OnClickSlot.AddUObject(this, &UGsUIHUDConsumable::OnClickItem);
		}
		if (false == _uiIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIHUDConsumable::OnLongPressIcon);
		}
	}
	else
	{
		if (false == _uiIcon->OnClickSlot.IsBoundToObject(this))
		{
			_uiIcon->OnClickSlot.AddUObject(this, &UGsUIHUDConsumable::OnClickItem);
		}
		if (false == _uiIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIHUDConsumable::OnLongPressIcon);
		}

		if(true == findItemList[0].IsValid())
		{
			_uiIcon->SetOwnedItem(findItemList[0].Pin()->GetDBID());
			_uiIcon->SetAllOwnedItemCountDisplay();
			_uiIcon->SetImageGrayscale(false);
		}
	}

	if (FGsServerOption* serverOption = GServerOption())
	{
		_isActiveAutoUse = serverOption->IsAutoUsePotion();
	}

	_uiIcon->SetActiveOnEffect(_isActiveAutoUse);
}

void UGsUIHUDConsumable::OnClickItem(UGsUIIconItem& InIcon)
{
	ToggleAutoPotionState();
}

void UGsUIHUDConsumable::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	OnOpenOption();
}

void UGsUIHUDConsumable::OnClickItemWindowsKey()
{
	// 키 입력 로직은 UGsUIHUDFrameMain::OnKeyboardActionOptionKey 를 통한다.
	// 절전모드에서는 받지 않는다.
	ToggleAutoPotionState();
}

void UGsUIHUDConsumable::SetEnableButton(bool bInEnable)
{
	if (_imgDimmed)
	{
		_imgDimmed->SetVisibility((bInEnable) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

void UGsUIHUDConsumable::ToggleAutoPotionState()
{
	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);

	if (_blockProc)
		return;
	
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::TOGGLE_AUTO_POTION);
	
	FGsNetSendServiceItem::SendUseAutoPotionState(!_isActiveAutoUse);
}

void UGsUIHUDConsumable::OnOpenOption()
{
	const UGsTable* tableData = FGsSchemaAutoPotionTabIndexInfo::GetStaticTable();
	if (nullptr == tableData)
	{
		return;
	}
	TArray<const FGsSchemaAutoPotionTabIndexInfo*> allrow;
	if (false == tableData->GetAllRows(allrow))
	{
		return;
	}
	if (false == allrow.IsValidIndex(0))
	{
		return;
	}
	const FGsSchemaAutoPotionTabIndexInfo* data = allrow[0];
	if (nullptr == data)
		return;

	UGsOptionManager* optionMgr = GOption();
	if (nullptr == optionMgr)
		return;

	optionMgr->SetOptionWindowMenuIndex(data->parentTabIndex);
	optionMgr->SetOptionWindowDivisionIndex(data->childTabIndex);
	optionMgr->SetScrollOffset(0.f);

	GUI()->OpenAndGetWidget("WindowOption");

	//GSLOG(Error, TEXT("data->parentTabIndex : %d"), data->parentTabIndex);
	//GSLOG(Error, TEXT("data->childTabIndex : %d"), data->childTabIndex);
}

void UGsUIHUDConsumable::OnToggleAutoPotionState()
{
	// 토글 시 클릭 이펙트 출력
	_potionTouchEffect->ShowVFX();

	// 패킷을 받아 OnUpdateOption 호출되면 _blockProc을 false로 초기화한다.
	_blockProc = true;
}

void UGsUIHUDConsumable::OnUpdateOption(const struct IGsMessageParam*)
{
	_blockProc = false;
}

const FGsItem* UGsUIHUDConsumable::GetIconItemData()
{
	if (_uiIcon)
	{
		return _uiIcon->GetItemIconData();
	}
	return nullptr;
}
