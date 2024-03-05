#pragma once
#include "CoreMinimal.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageGameObject.h"

#include "UI/UILib/Define/GsUIDefine.h"

#include "Summon/GsSummonDefine.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"

class AGsActorSummonSceneObject;
class UGsUITraySummon;
class FGsGameObjectDataLocalPlayer;
class FGsItem;

struct IGsMessageParam;
/*
	뽑기 시스템
*/
class FGsSummonHandler : public IGsEventProgressInterface
{

private:
	// 참조만한다
	// 실제 스폰 및 관리는 uobject 상속된 summon 매니저에서 한다
	AGsActorSummonSceneObject* _sceneObject = nullptr;
	TWeakObjectPtr<UGsUITraySummon> _traySummon;

	MsgSystemHandleArray _msgSystemNoParamDelegates;

	MsgSystemHandleArray _msgSystemParamDelegates;
	MsgGameObjHandleArray _msgGameObjectDelegates;
	
	// 소환 진행중인가
	bool _isSummonPlay = false;	
	// 조이스틱 클리어 했는지
	bool _isClearJoystick = false;

	// 소환 타입
	EGsSummonType _summonType = EGsSummonType::SUMMON_FAIRY;

	// 로컬 데이터
	FGsGameObjectDataLocalPlayer* _localData;
	// 이전에 navigaionbar ui가 켜져있었는가(마지막 끌때 참조)
	bool _isBeforeShowNavigationBarUI = false;
	// 기존 네비게이션 재화
	CurrencyType _currentNavigationCurrency = CurrencyType::NONE;
	// 재시도인가
	bool _isRetry = false;

	bool _isPlaySequence = false;

	// 소환 기본 소리 id
	uint64 _summonBaseSoundId = 0;
	// 페어리 시퀀스 이펙트 사운드 id
	uint64 _summonFairySequenceEffectSoundId = 0;

	// 고등급 등장 사운드 id
	// anim notify를 한개가 아니라 복수개도 설치할수 있다고 함
	// https://jira.com2us.com/jira/browse/C2URWQ-5524
	TArray<uint64> _arraySummonHighShowSoundId;

	// 고등급 획득 시퀀스 표시 카드인지
	// 원래는 카드에 있던 정보인데(클릭할때 출력용)
	// 획득 전 시네마틱(테미르 쾅) 추가 및 모두 획득 클릭시 시네마틱 출력 flow 변경으로
	// 여기서 저장
	bool _isHighSequenceCard = false;
	// 고등급 획득 시퀀스 id
	int _highSequenceId = 0;

	// 가상 함수
public:
	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;
	

	// 로직 함수
public:
	// 초기화(최초 한번)
	void Init();
	// 정리(마지막 한번)
	void Close();
	// 소환 시작
	void StartSummon(EGsSummonType In_summonType);
	// 소환 종료
	void EndSummon(bool Is_sendEventProgress = true);


	// 소환 정보 세팅(페어리)
	void SetSummonFairyInfo(TArray<FairyId> In_arrId);
	// 소환 정보 세팅(코스튬)
	void SetSummonCostumeInfo(TArray<CostumeId> In_arrId);
	// 페어리 스폰(모두)
	void ShowAllUnveilObjects();
	// 고등급 페어리 소환석 바로 열기
	void OpenHighGradeStoneNow();
	// 소환 반복 시도
	void RetrySummon();
	// 조이스틱 복구
	void RestoreJoystick();
	// 재시도 시작
	void StartRetry();

	void StartSummonContents(const FVector& In_localLocaltion);
	
	void StartCommonAction(const FVector& In_localLocaltion);
	// stop sequence: summon
	void StopSummonSequence();

	// start compose interaction
	void StartComposeInteraction();
	// high get sequence finish
	void StopHighGetSequence();
	// https://jira.com2us.com/jira/browse/C2URWQ-4063
	// 처음엔 무조건 전체 카드 애니메이션 스킵 버튼(OPEN_ALL)
	void SetFirstButtonState();
	

	void PlayHighGetCardAnimation();

	void FinishSummonForced();

	// 소환 사운드 믹서 교체후 기본 음악(bgm) 출력
	void ChangeSoundSummonMixer(bool In_isSummonMode);
	// 페어리 이펙트 사운드 재생
	void PlayFairyEffectSound(bool In_isOn, bool In_isHighCardExist = false);
	// https://jira.com2us.com/jira/browse/C2URWQ-4326
	// 모든 뽑기 ui 닫기
	void CloseAllSummonUI();
	// 고등급 등장 사운드 출력(anim notify에서)
	// 시네마틱에서 스킵할때 사운드도 스킵하게 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-5524
	void PlaySummonHighShowSound(bool In_isOn, FString In_soundTblKey = "");

	// 고등급 등장 시네마틱 재생
	void PlayHighGetSequence();

	// 이벤트
public:
	void OnStopMovie(const IGsMessageParam* In_param);
	// 사망시 뽑기 닫기 처리
	void OnLocalPlayerDie(const IGsMessageParam*);
	// compose skip
	void OnSummonComposeSkip(const IGsMessageParam* In_param);

	void OnLocalWarp(const IGsMessageParam* In_param);
	//https://jira.com2us.com/jira/browse/C2URWQ-4326
	// 재연결 성공시
	void OnGameServerReconnectionSuccess();
	// get, set
public:
	// 소환 중인지
	bool GetIsSummonPlay()
	{
		return _isSummonPlay;
	}
	// 조이스틱 clear 했는지(복구 체크용)
	bool GetIsJoystickClear()
	{
		return _isClearJoystick;
	}

	// 로컬 성별(코스튬)
	CreatureGenderType GetLocalGender();

	// retry 아이템 정보 세팅(아이템 정보로)
	// 유효 체크 함수도 내부 있음	
	// 퀵슬롯 1, 2번에 등록한뒤(1: 페어리, 2: 코스튬)
	// 1, 2 연속해서 누르면
	// 1번(페어리) 성공, 2번(코스튬) 실패 
	// 하지만 다시뽑기 데이터는 2번이 저장되어서
	// 두번째는 코스튬뽑게됨
	// 유효하지 않으면 저장 안하게 처리
	//	https://jira.com2us.com/jira/browse/C2URWQ-5204
	void SetRetryDataByItemInfo(const FGsItem* InItem);

	// retry 아이템 정보 세팅
	void SetRetryDataItemByDBId(ItemDBId In_itemDBId);
	// retry bm shop 정보 세팅
	void SetRetryDataBMShopByProductId(BMProductId In_productId, uint16 In_amount);

	bool IsClickRetry()
	{
		return _isRetry;
	}
	// 고등급 획득 연출 정보 세팅
	void SetHighGetSequenceInfo(bool In_isHighSequenceCard, int In_highSequenceId)
	{
		_isHighSequenceCard = In_isHighSequenceCard;
		_highSequenceId = In_highSequenceId;
	}
};