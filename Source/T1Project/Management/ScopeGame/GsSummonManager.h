#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Summon/GsSummonDefine.h"

#include "Core/Public/Math/Color.h"

#include "GsSummonManager.generated.h"
/*
	뽑기 관리
*/

struct FGsFairyData;
struct FGsCostumeData;

struct FGsSchemaSummonConfig;
struct FGsSchemaItemCommon;

class IGsSummonUnveilObjectInterface;

class FGsSummonHandler;
class FGsSummonCardData;
class FGsGameObjectDataLocalPlayer;

class AGsActorSummonSceneObject;
class AGsActorSummonFairy;
class AGsActorSummonComposeSceneObject;

class USceneComponent;

UCLASS()
class UGsSummonManager : 
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

	// 멤버 변수
private:
	// 소환된 페어리 정보(등급에 의해 정렬된 데이터)
	TArray<TSharedPtr<FGsFairyData>> _arraySummonFairyData;
	// 소환된 코스튬 정보(등급에 의해 정렬된 데이터)
	TArray<TSharedPtr<FGsCostumeData>> _arraySummonCostumeData;

	TArray<FGsSummonCardData*> _arrayCardOrderData;

	// 반복 뽑기인가
	bool _isSummonRetry = false;
	
	// 재화 타입
	CurrencyType _summonCurrencyType = CurrencyType::NONE;
	// 재화 비용
	int _summonCurrencyCost = 0;

	// 재구매 bm product id
	BMProductId _retryBMProductId = 0;
	// 재구매 bm 수량
	uint16 _retryBMamount = 0;

	FString _retryHiveId;

	// 재소환 item id(아이콘 출력용)
	int _retryItemId = 0;
	// 구매 타입
	EGsSummonBuyType _buyType = EGsSummonBuyType::BM_SHOP;



	// 재시도 itemDbId
	ItemDBId _retryItemDbId;

	// 시작 컨텐츠
	EGsSummonOriginContentType _originContent;
	// 소환 타입
	EGsSummonType _summonType = EGsSummonType::SUMMON_FAIRY;

	UPROPERTY()
	AGsActorSummonSceneObject* _sceneObject;

	// actor scene object
	UPROPERTY()
	AActor* _actorSceneObject;

	EGsSummonSceneType _sceneType;

	AGsActorSummonComposeSceneObject* _composeSceneObject;
	
	// 드래그 랜덤 파티클 색상
	TArray<FLinearColor> _arrayDragRandomParticleColor;

	bool _isTestAutoRetrySummon = false;

	// 맵: 인덱스, 오브젝트
	TMap<int, IGsSummonUnveilObjectInterface*> _mapSummonUnveilObject;

	// 스폰된 페어리들
	UPROPERTY()
	TArray<AActor*> _arraySummonUnveilObject;

	UPROPERTY()
	AActor* _veilObject;

	// 로컬 데이터
	FGsGameObjectDataLocalPlayer* _localData;

	CostumeGrade _costumeComposeTryGrade;
	FairyGrade _fairyComposeTryGrade;

	bool _isComposeSuccess = false;

	int _composeResultSequenceId = -1;

	int _currentHighGetDataIndex = 0;

	bool _isPlaySequence = false;

	// 테미르 쾅 결정 등급 이펙트 인덱스
	int _currentHighGetPreEffectIndex;

	// 키: product id
	// 값: group id
	TMap<BMProductId, uint64> _mapProductCachingData;	
	// 이번뽑기로 추가된 마일리지
	int _addedSummonMileage = 0;

	// 뽑기 마일리지 최대값
	int _maxSummonMileage = 0;
	// 뽑기 마일리지 보상 아이템 갯수 저장해놓기(나중에 보상 획득 티커에서 출력용)
	int _summonMileageRewardItemCount = 0;
	// 레전드 등장 cinematic(검증용)
	TArray<int> _arrCostumeAppearCinematic;
	// 유니크 코스튬 마일리지로 받은 보상 횟수
	int _uniqueCostumeMileageRewardObtainCount = 0;
	// 유니크 페어리 마일리지로 받은 보상 횟수
	int _uniqueFairyMileageRewardObtainCount = 0;

	// 가상 함수
public:
	// 초기화
	virtual void Initialize()override;
	// 정리
	virtual void Finalize()override;

	// 로직 함수
