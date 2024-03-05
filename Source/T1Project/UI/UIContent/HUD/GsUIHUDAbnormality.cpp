// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDAbnormality.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconAbnormality.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"


void UGsUIHUDAbnormality::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnBuff)
	{
		_btnBuff->OnClicked.AddDynamic(this, &UGsUIHUDAbnormality::OnClickBuff);
	}
	
	_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	if (_slotHelper)
	{
		_slotHelper->Initialize(_iconSelector);
		_slotHelper->OnCreateIcon.AddDynamic(this, &UGsUIHUDAbnormality::OnCreateIcon);
		_slotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIHUDAbnormality::OnRefreshIcon);
	}	
}

void UGsUIHUDAbnormality::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterMessages();
}

void UGsUIHUDAbnormality::NativeDestruct()
{
	DeregisterMessages();

	_slotHelper->Clear();
	_iconSelector->RemoveAllChildren();

	_passivityDebuffList.Empty();
	_abnormalityDebuffList.Empty();

	Super::NativeDestruct();
}

void UGsUIHUDAbnormality::InvalidateAllInternal()
{
	UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localPlayer)
	{
		FGsAbnormalityHandlerBase* abnormalityHandler = localPlayer->GetAbnormalityHandler();
		if (abnormalityHandler)
		{
			SetUIBuffCount(abnormalityHandler->GetBuffCount());
			InvalidateDebuffList(abnormalityHandler, true);
		}
	}
}

void UGsUIHUDAbnormality::RegisterMessages()
{
	MGameObject& msgGameObject = GMessage()->GetGameObject();

	_msgHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_ABNORMALITY_ADD,
		this, &UGsUIHUDAbnormality::OnAddAbnormality));
	_msgHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::LOCAL_ABNORMALITY_REMOVE,
		this, &UGsUIHUDAbnormality::OnRemoveAbnormality));
	_msgHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::PASSIVITY_ADD,
		this, &UGsUIHUDAbnormality::OnAddPassivity));
	_msgHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::PASSIVITY_REMOVE,
		this, &UGsUIHUDAbnormality::OnRemovePassivity));
}

void UGsUIHUDAbnormality::DeregisterMessages()
{
	MGameObject& msgGameObject = GMessage()->GetGameObject();

	for (MsgGameObjHandle& handle : _msgHandleList)
	{
		msgGameObject.Remove(handle);
	}
	_msgHandleList.Empty();
}

void UGsUIHUDAbnormality::OnAddAbnormality(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>();
	if (nullptr == param)
	{
		return;
	}

	AbnormalityInstanceId instanceId = static_cast<AbnormalityInstanceId>(param->_first);

	if (UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (FGsAbnormalityHandlerBase* abnormalityHandler = localPlayer->GetAbnormalityHandler())
		{
			TWeakPtr<FGsAbnormalityData> abnormalityPtr = abnormalityHandler->GetAbnormalityData(instanceId);
			if (abnormalityPtr.IsValid())
			{
				if (abnormalityPtr.Pin()->GetTable() &&
					abnormalityPtr.Pin()->GetTable()->isBuff)
				{
					SetUIBuffCount(abnormalityHandler->GetBuffCount());
				}
				else
				{
					InvalidateDebuffList(abnormalityHandler, true);
				}
			}
		}
	}
}

void UGsUIHUDAbnormality::OnRemoveAbnormality(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>();
	if (nullptr == param)
	{
		return;
	}

	AbnormalityInstanceId instanceId = static_cast<AbnormalityInstanceId>(param->_first);

	if (UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (FGsAbnormalityHandlerBase* abnormalityHandler = localPlayer->GetAbnormalityHandler())
		{
			TWeakPtr<FGsAbnormalityData> abnormalityPtr = abnormalityHandler->GetAbnormalityData(instanceId);
			if (abnormalityPtr.IsValid())
			{
				if (abnormalityPtr.Pin()->GetTable() &&
					abnormalityPtr.Pin()->GetTable()->isBuff)
				{
					// 주의: 타이밍 상 데이터가 지워지기 전에 메시지가 오기 때문에 -1을 해준다		
					int32 buffCount = abnormalityHandler->GetBuffCount() - 1;
					if (0 > buffCount)
					{
						buffCount = 0;
					}

					SetUIBuffCount(buffCount);
				}
				else
				{
					// 주의: 타이밍 상 데이터가 지워지기 전에 메시지가 오기 때문에 그대로 지우려 했으나,
					// 쓰레기값 때문에 크래시 이슈가 있는듯 하여 다시 가져온 후 대상 제거
					abnormalityHandler->GetAbnormalityDebuffList(_abnormalityDebuffList, true);

					for (FGsAbnormalityData* iter : _abnormalityDebuffList)
					{
						if (instanceId == iter->GetInstanceId())
						{
							_abnormalityDebuffList.Remove(iter);
							break;
						}
					}

					InvalidateDebuffList(abnormalityHandler, false);
				}
			}
		}
	}
}

