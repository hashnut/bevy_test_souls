// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAbnormalityHandlerPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"


void FGsAbnormalityHandlerPlayer::Initialize(UGsGameObjectBase* Owner)
{
	Super::Initialize(Owner);

	// 메세지 바인딩
	FGsMessageHolder* msg = GMessage();

	//_listGameObjectMsgHandler.Emplace(
	//	msg->GetGameObject().AddRaw(MessageGameObject::UPDATE_USER_PARTS,
	//		this, &FGsAbnormalityHandlerPlayer::CallbackChnageParts));
}

void FGsAbnormalityHandlerPlayer::Finalize()
{
	FGsMessageHolder* msg = GMessage();
	for (MsgGameObjHandle& el : _listGameObjectMsgHandler)
	{
		msg->GetGameObject().Remove(el);
	}
	_listGameObjectMsgHandler.Empty();

	Super::Finalize();
}

void FGsAbnormalityHandlerPlayer::OnActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData, 
	bool IsNewDataAdded, bool isSpawnEffect)
{
	Super::OnActive(InstanceId, inData, IsNewDataAdded, isSpawnEffect);

	// 자기 캐릭터가 아닐 때만 전송
	if (IsNewDataAdded && _owner && EGsGameObjectType::LocalPlayer != _owner->GetObjectType())
	{
		FGsGameObjectMessageParamAbnormality param(_owner, InstanceId, false);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::ABNORMALITY_UPDATE_NOT_LOCAL, &param);
	}
}

void FGsAbnormalityHandlerPlayer::OnDeActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData)
{
	Super::OnDeActive(InstanceId, inData);

	// 자기 캐릭터가 아닐 때만 전송
	if (_owner &&
		EGsGameObjectType::LocalPlayer != _owner->GetObjectType())
	{
		FGsGameObjectMessageParamAbnormality param(_owner, InstanceId, true);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::ABNORMALITY_UPDATE_NOT_LOCAL, &param);
	}
}

bool FGsAbnormalityHandlerPlayer::IsTeamMate()
{
	if (_owner)
	{
		if (FGsGameObjectData* objData = _owner->GetData())
		{
			auto name = objData->GetName();
			if (GGuild()->IsGuild() && objData->GetGuildId() == GGuild()->GetGuildDBId())
				return true;

			return GSParty()->IsInPartyName(name);
		}

	}
	return false;
}

void FGsAbnormalityHandlerPlayer::CallbackChnageParts(const IGsMessageParam* Param)
{
	// 머터리얼 변경중이라면 변경된 머터리얼로 갱신
	// 아니라면 origin 정보 갱신
	UpdateMaterial();
}

