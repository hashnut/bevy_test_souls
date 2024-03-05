#pragma once
#include "CoreMinimal.h"

#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "ActorEx/GsCharacterPlayer.h"

#include "Summon/GsSummonUnveilObjectInterface.h"

#include "Animation/GsSummonCostumeAnimInstance.h"

#include "GsActorSummonCostume.generated.h"

/*
	소환 코스튬 bp
	커스터마이징 쪽 때문에 character player를 상속받아야함
*/

class FGsCustomizeHandler;
class FGsGameObjectDataLocalPlayer;
class UParticleSystemComponent;

UCLASS()
class T1PROJECT_API AGsActorSummonCostume : public AGsCharacterPlayer, public IGsSummonUnveilObjectInterface
{
	GENERATED_BODY()

private:
	// 등급
	CostumeGrade _grade = CostumeGrade::NONE;
	
	// 데이터 인덱스
	int _dataIndex = -1;
	
protected:
	// 등급 표시 파티클
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* _particleGrade;

	// 커스터마이징 핸들러
	FGsCustomizeHandler* _customizeHandler = nullptr;
	CostumeId _costumeId = 0;

	// 생성자
public:
	AGsActorSummonCostume();

// bp call function
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "SummonActor")
	void StartSpawnAni();

	// 가상 함수
public:
	virtual void BeginPlay() override;
	// 스케일 초기화
	virtual void InitScale() override;

	virtual bool IsHighGrade() override;
	virtual void DestroySelf() override;

	// 등급 이펙트 붙이기
	virtual void AttachGradeParticle() override;

	// 스폰 애니 출력
	virtual void PlaySpawnAni() override;

	// 스폰 위치 오프셋
	virtual FVector GetSpawnPosOffset() override;

	// 로직 함수
public:
	// 초기화
	void Initialize(FGsGameObjectDataLocalPlayer* In_localData, 
		CostumeId In_costumeId, CostumeGrade In_grade);
	// 정리
	void Finalize();

	// ani 상태 변경
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "SummonActor")
	void ChangeAniState(EGsStateSummonCostume In_state);

	// get, set 가상 함수
public:
	// hide 처리
	virtual void SetHide(bool In_isHide) override;

	virtual void SetDataIndex(int In_index) override
	{
		_dataIndex = In_index;
	}
	virtual int GetDataIndex() override
	{
		return _dataIndex;
	}

	// get, set
public:
	void SetGrade(CostumeGrade In_grade)
	{
		_grade = In_grade;
	}
	CostumeGrade GetGrade()
	{
		return _grade;
	}

	void SetCostumeId(CostumeId In_id)
	{
		_costumeId = In_id;
	}
	CostumeId GetCostumeId()
	{
		return _costumeId;
	}
};