// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlow/GameContents/ContentsLobby/GsContentsLobbyBase.h"

class FGsCustomizeHandler;
class AGsCharacterPlayer;
class ACharacter;
class UWorld;

/**
 * FGsStateLobbyCustomizing
 * 캐릭터 커스터마이징 로직을 담당하는 State
 */
class T1PROJECT_API FGsStateLobbyCharacterCustomizing : public FGsContentsLobbyBase
{
private:
	TWeakObjectPtr<class UGsUIWindowCustomize> _windowUI;

	ACharacter*						_characterSelect;
	UWorld*							_curWorld;
	AGsCharacterPlayer*				_characterCustomizing;
	FGsCustomizeHandler*			_customizeHandler;
	FTransform OldTransform;

public:
	FGsStateLobbyCharacterCustomizing();
	virtual ~FGsStateLobbyCharacterCustomizing();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;
};