public:
	// 카드 정렬 데이터 삭제
	void ClearCardOrderData();
	// 씬 오브젝트 스폰
	AGsActorSummonSceneObject* SpawnSceneObject(FVector& In_localPos);

	AActor* SpawnSceneObjectByType(EGsSummonSceneType In_sceneType, const FVector& In_localPos);

	// 씬 오브젝트 디스폰
	void DespawnSceneObject();

	// sequence reference
	void SummonAllObjects();

	void SummonFairy(int In_dataIndex);

	void SummonCostume(int In_dataIndex);

	void DespawnUnveilObjects();

	void MakeVeilObject();

	// 페어리 스폰
	IGsSummonUnveilObjectInterface* SpawnUnveilObject(FString In_path, int In_index);
	// get, set
public:
	// 소환된 페어리 데이터 추가
	void SetSummonFairyData(TArray<TSharedPtr<FGsFairyData>> In_arrData);
	// 소환된 코스튬 데이터 추가
	void SetSummonCostumeData(TArray<TSharedPtr<FGsCostumeData>> In_arrData);
	// 소환된 페어리 데이터 가져오기
	TArray<TSharedPtr<FGsFairyData>> GetSummonFairyData()
	{
		return _arraySummonFairyData;
	}
	TArray<TSharedPtr<FGsCostumeData>> GetSummonCostumeData()
	{
		return _arraySummonCostumeData;
	}

	TArray<FGsSummonCardData*> GetCardOrderData()
	{
		return _arrayCardOrderData;
	}
	// 현재 소환 타입의 데이터 갯수
	int GetCurrentSummonTypeDataCount();

	// 카드 위치 정렬
	// 등급순
	// 중앙부터 채운다(오잉)
	// 린2m 방식
	void SortCard(EGsSummonType In_summonType, 
		TArray<USceneComponent*> In_arrayPosData);

	void SetCardData(EGsSummonType In_summonType);
	// 이동할 카메라 회전, 위치 구하기
	void GetCamRotAndPos(USceneComponent* In_targetComponent, 
		const FVector& In_targetPos, FVector& Out_pos,
		FRotator& Out_rot, FVector& Out_lookTargetPos, float& Out_distCam);

	// 재시도 역시 컨텐츠에 따라 달라질수 있으므로 
	// 공용 결과에 세팅하면 안되고 
	// 전송 시점(컨텐츠에 따라)에 세팅 해야 합니다.
	// 반복 구매 데이터 세팅

	// retry 사용안함
	void SetNotUseRetry()
	{
		_isSummonRetry = false;		
	}

	// retry bm shop 세팅
	void SetRetryDataBMShop(
		CurrencyType In_currencyType,
		int In_currencyCost,
		BMProductId In_productId = 0,
		uint16 In_amount = 0)
	{
		_isSummonRetry = true;

		_buyType = EGsSummonBuyType::BM_SHOP;

		_summonCurrencyType = In_currencyType;
		_summonCurrencyCost = In_currencyCost;

		_retryBMProductId = In_productId;
		_retryBMamount = In_amount;


	}
	// retry 아이템
	void SetRetryDataItem(int In_retryItemId,
		ItemDBId In_itemDBId)
	{
		_isSummonRetry = true;

		_buyType = EGsSummonBuyType::ITEM;

		_retryItemId = In_retryItemId;

		_retryItemDbId = In_itemDBId;
	}
	

	bool GetIsSummonRetry()
	{
		return _isSummonRetry;
	}
	EGsSummonBuyType GetSummonBuyType()
	{
		return _buyType;
	}

	int GetRetryItemId()
	{
		return _retryItemId;
	}
	CurrencyType GetSummonCurrencyType()
	{
		return _summonCurrencyType;
	}
	int GetSummonCurrencyCost()
	{
		return _summonCurrencyCost;
	}

	BMProductId GetRetryBMProductId()
	{
		return _retryBMProductId;
	}
	uint16 GetRetryBMamount()
	{
		return _retryBMamount;
	}

	ItemDBId GetRetryItemDBId()
	{
		return _retryItemDbId;
	}
	// 결과(OnObtainFairyResult, SC_FAIRY_OBTAIN)는 
	// 공용(item 사용,  합성)으로 결과가 오는곳이므로
	// 받는쪽말고 
	// 서버 전송시점에(아이템 사용 req, 합성 req) 해야합니다.
	void SetSummonOriginContent(EGsSummonOriginContentType In_type)
	{
		_originContent = In_type;
	}

	EGsSummonOriginContentType GetSummonOriginContent()
	{
		return _originContent;
	}
	// 합성 스킵 타입인지
	bool GetIsComposeSkipType();

	// 소환 핸들러 접근
	static FGsSummonHandler* GetSummonHandler();
	// 연출 스킵인가
	bool IsSummonSkip();

	EGsSummonType GetSummonType() 
	{
		return _summonType;
	}

	AGsActorSummonSceneObject* GetSummonSceneObject()
	{
		return _sceneObject;
	}
	// 페어리 최소 최대 등급 입력
	void SetFairyMinMaxGrade(FairyGrade In_minGrade, FairyGrade In_maxGrade);
	// 코스툼 최소 최대 등급 입력
	void SetCostumeMinMaxGrade(CostumeGrade In_minGrade, CostumeGrade In_maxGrade);
	// 페어리 소환 아이템 파티클 등급 세팅
	void SetFairySummonItemParticleGrade(int32 In_itemId);
	// 페어리 소환 합성 파티클 등급 세팅
	void SetFairySummonComposeParticleGrade(FairyGrade In_composeGrade);
	// 페어리 소환 교체 파티클 등급 세팅
	void SetFairySummonWaitChangeParticleGrade(FairyGrade In_waitChangeGrade);
	// 코스튬 소환 아이템 파티클 등급 세팅
	void SetCostumeSummonItemParticleGrade(int32 In_itemId);
	// 코스튬 소환 합성 파티클 등급 세팅
	void SetCostumeSummonComposeParticleGrade(CostumeGrade In_composeGrade);
	// 코스튬 소환 교체 파티클 등급 세팅
	void SetCostumeSummonWaitChangeParticleGrade(CostumeGrade In_waitChangeGrade);
	// 페어리 소환 합성 시도 등급 저장(성공 실패 확인용)
	void SetFairySummonComposeTryGrade(FairyGrade In_composeGrade)
	{
		_fairyComposeTryGrade = In_composeGrade;
	}
	// 페어리 소환 합성 시도 등급 참조
	FairyGrade GetFairySummonComposeTryGrade()
	{
		return _fairyComposeTryGrade;
	}
	// 코스튬 소환 합성 시도 등급 저장(성공 실패 확인용)
	void SetCostumeSummonComposeTryGrade(CostumeGrade In_composeGrade)
	{
		_costumeComposeTryGrade = In_composeGrade;
	}
	// 코스튬 소환 합성 시도 등급 참조
	CostumeGrade GetCostumeSummonComposeTryGrade()
	{
		return _costumeComposeTryGrade;
	}

	void SetIsComposeSuccess(bool In_val);

	// test result type setting:  0~3
	void SetTestComposeResult(int In_resultType);

	bool GetIsComposeSuccess()
	{
		return _isComposeSuccess;
	}


	// 랜덤 등급 컬러 구하기
	FLinearColor GetRandomGradeColor();
	// 테스트 클라이언트 모드인가
	bool GetIsTestClientMode();

	void SetIsTestAutoRetrySummon(bool In_val)
	{
		_isTestAutoRetrySummon = In_val;
	}
	bool GetIsTestAutoRetrySummon()
	{
		return _isTestAutoRetrySummon;
	}

	AGsActorSummonFairy* GetSummonFairyByIndex(int In_index, bool In_isUseVeilObj = true);

	void SetLocalData(FGsGameObjectDataLocalPlayer* In_localData)
	{
		_localData = In_localData;
	}

	int GetCostumeIdByIndex(int In_index);
	bool GetCostumeHighValByIndex(int In_index);

	bool GetFairyHighValByIndex(int In_index);

	AActor* GetVeilObject()
	{
		return _veilObject;
	}

	AGsActorSummonComposeSceneObject* GetComposeSceneObject()
	{
		return _composeSceneObject;
	}

	int GetComposeResultSequenceId()
	{
		return _composeResultSequenceId;
	}

	void SetCurrentHighGetDataIndex(int In_val)
	{
		_currentHighGetDataIndex = In_val;
	}

	int GetCurrentHighGetDataIndex()
	{
		return _currentHighGetDataIndex;
	}

	bool GetFairysHighCardExist();
	bool GetCostumesHighCardExist();

	bool GetIsPlaySequence()
	{
		return _isPlaySequence;
	}

	void SetIsPlaySequence(bool In_val)
	{
		_isPlaySequence = In_val;
	}
	// bm product id로 group id 구하기
	uint64 GetGroupIdByProductId( BMProductId In_productId);
	// 합성: 재료가 레어 이상인가
	bool IsComposeByOverRare();

	// 테미르 쾅 결정 등급 이펙트 인덱스
	int GetCurrentHighGetPreEffectIndex()
	{
		return 	_currentHighGetPreEffectIndex;
	}

	void SetCurrentHighGetPreEffectIndex(int In_index)
	{
		_currentHighGetPreEffectIndex = In_index;
	}
	// config 값 찾기
	const FGsSchemaSummonConfig* FindSummonConfig();
	// 뽑기 마일리지 보상 아이템 테이블 접근
	const FGsSchemaItemCommon* FindSummonMileageRewardItemTable();
	// 뽑기 마일리지 보상 아이템 테이블 접근(타입에 따라 찾기)
	const FGsSchemaItemCommon* FindSummonMileageRewardItemTableBySummonType(bool In_isUniqueReward , EGsSummonType In_summonType);
	// 마일리지 최대 값
	int GetMaxSummonMileage()
	{
		return _maxSummonMileage;
	}
	// 마일리지 최대 값(유니크인가, 소환 타입)
	int GetMaxSummonMileageByType(bool In_isUniqueReward, EGsSummonType In_summonType);
	// 뽑기 마일리지 보상 아이템 갯수 계산
	int GetSummonMileageRewardItemCountCaculated();
	// 뽑기 마일리지 보상 아이템 이전 갯수 계산(이번 획득 마일리지 뺀값)
	int GetSummonMileageRewardItemPreCountCaculated();

	// 뽑기 마일리지 보상 아이템 갯수 저장된거 세팅
	void SetSummonMileageRewardItemCountSaved(int In_count)
	{
		_summonMileageRewardItemCount = In_count;
	}
	// 타입(유니크 타입, 뽑기 타입 )으로 나중에 뜰 티커 아이템 갯수 저장해놓기
	// 서버 send 시점에 저장하기
	void SaveSummonMileageRewardItemCountByType(bool In_isUniqueReward, EGsSummonType In_summonType);
	// 타입(유니크 타입, 뽑기 타입 )으로 나중에 뜰 티커 아이템 갯수 구하기
	int GetSummonMileageRewardItemCountByType(bool In_isUniqueReward, EGsSummonType In_summonType);
	// 소환 보상이 있는가(타입별로, 유니크, 에픽 포함)
	bool IsExistSummonMileageRewardByType(EGsSummonType In_summonType);

	// 뽑기 마일리지 보상 아이템 갯수 저장된거 접근
	int GetSummonMileageRewardItemCountSaved()
	{
		return _summonMileageRewardItemCount;
	}
	// 뽑기로 추가된 마일리지 세팅
	void SetAddedSummonMileage(int In_mileage)
	{
		_addedSummonMileage = In_mileage;
	}
	// 뽑기로 추가된 마일리지 접근
	int GetAddedSummonMileage()
	{
		return _addedSummonMileage;
	}

	TArray<int> GetArrCostumeAppearCinematic()
	{
		return _arrCostumeAppearCinematic;
	}
	// 유니크 마일리지 보상 획득 저장
	void SetUniqueMileageRewardObtainCount(int In_costumeRewardCount, int In_fairyRewardCount)
	{
		_uniqueCostumeMileageRewardObtainCount = In_costumeRewardCount;
		_uniqueFairyMileageRewardObtainCount = In_fairyRewardCount;
	}
	// 유니크 마일리지 보상 받았는가
	bool IsUniqueMileageObtained(EGsSummonType In_summonType);
	// 유니크 보상 받을수 있는가
	// 1: 마일리지 체크
	// 2: 기존 받았는가 체크
	bool IsPossibleUniqueMileageRewardByType(EGsSummonType In_summonType);
};

#define GSSummon() UGsScopeHolder::GetGameManager<UGsSummonManager>(UGsScopeGame::EManagerType::Summon)