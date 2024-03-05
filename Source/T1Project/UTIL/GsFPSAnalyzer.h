#pragma once

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

/* 
	누적 FPS 분석 도구 :
*/
class FGsFPSAnalyzer
{
private:
	bool  _Active = false;
	float _LastAccessTime = 0.0f;
private:
	float _MinFPS = 9999;
	float _MaxFPS = 0;
	float _AveFPS = 0;

private:
	int32 _FPS90 = 0;
	int32 _FPS60 = 0;
	int32 _FPS30 = 0;
	int32 _FPS25 = 0;
	int32 _FPS20 = 0;

private:
	int32 _SampleCount = 0;
	float _Interval = 0;

public:
	FGsFPSAnalyzer(float InInterval = 0.5f) : _Interval(InInterval) {}
	~FGsFPSAnalyzer() = default;
		
public:	
	void Reset();
	void Update(float InTime,  float /*InDelta*/);

private:	
	void AddSample(float FPSSample);

public:
	void SetActive(bool InActive) { _Active = InActive; }
	bool GetActive() { return _Active; }

public:
	float GetMinFPS() { return _MinFPS; }
	float GetMaxFPS() { return _MaxFPS; }
	float GetAveFPS() { return _AveFPS; }

public:
	int32 GetTotalCount() { return _SampleCount; }
};