#pragma once

/*
뽑기 카메라 기능 인터페이스
*/
class AGsActorSummonSceneObject;

class IGsSummonCameraWork
{
public:
	virtual ~IGsSummonCameraWork() = default;

public:
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update(float In_deltaTime) = 0;
	virtual void OnFinishLerp() {}
	virtual void SetOwner(AGsActorSummonSceneObject* In_owner) {}
};