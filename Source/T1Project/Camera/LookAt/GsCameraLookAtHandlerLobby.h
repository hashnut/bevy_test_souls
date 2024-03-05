#pragma once

#include "GsCameraLookAtHandlerBase.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

/*
* look at lobby
*/
class FGsCameraLookAtHandlerLobby : public FGsCameraLookAtHandlerBase
{
private:
	CreatureGenderType _gender;

	// virtual get, set
public:
	virtual void SetBoneRotation(const FQuat& In_quat) override;

	// get, set
public:
	void SetGender(CreatureGenderType In_gender)
	{
		_gender = In_gender;
	}

};