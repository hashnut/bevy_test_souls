/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRemoveCondition.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/BSGsRemoveCondition"))
struct DATACENTER_API FGsSchemaRemoveCondition
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="logout", Tooltip))
	bool logout;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="death", Tooltip))
	bool death;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireWeaponCheck", Tooltip))
	bool requireWeaponCheck;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="디스폰 시 제거되도록 설정합니다(서버사용)"))
	bool despawn;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="이상 상태 유지 시간을 체크할 경우 true / 아닐 경우 false"))
	bool durationTimeCheck;
	
public:
	FGsSchemaRemoveCondition();
	
protected:
	bool operator== (const FGsSchemaRemoveCondition& __Other) const
	{
		return FGsSchemaRemoveCondition::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
