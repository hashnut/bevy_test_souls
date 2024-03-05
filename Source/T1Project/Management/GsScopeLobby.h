// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsScopeBase.h"
#include "GsScopeLobby.generated.h"

/**
 * 로비 매니저 모음. 인게임 진입 시 전부 해제.
 */
UCLASS()
class T1PROJECT_API UGsScopeLobby final : public UGsScopeBase
{
	GENERATED_BODY()
	
public:
	enum EManagerType : uint8
	{
		LobbyCharacter = 0,
		LobbyCharacterPreset,
		LobbyData,
		PlayableEvent,
		// ...
		//-------------------------------------------------
		UObjectMgrMax,
		//-------------------------------------------------
		FObjectMgrStart = UObjectMgrMax,
		// ...
		//-------------------------------------------------
		FObjectMgrMax = FObjectMgrStart , // 항목 추가 후엔 NotObjectMgrMax, 로 변경
		//-------------------------------------------------
		Max = FObjectMgrMax
	};

public:
	virtual void Create() override;
};
