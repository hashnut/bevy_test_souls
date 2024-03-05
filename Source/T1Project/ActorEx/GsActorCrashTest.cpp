// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorCrashTest.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Components/SphereComponent.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Data/GsDataContainManager.h"

// Sets default values
AGsActorCrashTest::AGsActorCrashTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;	

	_sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	if (_sphereCollision)
	{
		_sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGsActorCrashTest::OnOverlap);
	}
}

void AGsActorCrashTest::BeginPlay()
{
	Super::BeginPlay();

	_isCrashTest = GData()->GetGlobalData()->isCrashltyicTestValid;
	_crashltyicParam = GData()->GetGlobalData()->CrashltyicParam;
}

void AGsActorCrashTest::OnOverlap(UPrimitiveComponent* inOverlappedComponent, AActor* inOtherActor, UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex, bool inIsFromSweep, const FHitResult& inSweepResult)
{
	//check ove
	if (false == _isCrashTest)
	{
		return;
	}

	UGsGameObjectBase* gameObject = GSGameObject()->FindObject(inOtherActor, EGsGameObjectType::LocalPlayer);
	UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(gameObject);
	if (nullptr == localPlayer)
	{
		return;
	}

	const FString& userName = GGameData()->GetUserData()->mUserName;

	if (userName.Contains(_crashltyicParam))
	{
		UGsGameObjectLocalPlayer* testPlayer = nullptr;
		testPlayer->ActorDespawn();
	}
}

