#pragma once
#include "CoreMinimal.h"

/*
	소환 뽑기 후 나오는 오브젝트 인터페이스
	기존 클래스(AGsCharacterPlayer)와 
	소환에서 쓰는 클래스(소환 전용 페어리)를  
	묶는 상위 클래스
*/
class IGsSummonUnveilObjectInterface
{
public:
	virtual ~IGsSummonUnveilObjectInterface() = default;
public:
	virtual void InitScale() {}
	virtual bool IsHighGrade() { return false; }

	virtual void SetHide(bool In_isHide) {}

	virtual void SetDataIndex(int In_index) {}
	virtual int GetDataIndex() { return -1; }

	virtual void DestroySelf() {}
	// 등급 이펙트 붙이기
	virtual void AttachGradeParticle() {}
	// 삭제 전 처리
	virtual void Finalize() {}
	// 스폰 애니 출력
	virtual void PlaySpawnAni() {}

	// 스폰 위치 오프셋
	virtual FVector GetSpawnPosOffset()
	{
		return FVector::ZeroVector;
	}
};
