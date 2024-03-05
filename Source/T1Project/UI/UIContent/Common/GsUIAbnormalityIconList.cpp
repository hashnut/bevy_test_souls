// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIAbnormalityIconList.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconAbnormality.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "Management/GsMessageHolder.h"


void UGsUIAbnormalityIconList::BeginDestroy()
{
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIAbnormalityIconList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_slotHelper->Initialize(_iconSelector);
	_slotHelper->OnCreateIcon.AddDynamic(this, &UGsUIAbnormalityIconList::OnCreateIcon);
	_slotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIAbnormalityIconList::OnRefreshIcon);
}

void UGsUIAbnormalityIconList::NativeConstruct()
{
	Super::NativeConstruct();
	
	_messageHandler = GMessage()->GetGameObject().AddUObject(MessageGameObject::ABNORMALITY_UPDATE_NOT_LOCAL, this,
		&UGsUIAbnormalityIconList::OnUpdateAbnormality);
}

void UGsUIAbnormalityIconList::NativeDestruct()
{	
	GMessage()->GetGameObject().Remove(_messageHandler);

	_slotHelper->Clear();
	_iconSelector->RemoveAllChildren();
	_owner = nullptr;
	_dataList.Empty();

	Super::NativeConstruct();
}

void UGsUIAbnormalityIconList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	IGsInvalidateUIInterface::CheckInvalidate();
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIAbnormalityIconList::InvalidateAllInternal()
{
	if (false == _owner.IsValid())
	{
		return;
	}

	FGsAbnormalityHandlerBase* handler = _owner->GetAbnormalityHandler();
	if (nullptr == handler)
	{
		return;
	}

	// 데이터 얻어오기
	_dataList.Empty();
	handler->GetAbnormalityDataList(_dataList);

	_slotHelper->RefreshAll(_dataList.Num());
	
	// 정보가 없을 땐 Collapse 처리
	SetVisibility((0 < _dataList.Num()) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIAbnormalityIconList::InvalidateImmediately()
{
	InvalidateAllInternal();
}

void UGsUIAbnormalityIconList::OnUpdateAbnormality(const IGsMessageParam* InParam)
{
	const FGsGameObjectMessageParamAbnormality* param = InParam->Cast<const FGsGameObjectMessageParamAbnormality>();	
	if (nullptr == param ||
		nullptr == param->_paramOwner)
	{
		return;
	}

	if (false == _owner.IsValid())
	{
		return;
	}

	if (param->_paramOwner->GetGameId() != _owner->GetGameId())
	{
		return;
	}

	// Visibility가 꺼져있으면 틱이 돌지 않으며, 틱이 돌아야 InvalidateAllInternal 갱신이 되기 때문에 켜준다
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 갱신. 잦은 갱신을 막기 위해 InvalidateInterface 사용.	
	InvalidateAll();
}

void UGsUIAbnormalityIconList::OnCreateIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconAbnormality* icon = Cast<UGsUIIconAbnormality>(InIcon);
	if (icon)
	{
		// 클릭 이벤트 연결
		icon->OnClickIcon.AddUObject(this, &UGsUIAbnormalityIconList::OnClickSlot);
	}
}

void UGsUIAbnormalityIconList::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	UGsUIIconAbnormality* icon = Cast<UGsUIIconAbnormality>(InIcon);
	if (icon)
	{
		// 값 세팅
		icon->SetData(_dataList[InIndex]);
	}
}

void UGsUIAbnormalityIconList::OnClickSlot(UGsUIIconAbnormality* InIcon)
{
	if (OnClickList.IsBound())
	{
		OnClickList.Broadcast(InIcon);
	}
}

void UGsUIAbnormalityIconList::SetTarget(UGsGameObjectBase* InTarget)
{ 
	_owner = InTarget;

	// 타겟 잡힌 즉시 갱신 한 번 수행. Visibility 때문 
	InvalidateAllInternal();
}
