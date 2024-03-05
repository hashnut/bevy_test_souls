#pragma once
#include "CoreMinimal.h"
#include "Camera/Define/GsCameraGameDefine.h"

#include "GameObject/Camera/Interface/GsCameraWork.h"

class UGsGameObjectLocalPlayer;
class UCurveFloat;

class FGsCameraRotationDialogCutLoop : public IGsCameraWork
{
	// member variable
private:
	bool _isRotaionLoopStart = false;
	//  변환 진행율(delta time * speed  누적값)
	float _progressRate = 0.0f;
	// 도착값
	FRotator _rotTo = FRotator::ZeroRotator;
	// 시작값
	FRotator _rotFrom = FRotator::ZeroRotator;
	// 현재 값
	FRotator _rotCurrent = FRotator::ZeroRotator;

	FRotator _rotStartCurrentVal = FRotator::ZeroRotator;

	// 현재 스피드
	float _nowSpeed = 0.f;

	// local 플레이어
	UGsGameObjectLocalPlayer* _local;
	// rotation rule: add-> minus -> add
	bool _isAdd = true;

	FRotator _dialogCutLoopOffset = FRotator::ZeroRotator;

	// virtual func
public:
	virtual void Update(float In_deltaTime) override;

	// logic func
public:
	void ChangeRotDirection();
	void StartRotation(const FRotator& In_currentRot);
	void StopRotation()
	{
		_isRotaionLoopStart = false;
	}

	// virtual get, set func
public:
	virtual void Initialize() override;
	virtual void SetWorkLocalPlayer(UGsGameObjectLocalPlayer* In_local) override { _local = In_local; }
	virtual void RemoveLocalPlayer() override;
	virtual void Finalize() override;

	// get, set func
public:	
	void SetRotation(const FRotator& In_rot);
	// 현재 사용 커브 접근
	UCurveFloat* GetCurve();
};