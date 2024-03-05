// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillHandlerBase.h"
#include "GsSkillDataBuilder.h"

#include "Message/GsMessageInput.h"
#include "Message/GsMessageGameObject.h"

#include "Input/GsInputEventMsgBase.h"

#include "GameObject/Reserve/GsReserveWork.h"

class FGsBuffSkillRunnerLocalPlayer;
class UGsGameObjectLocalPlayer;
class FGsSkill;

struct IGsMessageParam;
struct FGsGameObjectMessageParam;
struct FGsGameObjectMessageParamSkill;
struct FGsSkillNetData;

/**
 * 
 */
class T1PROJECT_API FGsSkillHandlerLocalPlayer  : 
	public FGsSkillHandlerBase,
	public IGsReserveWork
{
	using Super = FGsSkillHandlerBase;

public:
	// 스킬 유지 상태 확인
	// 평타 키입력과 수동 입력에 대한 처리
	// 이정보는 현재 스킬 관련에서만 처리 되고 있으므로 이곳에 선언
	// 추후 공통 룰로 적용될경우 상위 Class 로 이관
	// 키입력과 수동 입력은 나중에 들어온 정보가 유효하여 상호 베타적 관계
	// 02_DD\M1\04_전투\수동전투\M1PC수동전투.pptx
	enum InputPressType
	{
		ComboSkillbutton,
		CursorSkillButton,
		Joystick,

		InputPressType_MAX,
		InputPressType_LAST = InputPressType_MAX - 1
	};

	// 캡슐영역 안에 있는지도 확인이 필요
	enum AttackRangeInsideType
	{
		OutSide,			// 공격 가능 범위가 아님
		InSide,				// 공격 가능 범위에 포함됨
		OverLap,			// 겹처 있음
	};

protected:
	static InputPressType IsPressMode[InputPressType::InputPressType_MAX];

public:
	// 추후 메세지 타입이 정리되면 메세지 바인딩으로 처리 고려
    // OnSkill 호출과 순서를 고려해야함.
	static void SetPress(InputPressType Type);
	static void SetRelease(InputPressType Type);
	// 최종 입력 타입 얻기
	static InputPressType GetLastPressType();
	// 눌리고 있는 타입 검사
	static bool IsPressType(InputPressType Type);

protected:
	UGsGameObjectLocalPlayer* _local					= nullptr;
	// 버프형 Runner 클래스
	FGsBuffSkillRunnerLocalPlayer* _buffSkillRunner = nullptr;
	// 사용 스킬 데이터 구성 클래스
	FGsSkillDataBuilder _skillDataBuilder;

	// 스킬 정보 메세지 구조체
	FGsGameObjectMessageParamSkill* _messageParam;

	// 콤보 스킬 정보 캐싱
	TArray<const FGsSchemaSkillSet*> _listComboSkillData;
	// 콤보 스킬 네트워크 동기화용
	const FGsSchemaSkillSet* _nextComboSkillData = nullptr;
	// 캐스팅 스킬 네트워크 동기화용
	const FGsSchemaSkillSet* _castSkillData = nullptr;

	// 연속된 실패 에러는 체크하여 더이상 시도하지 않게 처리
	TPair<PD::Result, int32 > _skillfailedCount;

	int _checkNavMeshRaycastCount = 0;

	// 최적화된 스킬 사용 타이밍 체크
	// Cooltime정보와 Buff같은 지속형의 지속시간을 고려
	// Key : SkillId, Value : Abnormality Id (remainTime 가장 긴 타입)
	TMap<int, int> _mapSkillUsefulTime;

	bool _enableAutoBuffSkill{ false };
	float _enableDelayBuffSkill{ 0.f };

	// 메세지 델리게이트
	TArray<TPair<MessageInput, FDelegateHandle>> _actionDelegates;
	MsgGameObjHandleArray _gameobjectDelegates;
	MsgGameObjHandleArray _localObjectDelegates;

	// 딜레이 처리 타이머
	FTimerHandle _autoSkillTimerHandle;

public:
	FGsSkillHandlerLocalPlayer()			= default;
	virtual ~FGsSkillHandlerLocalPlayer()	= default;

public:
	// 내자신을 위해 사용하는 스킬인가?
	static bool IsMySelfSkill(const FGsSchemaSkillSet* SkillData);
	static float GetManaCost(const FGsSkill* In_skill);

public:
	virtual void Initialize(UGsGameObjectBase* Owner) override;
	virtual void Update(float Delta) override;
	virtual void Finalize() override;
	
	// 재시도 해라
	virtual void DoRetry() override;

public:
	virtual void LoadSkill() override;
	virtual void LoadSocial() override;

protected:
	virtual void CreateSkillRunner() override;
	virtual void CreateIdleEmotionRunner() override;
	
	virtual void LoadAnimation() override;
	void LoadSkillCache();

public:
	virtual bool OnSkill(int ID) override;
	virtual void StopSkill(int ID = 0) override;

	virtual bool OnReserveSkill();
	void OnMoveToSkill(UGsGameObjectBase* Target, int ApproachRange);
	// 타겟 위치까지의 Navi Path정보를 얻어 네비 이동이 유효한 위치까지 이동
	void OnMoveToSkill(FVector DestPos, float AcceptanceRadius);

	// 죽을때까지 대상을 공격하는 액션을 호출한다.
	void OnManualModeTargetAttackAction();
	
	// 강제로 스킬을 취소 시켜 버린다.
	virtual void CompulsionCancelSkill(int ID) override;

public:
	// SkillId가 0이면 무조건 클리어
	virtual void ClearReserveSkill(uint32 Id = 0);
	virtual void ClearCastingData(uint32 Id);

	// 외부사용 막음
private:
	bool UseSkillRequest(const class FGsSkill* SkillData);
	bool UseBuffRequest(const class FGsSkill* SkillData);
	bool SendSkill(class UGsGameObjectBase* TargetObject, const class FGsSkill* SkillData);
	bool SendBuffSkill(const class FGsSkill* SkillData);

	const class FGsSkill* ConvertGroupSkillData(const class FGsSkill* SkillData);

	FVector FindNaviPathValidPoint(UGsGameObjectBase* Target);
	void ClearRequestSkill(uint32 Id = 0);

	// 방어 코드 적용 - 특정 타겟에서 연속된 실패 상황은 해당 타겟을 클리어후 제외 처리 진행
	void ClearTargetAtSkillFailure();

public:
	// 서버 응답에 따른 처리
	virtual void NetSkillFailure(uint32 Id, PD::Result ErrResult) override;
protected:
	virtual void NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData) override;
	virtual void NetBuffSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData) override;
	virtual void NetActiveSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet, const FGsSchemaSkillSet* skillData) override;

	virtual void CallbackEndRunner(int inSkillID, bool inIsEnd) override;	

	void CallbackMoveToSkillEnd();
	void CallbackMoveToSkillFailed(const struct FPathFollowingResult& Result);
	void CallbackChangeSkillSet(const IGsMessageParam* Param);
	void CallbackAIModeActive(const IGsMessageParam* Param);
	void CallbackSkillCancelable(const IGsMessageParam* Param);
	void CallbackRemoveAbnormality(const IGsMessageParam * Param);
	void CallbackStateChangeClearData(const FGsGameObjectMessageParam* Param);
	void CallbackPressJoystick(const FGsInputEventMsgBase& In_msg);
	void CallbackUpdateBuffSkillAuto(const IGsMessageParam* Param);
	void CallbackCooltimeEnd(const IGsMessageParam * Param);
	void CallbackChangeSkillSlotAuto(const IGsMessageParam * Param);
	void CallbackTargetChange(const IGsMessageParam* In_param);
	void CallbackTargetClear(const IGsMessageParam * In_param);
	void CallbackClearOriginSkilldata(const IGsMessageParam * In_param);
	void CallbackAddAbnormality(const IGsMessageParam* In_param);
	void CallbackUpdateReaction(const IGsMessageParam* In_param);

	void CallbackStartBuffSkill(int skillid);
	void CallbackEndBuffSkill(int skillid);
	void CallbackUpdateBufSkill();

	void CallbackStartEmotionMotion(int32 motionID);
	void CallbackEndEmotionMotion();

