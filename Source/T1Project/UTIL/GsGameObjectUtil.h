// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedBossEnum.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GsGameObjectUtil.generated.h"

/**
 * UObject형 클래스 유틸 처리
 */

 struct FGsSchemaShapeData;
 struct FGsItemStatData;

 class UGsGameObjectBase;
 class UGsGameObjectPlayer;
 class AGsCharacterLocalPlayer;
 class AGsCharacterBase;
 
struct FGsStatDiffInfo
{
public:
	int32 _preStatValue;
	int32 _currentStatValue;

public:
	FGsStatDiffInfo()=default;
	virtual ~FGsStatDiffInfo()= default;
	FGsStatDiffInfo(int32 inPreStatValue, int32 inNextStatValue)
	{
		_preStatValue = inPreStatValue;
		_currentStatValue = inNextStatValue;
	}
};

UCLASS()
class T1PROJECT_API UGsGameObjectUtil : public UObject
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	static bool TryGenerateClientSpawnGameId(EGsGameObjectType inType, int32 inIndex, int64& outGameId);
#endif
	static EGsGameObjectType GetObjectType(NpcFunctionType inNpcFuncType);
	static EGsGameObjectType GetObjectType(CreatureSpawnType inCreatureSpawnType);
	static CreatureSpawnType GetObjectType(EGsGameObjectType inCreatureSpawnType);
	static EGsStateBase GetStatusType(NpcSpecialStatusType inStatusType);

	static EGsAbnormalityEffectMainType GetAbnormalityMainType(AbnormalityEffectType inAbnormalityType);

	static bool IsEnemy(class UGsGameObjectBase* Target);
	static bool CanTarget(class UGsGameObjectBase* Target);
	static bool CanInteraction(class UGsGameObjectBase* Target);

	// 서버 전송용 내캐릭터 Location, Rotation 접근 함수
	static FVector GetLocalPlayerLocation();
	static int16 GetLocalPlayerDirToAngle();

	static TArray<TWeakPtr<struct FGsFence>> GetCurrentEnteredFence();
	static FText GetNpcName(int32 inCreatureId);
	static FText GetRaceText(CreatureRaceType inRace);
	// 종족의 약점 재질(복수개 일수 있음)
	static FText GetRaceWeaknessItemMaterialText(CreatureRaceType inRace);
	static FText GetRankText(CreatureRankType inRace);
	static FText GetOffensiveText(bool inAgrressive);
	static FText GetMaterialText(ItemMaterial InType);
	static FText GetMonsterElementalText(MonsterElement InType);
	static FText GetChannelText(BossSpawnChannel InType);
	// remote 내소속인가(파티, 기사단 소속 되어있는지 체크)
	// 스캔, 락온에서 전용 사용
	static bool GetIsMyCrewRemote(UGsGameObjectBase* In_remote);
	// remote id가 내소속인가(파티, 기사단 소속 되어있는지 체크)
	// 매너 모드에서 전용 사용
	static bool GetIsMyCrewRemoteByGameId(int64 In_gameId);	

	// 위젯의 이름 show/ hide 옵션 갱신 
	static void UpdateWidgetShowNameOption(EGsGameObjectType In_type);
	// widget hp bar show/ hide cheat option update
	static void UpdateWidgetShowHPBar();

	// 인터랙션 버튼 위젯  show/ hide 옵션 갱신 
	static void UpdateWidgetShowInteractionButtonOption();
	// 로컬 타겟인가
	static bool IsLocalTarget(UGsGameObjectBase* In_object);

	// 현재 로컬 mp rate 값 구하기
	static float GetLocalMPRate();
	// 특정 bone 위치와 로컬의 차이 위치 구하기(z값, 인게임 카메라에서 사용할 값)
	static float GetLocalBonePosDiff(AGsCharacterLocalPlayer* In_localChar, 
		FName In_stateName,
		FName In_boneName,
		CreatureGenderType In_gender,
		float In_time = 0.0f);

	// a와 b bone 사이 거리 구하기
	static float GetLocalTwoBonePosDist(
		AGsCharacterBase* In_char,
		FName In_boneNameA, FName In_boneNameB);

	static bool GetStatDiff(const TArray<FGsItemStatData>& inSource, const TArray<FGsItemStatData>& inDest, OUT TMap<StatType, FGsStatDiffInfo>& outStatMap);

	static class FGsTargetHandlerLocalPlayer* FindTargetHandlerLocalPlayer();

	// 특정 기준 캡슐 콜리더 사이즈로 파티클 이펙트 스케일 얻기
	static FVector GetParticleEffectScale(class UGsGameObjectBase* In_object);
	static FVector GetParticleEffectScale(class AActor* In_Actor);

	static void MeshRelativeLocation(CreatureGenderType genderType, float customHeight,
		float capsuleHalfHeight, AGsCharacterBase* actor);

	static void SaveCameraBoneHeight(CreatureGenderType genderType, float customHeight);

	static float GetCameraBoneHeight(CreatureGenderType genderType, float customHeight = 50.0f);

	static float GetCameraBoneHeightByGameObjectPlayer(UGsGameObjectBase* In_gameObjectPlayer);

	static bool IsInputBlock(EGsKeyMappingType In_type = EGsKeyMappingType::TOUCH_MOVE);
	static bool IsInputFlagBlock(EGsKeyMappingInputFlags In_type = EGsKeyMappingInputFlags::INPUT_NONE);

	static bool GetSpawnMeCompleteFinFlag();

	static void SetSpawnMeCompleteFinFlag(bool InFlag);
};