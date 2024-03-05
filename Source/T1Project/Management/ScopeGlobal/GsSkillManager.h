// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Templates/Function.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Classes/GsManager.h"
#include "../Animation/GsAnimInstanceState.h"
#include "../GsScopeHolder.h"
#include "Skill/GsSkill.h"
#include "Skill/GsComboSkillGroup.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageCostume.h"
#include "Message/GsMessageInvasion.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Skill/Checker/GsSkillCheckArray.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "RedDot/GsRedDotCheck.h"
#include "RedDot/GsRedDotCheckFunc.h"
#include "Skill/GsSkillUIDataHandler.h"

#include "GsSkillManager.generated.h"


namespace PD
{
	namespace SC
	{
		struct PKT_SC_SKILL_READ;
		struct PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ;
		struct PKT_SC_ACK_SKILL_SLOT_AUTO_READ;
		struct PKT_SC_ACK_SKILL_ENCHANT_READ;
		struct PKT_SC_SKILL_LEARN_READ;
		struct PKT_SC_ACK_SKILL_SLOT_CLEAR_READ;
		struct PKT_SC_UPDATE_ACTIVE_SKILL_READ;
		struct PKT_SC_SKILL_BONUS_LEVEL_READ;		

		struct PKT_SC_START_SKILL_READ;
		struct PKT_SC_SKILL_FINISHED_READ;
		struct PKT_SC_ACK_START_SKILL_FAILURE_READ;
		struct PKT_SC_SKILL_DAMAGED_READ;
		struct PKT_SC_ACK_CHANGE_COSTUME_READ;
		struct PKT_SC_ACK_USE_SKILL_INTERVAL_READ;
	}
}

struct IGsMessageParam;
struct FGsSchemaSkillCommon;
struct FGsSchemaSkillReaction;
struct FGsSchemaUserCommonActionInfo;
struct FGsSchemaUserReactionInfo;
struct FGsSchemaIdleEmotionActionData;
class UGsGameObjectBase;

/**
 * 나의 스킬 데이터 정보 관리 클래스
 */
