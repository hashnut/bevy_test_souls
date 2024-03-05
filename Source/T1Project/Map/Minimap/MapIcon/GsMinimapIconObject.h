#pragma once

#include "CoreMinimal.h"
#include "Map/Minimap/MapIcon/GsMinimapIconBase.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "GsMinimapIconObject.generated.h"
/*	
	object 클래스를 가진 미니맵 아이콘 처리

	원래는 제일 기본이었다가
	object 없이 나오는 npc 대표 아이콘이 나와서
	상위 클래스 base 추가되고
	object 있는 아이콘으로 변경

	1. 갱신

	UGsGameObjectBase* 참조에서 크래쉬가 계속 나옴 
	1. F->U 타입으로 변경
	2. UGsGameObjectBase* 를 UPROPERTY() 처리
*/
class UGsUIIconBase;
class UGsGameObjectBase;
class UGsMinimapSceneImage;

UCLASS()
class T1PROJECT_API UGsMinimapIconObject: public UGsMinimapIconBase
{
	GENERATED_BODY()
	
protected:
	// 타겟
	UPROPERTY()
	UGsGameObjectBase* _targetObj = nullptr;

	// 생성자, 소멸자
public:
	virtual ~UGsMinimapIconObject();

	// 가상 함수
public:
	// 초기화
	virtual void Initialize(UGsGameObjectBase* In_targetObj, 
		UGsUIIconBase* In_iconWidget,
		EGsMapIconType In_iconType, float In_updateTime, UGsMinimapSceneImage* In_sceneImage,
		int In_depth, int64 In_gameId);
	// 클리어
	virtual void Finalize() override;
	// 위치 구하기
	virtual FVector2D GetPos() override;
	// 테이블 아이디 구하기
	virtual int GetTableId() override;

	// 로직 함수
public:	
	// 적인가
	bool IsEnemy();

	// get, set
public:
	FVector2D GetActorPos2D();

	UGsGameObjectBase* GetTargetObject() { return _targetObj; }
};	
