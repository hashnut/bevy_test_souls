// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupAbnormalityInfo.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/AbnormalityInfo/GsUIAbnormalityInfoEntry.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/Abnormality/GsAbnormalityDataDefine.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

void UGsUIPopupAbnormalityInfo::BeginDestroy()
{
	_toggleGroupTab.Clear();

	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupAbnormalityInfo::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupAbnormalityInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 닫기 버튼 이벤트 세팅
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupAbnormalityInfo::OnClickClose);

	// 탭 메뉴 생성 및 이벤트 세팅
	_toggleGroupTab.AddToggleWidgetByParentPanel(_panelTabRoot);
	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIPopupAbnormalityInfo::OnClickTab);	

	// 리스트 이벤트 세팅
	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelListRoot);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupAbnormalityInfo::OnRefreshEntry);
}

void UGsUIPopupAbnormalityInfo::NativeConstruct()
{
	Super::NativeConstruct();

	// 첫 탭을 기본으로 지정
	_toggleGroupTab.SetSelectedIndex(0, true);

	// 스크롤 리셋
	_scrollBox->ScrollToStart();

	RegisterMessages();	

	InvalidateAll();
}

void UGsUIPopupAbnormalityInfo::NativeDestruct()
{
	DeregisterMessages();

	_localPlayer = nullptr;
	_abnormalityList.Empty();
	_passivityList.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupAbnormalityInfo::InvalidateAllInternal()
{
	Super::InvalidateAllInternal();

	InvalidateList(_toggleGroupTab.GetSelectedIndex());
}

void UGsUIPopupAbnormalityInfo::OnInputCancel()
{
	Close();
}

void UGsUIPopupAbnormalityInfo::RegisterMessages()
{
	FGsMessageHolder* msgManager = GMessage();
	if (nullptr == msgManager)
	{
		return;
	}

	MGameObject& msgGameObject = msgManager->GetGameObject();
	_msgHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_ABNORMALITY_ADD,
		this, &UGsUIPopupAbnormalityInfo::OnAddAbnormality));
	_msgHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_ABNORMALITY_REMOVE,
		this, &UGsUIPopupAbnormalityInfo::OnRemoveAbnormality));
	_msgHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::PASSIVITY_ADD,
		this, &UGsUIPopupAbnormalityInfo::OnAddPassivity));
	_msgHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::PASSIVITY_REMOVE,
		this, &UGsUIPopupAbnormalityInfo::OnRemovePassivity));

	_msgUIHandle = msgManager->GetUI().AddUObject(MessageUI::BLESS_ARCAS_POINT,
		this, &UGsUIPopupAbnormalityInfo::OnArcasPoint);
}

void UGsUIPopupAbnormalityInfo::DeregisterMessages()
{
	FGsMessageHolder* msgManager = GMessage();
	if (nullptr == msgManager)
	{
		return;
	}

	MGameObject& msgGameObject = msgManager->GetGameObject();
	for (MsgGameObjHandle& handle : _msgHandleList)
	{
		msgGameObject.Remove(handle);
	}
	_msgHandleList.Empty();

	msgManager->GetUI().Remove(_msgUIHandle);
}