public:
	// 연계 스킬 데이터 설정
	void SetNextComboData(const FGsSchemaSkillSet * ComboSkillData);

	virtual void SetAutoIdleEmotionEnable(bool enbable) override;

public:
	virtual const FGsSkillNetData* GetAttackStateNetData() { return nullptr; }

	const FGsSkillReserve& GetReserveSkill() const;
	const FGsSchemaSkillSet* GetNextComboData() const;
	const FGsSchemaSkillSet* GetFirstComboData() const;
	const FGsCastingSkillRequest& GetCastInfo() const;

	// 특정 스킬만 검사할 경우, SkillId를 넘기고, 아닐 경우 기본 파라미터 넘기기. 
	const FGsSchemaSkillSet* GetNextAutoSkill(SkillCategorySet Type = SkillCategorySet::MAX, SkillId tId = INVALID_SKILL_ID);
	// 현재 진행중인 스킬이 Cancel 가능한 타이밍인지 확인

	FGsBuffSkillRunnerLocalPlayer* GetBuffSkillRunner() const { return _buffSkillRunner; }

public:
	AttackRangeInsideType IsInsideSkillRange(const FGsSchemaSkillSet* SkillData) const;
	AttackRangeInsideType IsInsideSkillRangeTarget(const FGsSchemaSkillSet* SkillData, UGsGameObjectBase* In_target) const;
	bool CheckNavMeshRaycast(const FVector& SrcPos, const FVector& DesPos);

	bool IsComboSkill(int SkillId) const;
	bool IsCasting() const;
	bool IsCastingComplete() const;
	// 서버 응답 대기중인가?
	bool IsRequestData() const;
	// 사용 하기 알맞은 스킬인가 확인
	bool IsUsefulSkill(int tId);	

	// 예약 데이터 있는가
	virtual bool IsReserveData() override;
	
	bool CanCancel();

protected:
	bool CanAutoUseSkill(const FGsSkill* InSkill);
	bool IsExistAbnormality(const FGsSkill* InSkill) const;
};
