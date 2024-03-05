#pragma once

/*
 카메라 기능 클래스들의 인터페이스
 초기화, 소멸처리의 의무화를 위한 추상 클래스
*/
class FGsCameraModeBase;
class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;

class IGsCameraWork
{
public:
    virtual ~IGsCameraWork() = default;

public:
    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
    virtual void Update(float In_deltaTime) = 0;
    virtual void OnFinishLerp() {}
    virtual void SetMode(FGsCameraModeBase* In_mode) {}
    virtual void SetWorkTarget(UGsGameObjectBase* In_target) {}
    virtual void SetWorkLocalPlayer(UGsGameObjectLocalPlayer* In_local) {}
    virtual void RemoveLocalPlayer() {}

    virtual void ClearFinishValue() {}
};