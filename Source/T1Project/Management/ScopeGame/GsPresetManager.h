// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/GsManager.h"

#include "Management/GsScopeHolder.h"

#include "Unlock/GsUnlockDefine.h"

#include "Message/GsMessageGameObject.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"


/**
 * 프리셋 정보 관리자
 */

enum EGsPresetId : PresetId
{
	FIRST = 1,
	SECOND = 2,
	THIRD = 3,
};

// 이거 쓰거나 티커 종류가 단순하다면 bool 로 변경해도 좋을듯하다.
enum class EGsPresetDimmedState : uint8
{
	NONE,

	COOLTIME,
	
	CROWD_CONTROL,

	CONTENTS_LOCKED,

	COOLTIME_WEAPON,

	ERROR,

};

struct IGsMessageParam;

class T1PROJECT_API FGsPresetManager final : public IGsManager
{
private:
	bool _requestUserPresetChange = false;

	float _cachedChangeCoolTime = 0.f;

	TMap<PresetId, FString> _presetNameMap;

	MsgGameObjHandleArray _gameObjectMsgHandlerList;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	// 현재 적용중인 프리셋 ID 반환
	PresetId GetCurrentPresetId();

	CostumeId GetCostumeId(PresetId InPresetId);
	FairyId GetFairyId(PresetId InPresetId);

	// 현재 Dimmed 상태 반환
	EGsPresetDimmedState GetDimmedState();
	EGsPresetDimmedState GetDimmedState(PresetId InPresetId);

	EGsUnlockContentsType GetUnlockContentsType(PresetId InPresetId);

	// 프리셋 변경
	void ReqUserPresetChange(PresetId InPresetId);
	void AckUserPresetChange();
	bool IsReqUserPresetChange() { return _requestUserPresetChange; }
	
	// 쿨타임
	float GetRemainCoolTimeRate(); // 잔여 쿨타임 비율 반환 (0~1)

	// 프리셋 이름
	void SetPresetName(PresetId InPresetId, const FString& InPresetName);
	FString GetPresetName(PresetId InPresetId);

	bool IsActivePreset();

protected:
	void OnLocalReaction(const IGsMessageParam* Param);

public:
	void ProcessNetError(int32 InResult);
};

#define GSPreset() UGsScopeHolder::GetGameManagerFType<FGsPresetManager>(UGsScopeGame::EManagerType::Preset)