#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GsPlayableEventNonPlayerComponent.generated.h"

class UGsGameObjectPlayableEventNonPlayer;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UGsPlayableEventNonPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//  사용할 skill id
	UPROPERTY(Category = PlayableEventNonPlayerComponent, EditAnywhere)
	int _useSkillId;

	// table id
	UPROPERTY(Category = PlayableEventNonPlayerComponent, EditAnywhere)
	int _tableId;

private:
	// 살아 있는지
	bool _isAlive = true;


	// 삭제등 관리용
	int _spawnId;

	// game object
	UGsGameObjectPlayableEventNonPlayer* _playableEventNonPlayer;

	// 생성자
public:
	UGsPlayableEventNonPlayerComponent();

	// 가상함수
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type inType) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 스킬 사용
	void UseSkill(TFunction<void(bool)> In_callbackSkillEnd);

	// get, set
public:
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "PlayableEventComponent")
	bool IsAlive()
	{
		return _isAlive;
	}
	UFUNCTION(BlueprintCallable, meta = (Tooltip = ""), Category = "PlayableEventComponent")
	bool IsPlaySequence();

	void SetAlive(bool In_isAlive)
	{
		_isAlive = In_isAlive;
	}
	int GetUseSkillId()
	{
		return _useSkillId;
	}

	int GetTableId()
	{
		return _tableId;
	}

	void SetGameObject(UGsGameObjectPlayableEventNonPlayer* In_gameObject)
	{
		_playableEventNonPlayer = In_gameObject;
	}

	void SetSpawnId(int In_id)
	{
		_spawnId = In_id;
	}
	int GetSpawnId()
	{
		return _spawnId;
	}
};