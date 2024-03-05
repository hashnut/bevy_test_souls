#pragma once


#include "CoreMinimal.h"
#include "Map/Minimap/MapIcon/GsMinimapIconObject.h"
#include "GsMinimapIconLocal.generated.h"

/*
	로컬 아이콘 처리
	1. 갱신( 타이머가 아닌 매틱 체크한다)
*/

UCLASS()
class T1PROJECT_API UGsMinimapIconLocal : public UGsMinimapIconObject 
{
	GENERATED_BODY()

	// 멤버 변수
private:
	// 로컬 회전 변했는지
	bool _localRotDirty = false;
	// 이전 회전값
	FRotator _oldLocalRot = FRotator::ZeroRotator;
	
	// 생성자, 소멸자
public:
	virtual ~UGsMinimapIconLocal() {}

	// 가상 함수
public:
	// 매틱 갱신
	virtual void Tick(float In_delta) override;

	// 아이콘 위치 세팅
	virtual void SetIconPos(FVector2D In_objectPos) override;

	// 로직함수
public:	
	// 회전 갱신
	void UpdateRotation();

};