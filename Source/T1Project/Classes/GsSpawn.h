#pragma once

#include "CoreMinimal.h"

//------------------------------------------------------------------------------
// 기본적인 예외처리 랩핑 스폰 코드, 
// 동일코드를 활용하면 디버깅할때도 용이함으로 해당코드 사용권장
//------------------------------------------------------------------------------
namespace GsSpawn
{
	// ActorClass::StaticClass()를 이용한 스폰
	// 타입이 고정적일때 사용
	template <typename ActorClass>
	static FORCEINLINE ActorClass* StaticClass(UWorld* World, const FVector& SpawnLocation, const FRotator& SpawnRotation)
	{
		if (nullptr == World)
			return nullptr;

		return World->SpawnActor<ActorClass>(ActorClass::StaticClass(), SpawnLocation, SpawnRotation);
	}


	/*
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARR_ANIM(	TEXT("/Game/InfinityBladeWarriors/Character/WarriorAnimBP.WarriorAnimBP_C"));
	//ConstructorHelpers::FClassFinder  *주위 블루프린트 끝에 _C를 붙여서 검색함 (클래스 검색) : WarriorAnimBP_C
	if (WARR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARR_ANIM.Class);
	}
	데이터 드리븐 형식 배치.
	*/
	static FORCEINLINE AActor* BPClass(UWorld* World, UClass* Class, const FVector& SpawnLocation, const FRotator& SpawnRotation, 
		FName Name = NAME_None)
	{
		if (nullptr == World)
			return nullptr;
		if (nullptr == Class)
			return nullptr;
		FActorSpawnParameters spawnParam =  FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (Name != NAME_None)
		{
			spawnParam.Name = Name;
		}

		return World->SpawnActor(Class, &SpawnLocation, &SpawnRotation, 
			spawnParam);
	}


	// 기존객체를 복사생성 배치 할때 사용한다.
	template <typename ActorClass>
	static FORCEINLINE ActorClass* Clone(ActorClass* ExistingActor, const FVector& SpawnLocation, const FRotator& SpawnRotation)
	{
		UWorld* World = ExistingActor->GetWorld();
		if (nullptr == World)
			return nullptr;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Template = ExistingActor;

		return World->SpawnActor<ActorClass>(ExistingActor->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
	}


	// 세부 설정 배치용
	template <typename ActorClass>
	static FORCEINLINE ActorClass* Detail(
		UWorld* World, UClass* Class, const FVector& SpawnLocation, const FRotator& SpawnRotation,
		AActor* Owner = nullptr, APawn* Instigator = nullptr)
	{
		if (nullptr == World)
			return nullptr;
		if (nullptr == Class)
			return nullptr;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Owner;
		SpawnParams.Instigator = Instigator;
		SpawnParams.bDeferConstruction = false;

		return World->SpawnActor<ActorClass>(Class, SpawnLocation, SpawnRotation, SpawnParams);
	}		
}
