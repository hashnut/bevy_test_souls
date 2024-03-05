#include "GsUIHUDSpiritShot.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/UseItemEffect/GsUseItemEffectHandler.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/UseItemEffect/SpiritShot/GsSpiritShotDefine.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemConsumable.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"

#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"

#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemSimple.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "Net/GsNetSendServiceWorld.h"


void UGsUIHUDSpiritShot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_potionTouchEffect->HideVFX();

	_checkerCloseLock.SetIntervalSecond(_closeLockTime);
}

void UGsUIHUDSpiritShot::NativeConstruct()
{

	if (_imgDimmed)
	{
		_imgDimmed->SetVisibility(ESlateVisibility::Hidden);
	}
	GItem()->PickHudSpiritShotItem();

	_blockProc = false;

	_hudHandlerList.Emplace(GMessage()->GetContentsHud().AddUObject(MessageContentHud::TOGGLE_SPIRIT_SHOT,
		this, &UGsUIHUDSpiritShot::OnToggleSpiritShotState));

	_gameObjectHandlerList.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::LOCAL_UPDATE_SPIRIT_SHOT,
		this, &UGsUIHUDSpiritShot::OnUpdateOption));

	Super::NativeConstruct();
}

void UGsUIHUDSpiritShot::NativeDestruct()
{
	_iconSelector->ClearChildren();

	if (_uiIcon)
	{
		// _uiIcon이 Destruct 시 OnFinishedItemIconCoolTime.Clear()하긴 하지만 명시적으로 해제 코드 
		if (_uiIcon->OnFinishedItemIconCoolTime.IsBound())
		{
			_uiIcon->OnFinishedItemIconCoolTime.Clear();
		}
	}

	// 아이콘 Panel에서 제거, Pool에 반납이 이루어진다.
	_iconSelector->RemoveAllChildren();

	_uiIcon = nullptr;

	if(FGsMessageHolder* msg = GMessage())
	{ 
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
	}

	Super::NativeDestruct();
}

void UGsUIHUDSpiritShot::InvalidateAllInternal()
{
	// 추후 여러 아이템이 등록 가능하다면, 서버패킷 연동 처리 필요
	if (nullptr == _uiIcon)
	{
		_uiIcon = Cast<UGsUIIconItemSimple>(_iconSelector->CreateIcon());
		_iconSelector->AddChild(_uiIcon);
		//_uiIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);

		if (false == _uiIcon->OnClickSlot.IsBoundToObject(this))
		{
			_uiIcon->OnClickSlot.AddUObject(this, &UGsUIHUDSpiritShot::OnClickItem);
		}
	}

	if (_uiIcon == nullptr)
	{
		return;
	}

	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}

	EGsSpiritShotState state = EGsSpiritShotState::HUD_OFF;
	if (FGsGameObjectDataLocalPlayer* data = local->GetCastData<FGsGameObjectDataLocalPlayer>())
	{
		state = data->GetSpiritShotState();
	}

	bool isActive = (state == EGsSpiritShotState::HUD_ON) ? true : false;

	_uiIcon->SetActiveOnEffect(isActive);

	uint64 itemDBID = GItem()->GetSpiritShotItemDBID();
	if (0 < itemDBID)
	{		
		_uiIcon->SetOwnedItem(itemDBID);
		_uiIcon->SetAllOwnedItemCountDisplay();
		_uiIcon->SetImageGrayscale(false);		
	}
	else
	{		
		int itemTblId = 0;
		if (const FGsSchemaItemCommon* item = GItem()->FindItemSchemaDataBySubCategory(ItemCategorySub::SPIRIT_SHOT))
		{
			itemTblId = item->id;
		}
		_uiIcon->SetNotOwnItem(itemTblId, 0);
		_uiIcon->SetImageGrayscale(true);		
	}
	_uiIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
}

void UGsUIHUDSpiritShot::OnClickItem(UGsUIIconItem& InIcon)
{
	ToggleSpiritShotState();
}
void UGsUIHUDSpiritShot::ClickedSpiritShot()
{
	ToggleSpiritShotState();
}

void UGsUIHUDSpiritShot::SetEnableButton(bool bInEnable)
{
	if (_imgDimmed)
	{
		_imgDimmed->SetVisibility((bInEnable) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

void UGsUIHUDSpiritShot::ToggleSpiritShotState()
{
	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);

	if (_blockProc)
		return;

	// 시간이 되지 않음
	if (_checkerCloseLock.IsTimeOver() == false)
	{
		return;
	}
	// 텀 갱신
	_checkerCloseLock.UpdateTime();

	// 서버에 아이템 on, off 상태 변경 요청
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}

	if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(local))
	{
		// 요청할꺼는 현상태랑 반대
		bool reqIsUIOn = true;

		EGsSpiritShotState state = EGsSpiritShotState::HUD_OFF;
		FGsGameObjectDataLocalPlayer* data = localPlayer->GetCastData<FGsGameObjectDataLocalPlayer>();
		if (data != nullptr)
		{
			state = data->GetSpiritShotState();

			switch (state)
			{
			case EGsSpiritShotState::HUD_ON:
			{
				reqIsUIOn = false;
			}
			break;
			}
		}

		GMessage()->GetContentsHud().SendMessage(MessageContentHud::TOGGLE_SPIRIT_SHOT);

		FGsNetSendServiceWorld::SendReqSpiritShotOnOff(reqIsUIOn);
	}
}

void UGsUIHUDSpiritShot::OnToggleSpiritShotState()
{
	// 토글 시 클릭 이펙트 출력
	_potionTouchEffect->ShowVFX();

	// 패킷을 받아 OnUpdateOption 호출되면 _blockProc을 false로 초기화한다.
	_blockProc = true;
}

void UGsUIHUDSpiritShot::OnUpdateOption(const struct IGsMessageParam*)
{
	_blockProc = false;
}

