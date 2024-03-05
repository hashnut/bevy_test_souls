// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/ObjectClass/GsGameObjectPropObject.h"
#include "GameObject/Interaction/GsGameObjectInteractionInterface.h"
#include "Quest/GsGameObjectQuestTargetInterface.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "GsGameObjectPropInteraction.generated.h"

/**
 * 설명 : PropData를 활용한 인터렉션 가능한 프랍의 부모 클래스
 */

UCLASS()
class T1PROJECT_API UGsGameObjectPropInteraction : public UGsGameObjectPropObject, public IGsGameObjectInteractionInterface, public IGsGameObjectQuestTargetInterface
{
	GENERATED_BODY()

protected:
	const struct FGsSchemaPropData* _propData;

protected:
	class FGsGameObjectStateManager* _fsm = nullptr;	

	//quest
protected:
	//인터렉션 콜리전 
	bool _isInteractionActive = false;
	// 퀘스트 타겟 
	bool _isQuestTargetActive = false;
	//인터렉션 가능 여부, 프랍의 최대 인터렉션 횟수를 초과하면 false
	bool _canInteraction = true;	
	//Player가 Interaction 중인 여부
	bool _isInteracting = false;
	//quest type이 아닌 프랍은 플레이어 인터렉션 성공 후 인터렉션 버튼을 키지 말아야하는 경우가 있다. 
	bool _isCanActiveInteractionWidget;

protected:
	//동시에 인터렉션 하는 유저의 수 
	int32 _interactionStartUserCount;
	//여태까지 인터렉션 하는 유저의 수 
	int32 _interactionEndUserCount;

protected:
	UPROPERTY()
	class APlayerCameraManager* _camera;
	float _newInteractionButtonSize;
	float _oldInteractionButtonSize;	

protected:
	UPROPERTY()
	class UGsSimpleTimer* _actionTimer;

public:
	UGsGameObjectPropInteraction();
	virtual ~UGsGameObjectPropInteraction();

public:
	virtual void Initialize(const FGsSpawnParam* SpawnParam) override;
	virtual void Update(float Delta) override;
	virtual void Finalize() override;

	virtual void CreateSubClass() override;
protected:
	virtual void InitializeFSM() override;

protected:
	virtual void InitializeActor(AActor* Owner) override;
	virtual void ActorDespawn() override;

	virtual void RemoveSubClass() override;

public:
	virtual void OnDie() override;

protected:
	virtual void UpdateInteractionColliderSize(float In_rate = 0.0f) override;
	virtual void DrawDebugInteractionRange() override;
	virtual void OnClickInteract(bool In_isByWidget) override;

public:
	virtual class FGsGameObjectStateManager* CreateBaseFSM();
	virtual EGsGameObjectType GetObjectType() const override;
	virtual class FGsGameObjectStateManager* GetBaseFSM() const override { return _fsm; }

public:
	virtual void SetInteractionActive(bool In_isActive, int In_Index = 0) override;
	virtual bool GetIsActiveInteraction() override { return _isInteractionActive; }
	virtual void SetQuestTargetActive(bool In_isActive, int In_Index = 0) override;
	virtual bool GetQuestTargetActive() const override;

protected:
	virtual void SetQuestMarkActive(bool In_isActive, int In_typeIndex = 0) override;

public:
	virtual void SetCanInteraction(bool inInteraction);
	virtual bool GetCanInteraction() const { return _canInteraction; }	
	
	virtual bool IsQuestTarget() override;

	void SetPropSpawnState(PD::SC::PKT_SC_SPAWN_PROP_READ* inSpawnProp);
	void UpdatePropState(PD::SC::PKT_SC_UPDATE_PROP_READ* inPkt);

	bool IsFinish();
	bool IsFinish(int32 inInteractionEndUserCount);
	static bool IsFinish(int32 inMultipleInteractionUserCount, int32 inInteractionEndUserCount);

	bool CanInteraction(int32 inInteractionStartUserCount, int32 inInteractionEndUserCount);
	static bool	CanInteraction(int32 inMultipleInteractionCountMax, int32 inMultipleInteractionUserCount, int32 inInteractionStartUserCount, int32 inInteractionEndUserCount);	
	virtual void SetInteractPropEnd(bool inSuccess) override;

	virtual void UpdatePropState() {};

	//widget
public:
	virtual void InvalidWidget() override;
	virtual void HideWidget() override;

protected:
	//퀘스트 종류 프랍은 인터렉션 후 실패가 나지 않는 이상 퀘스트 마크와 인터렉션 버튼이 꺼진다. 
	//하지만 퀘스트 종류 프랍이 아닌 것은 인터렉션 후 다시 PropAction에서 PropIdle로 가면서 PropIdle Enter에서 퀘스트 마크와 인터렉션 버튼을 킨다. 
	//퀘스트를 제외한 대부분 프랍은 워프 종류 이기 때문에 Idle로 복구해도 인터렉션 버튼을 끈다. 
	virtual bool CheckNeedToDeactiveInteractionWidget() { return true; }

protected:
	virtual bool IsActiveQuestTargetMark();
	virtual bool IsCanInteractionInternal() { return true; }

//Action timer
private:
	void InitActionTimer();

public:
	void StartActionTimer();
	UFUNCTION()
	void OnActionFinish(const UObject* inTimer);
	//프랍 인터렉션 끝 연출을 위한 이펙트
	void PlayActionFinishEffect();
	//프랍 사망 상태 연출을 위한 Effect
	void PlayFinishEffect();	

private:
	void PlayEffect_Internal(const FSoftObjectPath& inSoftPath);

public:
	void DrawDebugProp();

public:
	virtual void StartFadeIn() {}
	virtual void StartFadeOut() {}

public:
	const FGsSchemaPropData* GetPropTableData() const { return _propData; }
};
