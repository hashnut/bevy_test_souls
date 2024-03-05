// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsAreaEnvController.generated.h"

/*
 * AGsAreaEnvController
 * : BP_ENVController 의 부모 클래스. 시간, 환경 연출을 담당한다.
 */

UCLASS()
class T1PROJECT_API AGsAreaEnvController : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGsAreaEnvController();
	
public:
	virtual void BeginDestroy() override;

public:
	// 내 캐릭터 스폰이 끝나는 타이밍에 호출됨. BP에서 필요할 경우 사용하시면 됩니다
	UFUNCTION(BlueprintImplementableEvent, Category = "GsEnv")
	void LocalPlayerSpawnComplete();

	// 현재 무조건 true를 주고 있으므로 무시
	// - 예전에 시간을 흐르게 할 것인지 여부를 결정하려고 만들었으나, Tick은 무조건 돌아야되서 true로 달라고 요청받음	
	UFUNCTION(BlueprintImplementableEvent, Category = "GsEnv")
	void SetEnvEnableDayTime(bool bInEnable);	
	
	// 24시간을 몇 분에 흐를 것인가
	// - 내 캐릭터 스폰 타이밍에 전송
	// - 치트: SetAreaEnvTimeCycleDuration 로 변경가능
	UFUNCTION(BlueprintImplementableEvent, Category = "GsEnv")
	void SetEnvTimeCycleDuration(float InMinutes);

	// 위에 세팅한 24시간 기준 분과 현재 실제시간을 기준으로 Env시간을 연산하여 전달.
	// - InTransitionDuration: 현재 1 고정 값으로 주고 있음. 시간으로 인해 환경 변화가 일어났을 때 보간에 걸리는 시간(초)
	// - 치트: SetAreaEnvTime 로 변경가능
	UFUNCTION(BlueprintImplementableEvent, Category="GsEnv")
	void SetEnvTime(float InHours, float InTransitionDuration);

	// 위의 Env시간과 짜여진 스케줄을 바탕으로 현재 날씨를 전달
	// - 모든 유저가 동일한 날씨를 보도록 하기 위함
	// - 해당 umap에 날씨에 대한 환경설정이 되어있어야 동작
	UFUNCTION(BlueprintImplementableEvent, Category="GsEnv")
	void SetEnvWeather(EGsEnvWeather InWeather);

	// 기본 설정 Env가 아닌 특정 Env로 바꿔야 할 상황에 이벤트 전송
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "GsEnv")
	void StartEnvEvent(EGsEnvEvent InParam);

	// 변경했던 이벤트가 종료되었을 때 이벤트 전송. 아트팀 EndEnvEventWithNext 적용되면 삭제 예정
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "GsEnv")
	void EndEnvEvent(EGsEnvEvent InParam);

	// 변경했던 이벤트가 종료되었을 때 이벤트 전송
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "GsEnv")
	void EndEnvEventWithNext(EGsEnvEvent InCurrEnd, EGsEnvEvent InNextStart);

	// 스캔 이펙트 출력 시작
	UFUNCTION(BlueprintImplementableEvent, Category = "GsEnv")
	void StartFireScanEffect(FVector In_centerPos, float In_maxDistance);

public:
	// Sound처리를 위해 BP에서 불러주는 함수
	UFUNCTION(BlueprintCallable, Category="GsEnv")
	void OnChangeTimeline(EGsEnvTimeline InTimeline);

public:
	// BP에서 파티클 컴포넌트 가져올 때 편의를 위해 제공
	UFUNCTION(BlueprintCallable, Category = "GsEnv")
	class UParticleSystemComponent* GetLocalPlayerParticle();

	// 대화 연출에서 바라보는 대상 라이트 처리에 사용. 시네마틱에선 End만 사용
public:
	// 시작(바라 보는 대상)
	// - 시네마틱 시작 시 별도 호출하지 않음
	UFUNCTION(BlueprintImplementableEvent, Category = "GsEnv")
	void StartSeparateLightSource(AActor* In_targetActor);
	// 종료
	// - 시네마틱 종료시에도 호출함
	UFUNCTION(BlueprintImplementableEvent, Category = "GsEnv")
	void EndSeparateLightSource();

protected:
	// 코드에서만 사용
	class UCameraComponent* GetLocalPlayerCamera();
};
