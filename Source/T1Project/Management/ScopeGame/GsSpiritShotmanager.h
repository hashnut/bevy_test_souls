#pragma once

#include "Classes/GsManager.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageContents.h"
//#include "Shared/Client/SharedEnums/SharedItemEnum.h"
//#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "SpiritShot/GsSpiritShotPassivityCollection.h"
#include "SpiritShot/GsSpiritShotSumPassivityCollection.h"

#include "DataSchema/SpiritShot/GsSchemaSpiritShotEffect.h"
#include "Message/GsMessageContents.h"

class FGsSpiritShotCategoryData;
class FGsSpiritShotLevelData;
class FGsSpiritShotStatData;

struct IGsMessageParam;

/**
 * 천마석 성장 매니저 : 천마석 시스템에 필요한 데이터를 TSharedRef 타입으로 관리한다.
 * 카테고리/레벨/스탯 데이터는 정확히 필요한 만큼만 존재하며, Manager 에서만 찾을 수 있음 (DRY)
 * 컨테이너들은 단지 데이터를 간편하게 찾는 것에 이용될 뿐이다.
 */
class FGsSpiritShotManager final : 
	public IGsManager
{
	friend class FGsGameStateSpiritShot;
	friend class FGsNetMessageHandlerSpiritShot;

// 언락 메시지 수령은 State 에서 관리한다
//private:
//	TPair<MessageContentUnlock, FDelegateHandle> _unlockDelegate;

private:
	/** 천마석 레벨 ID 별 카테고리 정보 캐싱 */
	TMap<SpiritShotLevelId, SpiritShotCategoryId> _levelIdToCategoryId;

	/** 천마석 레벨 ID 별 정보 캐싱 */
	TMap<SpiritShotLevelId, TSharedRef<FGsSpiritShotLevelData>> _levelIdToData;

	/** 플레이어의 카테고리별 천마석 강화 정보 상태 */
	TMap<SpiritShotCategoryId, TSharedRef<FGsSpiritShotCategoryData>> _categoryIdToData;

	/** 카테고리별 레벨 리스트 (레벨 단계를 파악하기 위함) */
	TMap<SpiritShotCategoryId, TArray<SpiritShotLevelId>> _categoryIdToLevelIdList;

	/** 카테고리 리스트 (오름차순) */
	TArray<SpiritShotCategoryId> _categoryIdList;

	/** 천마석 사용 효과 */
	TArray<TPair<StatType, int32>> _consumeStatData;

	/** 천마석 잠식 레벨 효과 */
	TArray<TSharedRef<FGsSpiritShotStatData>> _statSumDataList;

	/** 언락 컨텐츠 Id 와 CategoryId 매핑 */
	TMap<UnlockContentsId, SpiritShotCategoryId> _unlockIdToCategoryId;

	/** 레벨 Id 와 PassivitySetId 매핑 */
	TMap<SpiritShotLevelId, PassivityId> _levelIdToPassivityId;

private:	
	/** 천마석 잠식 종합 레벨 */
	int32 _sumLevel;

	/** 천마석 잠식 종합 스탯 데이터 (각 카테고리의 달성 레벨에 해당하는 스탯의 합) */
	TSharedRef<FGsSpiritShotStatData> _sumStatData;

	/** 천마석 잠식 합산 레벨별 레벨-PassivityId 리스트 (각 레벨별 스탯 정보) */
	TArray<TPair<Level, PassivityId>> _sumLevelPassivityIdPairList;

	/** 천마석 잠식 종합 레벨에 따른 이펙트 리소스/스케일 페어 리스트 */
	TArray<TPair<Level, FGsSchemaSpiritShotEffect>> _sumLevelEffectDataPairList;


	/************************************************************************/
	/* 컬렉션                                                               */
	/************************************************************************/
private:
	/** 천마석 사용 효과 (각 카테고리의 레벨 효과 통합) */
	FGsSpiritShotPassivityCollection _passivityCollectionData;

	/** 천마석 누적 레벨 효과 (천마석 통합 달성 레벨 효과) */
	FGsSpiritShotSumPassivityCollection _sumPassivityCollectionData;

private:
	/** 언락 관련 메시지 */
	//TPair<MessageContentUnlock, FDelegateHandle> _unlockDelegate;

	/************************************************************************/
	/* 인스턴싱																*/
	/************************************************************************/

public:
	FGsSpiritShotManager();

	void Initialize() final;
	void Finalize() final;

	/**
	 * 매니저까지 Message 를 받으면 난잡해지므로, 아래 두 곳 중 하나를 선택하자.
	 * [패킷 메시지] 의 경우, NetMessageHandler 가 받아 에서 매니저 함수를 직접 호출하고,
	 * [인게임 메시지] 의 경우, State 가 받아 매니저 함수를 직접 호출한다.
	 * 특정 상태에 도달하기 위한 진입점의 가지수를 줄임으로써, 디버깅 효율을 높일 수 있다.
	 */
	//void RegisterMessages();
	//void UnRegisterMessages();

private:
	/** 게임 시작 시, 각종 데이터 캐싱 */
	bool InitializeData();


	/************************************************************************/
	/* 패킷	메시지															*/
	/************************************************************************/
private:
	/** [패킷] 서버에서 천마석 데이터 수신 (로비 -> 인게임 진입 시) */
	void SpiritShotInfo(const IGsMessageParam* InParam);

	/** [패킷] 서버에서 천마석 강화 정보 수신 */
	void AckEnchant(const IGsMessageParam* InParam);


	/************************************************************************/
	/* 인게임 메시지                                                        */
	/************************************************************************/
private:
	//void OnContentsUnlock(const struct IGsMessageParam* InParam);

	/** 특정 카테고리 열렸을 때 데이터 갱신. 상태 변경 여부 리턴 */
	bool OnUnlockCategory(UnlockContentsId InContentsId);


	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
public:
	/** 합산 레벨 조회 */
	int32 GetSumLevel() const;

	/** 기본 카테고리 Id 조회 (최상단 값) */
	SpiritShotCategoryId GetDefaultCategoryId() const;

	/** 기본 카테고리의 레벨 Id 조회 */
	SpiritShotLevelId GetDefaultLevelId() const;

	/** 카테고리 데이터 반환 */
	const FGsSpiritShotCategoryData* GetCategoryData(const SpiritShotCategoryId InCategoryId) const;

	/** 해당 카테고리의 레벨 데이터 반환 */
	const FGsSpiritShotLevelData* GetLevelData(const SpiritShotCategoryId InCategoryId) const;

	/** 특정 레벨의 레벨 데이터 반환 */
	const FGsSpiritShotLevelData* GetLevelDataWithLevel(const SpiritShotLevelId InLevelId) const;

	/** 해당 인덱스 번째의 카테고리 데이터 반환 */
	const FGsSpiritShotCategoryData* GetCategoryDataAtIndex(const int32 InIndex) const;

	/** 특정 인덱스 번째의 카테고리 Id 반환 */
	SpiritShotCategoryId GetCategoryIdAtIndex(const int32 InIndex) const;

	/** 카테고리 Id 리스트 반환 (오름차순) */
	const TArray<SpiritShotCategoryId>& GetCategoryIdList() const;

	/** 카테고리의 수 반환 */
	int32 GetTotalCategoryCount() const;

	/** 달성 레벨의 합산 스탯 데이터 조회 */
	TSharedRef<FGsSpiritShotStatData> GetSumStatData() const;

	/** 현재 달성한 각 카테고리의 레벨별로 가지게 될 PassivtyId List 의 합 */
	TArray<PassivityId> GetTotalPassivityIdList() const;

	/** 통합 레벨 별로 얻게 되는 PassivityId 의 리스트 */
	TArray<TPair<Level, PassivityId>> GetSumLevelPassivityIdPairList() const;

	/** 천마석 사용 효과 컬렉션 조회 (각 카테고리의 레벨 효과 통합) */
	const FGsSpiritShotPassivityCollection* GetPassivityCollectionData() const;

	/** 천마석 누적 레벨 효과 컬렉션 조회 (천마석 통합 달성 레벨 효과) */
	const FGsSpiritShotSumPassivityCollection* GetSumPassivityCollectionData() const;

	/** 인풋으로 들어온 누적 레벨에 맞는 천마석 이펙트 경로 반환 */
	FSoftObjectPath GetSpiritShotEffectPath(Level InLevel) const;

	/** 인풋으로 들어온 누적 레벨에 맞는 천마석 이펙트 스케일 반환 (배수 바로 곱할 수 있도록) */
	float GetSpiritShotEffectScale(Level InLevel) const;


	/************************************************************************/
	/* Mutator                                                              */
	/************************************************************************/
private:
	/** 합산 레벨 및 관련 데이터 업데이트 */
	void InvalidateSumData();
	
	/** 천마석 사용 효과 컬렉션 데이터 업데이트 */
	void InvalidatePassivityCollectionData();

	/** 천마석 누적 레벨 효과 컬렉션 데이터 업데이트 */
	void InvalidateSumPassivityCollectionData();
	

	/************************************************************************/
	/* static Helpers                                                       */
	/************************************************************************/
public:

};

#define GSSpiritShot() UGsScopeHolder::GetGameManagerFType<FGsSpiritShotManager>(UGsScopeGame::EManagerType::SpiritShot)