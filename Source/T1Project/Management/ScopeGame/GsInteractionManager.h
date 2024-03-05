#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/SharedPrimitiveTypes.h"
#include "DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"

class UGsGameObjectLocalPlayer;
class UGsGameObjectBase;
class FGsInteractionNpcInfo;
class FGsTownInteractionNpcInfo;
class UGsWidgetComponentButton;
class FGsItem;

struct IGsMessageParam;
struct FGsSchemaNpcFunctionData;

//---------------------------------
// 인터랙션 관리
//---------------------------------
class FGsInteractionManager :
	public IGsManager, public IGsEventProgressInterface
{
	// 멤버 변수
private:
	// 메시지 (해지용)
	MsgGameObjHandleArray _interactionDelegate;	
	// 로컬 플레이어
	UGsGameObjectLocalPlayer* _localPlayer = nullptr;
	// 인터랙션 범위 그릴지
	bool _isDrawInteractionRange = false;

	// key: slot index(FGsSchemaNpcFunctionData _slotOrderNumber)
	// value: same NpcFunctionType npcs
	TMap<int, TArray<FGsTownInteractionNpcInfo*>> _mapTownInteractionNpcInfo;

	// 인터랙션 npc 정보(클라이언트 스폰한넘만)
	TArray<FGsInteractionNpcInfo*> _arrayClientSpawnInteractionNpcInfo;

	// server info(event shop npc)
	// key: slot index(FGsSchemaNpcFunctionData _slotOrderNumber)
	// value: same NpcFunctionType npcs
	TMap<int, TArray<FGsTownInteractionNpcInfo*>> _mapServerTownInteractionNpcInfo;
	// interaction npc info(server spawn)
	TArray<FGsInteractionNpcInfo*> _arrayServerSpawnInteractionNpcInfo;

	// 이벤트 우선순위 진행  예약 데이터
	int _reserveNpcTblId = 0;
	uint32 _reserveSpawnId = 0;
	int64 _reserveGameId = 0;
	NpcFunctionType _reserveNpcFunctionType = NpcFunctionType::NONE;
	// 위젯 컴퍼넌트 버튼들
	// 터치시 순서에 따른 처리
	// 1. 인터랙션 버튼 체크
	// 2. 인터랙션 대상 몸통 
	// 3. 바닥 터치
	TArray<UGsWidgetComponentButton*> _arrayWCButton;
	// 임시 버튼 그릴 위치 
	FVector2D _testTouchScreenPos;

	FVector2D _testWidgetShiftPos;

	FVector2D _testWidgetSize;

	bool _testShowInteractionButtonPos = false;

	ItemCategorySub _currentConsumeItemCategory = ItemCategorySub::NONE;

	bool _guildDungeonWarpReq = false;

	// 생성자, 소멸자
public:
	virtual ~FGsInteractionManager();

	// 가상 함수
public:
	// 초기화(처음 한번만)
	virtual void Initialize() override;
	// 해제
	virtual void Finalize() override;

	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

	// 로직함수
private:
	// 메시지 바인딩 처리
	void BindMessage();
	// npc list info 클리어
	void ClearNpcListInfo();

public:
	// 캐릭터 삭제될때
	void RemoveCharacter();
	// 인터랙션 시도(타겟으로)
	void TryInteractionGameObject(UGsGameObjectBase* In_target);
	// 인터랙션 시도(tbl id와 위치로)
	void TryInteraction(EGsGameObjectType In_type, int In_tblId, int In_mapId, FVector In_targetPos, float In_addAcceptanceRadius = 0.0f);
	void RecvTryInteractAck(PD::SC::PKT_SC_ACK_TRY_INTERACT_READ& In_packet,
		UGsGameObjectBase* In_findObj, int64 In_gameId);
	void RecvNpcEventShopOpenAck(PD::SC::PKT_SC_ACK_NPC_EVENT_SHOP_OPEN_READ& In_packet);

	void RecvEnterTerritoryEventShopData(PD::SC::PKT_SC_NOTIFY_ENTER_TERRITORY_EVENT_SHOP_DATA_READ& In_packet);
	void RecvLeaveTerritoryEventShopData(PD::SC::PKT_SC_NOTIFY_LEAVE_TERRITORY_EVENT_SHOP_DATA_READ& In_packet);

	// 위젯 버튼 추가 
	void AddWidgetComponentButton(UGsWidgetComponentButton* In_wcButton);
	// 위젯 버튼 삭제
	void DelWidgetComponentButton(UGsWidgetComponentButton* In_wcButton);
	// 인터랙션 버튼 클릭 체크
	bool CheckInteractionButtonClick();

	static void InteractionErrorMessage(int32 inError);

	void RemoveEventShopServerTownInteractionInfo(int In_npcTblId);

	void TryConsumeItem(const FGsItem* InItem);

	void ClearCurrentConsumeCategory();

	// 메시지 바인딩
public:
	// 인터랙션 버튼 클릭
	void OnInteractionButtonClick(const IGsMessageParam* In_param);
	// 인터랙션 디버그 범위 그리기
	void OnInteractionDebugDrawRange(const IGsMessageParam* In_param);
	// 클라 스폰 리스트 로드 끝
	void OnClientSpawnListLoadFinish(const IGsMessageParam* In_param);
	// 인터랜션이 발생 했을 때
	void OnInteractionNpcContents(const IGsMessageParam* In_data);
	// 프랍 오브젝트 상태 갱신
	void OnPropTargetUpdate(const IGsMessageParam* In_data);
	// 인터랙션 전, 침공프랍 register 처리
	void OnInteractionInvasionRegister(const IGsMessageParam* In_param);
	//https://jira.com2us.com/jira/browse/C2URWQ-3121
	// quest progress fail work
	void OnInteractionContentsByQuestCancle(const IGsMessageParam* In_param);

	// get, set
public:
	// 캐릭터 세팅
	void SetCharacter(UGsGameObjectLocalPlayer* In_char);
	// 인터랙션 범위 그릴지
	bool GetIsDrawInteractionRange() { return _isDrawInteractionRange; }
	FVector GetNpcPos(int In_npcId);
	// event shop
	FVector GetServerNpcPos(int In_npcId);
	TArray<FGsTownInteractionNpcInfo*> GetTownNpcDataBySlotId(int In_slotId);
	TArray<FGsTownInteractionNpcInfo*> GetServerTownNpcDataBySlotId(int In_slotId);

	void SendInteractionCancel();

	
	void SetTestTouchScreenPos(FVector2D In_pos)
	{
		_testTouchScreenPos = In_pos;
	}

	FVector2D GetTestTouchScreenPos() { return _testTouchScreenPos; }

	void SetTestWidgetShiftPos(FVector2D In_pos)
	{
		_testWidgetShiftPos = In_pos;
	}

	FVector2D GetTestWidgetShiftPos() { return _testWidgetShiftPos; }

	void SetTestWidgetSize(FVector2D In_pos)
	{
		_testWidgetSize = In_pos;
	}

	FVector2D GetTestWidgetSize() { return _testWidgetSize; }


	const FGsSchemaNpcFunctionData* FindNpcFunctionDataBySlotId(int In_slotId);
	int32 GetReserveNpcId(){return _reserveNpcTblId;}
	NpcFunctionType GetReserveNpcFunctionType(){return _reserveNpcFunctionType;}

	bool GetTestShowInteractionButtonPos() { return _testShowInteractionButtonPos; }
	void SetTestShowInteractionButtonPos(bool In_val) {  _testShowInteractionButtonPos = In_val; }

	void SetGuildDungeonWarpReq(bool In_val)
	{
		_guildDungeonWarpReq = In_val;
	}
	// 슬롯 넘버 구하기(그림자 전장 대응)
	// https://jira.com2us.com/jira/browse/CHR-24545
	int GetSlotOrderNumber(const FGsSchemaNpcFunctionData* In_findData);

	// 그림자 전장에서 적군인가(나 iff, 타겟 npc iff)
	bool IsEnemyIffShadowBattleField(IffFactionType In_localIff, IffFactionType In_npcIff);
};

#define GSInteraction() UGsScopeHolder::GetGameManagerFType<FGsInteractionManager>(UGsScopeGame::EManagerType::Interaction)