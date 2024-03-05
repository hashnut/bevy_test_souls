// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAbnormalityHandlerLocalPlayer.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "GameObject/Abnormality/GsAbnormalityDataDefine.h"
#include "GameObject/Abnormality/GsPassivityHelper.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/HUD/GsScreenFilterLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateInput.h"
#include "GameObject/State/GsGameObjectStateManager.h"

#include "CrowdControl/GsCrowdControlLocalPlayer.h"

#include "T1Project.h"

void FGsAbnormalityHandlerLocalPlayer::Initialize(UGsGameObjectBase* Owner)
{
	Super::Initialize(Owner);

	// 콜렉션 패시비티 정보는 서버가 값을 주지 않는다. 체크해서 _mapPassivity를 채운다.
	CheckCollection();

	// 메세지 바인딩
	//FGsMessageHolder* msg = GMessage();

	//_listGameObjectMsgHandler.Emplace(
	//	msg->GetGameObject().AddRaw(MessageGameObject::UPDATE_USER_PARTS,
	//		this, &FGsAbnormalityHandlerLocalPlayer::CallbackChnageParts));
}

void FGsAbnormalityHandlerLocalPlayer::Finalize()
{
	FGsMessageHolder* msg = GMessage();
	for (MsgGameObjHandle& el : _listGameObjectMsgHandler)
	{
		msg->GetGameObject().Remove(el);
	}
	_listGameObjectMsgHandler.Empty();

	_passivityDebuffList.Empty();
	_mapPassivity.Empty();

	Super::Finalize();
}

FGsCrowdControlBase* FGsAbnormalityHandlerLocalPlayer::CreateCrowdControl()
{
	return new FGsCrowdControlLocalPlayer();
}

void FGsAbnormalityHandlerLocalPlayer::Set(PD::SC::PKT_SC_PASSIVITY_ADDED_READ* InPacket)
{
	DefPassiveKey key = DefPassiveKey(InPacket->PassivityId(), InPacket->OwnerType(), InPacket->OwnerId());

	if (FGsPassivityData* data = _mapPassivity.Find(key))
	{
		// 이런 상황은 없어보이지만, 관련 갱신이 필요할 경우 메시지 추가하여 처리할 것.
		return;
	}

	FGsPassivityData newData;
	newData._passivityId = InPacket->PassivityId();
	newData._ownerType = InPacket->OwnerType();
	newData._ownerId = InPacket->OwnerId();
	newData._data = UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(InPacket->PassivityId());
	if (nullptr == newData._data)
	{
		GSLOG(Error, TEXT("not found PassivitySet Data %d"), InPacket->PassivityId());
		return;
	}

	FGsPassivityData* addedData = &_mapPassivity.Emplace(key, newData);
	if (false == addedData->_data->isBuff)
	{
		_passivityDebuffList.AddUnique(key);
	}

	// 메시지 보내기
	FGsGameObjectMessageParamPassivity param(_owner, InPacket->PassivityId(), InPacket->OwnerType(), 
		InPacket->OwnerId(), addedData->_data->isBuff);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_ADD, &param);
}

void FGsAbnormalityHandlerLocalPlayer::Set(PD::SC::PKT_SC_PASSIVITY_REMOVED_READ* InPacket)
{
	DefPassiveKey key = DefPassiveKey(InPacket->PassivityId(), InPacket->OwnerType(), InPacket->OwnerId());
	if (FGsPassivityData* data = _mapPassivity.Find(key))
	{
		bool bIsBuff = false;
		if (data->_data)
		{
			bIsBuff = data->_data->isBuff;
			if (false == bIsBuff)
			{
				_passivityDebuffList.Remove(key);
			}
		}

		_mapPassivity.Remove(key);

		// 메시지 보내기
		FGsGameObjectMessageParamPassivity param(_owner, InPacket->PassivityId(), InPacket->OwnerType(),
			InPacket->OwnerId(), bIsBuff);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_REMOVE, &param);
	}
}

int32 FGsAbnormalityHandlerLocalPlayer::GetBuffCount() const
{
	int32 abnormalityCount = Super::GetBuffCount();

	int passiveCount = 0;
	int debuffCount = 0;
	for (const TPair<DefPassiveKey, FGsPassivityData>& iter : _mapPassivity)
	{
		if (iter.Value._data &&
			false == iter.Value._data->hideUI)
		{
			++passiveCount;

			if (false == iter.Value._data->isBuff)
			{
				++debuffCount;
			}
		}
	}

	// 버프갯수 = 전체에서 디버프 갯수를 뺀 값. hideUI 처리된 항목은 무시
	return  abnormalityCount + passiveCount - debuffCount;
}

