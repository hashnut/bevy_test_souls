#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "Summon/GsSummonUnveilObjectInterface.h"

#include "GsActorSummonUnveilObject.generated.h"

/*
	뽑기 공개된 object
*/

class USkeletalMeshComponent;
class UParticleSystemComponent;

UCLASS()
class T1PROJECT_API AGsActorSummonUnveilObject : public AActor, public IGsSummonUnveilObjectInterface
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	// 몸통
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
	// 얼굴
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshFace;
	// 등급 표시 파티클
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* _particleGrade;
private:
	// 데이터 인덱스
	int _dataIndex = -1;

	// 생성자
public:
	AGsActorSummonUnveilObject();

	// 가상 함수
public:
	virtual void BeginPlay() override;
	virtual void DestroySelf() override;
	virtual void Finalize() override;

	// 로직 함수
public:	
	
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

	// body mesh 접근
	USkeletalMeshComponent* GetPartsSkeletalMeshComponentBody()
	{
		return Mesh;
	}
	// face mesh 접근
	USkeletalMeshComponent* GetPartsSkeletalMeshComponentFace()
	{
		return MeshFace;
	}

};