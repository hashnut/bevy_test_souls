#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Message/GsMessageSystem.h"
#include "GsActorSequence.generated.h"

struct IGsMessageParam;

UCLASS()
class T1PROJECT_API AGsActorSequence : public AActor
{
	GENERATED_BODY()

private:
	MsgSystemHandleArray	_listSystemDelegate;

	// 가상 함수
public:
	// 초기화
	virtual void BeginPlay() override;
	// 종료 정리
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void OnSequencePlayerStart(const IGsMessageParam* inParam);
	void OnSequencePlayerEnd(const IGsMessageParam* In_param);

	//bp call function
public:
	// bp에서 시퀀스 시작 호출 받기용
	UFUNCTION(BlueprintImplementableEvent, Category = "ActorSequence")
	void StartSequence();
	// bp에서 시퀀스 종료 호출 받기용
	UFUNCTION(BlueprintImplementableEvent, Category = "ActorSequence")
	void EndSequence();
};