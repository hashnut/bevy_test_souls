// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsGameObjectPropInteraction.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "Quest/EGsVisibleType.h"
#include "GsGameObjectPropTouch.generated.h"

/**
 * 설명 : 프랍의 버튼을 길게 눌러 인터렉션 하는 프랍 클래스
 */

UCLASS()
class T1PROJECT_API UGsGameObjectPropTouch  : public UGsGameObjectPropInteraction
{
	GENERATED_BODY()

protected:
	MsgGameObjHandle _propProgressStart;
	MsgGameObjHandle _propProgressEnd;
	MsgGameObjHandle _propProgressCancel;

protected:
	int32 _currentUserCount;
	int32 _maxUserCount;	
	float _oldColliderSize = 0.0f;	

	//Quest 연출
protected:
	bool _isInitVisible = true;
	EGsVisibleType _visibleType;
	TArray<FGsSpawnObjectShowHide> _questCondtion;		

protected:
	FGsInteractionMessageParam _interactionMessageParam;	

protected:
	UPROPERTY()
	class UGsUIHUDPropTouchProgress* _progressWidget = nullptr;
	class FGsInteractionHandlerLocalPlayer* _interactionHandler = nullptr;

public:
	virtual void Initialize(const FGsSpawnParam* SpawnParam) override;
	virtual void Update(float Delta) override;
	virtual void UpdateVisible() override;

protected:
	virtual void InitializeActor(AActor* Owner) override;
	virtual void ActorDespawn() override;

protected:
	virtual void OnClickInteract(bool In_isByWidget) override;
	virtual void DrawDebugInteractionRange() override;

public:
	void StartPropInteraction(const struct IGsMessageParam* inMessage);
	void EndPropInteraction(const struct IGsMessageParam* inMessage);
	virtual void CancelPropInteraction() override;

	//widget
public:
	virtual void SetInteractionActive(bool In_isActive, int In_Index = 0) override;
	virtual bool IsForceHideInteractionButton();

public:
	virtual void StartFadeIn() override;	

protected:	
	void SetProgressWidget();
	void SetInteractionHandler();	
	void HideInteractProgressWidget();	

protected:
	void UpdateInteractionProgress();
	virtual bool IsCanInteractionInternal() override;

public:
	virtual void SendInteractionStart(const struct FGsPropInteractionMessageParamContents* inParam);
	virtual void SendInteractionEnd(const struct FGsPropInteractionMessageParamContents* inParam);

public:
	virtual EGsGameObjectType GetObjectType() const override;
	virtual void SetVisibleInfo(const FGsSchemaSpawnElement* inVisible) override;
	// visible 바로 세팅(대사 연출 관련 처리)
	virtual void SetGameObjectActorVisible(bool In_isVisible) override;
};