void UGsUIPopupAbnormalityInfo::InvalidateList(int32 InIndex)
{
	FGsAbnormalityHandlerBase* abnormalityHandler = GetAbnormalityHandler();
	if (nullptr == abnormalityHandler)
	{
		return;
	}

	_abnormalityList.Empty();
	_passivityList.Empty();

	int32 dataCount = 0;

	// 0: 효과정보, 1: 패시브
	if (0 == InIndex)
	{
		abnormalityHandler->GetAbnormalityDataList(_abnormalityList);
		if (0 == _abnormalityList.Num())
		{
			// Text: 적용 중인 효과가 없습니다.
			FText::FindText(TEXT("AbnormalityText"), TEXT("AbnormalBlankMsg_Normal"), _textEmptyList);
		}
		else
		{
			dataCount = _abnormalityList.Num();
		}
	}
	else if (1 == InIndex)
	{
		abnormalityHandler->GetPassiveDataList(_passivityList);
		if (0 == _passivityList.Num())
		{
			// TEXT: 적용 중인 패시브 효과가 없습니다.
			FText::FindText(TEXT("AbnormalityText"), TEXT("AbnormalBlankMsg_Passive"), _textEmptyList);
		}
		else
		{
			dataCount = _passivityList.Num();
			_passivityList.Sort([](const FGsPassivityData& lhs, const FGsPassivityData& rhs)
				{
					return lhs._ownerType == PassivityOwnerType::ARCAS;
				}
			);
		}
	}

	// 리스트가 비어있을 경우 보여주는 UI
	_panelEmptyRoot->SetVisibility((0 == dataCount) ? 
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	// 리스트 갱신
	_slotHelper->RefreshAll(dataCount);
}

void UGsUIPopupAbnormalityInfo::OnAddAbnormality(const IGsMessageParam* InParam)
{
	InvalidateAll();
}

void UGsUIPopupAbnormalityInfo::OnRemoveAbnormality(const IGsMessageParam* InParam)
{
	// 주의: 데이터가 지워지기 전에 이 메시지가 오지만, InvalidateAll 으로인해 다음틱에 갱신을 하므로 다른 처리를 안함
	InvalidateAll();
}

void UGsUIPopupAbnormalityInfo::OnAddPassivity(const IGsMessageParam* InParam)
{
	const FGsGameObjectMessageParamPassivity* param = InParam->Cast<const FGsGameObjectMessageParamPassivity>();
	if (param && param->_paramOwner)
	{
		if (EGsGameObjectType::LocalPlayer == param->_paramOwner->GetObjectType())
		{
			// 갱신. 잦은 갱신을 막기 위해 InvalidateInterface 사용.
			InvalidateAll();
		}
	}
}

void UGsUIPopupAbnormalityInfo::OnRemovePassivity(const IGsMessageParam* InParam)
{
	const FGsGameObjectMessageParamPassivity* param = InParam->Cast<const FGsGameObjectMessageParamPassivity>();
	if (param && param->_paramOwner)
	{
		if (EGsGameObjectType::LocalPlayer == param->_paramOwner->GetObjectType())
		{
			// 갱신. 잦은 갱신을 막기 위해 InvalidateInterface 사용.
			InvalidateAll();
		}
	}
}

void UGsUIPopupAbnormalityInfo::OnArcasPoint(const IGsMessageParam*)
{
	if (1 == _toggleGroupTab.GetSelectedIndex())
	{
		_slotHelper->RefreshByIndex(0);
	}
}

void UGsUIPopupAbnormalityInfo::OnClickTab(int32 InIndex)
{
	InvalidateList(InIndex);
}

void UGsUIPopupAbnormalityInfo::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	int32 currTabIndex = _toggleGroupTab.GetSelectedIndex();
	if (0 == currTabIndex)
	{
		// 상태이상
		if (_abnormalityList.IsValidIndex(InIndex))
		{
			if (UGsUIAbnormalityInfoEntry* slot = Cast<UGsUIAbnormalityInfoEntry>(InEntry))
			{
				slot->SetData(_abnormalityList[InIndex]);
			}
		}

	}
	else if (1 == currTabIndex)
	{
		// 패시브
		if (_passivityList.IsValidIndex(InIndex))
		{
			if (UGsUIAbnormalityInfoEntry* slot = Cast<UGsUIAbnormalityInfoEntry>(InEntry))
			{
				slot->SetData(_passivityList[InIndex]);
			}
		}
	}
}

void UGsUIPopupAbnormalityInfo::OnClickClose()
{
	OnInputCancel();
}

UGsGameObjectBase* UGsUIPopupAbnormalityInfo::GetLocalPlayer()
{
	if (false == _localPlayer.IsValid())
	{
		if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
		{
			_localPlayer = gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer);
		}
	}

	return _localPlayer.Get();
}

FGsAbnormalityHandlerBase* UGsUIPopupAbnormalityInfo::GetAbnormalityHandler()
{
	if (UGsGameObjectBase* owner = GetLocalPlayer())
	{
		return owner->GetAbnormalityHandler();
	}

	return nullptr;
}
