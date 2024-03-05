#pragma once

#include "GsStateLocalPlayerBase.h"

/*
 local 인터랙션 상태
*/
class UGsGameObjectBase;
class FGsInteractionHandlerLocalPlayer;

struct FGsReserveInteractionInfo;
struct FGsSchemaNpcData;
struct FGsSchemaPropData;


class FGsStateLocalPlayerInteraction : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerInteraction>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	static bool IsInsideInteractionRangeByTableId(UGsGameObjectBase* In_owner, int In_targetNpcTblId);
	// 인터랙션 범위 안인가
	static bool IsInsideInteractionRange(UGsGameObjectBase* In_owner, UGsGameObjectBase* In_target,
		const FGsReserveInteractionInfo& ReserveData);

public:
	virtual bool Ready( UGsGameObjectBase* Owner) override;
	virtual void ReEnter( UGsGameObjectBase* Owner) override;
	virtual void Update( UGsGameObjectBase* Owner, float Delta) override;

private:
	// Npc 인터렉션 처리
	void PlayInteraction_Npc( UGsGameObjectBase* _owner, const  FGsSchemaNpcData* _npcData,  FGsInteractionHandlerLocalPlayer* _handler);
	//Prop 인터렉션 처리
	void PlayInteraction_Prop( UGsGameObjectBase* _owner, const  FGsSchemaPropData* _propData,  FGsInteractionHandlerLocalPlayer* _handler);
	// Camp 인터렉션 처리
	void PlayInteraction_Camp( UGsGameObjectBase* _owner, const  FGsSchemaNpcData* _npcData,  FGsInteractionHandlerLocalPlayer* _handler);
	// Sanctum 인터렉션 처리
	void PlayInteraction_Sanctum(UGsGameObjectBase* _owner, const  FGsSchemaNpcData* _npcData, FGsInteractionHandlerLocalPlayer* _handler);
	// 침공 포털 active/deactive에 따른 인터렉션 처리
	void PlayInteraction_PropInvasion(UGsGameObjectBase* _owner, const  FGsSchemaPropData* _propData, FGsInteractionHandlerLocalPlayer* _handler);

private:
	void SendCancel(UGsGameObjectBase* _owner) const;


	// 인터랙션 위치 이동 범위 안인가
	bool IsInsideInteractionMovePosRange( UGsGameObjectBase* In_owner,
		int In_mapId,
		FVector In_targetPos, float In_addAcceptanceRadius =0.0f);

};