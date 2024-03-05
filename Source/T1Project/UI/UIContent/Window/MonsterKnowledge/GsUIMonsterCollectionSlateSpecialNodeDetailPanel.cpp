#include "GsUIMonsterCollectionSlateSpecialNodeDetailPanel.h"
#include "GsUIMonsterCollectionSpecialNode.h"
#include "../MonsterKnowledgeCollection/GsSlateSpecialNode.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeUICaptureData.h"
#include "../GameObject/Stat/GsStatHelper.h"
#include "../Currency/GsCostPackage.h"
#include "../SpiritShot/GsUISpiritShotMaterialListItem.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMonsterKnowledgeCollection.h"
#include "Engine/Public/TimerManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"



void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOpenNode->OnClicked.AddDynamic(this, &UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::OnClickedOpenNode);

	MMonsterKnowledgeCollection& MessageUiHolder = GMessage()->GetMonsterKnowledgeCollection();
	_messageUiDelegate = MessageUiHolder.AddUObject(MessageContentMonsterKnowledge::SPECIAL_NODE_EFFECT, this, &UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::PlaySpecialNodeResult);
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::NativeConstruct()
{
	Super::NativeConstruct();

	_focusNodeData = nullptr;

	// 사용 관련해서는 딱히 잠금 처리 안해줘도 된다. 현재 애니메이션을 다 봐야 재생되도록 수정함 -> 일단 전환시 바로 재생
	// specialNodeState 내의 GsUIVFX 애니메이션이 끝나도, 아래 바인딩된 함수가 불리지 않는다. 이유는 도저히 알 수가 없다.
	// 일단 궁여지책으로, Timer 를 이용해 구현하도록 한다. 누군가 GsUIVFX 의 애니메이션이 끝났는데 바인딩된 함수가 불리지 않는 
	// 이유를 알아낸다면... 델리게이트를 이용하도록 수정해 주길 바람.
	//if (_specialNodeState->OnFinishNodeUse.IsBound() == false)
	//	_specialNodeState->OnFinishNodeUse.AddDynamic(this, &UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::OnFinishedSpecialNodeOpen);
	if (_specialNodeState->OnFinishNodeResult.IsBound() == false)
		_specialNodeState->OnFinishNodeResult.AddDynamic(this,		&UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::OnFinishedSpecialNodeResult);
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::OnClickedOpenNode()
{
	// 딤드는 재화 관련 로직에 엮여 있어서, 딤드까지는 아니고 클릭할 수 있는지 여부 정도만 체크하도록 하겠음
	if (_isNodeVfxPlaying)
		return;

	// 이제 개방 애니메이션 다 봐야 패킷 보내도록 수정
	//if (_focusNodeData && _callbackFunc)
	//{
	//	_callbackFunc(EMonsterCollectionSlateCallbackReason::ReqOpenSpecialNode, _focusNodeData->GetId());
	//}
	
	// 특수 노드 개방 애니메이션 연출 재생
	_specialNodeState->PlayNodeUse();
	_isNodeVfxPlaying = true;

	FGsUIHelper::Play2DSound(SOUND_SPECIAL_OPEN);

	OnStartSpecialNodeOpen();
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::OnStartSpecialNodeOpen()
{
	// 블루프린트에서 더미 애니메이션 재생할 뿐이다 -> Timer 로 바꿔서, 일정 시간 후 개방 요청 패킷을 전송한다
	GetWorld()->GetTimerManager().SetTimer(_openTimer, FTimerDelegate::CreateLambda([&]()
		{
			OnFinishedSpecialNodeOpen();

			GetWorld()->GetTimerManager().ClearTimer(_openTimer);
		}), 0.77f, false); 
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::OnFinishedSpecialNodeOpen()
{
	if (_focusNodeData && _callbackFunc)
	{
		_callbackFunc(EMonsterCollectionSlateCallbackReason::ReqOpenSpecialNode, _focusNodeData->GetId());
	}
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::OnFinishedSpecialNodeResult()
{
	_isNodeVfxPlaying = false;
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::SetData(const FGsMonsterKnowledgeSlate* InData, TFunction<void(EMonsterCollectionSlateCallbackReason, uint64)> InCallback)
{
	Super::SetData(InData, InCallback);

	_detailType = ENodeDetailType::Special;
	
	_textSpecialNodeTitle->SetText(InData->Title());
	_isNodeVfxPlaying = false;
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::Invalidate()
{
	Super::Invalidate();

	if (nullptr == _focusNodeData)
	{
		return;
	}

	_specialNodeState->Invalidate();
	_isNodeVfxPlaying = false;

	const static int32 SWITCHER_MATERIAL_INDEX = 0;
	const static int32 SWITCHER_COMPLETE_INDEX = 1;
	_switcherSpecialNodeMaterialInfo->SetActiveWidgetIndex(EMonsterStoneSlateNodeState::Complete != _focusNodeData->GetState() ? SWITCHER_MATERIAL_INDEX : SWITCHER_COMPLETE_INDEX);	
	
	InvalidateCurrency();
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::InvalidateCurrency()
{
	if (nullptr == _focusNodeData)
	{
		return;
	}

	const auto costElement = _focusNodeData->GetCostPackage().Get().GetCostElementAt(0);
	if (costElement)
	{
		bool isEnoughCost = costElement->PlayerHasEnough();
		ItemId itemEntryId = costElement->GetItemId();
		if (isEnoughCost)
		{
			// 2023/11/14 PKT - 재료 아이템이 존재 한다면 1. 귀속 아이템이 있다면 귀속 아이템 이름으로 뿌려준다. 2. 귀속 아이템이 없고 비귀속 아이템만 있다면 비귀속으로 뿌려준다.
			ItemId buffItemId = INVALID_ITEM_ID;
			const TSet<ItemId> groupItemSet = UGsItemManager::GetAllItemIdsTheSameGroup(itemEntryId);
			for (const ItemId itemId : groupItemSet)
			{
				if (0 < GItem()->FindByTID(itemId).Num())
				{
					buffItemId = itemId;
					break;
				}
			}

			if (INVALID_ITEM_ID != buffItemId)
			{
				itemEntryId = buffItemId;
			}
		}

		UGsUISpiritShotMaterialListItem::Parameters param;
		param._costType = costElement->_costType;
		param._currencyType = costElement->GetCurrencyType();
		param._itemId = itemEntryId;
		param._itemLevel = costElement->GetItemLevel();
		param._requireAmount = costElement->GetCostAmount();
		param._ownedAmount = costElement->PlayerHasAmountMax();
		param._isEnoughCostElement = isEnoughCost;

		_materialInfo->SetParameters(param);
	}
	
	switch (_focusNodeData->GetState())
	{
		case EMonsterStoneSlateNodeState::Disable:
		{
			_btnOpenNode->SetIsEnabled(false);
			
			FText textBtnName;
			FText::FindText(TEXT("CodexUIText"), TEXT("UI_Slate_Button_OpenSpecialNode"), textBtnName);
			_textBtnTitle->SetText(textBtnName);
		}	break;
		case EMonsterStoneSlateNodeState::Normal:
		{
			bool isEnable = (costElement) ? costElement->PlayerHasEnough() : false;
			_btnOpenNode->SetIsEnabled(isEnable);
			
			FText textBtnName;
			FText::FindText(TEXT("CodexUIText"), TEXT("UI_Slate_Button_OpenSpecialNode"), textBtnName);
			_textBtnTitle->SetText(textBtnName);
		
		}	break;
		case EMonsterStoneSlateNodeState::Complete:
		{
			_btnOpenNode->SetIsEnabled(false);
			
			FText textBtnName;
			FText::FindText(TEXT("CodexUIText"), TEXT("UI_Slate_Button_Finished"), textBtnName);
			_textBtnTitle->SetText(textBtnName);
			break;
		}
	}	
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::SetDetailData(const FGsSlateSpecialNode* InData)
{
	_focusNodeData = InData;

	if (nullptr == _focusNodeData)
	{
		return;
	}

	_specialNodeState->SetData(_focusNodeData);

	FText textStatName;
	FGsStatHelper::GetDisPlayStatName(_focusNodeData->StatSet().Key, textStatName);

	FText textStatValue;
	FGsStatHelper::GetDisPlayStatValueWithMarkFormat(_focusNodeData->StatSet().Key, _focusNodeData->StatSet().Value, textStatValue, true);

	// 2023/11/8 PKT - Stat Data
	_textSpecialNodeStatName->SetText(textStatName);
	_textSpecialNodeStatValue->SetText(textStatValue);

	// 2023/11/9 PKT - "만분율 값을 퍼센트로 변환하므로 value / 10000 * 100 = value * 0.01" .. 렇다고 한다.
	float point = float(_focusNodeData->GetProbability()) * 0.01f;
	FText percent = FText::FromString(FString::Format(TEXT("{0}%"), { FMath::FloorToInt(point)}));
	_textProbability->SetText(percent);

	Invalidate();
}

void UGsUIMonsterCollectionSlateSpecialNodeDetailPanel::PlaySpecialNodeResult(const IGsMessageParam* InParam)
{
	const FGsMessageSpecialNodeResult* param = StaticCast<const FGsMessageSpecialNodeResult*>(InParam);
	if (param == nullptr)
		return;

	if (_specialNodeState == nullptr)
		return;

	_isNodeOpenSuccess = param->_result;

	if (_isNodeOpenSuccess)
		_specialNodeState->PlayNodeSuccess();
	else
		_specialNodeState->PlayNodeFail();
}
