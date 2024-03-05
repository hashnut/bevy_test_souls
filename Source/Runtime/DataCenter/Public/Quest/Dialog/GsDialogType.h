#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGsNpcDialogActorStateType : uint8
{
	NONE = 0 UMETA(Tooltip = "없음"),
	DIALOG_SPAWN = 1 UMETA(Tooltip = "대사연출 스폰"),
	DIALOG_DESPAWN = 2 UMETA(Tooltip = "대사연출 디스폰"),
	GLOBAL_SPAWN = 3 UMETA(Tooltip = "글로벌 스폰"),
	GLOBAL_DESPAWN = 4 UMETA(Tooltip = "글로벌 디스폰"),
};

UENUM(BlueprintType)
enum class EGsNpcDialogActorType : uint8
{
	ACTOR_MY = 0 UMETA(Tooltip = "나"),
	ACTOR_TARGET = 1 UMETA(Tooltip = "인터랙션 타겟"),
	ACTOR_NON_TARGET = 2 UMETA(Tooltip = "인터랙션 타겟 외 NPC"),
};

UENUM(BlueprintType)
enum class EGsNpcDialogItemResPosType : uint8
{
	LEFT = 0 UMETA(Tooltip = "왼쪽"),
	CENTER = 1 UMETA(Tooltip = "중앙"),
	RIGHT = 2 UMETA(Tooltip = "오른쪽"),
};

UENUM(BlueprintType)
enum class EGsNpcDialogFairyPosType : uint8
{
	LEFT = 0 UMETA(Tooltip = "왼쪽"),
	RIGHT = 2 UMETA(Tooltip = "오른쪽"),
};

UENUM(BlueprintType)
enum class EGsDialogTextColorType : uint8
{
	TEXT_COLOR_DEFAULT = 0 UMETA(Tooltip = "디폴트"),
	TEXT_COLOR_MY = 1 UMETA(Tooltip = "나(플레이어)"),
	TEXT_COLOR_FAIRY = 2 UMETA(Tooltip = "페어리"),
};

UENUM(BlueprintType)
enum class EGsNpcDialogType : uint8
{	
	DIALOG_TYPE_DIALOG = 0 UMETA(Tooltip = "다이얼로그"),
	DIALOG_TYPE_NARRATION = 1 UMETA(Tooltip = "나래이션"),
};