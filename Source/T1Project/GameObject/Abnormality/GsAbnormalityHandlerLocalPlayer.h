// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsAbnormalityHandlerPlayer.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Message/GsMessageGameObject.h"

/**
 * 
 */
const TArray<AbnormalityEffectType> CAN_ENTER_DUNGEON_ABNORMALITY_ARRAY = {
	AbnormalityEffectType::CC_STUN,
	AbnormalityEffectType::CC_FEAR,
	AbnormalityEffectType::CC_KNOCK_BACK,
	AbnormalityEffectType::CC_POLYMORP
	};

class T1PROJECT_API FGsAbnormalityHandlerLocalPlayer final : 
	public FGsAbnormalityHandlerPlayer
{
	using Super = FGsAbnormalityHandlerPlayer;

public:
	using DefPassiveKey = TTuple<PassivityId, PassivityOwnerType, PassivityOwnerId>;

protected:
	TMap<DefPassiveKey, struct FGsPassivityData> _mapPassivity;
	TArray<DefPassiveKey> _passivityDebuffList;
	TArray<MsgGameObjHandle> _listGameObjectMsgHandler;

public:
	FGsAbnormalityHandlerLocalPlayer() = default;
	virtual ~FGsAbnormalityHandlerLocalPlayer() = default;

public:
	virtual void Initialize(class UGsGameObjectBase* Owner) override;
	virtual void Finalize() override;

protected:
	virtual FGsCrowdControlBase* CreateCrowdControl() override;

public:
	virtual void Add(const AbnormalityData & NetData, bool isRefresh = true) override;
	virtual void Remove(AbnormalityInstanceId InstanceId) override;

public:
	// Passivity
	virtual void Set(PD::SC::PKT_SC_PASSIVITY_ADDED_READ * InPacket) override;
	virtual void Set(PD::SC::PKT_SC_PASSIVITY_REMOVED_READ* InPacket) override;
	
	virtual int32 GetBuffCount() const override;
	virtual bool GetPassiveDataList(OUT TArray<struct FGsPassivityData*>& OutList) override;
	virtual bool GetPassivityDebuffList(OUT TArray<struct FGsPassivityData*>& OutList, bool bUseSort) override;
	virtual void OnUpdateCollectionPassivity(int32 InPassivityOwnerType) override;	

protected:
	virtual void OnActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData, bool IsNewDataAdded, bool isSpawnEffect = true) override;
	virtual void OnDeActive(AbnormalityInstanceId InstanceId, TWeakPtr<FGsAbnormalityData> inData) override;

	virtual void UpdateAbnormalityEffectType(AbnormalityEffectType EffectType, bool IsAdd) override;

private:
	void CallbackChnageParts(const struct IGsMessageParam* Param);

protected:
	void CheckCollection();
	void UpdateCollection(PassivityOwnerType InType, bool bInSendMessage);

public:
	virtual bool IsTeamMate() override { return true; }

public:
	bool IsPassivity(PassivityId InId) const;
};
