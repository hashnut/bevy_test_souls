#include "GsFPSAnalyzer.h"

#if WITH_ENGINE
extern ENGINE_API float GAverageFPS;
#else
float GAverageFPS = 0.0f;	// fake
#endif // WITH_ENGINE

void FGsFPSAnalyzer::Reset()
{
	_MinFPS = 9999;
	_MaxFPS = 0;
	_AveFPS = 0;
	_FPS90 = 0;
	_FPS60 = 0;
	_FPS30 = 0;
	_FPS25 = 0;
	_FPS20 = 0;

	_SampleCount = 0;
	_LastAccessTime = 0.0f;
}

//bak1210 : 내부에서 
void FGsFPSAnalyzer::Update(float fTime, float /*fDelta*/)
{
	if (false == _Active)
	{
		return;
	}

	if (_LastAccessTime == 0.0f)
	{
		_LastAccessTime = fTime;
	}

	float delta = fTime - _LastAccessTime;
	
	if (delta < _Interval)
	{
		return;
	}

	_LastAccessTime = fTime;


	AddSample(GAverageFPS);
}

void FGsFPSAnalyzer::AddSample(float FPSSample)
{
	_SampleCount += 1;

	if (FPSSample >= 90)
	{
		_FPS90++;
	}

	if (FPSSample >= 60)
	{
		_FPS60++;
	}

	if (FPSSample >= 30)
	{
		_FPS30++;
	}

	if (FPSSample >= 25)
	{
		_FPS25++;
	}

	if (FPSSample >= 20)
	{
		_FPS20++;
	}

	if (FPSSample > _MaxFPS)
	{
		_MaxFPS = FPSSample;
	}

	if (FPSSample < _MinFPS)
	{
		_MinFPS = FPSSample;
	}

	_AveFPS = (FPSSample + (float)(_SampleCount - 1) * _AveFPS) / (float)_SampleCount;
}