/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UParticleSystem;
#include "GsSchemaAbnormalityEffect.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/BSGsAbnormalityEffect"))
struct DATACENTER_API FGsSchemaAbnormalityEffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획팀 내용 구분용 명칭", DisplayName="note"))
	FString note;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="particleSpawnType"))
	EGsAbnormalityEffectParticleSpawnType particleSpawnType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="particleEffect"))
	UParticleSystem* particleEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="emitterAttachType"))
	EGsAbnormalityEffectParticleType emitterAttachType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="socketName"))
	FName socketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="posOffset"))
	FVector posOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="soundRes"))
	FGsSchemaSoundResDataRow soundRes;
public:
	FGsSchemaAbnormalityEffect();
	
protected:
	bool operator== (const FGsSchemaAbnormalityEffect& __Other) const
	{
		return FGsSchemaAbnormalityEffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