bool FGsAbnormalityHandlerLocalPlayer::GetPassiveDataList(OUT TArray<FGsPassivityData*>& OutList)
{
	OutList.Empty();
	for (TPair<DefPassiveKey, FGsPassivityData>& iter : _mapPassivity)
	{
		FGsPassivityData* data = &iter.Value;
		if (data->_data &&
			false == data->_data->hideUI)
		{
			OutList.Emplace(data);
		}
	}

	return true;
}

bool FGsAbnormalityHandlerLocalPlayer::GetPassivityDebuffList(OUT TArray<FGsPassivityData*>& OutList, bool bUseSort)
{
	OutList.Empty();

	for (TPair<DefPassiveKey, FGsPassivityData>& iter : _mapPassivity)
	{
		FGsPassivityData* data = &iter.Value;

		if (data->_data &&
			false == data->_data->isBuff &&
			false == data->_data->hideUI)
		{
			OutList.Emplace(data);
		}
	}

	if (bUseSort)
	{
		// 환경 속성을 앞으로
		OutList.Sort([](FGsPassivityData& lhs, FGsPassivityData& rhs)
			{
				return (lhs.IsEnvironment()) ? true : false;
			});
	}
	
	return true;
}

void FGsAbnormalityHandlerLocalPlayer::OnUpdateCollectionPassivity(int32 InPassivityOwnerType)
{
	UpdateCollection(static_cast<PassivityOwnerType>(InPassivityOwnerType), true);
}

void FGsAbnormalityHandlerLocalPlayer::Add(const AbnormalityData& NetData, bool isRefresh)
{
	Super::Add(NetData, isRefresh);

	FGsLocalGameObjectMessageParamAbnormality param(_mapAbnormality);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_ABNORMALITY_UPDATE, &param);
}

void FGsAbnormalityHandlerLocalPlayer::Remove(AbnormalityInstanceId InstanceId)
{
	Super::Remove(InstanceId);

	FGsLocalGameObjectMessageParamAbnormality param(_mapAbnormality);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_ABNORMALITY_UPDATE, &param);
}

void FGsAbnormalityHandlerLocalPlayer::OnActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData, bool IsNewDataAdded, bool isSpawnEffect)
{
	Super::OnActive(InstanceId, inData, IsNewDataAdded, isSpawnEffect);

	if (auto Data = inData.Pin())
	{
		AbnormalityEffectType effectType = Data->GetEffectType();

		if (IsNewDataAdded)
		{
			UGsGameObjectLocalPlayer* local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
			const FGsSchemaAbnormalitySet* table = Data->GetTable();
			if (table && table->screenFilter.screenFilterUse)
			{
				FGsScreenFilterLocalPlayer* screenFilter = local->GetScreenFilter();
				screenFilter->ActiveScreenFilter(EGsScreenFilterType::Abnormality, table->screenFilter);
			}
			
			if (effectType == AbnormalityEffectType::CC_TAUNT)
			{
				if (FGsTargetHandlerBase* targetHandler = local->GetTargetHandler())
				{
					if (UGsGameObjectBase* target = GSGameObject()->FindObject(EGsGameObjectType::Creature, Data->GetCasterGuid()))
					{
						targetHandler->SetTarget(target, true, FGsTargetHandlerBase::SelectTargetFilter::Abnormality_Taunt);
					}
				}
			}
			else if (effectType == AbnormalityEffectType::UTIL_CONFUSE)
			{
				if (AGsCharacterLocalPlayer* characterLocal = local->GetLocalCharacter())
				{
					characterLocal->SetConfuseEffect(true);
				}
			}
		}

		// first : InstanceId, second : AbnormalityEffectType
		FGsPrimitivePairInt32 param(InstanceId, (uint32)effectType);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_ABNORMALITY_ADD, &param);
	}
}

