#pragma once
#include "CoreMinimal.h"

// 다각형으로 구정보를 만들때 쓰이는 인터페이스 
// 마을에 활용되는 펜스정보를 구정보로 추출할때 사용
class FGsSphereInterface
{
public:
	virtual int GetVertexCount(void) const = 0;
	virtual bool GetVertex(int index, FVector& OUT vect) const = 0;
};


// 구정보( 언리얼 Sphere + 충돌을 처리한다.
class FGsSphere : protected FSphere
{
public:
	bool RayIntersection(const FVector& rayOrigin, const FVector& rayDirection);
	bool RayIntersectionInFront(const FVector& rayOrigin, const FVector& rayDirection);

public:
	// 월드좌표기준의 포지션 리스트로 해당점들을 모두 포함하는 구체 정보를 생성합니다.
	void Compute(const FGsSphereInterface& source);

public:
	virtual void SetRadius(float r) { W = r; }
	float GetRadius(void) const { return W; }
	const FVector& GetCenter() const { return Center; }
};