#pragma once
#include "GsStageLobbyBase.h"

class USkeletalMeshComponent;
class ACharacter;
//------------------------------------------------------------------------------
// 스테이지 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class FGsStageLobbyCharacterCustomizing : public FGsStageLobbyBase
{
public:
	FGsStageLobbyCharacterCustomizing();
	virtual ~FGsStageLobbyCharacterCustomizing();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

public:
	virtual class AGsCharacterLobbyPlayer* SetCharacter(CreatureGenderType InGenderType)override;

protected:
	virtual ACharacter* SpawnActor(CreatureGenderType InGenderType) override;

public:
	void InitializeActorRotation();
	//get
public:
	virtual FVector GetCurrentHeadPos(USkeletalMeshComponent* In_skeletalMeshComponent) override;
	virtual FName GetCurrentPosTag() override;
};