void FGsAbnormalityHandlerLocalPlayer::OnDeActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData)
{
	if (auto Data = inData.Pin())
	{
		AbnormalityEffectType effectType = Data->GetEffectType();

		UGsGameObjectLocalPlayer* local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
		const FGsSchemaAbnormalitySet* table = Data->GetTable();
		if (table && table->screenFilter.screenFilterUse)
		{
			FGsScreenFilterLocalPlayer* screenFilter = local->GetScreenFilter();
			screenFilter->DeactiveScreenFilter(EGsScreenFilterType::Abnormality);
		}

		if (effectType == AbnormalityEffectType::UTIL_CONFUSE)
		{
			if (AGsCharacterLocalPlayer* characterLocal = local->GetLocalCharacter())
			{
				characterLocal->SetConfuseEffect(false);
			}
		}
		//if (effectType == AbnormalityEffectType::CC_FEAR)
		//{
		//	UGsGameObjectLocalPlayer* local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
		//	// GUI()->SetHitTestInvisible(false);
		//	local->SwitchSpectatorMode(false);
		//}

		FGsPrimitivePairInt32 param(InstanceId, (uint32)effectType);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_ABNORMALITY_REMOVE, &param);
	}

	Super::OnDeActive(InstanceId, inData);
}

void FGsAbnormalityHandlerLocalPlayer::UpdateAbnormalityEffectType(AbnormalityEffectType EffectType, bool IsAdd)
{
	Super::UpdateAbnormalityEffectType(EffectType, IsAdd);

	switch (EffectType)
	{
	case AbnormalityEffectType::UTIL_MOUNT_VEHICLE:
	{
		UGsGameObjectLocalPlayer* local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
		local->SetFairyVehicleCondition(EGsFairyVehicleCondition::VEHICLE_NOT_ABNORMALITY, IsAdd);
	}
		break;
	default:
		break;
	}
}

void FGsAbnormalityHandlerLocalPlayer::CheckCollection()
{
	// TODO: 콜렉션들 있으면 등록...
	UpdateCollection(PassivityOwnerType::FAIRY_COLLECTION, false);
	UpdateCollection(PassivityOwnerType::GUILD_BUILDING, false);
	UpdateCollection(PassivityOwnerType::ITEM_COLLECTION, false);
	UpdateCollection(PassivityOwnerType::COSTUME_COLLECTION, false);
	UpdateCollection(PassivityOwnerType::MULTI_LEVEL_RANK, false);
	UpdateCollection(PassivityOwnerType::GUILD_ITEM_COLLECTION, false);
	UpdateCollection(PassivityOwnerType::MONSTER_BOOK, false);
	UpdateCollection(PassivityOwnerType::TITLE_COLLECTION, false);
	UpdateCollection(PassivityOwnerType::TITLE_EQUIP, false);
	UpdateCollection(PassivityOwnerType::SPIRIT_SHOT_ENCHANT, false);
	UpdateCollection(PassivityOwnerType::SPIRIT_SHOT_ENCHANT_SUM_LEVEL, false);
	UpdateCollection(PassivityOwnerType::CODEX, false);
}

void FGsAbnormalityHandlerLocalPlayer::UpdateCollection(PassivityOwnerType InType, bool bInSendMessage)
{
	DefPassiveKey key = DefPassiveKey(0, InType, 0);	

	// 데이터 생성
	const IGsCollectionData* collectionData = FGsPassivityHelper::GetCollectionData(InType);
	if (collectionData &&
		0 < collectionData->GetPassivityCount())
	{
		// 이미 있으면 패스
		if (FGsPassivityData* data = _mapPassivity.Find(key))
		{
			return;
		}

		FGsPassivityData newData(0, InType, 0);

		// 콜렉션 저장
		newData._collectionData = collectionData;
		newData._collectionTable = FGsPassivityHelper::GetCollectionTableData(InType);

		// 콜렉션 화면 표시 위해 가장 첫 패시브 데이터를 가져와 테이블을 채움
		newData._data = UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(
			collectionData->GetPassivityId(0));
		
		_mapPassivity.Emplace(key, newData);

		if (bInSendMessage)
		{
			FGsGameObjectMessageParamPassivity param(_owner, 0, InType, 0, true);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_ADD, &param);
		}
	}
	else
	{
		// 삭제
		if (FGsPassivityData* data = _mapPassivity.Find(key))
		{
			_mapPassivity.Remove(key);

			if (bInSendMessage)
			{
				FGsGameObjectMessageParamPassivity param(_owner, 0, InType, 0, true);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_REMOVE, &param);
			}
		}
	}
}

void FGsAbnormalityHandlerLocalPlayer::CallbackChnageParts(const IGsMessageParam* Param)
{
	UpdateMaterial();
}

bool FGsAbnormalityHandlerLocalPlayer::IsPassivity(PassivityId InId) const
{
	for (const TPair<DefPassiveKey, FGsPassivityData>& iter : _mapPassivity)
	{
		if (InId == iter.Value._passivityId)
		{
			return true;
		}
	}

	return false;
}
