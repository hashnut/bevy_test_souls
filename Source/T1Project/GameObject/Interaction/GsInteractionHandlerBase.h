#pragma once

#include "GameObject/GsGameObjectHandler.h"

class UGsGameObjectBase;

//---------------------------------------
// 인터랙션 핸들러 기본
//---------------------------------------
class FGsInteractionHandlerBase : public IGsGameObjectHandler
{
protected:
	// 인터랙션 대상
	TWeakObjectPtr<UGsGameObjectBase> _interactionTarget;
	
	// 핸들러 소유자
	TWeakObjectPtr<UGsGameObjectBase> _owner;

	// 생성자, 소멸자
public:
	FGsInteractionHandlerBase() : IGsGameObjectHandler(GameObjectHandlerType::INTERACTION) {};
	virtual ~FGsInteractionHandlerBase() = default;

	// 로직 함수
public:
	// 초기화
	virtual void Initialize(UGsGameObjectBase* In_owner) override;
	virtual void Finalize() override {}

	virtual void ClearTarget();

	// 인터랙션 시작( 서버에서 내려줄때 처리)
	virtual void StartInteraction(UGsGameObjectBase* In_taret);
	virtual void EndInteraction(UGsGameObjectBase* In_taret);
	virtual void CancelInteration(UGsGameObjectBase* In_taret);
	virtual void CancelInteration();

	// get, set
public:
	virtual bool IsInteraction() const;
	// 타겟 접근
	UGsGameObjectBase* GetInteractionTarget() const;
};