void UGsUIHUDAbnormality::OnAddPassivity(const IGsMessageParam* InParam)
{
	const FGsGameObjectMessageParamPassivity* param = InParam->Cast<const FGsGameObjectMessageParamPassivity>();
	if (param && param->_paramOwner)
	{
		// 로컬 플레이어 아니면 무시
		if (EGsGameObjectType::LocalPlayer == param->_paramOwner->GetObjectType())
		{
			if (FGsAbnormalityHandlerBase* abnormalityHandler = param->_paramOwner->GetAbnormalityHandler())
			{
				if (param->_bIsBuff)
				{
					SetUIBuffCount(abnormalityHandler->GetBuffCount());
				}
				else
				{
					InvalidateDebuffList(abnormalityHandler, true);
				}
			}
		}
	}
}

void UGsUIHUDAbnormality::OnRemovePassivity(const IGsMessageParam* InParam)
{
	const FGsGameObjectMessageParamPassivity* param = InParam->Cast<const FGsGameObjectMessageParamPassivity>();
	if (param && param->_paramOwner)
	{
		// 로컬 플레이어 아니면 무시
		if (EGsGameObjectType::LocalPlayer == param->_paramOwner->GetObjectType())
		{
			if (FGsAbnormalityHandlerBase* abnormalityHandler = param->_paramOwner->GetAbnormalityHandler())
			{
				if (param->_bIsBuff)
				{
					SetUIBuffCount(abnormalityHandler->GetBuffCount());
				}
				else
				{
					InvalidateDebuffList(abnormalityHandler, true);
				}
			}
		}
	}
}

void UGsUIHUDAbnormality::OnClickBuff()
{
	OpenPopupAbnormalityInfo();

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDAbnormality::OnCreateIcon(UGsUIIconBase* InIcon)
{
	if (_isSaveBatteryMode)
		return;

	UGsUIIconAbnormality* icon = Cast<UGsUIIconAbnormality>(InIcon);
	if (icon)
	{
		// 클릭 이벤트 연결
		icon->OnClickIcon.AddUObject(this, &UGsUIHUDAbnormality::OnClickIcon);
	}
}

void UGsUIHUDAbnormality::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (_isSaveBatteryMode)
		return;

	UGsUIIconAbnormality* icon = Cast<UGsUIIconAbnormality>(InIcon);
	if (nullptr == icon)
	{
		return;
	}

	// 패시브 부터 찍고, 그 다음에 버프 찍음
	int32 passiveCount = _passivityDebuffList.Num();
	if (InIndex < passiveCount)
	{
		icon->SetData(_passivityDebuffList[InIndex]);
	}
	else
	{
		int32 abnormalityIndex = InIndex - passiveCount;

		if (_abnormalityDebuffList.IsValidIndex(abnormalityIndex))
		{
			icon->SetData(_abnormalityDebuffList[abnormalityIndex]);
		}
	}
}

void UGsUIHUDAbnormality::OnClickIcon(UGsUIIconAbnormality* InIcon)
{
	OpenPopupAbnormalityInfo();
}

void UGsUIHUDAbnormality::SetUIBuffCount(int32 InCount)
{
	_textBlockBuffCount->SetText(FText::AsNumber(InCount));
}

void UGsUIHUDAbnormality::InvalidateDebuffList(FGsAbnormalityHandlerBase* InHandler, bool bRefreshAbnormality)
{
	if (InHandler)
	{
		InHandler->GetPassivityDebuffList(_passivityDebuffList, true);

		if (bRefreshAbnormality)
		{
			InHandler->GetAbnormalityDebuffList(_abnormalityDebuffList, true);
		}
	}

	int32 count = _passivityDebuffList.Num() + _abnormalityDebuffList.Num();
	if (count > _maxShowDebuffCount)
	{
		count = _maxShowDebuffCount;
	}

	_slotHelper->RefreshAll(count);
}

void UGsUIHUDAbnormality::OpenPopupAbnormalityInfo()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		// 효과 정보창 열기
		uiMgr->Open(TEXT("PopupAbnormalityInfo"));
	}
}

void UGsUIHUDAbnormality::SetSaveBatteryMode()
{
	_isSaveBatteryMode = true;
	_iconSelector->SetVisibility(ESlateVisibility::Hidden);

	if (_btnBuff && _btnBuff->OnClicked.IsBound())
	{
		_btnBuff->OnClicked.RemoveDynamic(this, &UGsUIHUDAbnormality::OnClickBuff);
	}
}
