#pragma once
#include "GsStageLobbyBase.h"

class USkeletalMeshComponent;
//------------------------------------------------------------------------------
// 스테이지 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class FGsStageLobbyCharacterSelect : public FGsStageLobbyBase
{
	using Super = FGsStageLobbyBase;

public:
	FGsStageLobbyCharacterSelect();
	virtual ~FGsStageLobbyCharacterSelect();

protected:
	CreatureGenderType _currentGenderType;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

public:
	virtual class AGsCharacterLobbyPlayer* SetCharacter(CreatureGenderType InGenderType) override;

	virtual void ShowCharacter(bool bInShow) override;

protected:
	virtual class ACharacter* SpawnActor(CreatureGenderType InGenderType) override;
	
	//get
public:
	virtual FVector GetCurrentHeadPos(class USkeletalMeshComponent* In_skeletalMeshComponent) override;
	virtual FName GetCurrentPosTag() override;
};

