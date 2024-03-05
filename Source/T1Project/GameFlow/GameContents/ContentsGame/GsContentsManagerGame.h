// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../Classes/GsState.h"
#include "../../../Classes/GsStateMng.h"
#include "../GsContentsMode.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Net/GSNet.h"

//------------------------------------------------------------------------------
// 게임모드를 관리할때 필요한 인스턴스 할당관리자(동일 인스턴스가 아닌 상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class FGsContentsGameAllocator : TGsSharedMapAllocator<FGsContentsMode::InGame, FGsContentsGameBase>
{
public:
	FGsContentsGameAllocator() 
	{
	}

	virtual ~FGsContentsGameAllocator() 
	{
	}

public:
	virtual FGsContentsGameBase* Alloc(FGsContentsMode::InGame inMode) override;
};

namespace PD
{
	namespace GC
	{
		enum class GCPacketId : uint16;
	}
	namespace SC
	{
		enum class SCPacketId : uint16;
	}
}


//------------------------------------------------------------------------------
// 게임모드를 관리
//------------------------------------------------------------------------------
class FGsContentsManagerGame :
	public TGsStateMng<FGsContentsGameBase, FGsContentsGameAllocator>
{
	using Super = TGsStateMng<FGsContentsGameBase, FGsContentsGameAllocator>;
	
private:
	TArray<TPair<PD::GC::GCPacketId, FDelegateHandle>>				_listGcpacketDelegate;
	TArray<TPair<PD::SC::SCPacketId, FDelegateHandle>>				_listScPacketDelegate;
	TArray<TPair<MessageContents, FDelegateHandle>>					_listContentsDelegate;	
	MsgUIHandleArray												_listUIDelegate;

public:
	virtual ~FGsContentsManagerGame(){};

public:
	virtual void RemoveAll() override;
	virtual void InitState() override;

public:
	void Update(float InDeltaTime);
	void OnReconnectionEnd();

private:
	void InitMsgHandle();

public:
	// ChangeState 후, 해당 StatObject에 접근할일이 생길경우 사용
	template<typename T = FGsContentsGameBase>
	T* GetContentsStateObject(FGsContentsMode::InGame InStateType)
	{
		for (const auto &iter : GetContainer())
		{
			if (InStateType == iter.Key)
			{
				FGsContentsGameBase* base = iter.Value.Get();
				return static_cast<T*>(base);
			}			
		}

		return nullptr;
	}
	

public:
	/*void KickedByReason(FGsNet::Packet &inPacket);*/


protected:
	void OnStateHud();
	void OnStateSkill();
	void OnStateOption();
	void OnStateQuest();
	void OnStateQuestBoard();
	void OnStateContinentalMap();
	void OnStateRegionMap();
	void OnStateFairy();
	void OnStateNpcShop();
	void OnStateGuild();
	void OnStateCostume();
	void OnStateMail();
	void OnStateItemCollection();
	void OnStateDungeon();
	void OnStateNpcDepot();
	void OnStateMarket();
	void OnStateCraft();
	void OnMultiLevelRank();
	void OnOfflinePlay();
	void OnStateBMShop();
	void OnCommunity();
	void OnStateRanking();
	void OnStateAchievement();
	void OnStateMonsterCollection();
	void OnStateTitle();
	void OnStateGameStatistics();
	void OnStateUserProfile();
	void OnStateBoss();
	void OnStatePKBook();
	void OnStateRepair();
	void OnStateRecompose();
	void OnStateKeyBoardMapping();
	void OnStateInvade();
	void OnStateSeal();
	void OnStateBattlePass();
	void OnStateSiegeWarfare();
	void OnStateExchangeCostume();
	void OnStateSpiritShot();
	void OnStateAgitDungeon();
	void OnStateStarLegacy();

	void OnRecoverToDefaultState(const IGsMessageParam*);

	// get
public:
	FGsContentsMode::InGame GetCurrentContentsMode() const;
};
