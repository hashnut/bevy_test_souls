#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Engine/EngineBaseTypes.h"
#include "GsMoviePostProcess.generated.h"

class UPostProcessComponent;
class UAudioComponent;
class USoundWave;

UCLASS()
class AGsMoviePostProcess : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movie")
	UPostProcessComponent* _postProcessComponent;
	// 뮤직
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundWave* _soundWave;
	UPROPERTY()
	UAudioComponent* _audioComp;
	// 변경 시작인지
	bool _isChangeStart = false;
	// 보간 끝 콜백
	TFunction<void()> _callbackLerpEnd;

	// 시작 시간
	int64 _startTimeTick;
	// 최대 시간
	double _maxTimeTick;

	// 생성자
public:
	AGsMoviePostProcess();

	// 가상함수
public:
	
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
protected:

	virtual void BeginPlay() override;

public:	
	// 종료 호출
	void OnFinish();
	void StartLerp(TFunction<void()> In_callbackLerpEndFunc);

public:
	void SetRate(float In_rate);
	float GetRate();
};