#pragma once

#include "GsGameObjectHandlerEnum.h"

/*
	이클래스의 목적은 게임오브젝트 내에 핸들러들의 일관된 생성/소멸 절차를 보장하기 위해 설계되었음 	
*/

class UGsGameObjectBase;

class T1PROJECT_API IGsGameObjectHandler
{	
protected:
	GameObjectHandlerType _type = GameObjectHandlerType::NONE;
	bool _privateUpdate = false;
public:
	IGsGameObjectHandler(GameObjectHandlerType inType) : _type(inType) {}
	virtual ~IGsGameObjectHandler();

public:
	// 생성 소멸관련 구현은 필수 항목
	virtual void Initialize(UGsGameObjectBase* owner) = 0;
	virtual void InitializeActor(class AActor* Owner) {};
	virtual void Finalize() = 0;
	void SetPrivateUpdate(bool inFlag) { _privateUpdate = inFlag; }
	

	// 간략히 구성된 타입검사
	bool IsA(GameObjectHandlerType inType)
	{
		check(inType != GameObjectHandlerType::NONE);
		return _type == inType;
	}

	bool IsPrivateUpdate() { return _privateUpdate; }

public:
	virtual void Update(float Delta) {};
};

