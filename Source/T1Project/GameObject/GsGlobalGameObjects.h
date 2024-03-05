#pragma once

#include "CoreMinimal.h"
#include "../Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "GsGlobalGameObjects.generated.h"

/*
	이클래스의 목적은 연관 클래스 UGsGameObjectManager (스폰 컴포넌트)의 생명주기외에 
	전역으로 1번만 생성되고 유지되는 GsGameObject를 관리하기 위한 목적으로 제작되었음
	
	상위 부모는 게임전역 인스턴스인 GsGameInstance에서 관리됨으로 그규칙에 준하여 작성되었음

	또한 이클래스에 추가적으로 맴버를 추가할때 GsGameObject하위에 View클래스(Actor)는 전역관리가 되지 않으니
	Actor의 소멸주기관리를 신경써야 합니다.
*/

class UGsGameObjectLocalPlayer;

UCLASS()
class T1PROJECT_API UGsGlobalGameObjects :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UGsGameObjectLocalPlayer*	_localPlayer = nullptr;

	//IGsManager
public:
	virtual void Initialize() override ;
	virtual void Finalize() override;
	virtual void Shutdown() override;

	//Implementation
public:
	void RemoveLocalPlayer();
	
public:
	UGsGameObjectLocalPlayer* GetLocalPlayer() const { return _localPlayer; }
	void SetLocalPlayer(UGsGameObjectLocalPlayer* inLocal) { _localPlayer = inLocal; }
};

#define GGamObjects() UGsScopeHolder::GetGlobalManager<UGsGlobalGameObjects>(UGsScopeGlobal::EManagerType::GGameObjects)