UCLASS()
class T1PROJECT_API UGsSkillManager :
	public UObject,
	public IGsManager,
	public IGsRedDotCheck
{
	GENERATED_BODY()

protected:
	// 실제 Buffer에 Update 되기전 Temp Data
	struct FUpdateSkillData
	{
		FGsSkill _data;
		void ApplySkill(const FGsSkill* inSkill)
		{
			FGsSkill* applay = const_cast<FGsSkill*>(inSkill);
			applay->Set(_data);
		}
	};

	using DefSkillMap = TMap<int, TSharedPtr<const FGsSkill>>;
	using DefObjectKey = TPair<EGsGameObjectType, int>;		//Object Type | GameObjectTId
	using DefSkillSlotKey = TPair<SkillSlotId, const FGsSkill*>;
	using DefWeaponSkillCatergorySetKey = TPair<CreatureWeaponType, SkillCategorySet>;
	using DefNpcCommonMap = TMap<CommonActionType, TArray<const FGsSchemaSkillCommon*>>;
	using DefNpcReactionMap = TMap<EReactionType, TArray<const FGsSchemaSkillReaction*>>;
	using DefPlayerCommonMap = TMap<CommonActionType, const FGsSchemaUserCommonActionInfo*>;
	using DefPlayerReactionMap = TMap<EReactionType, const FGsSchemaUserReactionInfo*>;
	using DefSkillSlotMap = TMap<SkillSlotId, const FGsSkill*>; // SkillSlotId (즉발형: 1~5, 지속형: 6~13)		
	using DefCostumeMsg = TPair<MessageCostume, FDelegateHandle>;
	using DefPresetSkillSlotMap = TMap<PresetId, DefSkillSlotMap>;

	//----------------------------------------------------------------------------------
	// LocalPlayer용 데이터 정보
	// FGsSkillHandlerLocalPlayer 클래스의 역활과 경계가 모호한 부분들이 많으므로 추후 잘 정리해야할수있다..
protected:
	// 무기 타입별 스킬 정보
	TMap<CreatureWeaponType, DefSkillMap> _mapSkillData;

	// 현재 활성화된 무기 스킬 정보
	const DefSkillMap* _currentWeaponSkillSet;

	// 현재 무기 타입에서 배운 스킬 정보 캐싱
	DefSkillMap _currentWeaponActiveSkillSet;

	// WeaponType | SkillCatergorySet 멀티키 타입 구성으로 이루어진 배운 스킬 최정 정보 (안배운 스킬은 기본) 캐싱
	TMap<DefWeaponSkillCatergorySetKey, TArray<TSharedPtr<const FGsSkill>>> _mapSkillUiset;

	// 스킬 슬롯. skillSlotId는 대역으로 즉발형/지속형 구분(즉발형: 1~5, 지속형: 6~13)
	TMap<CreatureWeaponType, DefSkillSlotMap> _mapSkillSlotData;
	// 스킬 슬롯 MessageSend Update용 버퍼
	TArray<const FGsSkill*> _listSkillSlotUpdateBuffer;
	// 프리셋별 스킬 슬롯
	TMap<CreatureWeaponType, DefPresetSkillSlotMap> _mapPresetSkillSlotData;

	// 체인 스킬 정보
	TArray<FGsComboSkillGroup> _listChainSkillData;
	TFunction<void(FGsComboSkillGroup*)> _callbackChainSkillEnd;

	// 현재 선택된 무기 타입
	CreatureWeaponType _currentWeaponType = CreatureWeaponType::ALL;
	CreatureGenderType _currentGenderType = CreatureGenderType::MAX;

	// 스킬 유효성 체크 클래스
	// 사용이전 체크되야 하는 목록은 여기서 관리
	// 사용과 동시에 체크 되야 하는 것은  FGsSkillHandlerLocalPlayer 클래스에서 관리한다.
	FGsSkillCheckArray _skillCheck;
	//----------------------------------------------------------------------------------

protected:
	// NonPlayer 타입 CommonAction, Reaction 스킬 정보
	TMap<DefObjectKey, DefNpcCommonMap>		_mapCommonActionData;
	TMap<DefObjectKey, DefNpcReactionMap>	_mapReactionData;

	// Player 타입CommonAction, Reaction 스킬 정보
	TMap<CreatureGenderType, DefPlayerCommonMap>	_mapPlayerCommonActionData;
	TMap<CreatureGenderType, DefPlayerReactionMap>	_mapPlayerReactionData;

protected:
	// 불특정 메세지 타입이 바인딩 될수 있기때문에 
	// TGsMessageDelegateList클래스 상속을 받지 않는다.
	MsgGameObjHandleArray	_listGameObjectMsg;
	MsgStageHandleArray		_listStageMsg;
	MsgUIHandleArray		_listUiMsg;
	MsgHudHandleArray		_listReserveMsg;
	MsgInvasionHandleArray	_listInvasionMsg;
	TArray<DefCostumeMsg>	_listCostumeMsg;


	// 실제 Buffer에 Update 되기전 Temp Data
	FUpdateSkillData _updateSkillData;

	// 스킬 습득, 레드닷 체크 위한 보유 스킬북 저장
	TMap<SkillId, TArray<ItemId>> _inventorySkillBookMap;
	// 강화 레드닷 체크
	FGsRedDotCheckFunc _reinforceChecker;
	// 스킬 강제막기
	bool _bForcedDisableSkill = false;

	// hud skill target select show 
	bool _isShowSkillTargetSelect = false;
	int _currentSkillTargetSlotId = 0;

protected:
	TUniquePtr<FGsSkillUIDataHandler> _skillUIDataHandler; // 스킬 UI 관련 데이터 관리

	// 각 스킬을 획득할 수 있는 ItemId 목록을 저장해 둠.
	// 키:업그레이드스킬ID, 값:패시브스킬ID
	TMap<SkillId, TSet<SkillId>> _upgradeSkillIdAndPassiveSkillId;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	virtual void Update(float inTick) override;

	// 맵전환등 World이동 관련 대응
	void LoadTopoStart();

	void ChangeWeapon(CreatureWeaponType WeaponType);
	// IGsRedDotCheck override
	virtual bool CheckRedDot(bool bInUpdate = true) override;
	void SetIsNew(const FGsSkill* InSkill, bool bIsNew);

public:
	// Gender Type에 따라 초기 데이터 설정을 한다.
	void InitializeData(CreatureGenderType GenderType);
	void ClearData();

protected:
	void InitializeSkillSet(CreatureGenderType GenderType);
	void InitializeCommonAction(CreatureGenderType GenderType);
	void InitializeReaction(CreatureGenderType GenderType);

	void ClearDataSkillSet();
	void ClearDataCommonAction();
	void ClearDataReaction();

protected:
	// 캐싱 데이터 버퍼 설정 하기
	void MakeDataWeaponActiveSkillSet();
	void MakeDataSkillUiSet();

protected:
	// 실제 Data Buffer에 갱신
	void UpdateData(const FGsSkill* Data);
	// 스킬 슬롯 데이터 갱신
	void UpdateSkillSlotData(EGsSkillSlotCheckType InCheckType, SkillSlotId Index);

protected:
	void UpdateWeaponSkillSet(CreatureWeaponType WeaponType);
	void NextChainSkill(int32 SkillId);

	// CommonAction, Reaction Data Load
protected:
	// Npc, Player의 데이터 구조가 달라서 따로 처리가 필요하다.
	void LoadCommonActionDataNpc(OUT TArray<const FGsSchemaSkillCommon*>& OutCommonSet, int GameObjectTId);
	void LoadReactionDataNpc(OUT TArray<const FGsSchemaSkillReaction*>& OutReactionSet, int GameObjectTId);
	void LoadReactionDataPolymorphShape(OUT TArray<const FGsSchemaSkillReaction*>& OutReactionSet, int GameObjectTId);

	const FGsSchemaUserCommonActionInfo* LoadCommonActionDataPlayer(CreatureGenderType GenderType,
		CreatureWeaponType WeaponType, CommonActionType Type);
	const FGsSchemaUserReactionInfo* LoadReactionDataPlayer(CreatureGenderType GenderType, CreatureWeaponType WeaponType,
		EReactionType Type);

	void AddNpcCommonActionData(const DefObjectKey& key, const FGsSchemaSkillCommon* Data);
	void AddNpcReactionData(const DefObjectKey& key, const FGsSchemaSkillReaction* Data);

	void RemoveNpcCommonActionData(EGsGameObjectType Type, int GameObjectTId);
	void RemoveNpcReactionData(EGsGameObjectType Type, int GameObjectTId);

	// 레드닷 관련 함수
public:
	bool CheckReinforce();
	bool IsReinforce() const;

	// 서버 패킷 연동 함수 모음
public:
	void NetRecvUserSkillSet(PD::SC::PKT_SC_SKILL_READ* InPacket);
	void NetChangeWeaponFinishedAck(PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ* InPacket);
	void NetAckSkillSlotSet(SkillSlotInfo& InSlotInfo, CreatureWeaponType InWeaponType = CreatureWeaponType::MAX);
	void NetAckSkillSlotAuto(PD::SC::PKT_SC_ACK_SKILL_SLOT_AUTO_READ* InPacket);
	void NetAckSkillEnchant(PD::SC::PKT_SC_ACK_SKILL_ENCHANT_READ* InPacket);
	void NetRecvSkillLearn(PD::SC::PKT_SC_SKILL_LEARN_READ* InPacket);
	void NetAckSkillSlotClear(PD::SC::PKT_SC_ACK_SKILL_SLOT_CLEAR_READ* InPacket);
	void NetUpdateActiveSkill(PD::SC::PKT_SC_UPDATE_ACTIVE_SKILL_READ* InPacket);
	void NetSkillBonusLevelNotify(PD::SC::PKT_SC_SKILL_BONUS_LEVEL_READ* InPacket);
	void NetCostumeSkillLearn(SkillId InActiveSkillId);
	void NetCostumeSkillRemoved(PD::SC::PKT_SC_ACK_CHANGE_COSTUME_READ* InPacket);	
	void NetUseSkillCylceAck(PD::SC::PKT_SC_ACK_USE_SKILL_INTERVAL_READ* InPacket);

	void NetStartSkill(PD::SC::PKT_SC_START_SKILL_READ* InPacket);
	void NetSkillFinished(PD::SC::PKT_SC_SKILL_FINISHED_READ* InPacket);
	void NetSkillFailure(PD::SC::PKT_SC_ACK_START_SKILL_FAILURE_READ* InPacket);
	void NetSkillDamaged(PD::SC::PKT_SC_SKILL_DAMAGED_READ* InPacket);

	//
protected:
	void OnInitializeData(const IGsMessageParam* Param);

	void OnCheckSkillSlot();
	void OnCheckSkillSlot(EGsSkillSlotCheckType InCheckType);
	void OnForcedDisableSkill(const IGsMessageParam* InParam);	

	void OnSkillSlotReserve(int32 InSkillId);
	void OnSkillSlotReserveClear(int32 InSkillId);

	void CallbackChainSkillEnd(class FGsComboSkillGroup* SkillGroup);

	void CallbackCheckCooltime(const IGsMessageParam* InParam);
	void CallbackCheckAbnormality(const IGsMessageParam* InParam);
	void CallbackCheckStage(const IGsMessageParam* Param);

	void CallbackCostumeChanged(const IGsMessageParam* Param);
	void CallbackPresetChanged(const IGsMessageParam* InParam);

public:
	void CheckSkillSlotCost();

	// Getter
	// CreatureWeaponType == CreatureWeaponType::MAX : 현재 장착 무기 기준으로 찾음
public:
	// 무기 타입 스킬셋 정보
	int GetWeaponSkillSet(OUT TArray<TSharedPtr<const FGsSkill>>& OutSkillSet, CreatureWeaponType WeaponType = CreatureWeaponType::MAX) const;
	// 현재 활성화된 무기중 사용 가능한(Lean) 스킬정보
	int GetCurrentWeaponLeanSkillSet(OUT TArray<TSharedPtr<const FGsSkill>>& OutSkillSet) const;
	// SkillUiSet 정보 얻기
	int GetSkillUiSet(OUT TArray<TSharedPtr<const FGsSkill>>& OutSkillSet, CreatureWeaponType WeaponType, SkillCategorySet CategorySet);

	// 스킬 찾기
	TSharedPtr< const FGsSkill> FindSkillSharedPtr(int tId, CreatureWeaponType WeaponType = CreatureWeaponType::MAX, bool bSearchAll = false) const;
	TWeakPtr<const FGsSkill> FindSkillWeakPtr(int tId, CreatureWeaponType WeaponType = CreatureWeaponType::MAX, bool bSearchAll = false) const;

	const FGsSkill* FindSkill(int tId, CreatureWeaponType WeaponType = CreatureWeaponType::MAX, bool bSearchAll = false) const;
	// 현재 무기 타입에서 활성화된 스킬 찾기
	const FGsSkill* FindActiveSkill(int tId) const;
	// 슬롯 셋 얻어오기
	const DefSkillSlotMap* GetSkillSlotMap(CreatureWeaponType InWeapon) const;
	// 현재 활성화된 무기의 슬롯 셋 얻어오기
	const DefSkillSlotMap* GetCurrentSkillSlotMap() const;
	// 해당하는 슬롯에 대한 스킬 정보 얻기
	const FGsSkill* FindSkillSlot(SkillSlotId Index) const;
	// 현재 등록된 체인스킬 찾기
	const FGsComboSkillGroup* FindChainSkillGroup(int tId) const;
	// 평타스킬 찾기
	const FGsSkill* FindNormalSkill(CreatureWeaponType InWeapon) const;
	// 슬롯 셋 얻어오기 : 무기 + 프리셋
	const DefSkillSlotMap* GetSkillSlotMapByPresetId(CreatureWeaponType InWeapon, PresetId InPresetId) const;

	CreatureWeaponType GetCurrentWeaponType() const { return _currentWeaponType; }

	bool IsAlreadyLearnedSkill(SkillId InSkillId) const;

	// 스킬 레벨셋 정보 얻기 (util)
	static const struct FGsSchemaSkillLevelInfo* FindSkillLevelSet(int InTId, uint8 InSkillLevel);
	// 스킬 레벨셋 첫번째 판정 영역 얻기
	static const struct FGsSchemaSkillCollision* FindSkillLevelSetToColliosion(int InTId, uint8 InSkillLevel);
	// 콤보 스킬 EGsAnimSlotType 판별 함수 
	// 몽타주의 구성은 일반1스킬 : 0, 크리1스킬 : 1, 일반2스킬 3, 크리2스킬 4 슬롯에 맞춰 구성한다.
	static EGsAnimSlotType GetComboSkillAnimSlotType(const struct FGsSchemaSkillSet* InSkillData,
		EGsAnimSlotType InPreveAnimSlotType, bool InCurrentCritical);

	SkillId FindPassiveSkillIdByUpgradeSkill(const SkillId InSkillId);

protected:
	void SetSkillSlot(CreatureWeaponType InWeapon, SkillSlotId InCurrSlotId, const FGsSkill* InSkill, bool bInUpdate = false);
	void SetSkillSlotPreset(const FGsSkill* InSkill, bool bInUpdate = false);
	void SetChainSkill(const FGsSkill* InSkill);

	void RestoreChainSkill(const FGsComboSkillGroup& ComboSkillGroup);

	void RemoveSkillList(TArray<TPair<SkillId, bool>>& removeSkillList);

public:
	// NonPlayer
	int GetNpcCommonActionSet(OUT TArray<const FGsSchemaSkillCommon*>& OutCommonSet, int GameObjectTId);
	int GetNpcCommonActionSet(OUT TArray<const FGsSchemaSkillCommon*>& OutCommonSet, int GameObjectTId, CommonActionType FindType);

	int GetNpcReactionSet(OUT TArray<const FGsSchemaSkillReaction*>& OutReactionSet, int GameObjectTId);
	int GetNpcReactionSet(OUT TArray<const FGsSchemaSkillReaction*>& OutCommonSet, int GameObjectTId, EReactionType FindType);

	// Player
	int GetPlayerSocialSet(OUT TArray<const FGsSchemaSkillCommon*>& OutCommonSet, UGsGameObjectBase* ObjectClass);

	int GetPolymorphShapeReactionSet(OUT TArray<const FGsSchemaSkillReaction*>& OutReactionSet, int GameObjectTId);

public:
	const FGsSchemaSkillCommon* GetPlayerCommonActionData(UGsGameObjectBase* ObjectClass, CommonActionType CommonType);
	const FGsSchemaSkillCommon* GetNpcCommonActionData(UGsGameObjectBase* ObjectClass, CommonActionType CommonType);

	const FGsSchemaSkillReaction* GetReactionData(UGsGameObjectBase* ObjectClass, EReactionType ReactionType);
	const FGsSchemaSkillReaction* GetNpcReactionData(UGsGameObjectBase* ObjectClass, EReactionType ReactionType);
	const FGsSchemaSkillReaction* GetPlayerReactionData(UGsGameObjectBase* ObjectClass, EReactionType ReactionType);

	const FGsSchemaSkillReaction* GetNpcReactionDataByTableID(int32 GameObjectTId, EReactionType ReactionType);

public:
	FGsSkillUIDataHandler* GetSkillUIDataHandler() { return _skillUIDataHandler.Get(); }

public: //idle감정Motion
	const FGsSchemaIdleEmotionActionData* GetIdleEmotionActionData(UGsGameObjectBase* ObjectClass);
	const FGsSchemaIdleEmotionActionData* GetPlayerIdleEmotionActionData(UGsGameObjectBase* ObjectClass);
	const FGsSchemaIdleEmotionActionData* GetNpcIdleEmotionActionData(UGsGameObjectBase* ObjectClass);

public:
	// 스킬을 강제로 막았는가
	bool IsForcedDisableSkill() const { return _bForcedDisableSkill; }

public:
	bool GetIsShowSkillTargetSelect() { return _isShowSkillTargetSelect; }
	void SetIsShowSkillTargetSelect(bool In_isShowSkillTargetSelect, int In_slotId = -1);
	int GetCurrentSkillTargetSlotId()
	{
		return _currentSkillTargetSlotId;
	}
};

#define GSkill() UGsScopeHolder::GetGlobalManager<UGsSkillManager>(UGsScopeGlobal::EManagerType::Skill)