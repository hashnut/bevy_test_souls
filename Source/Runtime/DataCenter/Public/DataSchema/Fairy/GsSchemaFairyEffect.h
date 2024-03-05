/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyEffect.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyEffect"))
struct DATACENTER_API FGsSchemaFairyEffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ParticlePath"))
	FSoftObjectPath ParticlePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기본 파티클외에 사용이 필요한 파티클 정보", DisplayName="ETCParticlePath"))
	FSoftObjectPath ETCParticlePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="페어리 소환 대상에게 붙일 이펙트 정보", DisplayName="OwnerParticlePath"))
	FSoftObjectPath OwnerParticlePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SocketName"))
	FString SocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="Scale"))
	float Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="EffectOffset"))
	FVector EffectOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="EffectRotator"))
	FRotator EffectRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SoundRes"))
	FGsSchemaSoundResDataRow SoundRes;
public:
	FGsSchemaFairyEffect();
	
protected:
	bool operator== (const FGsSchemaFairyEffect& __Other) const
	{
		return FGsSchemaFairyEffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
