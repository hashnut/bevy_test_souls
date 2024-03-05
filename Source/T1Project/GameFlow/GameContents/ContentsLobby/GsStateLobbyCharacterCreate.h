// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsContentsLobbyBase.h"
#include "Message/GsMessageSystem.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

/**
 * 캐릭터 생성
 */
class AActor;
class AGsActorLobbyObject;

class T1PROJECT_API FGsStateLobbyCharacterCreate : public FGsContentsLobbyBase
{
private:
	TWeakObjectPtr<class UGsUIWindowCharacterCreate> _windowUI;
	AGsActorLobbyObject* _actorPc;

private:
	int32 _sequenceResId;
	int32 _selectChIndex;

public:
	FGsStateLobbyCharacterCreate();
	virtual ~FGsStateLobbyCharacterCreate();	

public:
	virtual void Enter() override;
	virtual void Exit() override;

private:
	// 남/여 교체
	void OnChangeGender(CreatureGenderType inType);
	void OnClickCustomizing();

	void OnChangeyCharacter(int inIndex);
protected:
	virtual void OnResetUI(UGsUIManager::FOnUIManagerResetUI& resetHandler)  override;
};
